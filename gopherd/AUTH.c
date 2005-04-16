/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/AUTH.c,v $
 *
 * Paul Lindner, University of Minnesota DCS.
 *
 * Copyright 1991, 92, 93, 94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: AUTH.c
 * Routines to verify authentication
 *********************************************************************
 * Revision History:
 * $Log: AUTH.c,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.17  1995/10/31  16:54:28  lindner
 * Fix for checking for a group
 *
 * Revision 3.16  1995/09/25  05:02:28  lindner
 * Convert to ANSI C
 *
 * Revision 3.15  1995/06/30  20:33:08  lindner
 * Add group authentication
 *
 * Revision 3.14  1995/06/02  17:24:36  lindner
 * Add the ability to specify alternate password files for unixfile auth.
 *
 * Revision 3.13  1995/02/06  21:25:13  lindner
 * Better stdio include
 *
 * Revision 3.12  1995/02/02  17:13:52  lindner
 * Fix memory leaks
 *
 * Revision 3.11  1994/12/31  07:45:22  lindner
 * Remove Item is protected note
 *
 * Revision 3.10  1994/12/15  17:32:21  lindner
 * Fix for A/UX and undefined FILE*, AIX uses compat fgetpwent routines now
 *
 * Revision 3.9  1994/12/12  18:19:07  lindner
 * Fix aix hack
 *
 * Revision 3.8  1994/12/12  17:41:08  lindner
 * Hack around AIX
 *
 * Revision 3.7  1994/12/12  16:58:47  lindner
 * Use fgetpwent() instead of setpwfile()
 *
 * Revision 3.6  1994/12/10  06:12:13  lindner
 * Add support for a custom passwd file just for the gopher server
 *
 * Revision 3.5  1994/10/13  05:17:46  lindner
 * Compiler complaint fixes
 *
 * Revision 3.4  1994/07/22  22:27:20  lindner
 * NO_AUTHENTICATION mods
 *
 * Revision 3.3  1994/07/21  15:45:34  lindner
 * Fix for systems without unistd.h
 *
 * Revision 3.2  1994/07/03  21:18:11  lindner
 * Add initgroup() call
 *
 * Revision 3.1  1994/06/29  05:34:19  lindner
 * New authentication routines
 *
 *
 *********************************************************************/

#ifndef NO_AUTHENTICATION

#include "AUTH.h"
#include "compatible.h"

#include "Malloc.h"

#include "fileio.h"		/* For running scripts */
#include "String.h"		/* strchr and friends */

#include "Debug.h"
#include <stdio.h>

#ifndef VMS
#  include <pwd.h>
#endif

AUTH*
AUTHnew()
{
     AUTH* temp;

     temp = (AUTH*) malloc(sizeof(AUTH));
     temp->type = AUTH_UNINITED;
     temp->name = STRnew();
     temp->scriptname = STRnew();
     temp->authfcn    = NULL;
     temp->authfcnask = NULL;

     return(temp);
}


void
AUTHdestroy(AUTH *auth)
{
     if (auth->name)
	  STRdestroy(auth->name);

     if (auth->scriptname)
	  STRdestroy(auth->scriptname);

     free(auth);
}

void
AUTHinit(AUTH *auth)
{
     STRinit(auth->name);
     STRinit(auth->scriptname);
     auth->authfcn    = NULL;
     auth->authfcnask = NULL;
     auth->type       = AUTH_UNINITED;
}

AUTH*
AUTHcpy(AUTH *dest, AUTH *orig)
{
     STRcpy(dest->name, orig->name);
     STRcpy(dest->scriptname, orig->scriptname);

     dest->authfcn    = orig->authfcn;
     dest->authfcnask = orig->authfcnask;
     dest->type       = orig->type;

     return(dest);
}

/*
 * A very generic ASK block
 */

char**
AUTHgenericAsk(char *directory, char *defuser, char *defpw)
{
     static char **Askstorage = NULL;

     Debug("Trying to do the generic ask block for %s\n", directory);

     if (Askstorage == NULL) {
	  Askstorage = (char**)malloc(sizeof(char*)*5);
	  
	  Askstorage[0] = (char*) malloc(sizeof(char) * 80);
	  Askstorage[1] = (char*) malloc(sizeof(char) * 80);
	  Askstorage[2] = (char*) malloc(sizeof(char) * 80);
	  Askstorage[3] = NULL;
     }

     if (defuser == NULL)
	  defuser = "";
 
     if (defpw == NULL)
	  defpw = "";

/*     if (directory == NULL)
	  strcpy(Askstorage[0], "Note: This item is protected");
     else
	  sprintf(Askstorage[0], "Note: The item %s is protected", directory);
*/

     sprintf(Askstorage[1], "Ask: Your User Name:\t%s", defuser);
     sprintf(Askstorage[2], "AskP: Your Password:\t%s", defpw);

     return(Askstorage);
}

