/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/command.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992, 1993 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: command.c
 * Routines to parse commands from the client.
 *********************************************************************
 * Revision History:
 * $Log: command.c,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.37  1995/11/03  17:09:36  lindner
 * coen: add '4' and '5' to valid prefixes.
 *
 * Revision 3.36  1995/10/11  19:22:33  lindner
 * moo
 *
 * Revision 3.35  1995/10/11  19:21:24  lindner
 * moo
 *
 * Revision 3.34  1995/10/11  19:10:57  lindner
 * Fix for POST
 *
 * Revision 3.33  1995/10/11  04:58:46  lindner
 * fix for 512byte HTTP post limit.
 *
 * Revision 3.32  1995/09/25  05:02:30  lindner
 * Convert to ANSI C
 *
 * Revision 3.31  1995/08/30  19:27:23  lindner
 * Remove some tests..
 *
 * Revision 3.30  1995/08/29  07:10:12  lindner
 * Many changes to support HTTP
 *
 * Revision 3.29  1995/07/06  20:12:17  lindner
 * More mods for HTTP serving
 *
 * Revision 3.28  1995/06/09  04:48:16  lindner
 * Start incorporating Don Gilbert's POST code
 *
 * Revision 3.27  1995/05/02  04:04:24  lindner
 * Fix problem with NO_AUTH
 *
 * Revision 3.26  1995/05/01  04:00:46  lindner
 * Fixes for mindexd, etc from Steve Hsieh
 *
 * Revision 3.25  1995/04/15  07:12:35  lindner
 * add HTML processing
 *
 * Revision 3.24  1995/01/04  17:42:59  lindner
 * small renaming
 *
 * Revision 3.23  1994/12/10  08:24:56  lindner
 * Start building HTTP parser
 *
 * Revision 3.22  1994/12/03  02:14:44  lindner
 * Fix predone searches
 *
 * Revision 3.21  1994/12/02  19:51:14  lindner
 * Weird p removed and fix for prespecified searches
 *
 * Revision 3.20  1994/11/24  08:07:54  lindner
 * Fix for command routines with authentication
 *
 * Revision 3.19  1994/10/19  03:31:59  lindner
 * Another NO_AUTH code hack...
 *
 * Revision 3.18  1994/07/31  04:38:20  lindner
 * Add optional pre-specified search string..
 *
 * Revision 3.17  1994/06/29  05:29:19  lindner
 * Remove Admit1, put in cool stuff
 *
 * Revision 3.16  1994/04/25  20:49:03  lindner
 * Fix for debug code
 *
 * Revision 3.15  1994/04/08  21:08:34  lindner
 * Gcc -Wall fix
 *
 * Revision 3.14  1994/03/30  21:36:34  lindner
 * Fix for binary ask data from Don Gilbert
 *
 * Revision 3.13  1994/03/15  17:57:41  lindner
 * Fix for SCO compiler
 *
 * Revision 3.12  1994/03/08  15:55:25  lindner
 * gcc -Wall fixes
 *
 * Revision 3.11  1994/03/04  23:26:04  lindner
 * Fix for changes in strstring.h
 *
 * Revision 3.10  1994/01/20  06:37:03  lindner
 * Addition for 'h' prefix for selector string
 *
 * Revision 3.9  1993/11/02  05:53:40  lindner
 * more docs
 *
 * Revision 3.8  1993/10/04  06:49:20  lindner
 * ASK data stored in core, for a while..
 *
 * Revision 3.7  1993/09/30  16:56:53  lindner
 * Fix for WAIS and $ requests
 *
 * Revision 3.6  1993/07/29  20:13:32  lindner
 * Removed dead variables
 *
 * Revision 3.5  1993/07/27  05:27:40  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.4  1993/07/23  03:10:53  lindner
 * Added CMDgetFile() fcn
 *
 * Revision 3.3  1993/04/09  16:50:24  lindner
 * nothing
 *
 * Revision 3.2  1993/03/24  20:19:40  lindner
 * Fixed memory leak
 *
 * Revision 3.1  1993/03/19  19:56:53  lindner
 * New CMD object
 *
 *
 *********************************************************************/



#include "Malloc.h"
#include "String.h"
#include <stdio.h>
#include "util.h"

#include "command.h"
#include "openers.h"
#include "Debug.h"

static char *CMDticketfromLine(CMDobj *cmd, char *input);
static void  CMDpushAskline(CMDobj *cmd, char *line);
static void  CMDgetXtra(CMDobj *cmd, int fd, int extradata);



