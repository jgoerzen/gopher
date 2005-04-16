/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/gopherdconf.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: gopherdconf.c
 * Routines to parse the gopherd.conf file.
 *********************************************************************
 * Revision History:
 * $Log: gopherdconf.c,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.44  1995/09/26  04:59:46  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.43  1995/09/25  22:10:36  lindner
 * blobbo..
 *
 * Revision 3.42  1995/09/25  05:02:36  lindner
 * Convert to ANSI C
 *
 * Revision 3.41  1995/09/09  03:20:27  lindner
 * Better Error checks
 *
 * Revision 3.40  1995/08/30  20:13:35  lindner
 * Remove tixfile
 *
 * Revision 3.39  1995/06/30  20:33:30  lindner
 * Add group authentication
 *
 * Revision 3.38  1995/06/02  17:24:38  lindner
 * Add the ability to specify alternate password files for unixfile auth.
 *
 * Revision 3.37  1995/04/15  07:14:18  lindner
 * Add http port to gopherd.conf
 *
 * Revision 3.36  1995/02/16  22:32:39  lindner
 * HTML icon support
 *
 * Revision 3.35  1995/02/13  19:07:06  lindner
 * Add MaxConnection limit for total server
 *
 * Revision 3.34  1995/02/11  06:21:26  lindner
 * remove pid junk
 *
 * Revision 3.33  1995/02/06  22:14:50  lindner
 * remove unused variables
 *
 * Revision 3.32  1995/02/02  17:13:54  lindner
 * Fix memory leaks
 *
 * Revision 3.31  1994/12/15  17:33:19  lindner
 * Add BlockScript: keyword support
 *
 * Revision 3.30  1994/12/08  05:23:00  lindner
 * Fix wording problem in integrity check
 *
 * Revision 3.29  1994/10/10  18:39:36  lindner
 * Change auxconf: to use regular expressions
 *
 * Revision 3.28  1994/09/29  19:59:16  lindner
 * Force people to update their gopherd.conf files
 *
 * Revision 3.27  1994/08/18  22:28:17  lindner
 * Abstract for top level and malloc casts
 *
 * Revision 3.26  1994/08/03  03:33:25  lindner
 * Include files in SERVERDIR
 *
 * Revision 3.25  1994/07/22  22:56:18  lindner
 * More NO_AUTH stuff..
 *
 * Revision 3.24  1994/07/22  22:26:25  lindner
 * NO_AUTHENTICATION mods
 *
 * Revision 3.23  1994/07/22  16:37:07  lindner
 * Add INCLUDE directive for gopherd.conf files
 *
 * Revision 3.22  1994/07/21  17:23:05  lindner
 * Add File Separator code
 *
 * Revision 3.21  1994/06/29  05:30:08  lindner
 * Add code to handle lists of authscripts and authitems
 *
 * Revision 3.20  1994/03/31  22:46:26  lindner
 * Allow whitespace in the gopherd.conf file before tokens
 *
 * Revision 3.19  1994/03/17  21:18:39  lindner
 * Massive reworking of access limits
 *
 * Revision 3.18  1994/03/17  04:30:17  lindner
 * VMS fixes gopherd.h
 *
 * Revision 3.17  1994/03/08  15:56:01  lindner
 * gcc -Wall fixes
 *
 * Revision 3.16  1994/01/21  03:59:11  lindner
 * Add support for timezone variable and fix return type
 *
 * Revision 3.15  1993/12/09  20:48:16  lindner
 * Debug fixes
 *
 * Revision 3.14  1993/10/27  20:36:59  lindner
 * Plug memory leak
 *
 * Revision 3.13  1993/10/20  03:18:17  lindner
 * Code for ignore_patt:
 *
 * Revision 3.12  1993/10/11  04:39:49  lindner
 * Allow quotes in dirname for auxconf
 *
 * Revision 3.11  1993/10/07  05:19:54  lindner
 * Update for gateway items and GDCevalDir
 *
 * Revision 3.10  1993/10/04  06:48:57  lindner
 * mods to support auxconf files
 *
 * Revision 3.9  1993/09/30  17:05:11  lindner
 * start of subconf
 *
 * Revision 3.8  1993/09/21  04:16:50  lindner
 * Move cache settings into gopherd.conf
 *
 * Revision 3.7  1993/08/23  18:46:17  lindner
 * Crude addition of a veronica top-level block
 *
 * Revision 3.6  1993/08/20  18:03:07  lindner
 * Mods to allow gopherd.conf files control ftp gateway access
 *
 * Revision 3.5  1993/07/27  05:27:49  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.4  1993/07/23  03:19:20  lindner
 * Mods for using decoder:'s
 *
 * Revision 3.3  1993/04/15  17:09:22  lindner
 * none
 *
 * Revision 3.2  1993/03/24  20:25:14  lindner
 * Addition for secureusers file
 *
 * Revision 3.1.1.1  1993/02/11  18:02:51  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.3  1993/02/09  22:30:56  lindner
 * Additions for multi-languages
 *
 * Revision 1.2  1993/01/30  23:57:44  lindner
 * Gopher+ stuff
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#include "gopherdconf.h"
#include "Malloc.h"
#include "String.h"
#include <stdio.h>
#include "util.h"
#include "Debug.h"
#include "conf.h"