/*
 * Check to see if 'password' is the users password.
 */

static AUTHresult
AUTHunix_checkuser(struct passwd *pw, char *password)
{
     char salt[3];
     char *encodedpw;

     if (password == NULL)
	  return(AUTHRES_OK);	/* Group check, gack! */

     strncpy(salt, pw->pw_passwd, 2);
     
     encodedpw = (char*) crypt(password, salt);

     if (strcmp(encodedpw, pw->pw_passwd) == 0) {
	  if (getuid() == 0) {
	       initgroups(pw->pw_name, pw->pw_gid);
	       setuid(pw->pw_uid);
	       setgid(pw->pw_gid);
	  }
	  return(AUTHRES_OK);
     }
     return(AUTHRES_BADPW);
}


/*
 * Use the password file in etc/passwd or specified in gopherd.conf
 * in the gopher-data directory to authenticate users.  
 * Also check the groups file
 */

AUTHresult
AUTHunixfile(AUTH *auth, char *username, char *password, char *hostname,
	     char *hostip, char *extra_args)
{
#if !defined(VMS)
     char          *pwfilename = "etc/passwd";
     FILE          *ourpasswd;
     struct passwd *pw;
     char          *groups = NULL, *cp;
     AUTHresult     pwcheck, groupresult; 

     Debug("Trying to authenticate %s\n", username);
     
     if (extra_args != NULL) {
	  /** First arg is password file, second is the list of groups **/
	  pwfilename = extra_args;
	  cp = strchr(pwfilename, ' ');
	  if (cp != NULL){
	       *cp = '\0';
	       groups = cp+1;
	  }
     }
     if (password == NULL) {
	  /** Just check the groups and get outta here... **/
	  if (groups != NULL) {
	       /** Now test if the user is in any of the
		 specified groups **/
	       do {
		    cp = strchr(groups, ',');
		    if (cp != NULL) *cp = '\0';
		    
		    groupresult = GGTtest(groups, username);
		    
		    if (groupresult == AUTHRES_OK)
			 return(groupresult);
			 
		    if (cp != NULL) groups = cp+1;
	       } while (cp != NULL);
	       return(AUTHRES_NOGROUP);
	  }
	  return(AUTHRES_OK);
	  ;
     }
     ourpasswd = fopen(pwfilename, "r");

     if (ourpasswd == NULL) {
	  Debug("Couldn't open password file\n", pwfilename);
	  return(AUTHRES_SYSERR);
     }
     
     if (fgetpwent(ourpasswd) == NULL) {
	  Debugmsg("fgetpwent failed...");
	  return(AUTHRES_SYSERR);
     }
     
     fseek(ourpasswd, 0L,SEEK_SET);

     /** Mimic getpwnam, sigh why isn't setpwfile() everywhere....**/
     
     while ((pw = fgetpwent(ourpasswd)) != NULL) {
	  if (strcmp(pw->pw_name, username) == 0) {
	       pwcheck = AUTHunix_checkuser(pw, password);
	       
	       if (groups != NULL && pwcheck == AUTHRES_OK) {
		    /** Now test if the user is in any of the
		      specified groups **/
		    do {
			 cp = strchr(groups, ',');
			 if (cp != NULL) *cp = '\0';
			 
			 groupresult = GGTtest(groups, username);
		    
			 if (groupresult == AUTHRES_OK)
			      return(groupresult);
			 
			 if (cp != NULL) groups = cp+1;
		    } while (cp != NULL);
		    return(AUTHRES_NOGROUP);
	       } else 
		    return(pwcheck);
	  }
     }
     return(AUTHRES_NOUSER);
#else
     return(AUTHRES_SYSERR);
#endif /*vms*/
}
     

/*
 * Do the unix style /etc/passwd authentication...
 */

AUTHresult
AUTHunix(AUTH *auth, char *username, char *password, char *hostname,
	     char *hostip, char *extra_args)
{
#ifndef VMS
     struct passwd *pw;

     pw = getpwnam(username);
     
     if (pw == NULL)
	  return(AUTHRES_NOUSER);

     return(AUTHunix_checkuser(pw, password));
#else
     return(AUTHRES_SYSERR)
#endif /*VMS*/
}