CMDobj *
CMDnew()
{
     CMDobj *temp;

     temp = (CMDobj *) malloc(sizeof(CMDobj));

     temp->selstr  = temp->command = temp->search = NULL;

     temp->datafromnet   = STRnew();
     temp->asklines      = STAnew(5);

     temp->view          = STRnew();
     temp->secureuser    = STRnew();
     temp->ticket        = STRnew();
     temp->httpcmd       = UnknownHTTPcmd;

     CMDsetGplus(temp, FALSE);
     CMDsetProtocol(temp, UnknownProtocol);
     return(temp);
}


void
CMDdestroy(CMDobj *cmd)
{
     STRdestroy(cmd->datafromnet);
     STAdestroy(cmd->asklines);
     STRdestroy(cmd->view);
     STRdestroy(cmd->ticket);
     STRdestroy(cmd->secureuser);

     free(cmd);
}

static char*
CMDparseHTTP(CMDobj *cmd, char *cp)
{
     int len;
     if (cp == NULL)
	  return(NULL);

     len = strlen(cp);


     if ((len >8) && (strcmp(cp + len - 9, " HTTP/1.0")==0)) {
	  *(cp + len - 9) = '\0';
	  len = strlen(cp);
	  CMDsetProtocol(cmd, HTTP1_0);
     }


     if ((len >= 5)) {
	  if (strncmp(cp, "GET /",5) == 0) {
	  
	       cp += 5;
	       if (len == 5) {
		    /** Top level HTTP transaction **/
		    cp = cp -2;
		    *cp = 'h';
	       }
	       CMDsetHTTPcmd(cmd, GET);
	       if (CMDgetProtocol(cmd) == UnknownProtocol)
		    CMDsetProtocol(cmd, HTTP0_9);
	       Fromhexstr(cp, cp);

	  } else if (strncmp(cp, "POST /", 5) == 0) {
	       cp += 5;
	       *cp = 'h';
	       CMDsetHTTPcmd(cmd, POST);
	       if (CMDgetProtocol(cmd) == UnknownProtocol)
		    CMDsetProtocol(cmd, HTTP0_9);
	       Fromhexstr(cp, cp);
	  }
     }

     return(cp);
}


void
CMDfromNet(CMDobj *cmd, int sockfd)
{
     char *cp, *selstr;
     char inputline[512];
     char *field1=NULL, *field2=NULL, *field3 = NULL;
     char *extradata = NULL;
     int length;
     
     length = readline(sockfd, inputline, sizeof(inputline));

     /** Set the alarm signal for about an hour, just in case.. **/
     (void) alarm(60 * 60);


     if (length <= 0) {
	  close(sockfd);
          /* FIXME: I (mdallen) changed this from 
           * LOGGopher("getcommand: readline error") to this.
           *
           * There doesn't seem to be a point in specifying a sockfd arg
           * except to make the compiler happy - LOGGopher doesn't use it.
           */
          /* 1 == stdout */
	  LOGGopher(1, "getcommand: readline error");
	  exit(-1);
     }

     ZapCRLF(inputline);

     Debug("Received: %s\n", inputline);
     if (strncmp("POST ",inputline,5) == 0) {
	  extradata = "4"; 
	  Debug("POST found\n",0);
     }

     CMDsetData(cmd, inputline);

     cp = CMDgetData(cmd);
     cp = CMDparseHTTP(cmd, cp);
     cp = CMDticketfromLine(cmd, cp);

     CMDsetSelstr(cmd, cp);
     selstr = cp;

     /** Find the first field, if it exists... **/
     cp = strchr(selstr, '\t');
     if (cp != NULL) {
	  *cp = '\0';
	  cp++;
	  
	  field1 = cp;
	  
	  /** find the second field, if it exists **/
	  cp = strchr(cp, '\t');
	  if (cp != NULL) {
	       *cp = '\0';
	       cp++;
	       field2 = cp;
	  } else {
	       /** find the third field, if it exists **/
	       if (cp != NULL) 
		    cp = strchr(cp, '\t');
	       if (cp != NULL) {
		    *cp = '\0';
		    cp++;
		    field3 = cp;
	       }
	  }

     }
     /** Okay, now decide which field is the search and 
       which is the command */
     
     if (*selstr == '7' || strncmp(selstr, "waissrc:",8)==0 ||
	 (field2 && strncmp(inputline, "exec+:", 6) == 0 && *field2 != '1') ||
	 strncmp(selstr, "mindex:",7) ==0 ||
	 strncmp(selstr, "halidate", 8)==0) {

	  char *questionmark;
	  /** If it's a type 7 search, search for a ? mark in the
    	   ** selector string.  (Yeah, yeah, it's weblike)
	   **/

	  questionmark = strrchr(selstr, '?');
	  if (questionmark != NULL) {
	       *questionmark = '\0';
	       CMDsetSearch(cmd, questionmark+1);
	       CMDsetCommand(cmd, field1);
	  } else {
	       CMDsetSearch(cmd, field1);
	       CMDsetCommand(cmd, field2);
	  }
     } else {
	  CMDsetCommand(cmd, field1);
	  CMDsetSearch(cmd, NULL);
     }

     /** Get the extra data (from an ask block etc.) if we've got
          an extra data flag... **/

     if (field3 != NULL)
	  extradata = field3;
     else if	 (CMDgetSearch(cmd) == NULL && field2 != NULL)
	  extradata = field2;


     if (extradata == NULL) {
	  int extraflags = 0;
	  if (CMDgetProtocol(cmd) == HTTP1_0)
	       extraflags |= 8;
	  if (CMDgetHTTPcmd(cmd) == POST)
	       extraflags |= 4;
	  CMDgetXtra(cmd, sockfd, extraflags);
     }
     else {
	  CMDgetXtra(cmd, sockfd, atoi(extradata));
     }

     /** Test to see if we've got Gplus behaviour */

     if (CMDgetCommand(cmd) != NULL && *CMDgetCommand(cmd) != '\0')
	  CMDsetGplus(cmd, TRUE);

     Debugmsg("Command:: ");
     Debug("selstr %s, ", CMDgetSelstr(cmd));
     Debug("command %s, ", CMDgetCommand(cmd));
     Debug("search %s, ", CMDgetSearch(cmd));
     Debug("user %s, ", CMDgetUser(cmd));
     Debug("ticket %s, ", CMDgetTicket(cmd));
}