/*********************************************/

GDCobj *
GDCnew()
{
     GDCobj *gdc;

     gdc = (GDCobj *) malloc(sizeof(GDCobj));

     gdc->Extensions   = EXAnew();

#ifndef NO_AUTHENTICATION
     gdc->Sites        = SiteArrayNew();
     gdc->Securityon   = FALSE;

     gdc->Authroutines = AUTHAnew(3);
     gdc->Authitems    = AUTHITEMSnew(3);
     gdc->Serverpw     = STRnew();
     gdc->groupdir     = STRnew();
     gdc->Defaccess    = ACC_FULL;
#endif

     gdc->RunFromInetd = FALSE;
     gdc->Caching      = TRUE;
     gdc->Cachetime    = 180;  /** Three minutes ***/

     gdc->Logfile      = STRnew();
     gdc->Errorfile    = STRnew();
     gdc->Data_Dir     = STRnew();
     gdc->Hostname     = STRnew();
     gdc->Port         = 0;
     gdc->chroot       = TRUE;
     gdc->BummerMsg    = STRnew();
     
     gdc->Admin        = STRnew();
     gdc->AdminEmail   = STRnew();
     gdc->Abstract     = STRnew();

     gdc->Site         = STRnew();
     gdc->Org          = STRnew();
     gdc->Geog         = STRnew();
     gdc->Loc          = STRnew();
     gdc->Lang         = STRnew();
     gdc->TZ           = 0;
     gdc->VeronicaIndex = TRUE;

     STRset(gdc->Logfile, "");
     STRset(gdc->Errorfile, "");
     STRset(gdc->BummerMsg, "");
     STRset(gdc->Hostname, "");

     gdc->other_dirs = STAnew(2);
     gdc->other_gdcs = STAnew(2);

     gdc->FileSeparators = STAnew(5);

     gdc->BlkScriptBlocks = STAnew(5);
     gdc->BlkScripts      = STAnew(5);

     gdc->MaxConnections  = -1;
     gdc->HTTPport        = 80;
     return(gdc);
}

void
GDCdestroy(GDCobj *gdc)
{
#ifndef NO_AUTHENTICATION
     EXAdestroy(gdc->Extensions);
     SiteArrDestroy(gdc->Sites);
     AUTHAdestroy(gdc->Authroutines);
     AUTHITEMSdestroy(gdc->Authitems);
     STRdestroy(gdc->Serverpw);
     STRdestroy(gdc->groupdir);
#endif

     STRdestroy(gdc->Logfile);
     STRdestroy(gdc->BummerMsg);
     STRdestroy(gdc->Hostname);
     STRdestroy(gdc->Lang);
     STRdestroy(gdc->Abstract);

     /** etc..**/

     STAdestroy(gdc->other_dirs);
     STAdestroy(gdc->other_gdcs);
     STAdestroy(gdc->FileSeparators);

     STAdestroy(gdc->BlkScriptBlocks);
     STAdestroy(gdc->BlkScripts);
     free(gdc);
}