AUTHresult
AUTHvalidate(AUTH *auth, char *username, char *password, char *hostname,
	     char *hostip, char *extra_args)
{
     int    result;
     char   *envstr;


     switch (AUTHgetType(auth)) {
     case AUTH_SCRIPT:
	  /*
	   * Need to set environment variables here...
	   * note envstr must be malloced.
	   */
	  envstr = (char*) malloc(sizeof(char) * (16+strlen(username)));
	  sprintf(envstr, "GOPHER_USER=%s", username);
	  putenv(envstr);
	  
	  envstr = (char*) malloc(sizeof(char) * (16+strlen(password)));
	  sprintf(envstr, "GOPHER_PW=%s", password);
	  putenv(envstr);
	  
	  envstr = (char*) malloc(sizeof(char) * (16+strlen(hostname)));
	  sprintf(envstr, "GOPHER_HOST=%s", hostname);
	  putenv(envstr);
	  
	  envstr = (char*) malloc(sizeof(char) * (16+strlen(hostip)));
	  sprintf(envstr, "GOPHER_IP=%s", hostip);
	  putenv(envstr);
	  
	  
	  result = FIOsystem(AUTHgetScriptName(auth));
	  
	  switch (result) {
	  case 0:			
	       return(AUTHRES_OK);
	  case 1:
	       return(AUTHRES_BADPW);
	  case 2:
	       return(AUTHRES_EXPIRED);
	  case 3:
	       return(AUTHRES_NOUSER);
	       
	  default:
	       return(AUTHRES_BADPW);
	  }
	  break;

     case AUTH_INTERNAL:
	  return(auth->authfcn(auth, username, password, hostname, hostip, extra_args));

     case AUTH_UNINITED:
	  return(AUTHRES_BADPW);

     }
     return(AUTHRES_OK);

}

/*************************************************************************
 * AUTH array routines (AUTHA)
 */

AUTHarray*
AUTHAnew(int size)
{
     AUTHarray *aa;
     AUTH      *auth;

     aa = DAnew((size),AUTHnew,AUTHinit,AUTHdestroy,AUTHcpy);

     auth = AUTHnew();
     if (auth == NULL)
	  return(NULL);
     
     /** Always supply some defaults... **/

     AUTHsetType(auth, AUTH_INTERNAL);
     AUTHsetName(auth, "unix");
     AUTHsetFcn(auth, AUTHunix);
     AUTHsetAskFcn(auth, AUTHgenericAsk);
     AUTHApush(aa, auth);


     AUTHsetType(auth, AUTH_INTERNAL);
     AUTHsetName(auth, "unixfile");
     AUTHsetFcn(auth, AUTHunixfile);
     AUTHsetAskFcn(auth, AUTHgenericAsk);
     AUTHApush(aa, auth);


     AUTHdestroy(auth);

     return(aa);
}


/*
 * Process a gopherd.conf style line and add it to the array
 * really only for scripts.
 */

boolean
AUTHAprocessLine(AUTHarray *autharr, char *inputline)
{
     AUTH *auth;
     char *cp;

     auth = AUTHnew();

     if (auth == NULL)
	  return(FALSE);

     cp = strchr(inputline, ' ');
     *cp = '\0';
     cp++;

     AUTHsetType(auth, AUTH_SCRIPT);
     AUTHsetScriptName(auth, cp);
     AUTHsetName(auth,inputline);
     AUTHsetAskFcn(auth, AUTHgenericAsk);
     
     AUTHApush(autharr, auth);
     
     AUTHdestroy(auth);

     return(TRUE);
}

/*
 * Find the authentication method, and pass the information to the
 * routine that handles it.
 */

AUTHresult
AUTHAvalidate(AUTHarray *autha, char *type, char *username, char *password,
	      char *hostname, char *hostip, char *extra_args)
{
     int i;
     AUTH *auth;

     if (type == NULL)
	  return(AUTHRES_BADPW);

     for (i=0; i < AUTHAgetTop(autha); i++) {
	  auth = AUTHAgetEntry(autha, i);
	  if (strcasecmp(type, AUTHgetName(auth)) == 0) {
	       return(AUTHvalidate(auth,username,password,hostname,hostip,extra_args));
	  }
     }

     /** Shouldn't get to here.. **/
     return(AUTHRES_BADPW);
}


/*
 * Find an AUTH item in the autha array
 */