static void
CMDpushAskline(CMDobj *cmd, char *line)
{
     String *temp = STRnew();

     STRset(temp, line);
     STApush(cmd->asklines, temp);

     STRdestroy(temp);
}

/* dgg need for binary ask data */
static void
CMDpushAskBinary(CMDobj *cmd, char *buf, long buflen)
{
     String *temp = STRnew();
     temp->data= buf;
     temp->len= buflen;
     STApush(cmd->asklines, temp);
     temp->data= NULL; /* caller owns buf */
     STRdestroy(temp);
}

#ifndef NO_AUTHENTICATION
static char index_64[128] = {
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
    52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1,
    -1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
    -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
};

#define char64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])

static void
CMDparseBase64userpass(CMDobj *cmd, char *str)
{
     int c1, c2, c3, c4;
     int DataDone = 0;
     char *donebuf = NULL, *donecp, *cp;

     donebuf = (char*) malloc(strlen(str)*sizeof(char) + 1 );
     donecp = donebuf;

     /* always reinitialize */

     while ((c1 = *(str++)) != '\0') {
	  if (isspace(c1)) {
	       continue;
	  }
	  if (DataDone) continue;
	  do {
	       c2 = *(str++);
	       if (c2 == 0) return;
	  } while (isspace(c2));
	  do {
	       c3 = *(str++);
	       if (c3 == 0) return;
	  } while (isspace(c3));
	  do {
	       c4 = *(str++);
	       if (c4 == 0) return;
	  } while (isspace(c4));
	  if (c1 == '=' || c2 == '=') {
	       DataDone=1;
	       continue;
	  }
	  c1 = char64(c1);
	  c2 = char64(c2);
	  *donecp++ = ((c1<<2) | ((c2&0x30)>>4));
	  
	  if (c3 == '=') {
	       DataDone = 1;
	  } else {
	       c3 = char64(c3);
	       *donecp++ = (((c2&0XF) << 4) | ((c3&0x3C) >> 2));
	       if (c4 == '=') {
		    DataDone = 1;
	       } else {
		    c4 = char64(c4);
		    *donecp++ = (((c3&0x03) <<6) | c4);
	       }
	  }
     }
     /** donebuf should look like this now:  username:pass **/

     cp = strchr(donebuf, ':');
     if (cp == NULL)
	  return;

     *cp++ = '\0';
     CMDsetUser(cmd, donebuf);
/*     CMDsetPW(cmd, cp);*/
       
       
}

#endif

/*
 * Retrieve extra data from the client request..  This stuff is optional
 *
 */