#define MAXGDCFLEVEL 10
static FILE *GDCfiles[MAXGDCFLEVEL]; /* can 'include' up to ten levels deep */
static int  GDCfilenum = -1;

static void
GDCfilePush(FILE *f)
{
     if ((++GDCfilenum) < MAXGDCFLEVEL)
	  GDCfiles[GDCfilenum] = f;
     else {
	  fprintf(stderr, "Nesting level too deep for include directive\n");
	  exit(-1);
     }
}

static
char *
GDCfileGetline(char *inputline, int  size)
{
     char* farg;

     while (NULL == (farg = fgets(inputline, size, GDCfiles[GDCfilenum]))) {
	  if (GDCfilenum == 0) {
	       fclose(GDCfiles[GDCfilenum]);
	       return(farg);
	  }

	  /** Try popping up to the previous file.. **/
	  fclose(GDCfiles[GDCfilenum]);
	  GDCfilenum--;
     }
     
     return(farg);
}


/*
 * Parse Gopherd.conf tokens..
 */

static
boolean
GDCtokens(GDCobj *gdc, char *token, char *rest)
{
     boolean success = TRUE;


     if (strcasecmp(token, "ADMIN")==0)
	  GDCsetAdmin(gdc, rest);
#ifndef NO_AUTHENTICATION
     else if (strcasecmp(token, "ACCESS") == 0) {
	  Accesslevel moo;

	  success = SiteProcessLine(gdc->Sites, rest, gdc->Defaccess);
	  moo = SiteDefAccess(gdc->Sites);

	  if (moo != ACC_UNKNOWN)
	       gdc->Defaccess = moo;

	  gdc->Securityon = TRUE;
     }
     else if (strcasecmp(token, "AUTHSCRIPT")==0) {
	  success = AUTHAprocessLine(gdc->Authroutines, rest);
     }
     else if (strcasecmp(token, "AUTHITEM")==0) {
	  success = AUTHITEMSprocessLine(gdc->Authitems, rest);
     }
     else if (strcasecmp(token, "SERVERPW")==0) {
	  GDCsetPW(gdc, rest);
     } 
     else if (strcasecmp(token, "GROUPDIR")==0) {
	  GDCsetGroupdir(gdc, rest);
	  success = GGTfromDir(rest);
     }
#endif
     else if (strcasecmp(token, "ADMINEMAIL")==0)
	  GDCsetAdminEmail(gdc, rest);
     else if (strcasecmp(token, "HOSTALIAS")==0)
	  GDCsetHostname(gdc, rest);
     else if (strcasecmp(token, "SITE")==0)
	  GDCsetSite(gdc, rest);
     else if (strcasecmp(token, "ORG")==0)
	  GDCsetOrg(gdc, rest);
     else if (strcasecmp(token, "LOC")==0)
	  GDCsetLoc(gdc, rest);
     else if (strcasecmp(token, "LOGFILE")==0)
	  GDCsetLogfile(gdc, rest);
     else if (strcasecmp(token, "ERRORFILE")==0)
	  GDCsetErrorfile(gdc, rest);
     else if (strcasecmp(token, "GEOG")==0)
	  GDCsetGeog(gdc, rest);
     else if (strcasecmp(token, "BUMMERMSG")==0)
	  GDCsetBummerMsg(gdc, rest);
     else if (strcasecmp(token, "LANGUAGE")==0) {
	  rest = skip_whitespace(rest);
	  GDCsetLang(gdc, rest);
     }
     else if (strcasecmp(token, "TZ") == 0)
	  GDCsetTZ(gdc,atoi(rest));
     else if (strcasecmp(token, "VIEWEXT")==0) {
	  success = EXAprocessLine(gdc->Extensions, EXT_VIEW, rest, 
				   GDCgetLang(gdc));
     }
     else if (strcasecmp(token, "BLOCKEXT")==0) {
	  success = EXAprocessLine(gdc->Extensions, EXT_BLOCK, rest, NULL);
     }
     else if (strcasecmp(token, "BLOCKREFEXT")==0) {
	  success = EXAprocessLine(gdc->Extensions, EXT_BLOCKREF, rest, NULL);
     }
     else if (strcasecmp(token, "IGNORE")==0) {
	  success = EXAprocessLine(gdc->Extensions, EXT_IGNORE, rest, NULL);
     }
     else if (strcasecmp(token, "IGNORE_PATT")==0) {
	  success = EXAprocessLine(gdc->Extensions, EXT_IGNOREPAT, rest, NULL);
     }
     else if (strcasecmp(token, "DECODER")==0) {
	  success = EXAprocessLine(gdc->Extensions, EXT_DECODER, rest, NULL);
     } else if (strcasecmp(token, "ABSTRACT") == 0) {
	  GDCsetAbstract(gdc, rest);
     }
     else if (strcasecmp(token, "VERONICAINDEX")==0) {
	  if (strcasecmp(rest, "no")==0)
	       GDCsetShouldIndex(gdc, FALSE);
	  else
	       GDCsetShouldIndex(gdc, TRUE);
     }
     else if (strcasecmp(token, "CACHETIME")==0) {
	  GDCsetCachetime(gdc, atoi(rest));
     }
     else if (strcasecmp(token, "FILESEP")==0) {
	  GDCaddFileSep(gdc, rest);
     } else if (strcasecmp(token, "HTTPPORT") == 0) {
	  GDCsetHTTPport(gdc, atoi(rest));
     }
     else if (strcasecmp(token, "INCLUDE") == 0) {
	  /** Check to see if we can open the file.. **/
	  /** Then push it onto the stack.. **/
	  FILE *gdcfile;

	  if (*rest != '/' && *rest != '.') {
	       char tmpstr[256];

	       sprintf(tmpstr, "%s/%s", GDCDIR, rest);
	       strcpy(rest, tmpstr);
	  }
	  if ((gdcfile = fopen(rest, "r")) == (FILE *) NULL) {
	       fprintf(stderr,"Cannot open file '%s'\n", rest);
	       exit(-1);
	  }
	  GDCfilePush(gdcfile);
	  
     }
     else if (strcasecmp(token, "BLOCKSCRIPT") == 0) {
	  char *cp;

	  /** Block name is first argument, script to run is second **/

	  cp = strchr(rest, ' ');
	  if (cp != NULL) {
	       *cp = '\0';
	       cp++;
	       
	       GDCpushBlockScript(gdc, rest, cp);

	  } else {
	       success = FALSE;
	  }
     }
     else if (strcasecmp(token, "AUXCONF")==0) {
	  /** Directory is first arg, conf file is second **/
	  char *cp;

	  if (*rest == '\"') {
	       /*** " Dir is in quotes.. ***/
	       rest++;
	       cp = strchr(rest, '\"'); /* " Fix for hilit19 */
	       if (cp != NULL) {
		    *cp = '\0';
		    cp ++;
	       }
	  } else {
	       cp = strchr(rest, ' ');
	  }

	  if (cp == NULL)
	       success = FALSE;
	  else {
	       *cp = '\0';
	       GDCpushOtherGDC(gdc, rest, cp+1);
	  }
     }
     else if (strcasecmp(token, "MAXCONNECTIONS")==0) {
	  int numconns = atoi(rest);

	  if (numconns > 1)
	       GDCsetMaxconns(gdc, atoi(rest));
     }
     else
	  success = FALSE;
     
     return(success);
}