AUTH*
AUTHAlocate(AUTHarray *autha, char *type)
{
     int i;
     AUTH *auth;

     if (type == NULL)
	  return(NULL);

     for (i=0; i < AUTHAgetTop(autha); i++) {
	  auth = AUTHAgetEntry(autha, i);
	  if (strcasecmp(type, AUTHgetName(auth)) == 0) {
	       return(auth);
	  }
     }

     /** Shouldn't get to here.. **/
     return(NULL);
}


	       

/*********************************************************************
 * AUTHITEMS array
 */

#include "Regex.h"


AUTHITEMS *
AUTHITEMSnew(int size)
{
     AUTHITEMS *ai;

     ai = (AUTHITEMS*) malloc(sizeof(AUTHITEMS));
     
     if (ai != NULL) {
	  ai->regexps    = STAnew(size);
	  ai->authtypes  = STAnew(size);
	  ai->extra_args = STAnew(size);
     }
     return(ai);
}

void
AUTHITEMSdestroy(AUTHITEMS *ai)
{
     if (ai == NULL)
	  return;

     STAdestroy(ai->regexps);
     STAdestroy(ai->authtypes);
     STAdestroy(ai->extra_args);
}

void
AUTHITEMSpush(AUTHITEMS *ai, char *regexp, char *authtype, char *extra_args)
{
     String *str;

     if (ai == NULL)
	  return;

     str = STRnew();
     if (str == NULL)
	  return;

     STRset(str, regexp);
     STApush(ai->regexps, str);

     STRset(str, authtype);
     STApush(ai->authtypes, str);
     
     STRset(str, extra_args);
     STApush(ai->extra_args, str);

     STRdestroy(str);
}

/*
 * Find a match for the item in the regular expressions
 *
 * returns NULL if no match found..
 */

char *
AUTHITEMSfindType(AUTHITEMS *ai, char *fname)
{
     int  i;
     char *rexp;

     if (ai == NULL)
	  return(NULL);

     for (i=0; i < STAgetTop(ai->regexps); i++) {
	  rexp = STAgetText(ai->regexps, i);

	  if (re_comp(rexp))
	       return(NULL);

	  if (re_exec(fname) == 1) {
	       /* We have a match */
	       return(STAgetText(ai->authtypes, i));
	  }
     }
     return(NULL);		/* No match found.. */
}

char *
AUTHITEMSfindExtraArgs(AUTHITEMS *ai, char *fname)
{
     int  i;
     char *rexp;

     if (ai == NULL)
	  return(NULL);

     for (i=0; i < STAgetTop(ai->regexps); i++) {
	  rexp = STAgetText(ai->regexps, i);

	  if (re_comp(rexp))
	       return(NULL);

	  if (re_exec(fname) == 1) {
	       /* We have a match */
	       return(STAgetText(ai->extra_args, i));
	  }
     }
     return(NULL);		/* No match found.. */
}


/*
 * Find the auth item required..
 */

AUTH*
AUTHITEMSfindAUTH(AUTHITEMS *ai, AUTHarray *aa, char *fname)
{
     int  i;
     char *rexp;

     if (ai == NULL)
	  return(NULL);

     for (i=0; i < STAgetTop(ai->regexps); i++) {
	  rexp = STAgetText(ai->regexps, i);
	  Debug("Trying %s\n", rexp);

	  if (re_comp(rexp))
	       return(NULL);

	  if (re_exec(fname) == 1) {
	       /* We have a match */
	       return(AUTHAlocate(aa, STAgetText(ai->authtypes, i)));

	  }
     }
     return(NULL);		/* No match found.. */
}



/*
 * Process a gopherd.conf style line and put it into the arrays.
 */

boolean
AUTHITEMSprocessLine(AUTHITEMS *ai, char *inputline)
{
     char *regex = NULL;
     char *extra_args = NULL;
     char *cp;

     regex = strchr(inputline, ' ');
     
     if (regex == NULL)
	  return(FALSE);
     
     *regex = '\0';
     regex++;

     cp = regex;
     if (*regex == '\'') {
	  cp ++;
	  while (*cp != '\'' && *cp != '\0') 
	       cp++;
	  if (cp == '\0')
	       cp = NULL;	/* No extra args.. */
     } else {
	  cp = strchr(regex, ' ');
     }

     if (cp != NULL)  {
	  *cp = '\0';
	  extra_args = cp+1;
     }

     /* inputline is the type of authentication, cp+1 is the
	regexp and optional filename... 
      */

     AUTHITEMSpush(ai, regex, inputline, extra_args);

     return(TRUE);
}

#endif