static void
CMDgetXtra(CMDobj *cmd, int fd, int extradata)
{
     char inputline[512];

     /** Siphon off data if it's there.. **/

     /** A ticket? **/
     if ((extradata & 0x2) == 0x2) {
	  ;
     }

     /** An ask block **/
     if ((extradata & 0x1) == 0x1) {

	  /** Okay, the next line is either +-1, or +bytes .. **/
	  readline(fd, inputline, sizeof(inputline));
	  ZapCRLF(inputline);
	  if (strcmp(inputline, "+-1")==0) {
	       while (readline(fd, inputline, sizeof(inputline))>0)  {
		    ZapCRLF(inputline);
		    ZapCRLF(inputline);
		    if (*inputline == '.' && *(inputline+1) == '\0')
			 break;

		    CMDpushAskline(cmd, inputline);
	       }
	  }
	  /* dgg: patch for askfile +bytecount or +-2 input forms */
	  /* dgg++ -- need this to read binary file from client */
	  else { 
	       /*inputline == "+-2" or  "+bytes..." */
	       long count, nbytes;

	       nbytes= atol(inputline+1);
	       if (nbytes == -2) do { /* read til close */
		    count= readrecvbuf(fd,inputline,sizeof(inputline));
		    if (count>0) CMDpushAskBinary(cmd, inputline, count);
	       } while (count>0);
	       else while (nbytes>0) { /* read til nbytes found */
		    count= sizeof(inputline);
		    if (count>nbytes) count= nbytes;
		    count= readrecvbuf(fd,inputline,count);
		    if (count>0) CMDpushAskBinary(cmd, inputline, count);
		    nbytes -= count;
	       }
          }
     } else if ((extradata & (4|8))) {
	  /*** HTTP headers and POST lines ***/
	  int postsize = 0;

	  Debug("HTTP and/or POST data\n",0);

	  while (readline(fd, inputline, sizeof(inputline))>0)  {
	       ZapCRLF(inputline);
	       ZapCRLF(inputline);
	       Debug("POSTed line: %s\n", inputline);
#ifndef NO_AUTHENTICATION	       
	       if (strncasecmp(inputline, "Authorization: Basic ",21) == 0) {
		    CMDparseBase64userpass(cmd, inputline+21);
	       } else if (strncasecmp(inputline, "Content-Length: ", 16) == 0) {
		    postsize = atol(inputline + 16);
	       }
#endif
	       CMDpushAskline(cmd, inputline);
	       

	       if (*inputline == '\0')  {
		    /*** end of HTTP headers ***/

		    if (postsize > 0) {
			 char *postdata;

			 postdata = (char*) malloc(sizeof(char) * (postsize + 1));

			 if (postdata != NULL) {
			      readrecvbuf(fd, postdata, postsize);
			 }
			 CMDpushAskline(cmd, postdata);
		    } 
		    break;
	       }
	  }
 	  Debug("POST data done.\n",0);
     }
 
}



static char *
CMDticketfromLine(CMDobj *cmd, char *input)
{
#ifndef NO_AUTHENTICATION
     char *cp;
     char *originput = input;
     
     if (*input != '*')
	  return(originput);

     input++;
     cp = strchr(input, ' ');
     if (cp == NULL)
	  return(originput);

     *cp = '\0';
     
     CMDsetUser(cmd,input);

     input = cp+1;
     cp = strchr(input, ' ');
     if (cp == NULL)
	  return(originput);

     *cp = '\0';
     CMDsetTicket(cmd, input);

     return(cp+1);
#else
     return(input);
#endif
}

/** Find an associated file from the selector string... Yuck! **/

char *
CMDgetFile(CMDobj *cmd)
{
     char *selstr = CMDgetSelstr(cmd);
     char *cp = NULL;


#ifndef NO_AUTHENTICATION
     if (strncmp(selstr, "validate ", 9) == 0)
	  selstr += 9;

#endif

     switch (*selstr) {
     case '0':
     case '1':
     case '4':
     case '5':
     case '7':
     case '9':
     case 'h':
     case 's':
     case 'I':
	  return(selstr+1);

     case 'm':
#ifndef NO_MINDEXD
	  if (strncmp(selstr,"mindex:",7)==0)
	       return(selstr+7);
	  else
#endif
	       return(selstr+1);

     case 'R':
	  cp = strchr(selstr, '-');
	  if (cp == NULL)  break;

	  cp++;
	  cp = strchr(cp, '-');
	  if (cp == NULL) break;
	  
	  return(cp+1);


	  /*** Exec: ***/
     case 'e':
	  cp = strrchr(selstr, ':');
	  if (cp!=NULL)
               return(cp+1);
	  else
               return(NULL);
	  break;


	  /** WAIS docid **/
#ifdef WAISSEARCH
     case 'w':
	  if (strncmp(selstr, "waissrc:",8)==0)
	       return(strchr(selstr, ':')+1);
#endif

     default:
	  if (*selstr == '\0') {
	       return("//");
	  } else {
	       return(NULL);
	  }
     }

     return(NULL);
     ;
}