void
GDCfromFile(GDCobj *gdc, char *filename)
{
     FILE *gdcfile;
     char inputline[512];
     char *cp, *linestart, *token, *restofline;
     boolean success;


     if ((gdcfile = fopen(filename, "r")) == (FILE *) NULL) {
	  printf("Cannot open file '%s'\n", filename);
	  exit(-1);
     }

     GDCfilePush(gdcfile);

     while (GDCfileGetline(inputline, sizeof(inputline))) {
	  int inputlen;
	  ZapCRLF(inputline);

	  inputlen = strlen(inputline);	  

	  while ((inputlen > 1) && inputline[inputlen-1] == '\\') {
	       inputline[inputlen-1] = '\n';
	       if (!GDCfileGetline(inputline + inputlen, 
			      sizeof(inputline)-inputlen))
		    break;
	       ZapCRLF(inputline+inputlen);
	       inputlen += strlen(inputline+inputlen);
	  }

	  /** Eat whitespace **/
 	  linestart= inputline;
	  for ( ; *linestart != '\0' && *linestart <= ' '; linestart++) ;
 
 	  if (*linestart == '#' || *linestart == '\0') /** Ignore comments **/
	       continue;

	  cp = strchr(linestart, ':');
	  if (cp == NULL) {
	       fprintf(stderr, "Bad line '%s'\n", inputline);
	       fprintf(stderr, "Line needs a colon\n");
	       exit(-1);
	  }
	  *cp = '\0';
	  token      = linestart;
	  restofline = cp+1;
	  while (*restofline == ' ' || *restofline == '\t')
	       restofline++;
	  
	  success = GDCtokens(gdc, token, restofline);
	  if (!success) {
	       fprintf(stderr, "Bad line '%s'\n", inputline);
	       if (strcasecmp(inputline, "ext")==0)
		    fprintf(stderr, "please upgrade your gopherd.conf file\n");
	       exit(-1);
	  }
     }
}


void
GDCpushOtherGDC(GDCobj *gdc, char *dir, char *conffile)
{
     String *temp;

     temp = STRnew();
     STRset(temp, dir);
     STApush(gdc->other_dirs, temp);

     STRset(temp, conffile);
     STApush(gdc->other_gdcs, temp);

     STRdestroy(temp);
}


void
GDCpushBlockScript(GDCobj *gdc, char *blockname, char *scriptname)
{
     String *temp;

     temp = STRnew();
     STRset(temp, blockname);
     STApush(gdc->BlkScriptBlocks, temp);

     STRset(temp, scriptname);
     STApush(gdc->BlkScripts, temp);

     STRdestroy(temp);
}
/*
 * Search for otherdir gdcs
 */

char *
GDCfindOtherGDC(GDCobj *gdc, char *dir)
{
     int  i;

     for (i=0; i<STAgetTop(gdc->other_dirs); i++)
	  
	  if (strcmp(STAgetText(gdc->other_dirs, i), dir))
	       return(STAgetText(gdc->other_gdcs, i));
     
     return(NULL);
}


boolean
GDCevalDir(GDCobj *gdc,  char *dir)
{
     int  i, gdcdirlen;
     char *gdcdir, *conf;
     int dirlen;
     if (dir == NULL)
	  return(FALSE);

     dirlen = strlen(dir);

     for (i=0; i<STAgetTop(gdc->other_dirs); i++) {
	  gdcdir = STAgetText(gdc->other_dirs, i);
	  gdcdirlen = strlen(gdcdir);

	  if (re_comp(gdcdir))
	       return(TRUE);
	  if (re_exec(dir)) {
	       conf = STAgetText(gdc->other_gdcs, i);
	       GDCfromFile(gdc, conf);
	       Debug("Loading up aux gopherd.conf file %s", conf);
	       Debug("For item %s\n", gdcdir);
	  }
     }
     return(TRUE);
}

#ifndef NO_AUTHENTICATION
AccessResult
GDCaccess(GDCobj *gdc, char *hostname, char *ipnum, int sessions, 
	  Accesslevel testfor)
{
     AccessResult test;

     Debug("Testing %s/", hostname);
     Debug("%s for access\n", ipnum);

     if (gdc->Securityon == FALSE)
	  return(SITE_OK);

     test = SiteAccess(gdc->Sites, hostname, ipnum, testfor, sessions);

     return(test);
     ;
}


char *
GDCauthType(GDCobj *gdc, char *item)
{
     if (item == NULL)
	  return(NULL);

     return(AUTHITEMSfindType(gdc->Authitems, item));
}

char *
GDCauthExtraArgs(GDCobj *gdc, char *item)
{
     if (item == NULL)
	  return(NULL);

     return(AUTHITEMSfindExtraArgs(gdc->Authitems, item));
}

AUTHresult
GDCvalidate(GDCobj *gdc, char *type, char *username, 
	    char *password, char *hostname, char *hostip, char *extra_args)
{
     if (type == NULL)
	  return(AUTHRES_OK);	/* If no auth method then it's okay. */

     return(AUTHAvalidate(gdc->Authroutines,type, username, password,
			  hostname, hostip, extra_args));
}

/*
 * Find the ask block for an authenticated item..
 */

char **
GDCauthAsk(GDCobj *gdc, char *path)
{
     AUTH *auth;

     Debug("Looking for %s\n", path);

     auth = AUTHITEMSfindAUTH(gdc->Authitems, gdc->Authroutines, path);

     if (auth == NULL) {
	  Debug("Couldn't find %s\n", path);
	  return(NULL);
     }
     return(AUTHgetAskFcn(auth)(path, "", ""));
}

#endif

/** Is this file ignored? **/
boolean
GDCignore(GDCobj *gdc, char *fname)
{
     Extobj *ext;

     ext = EXnew();

     if (EXAsearch(gdc->Extensions, ext, fname, EXT_IGNORE)) {
	  EXdestroy(ext);
	  return(TRUE);
     } else {	  
	  EXdestroy(ext);
	  return(FALSE);
     }
}


boolean 
GDCViewExtension(GDCobj *gdc, char *fext, Extobj **extin)
{
     Extobj *ext;
     
     ext = EXnew();

     if (EXAsearch(gdc->Extensions, ext, fext, EXT_VIEW)) {
	  *extin = ext;
	  return(TRUE);
     }
     else {
	  EXdestroy(ext);
	  return(FALSE);
     }

}

boolean 
GDCBlockExtension(GDCobj *gdc, char *fext, Extobj *ext)
{

     if (EXAsearch(gdc->Extensions, ext, fext, EXT_BLOCK)) {
	  return(TRUE);
     }
     else
	  return(FALSE);
}



void
GDCaddFileSep(GDCobj *gdc, char *expr)
{
     String *temp;

     temp = STRnew();
     if (temp == NULL)
	  return;
     
     STRset(temp, expr);
     STApush(gdc->FileSeparators, temp);
     STRdestroy(temp);
}

static boolean
GDCisBlank(char *string)
{
     if (string == NULL || 
	 strncmp(string, "blank", 5) == 0)
	  return TRUE;
     else
	  return(FALSE);
}

void
GDCintegrityCheck(GDCobj *gdc)
{
     if (GDCisBlank(GDCgetSite(gdc)))
	  fprintf(stderr, "** Please set the Site: line in your configuration file!\n");

     if (GDCisBlank(GDCgetOrg(gdc)))
	  fprintf(stderr, "** Please set the Org: line in your configuration file!\n");

     if (GDCisBlank(GDCgetLoc(gdc)))
	  fprintf(stderr, "** Please set the Loc: line in your configuration file!\n");

     if (GDCisBlank(GDCgetAbstract(gdc)))
	  fprintf(stderr, "** Please set the Abstract: line in your configuration file!\n");

     if (GDCisBlank(GDCgetAdmin(gdc)))
	  fprintf(stderr, "** Please set the Admin: line in your configuration file!\n");

     if (GDCisBlank(GDCgetAdminEmail(gdc)))
	  fprintf(stderr, "** Please set the AdminEmail: line in configuration file!\n");

}


