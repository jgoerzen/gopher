/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.4 $
 * $Date: 2001/01/17 19:30:25 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/ftp.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: ftp.c
 * Routines to translate gopher protocol to ftp protocol.
 *********************************************************************
 * Revision History:
 * $Log: ftp.c,v $
 * Revision 1.4  2001/01/17 19:30:25  jgoerzen
 * Change many sprintf -> snprintf
 *
 * Revision 1.3  2000/12/21 05:33:20  s2mdalle
 *
 *
 * Miscellaneous code cleanups
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.52  1995/11/03  18:13:58  lindner
 * Coen: ftp gw fixes..
 *
 * Revision 3.51  1995/09/28  21:11:29  lindner
 * More fixes...
 *
 * Revision 3.50  1995/09/26  05:00:46  lindner
 * Minor fix..
 *
 * Revision 3.49  1995/09/26  04:59:44  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.48  1995/09/25  22:06:56  lindner
 * Ansification bugs
 *
 * Revision 3.47  1995/09/25  05:02:33  lindner
 * Convert to ANSI C
 *
 * Revision 3.46  1995/09/09  03:20:06  lindner
 * FTP username and password support
 *
 * Revision 3.45  1995/08/29  07:10:49  lindner
 * URL code changes..
 *
 * Revision 3.44  1995/07/29  04:39:03  lindner
 * Add URL ftp support for the server
 *
 * Revision 3.43  1995/02/20  23:36:50  lindner
 * Fix for searches
 *
 * Revision 3.42  1995/02/06  21:24:42  lindner
 * Fix for pedantic compilers
 *
 * Revision 3.41  1994/11/18  21:41:14  lindner
 * Remove unused variable, memory leak
 *
 * Revision 3.40  1994/11/16  18:53:16  lindner
 * Eliminate extra hostname lookup in ftp-gw
 *
 * Revision 3.39  1994/10/13  05:17:47  lindner
 * Compiler complaint fixes
 *
 * Revision 3.38  1994/08/01  21:59:37  lindner
 * Less strict OS/2 ftp gw
 *
 * Revision 3.37  1994/07/31  04:56:49  lindner
 * Mondo new stuff for ftp gateway
 *
 * Revision 3.36  1994/07/21  15:45:06  lindner
 * Gopher+ FTP gateway (modified from patch from Brian Coan)
 *
 * Revision 3.35  1994/05/24  06:52:30  lindner
 * Fix for spinning ftp processes from R.S. Jones
 *
 * Revision 3.34  1994/05/18  04:28:21  lindner
 * Fixes for ftp from Jonzy
 *
 * Revision 3.33  1994/04/25  20:49:05  lindner
 * Fix for debug code
 *
 * Revision 3.32  1994/04/07  17:27:59  lindner
 * Recognize more Unix ftp sites as Unix
 *
 * Revision 3.31  1994/03/31  21:12:34  lindner
 * FTP jumbo patch, fix for skipping permissions bits with large file sizes, Prettier continuation lines 
 *
 * Revision 3.30  1994/03/17  21:58:16  lindner
 * Fix for bad ftp servers
 *
 * Revision 3.29  1994/03/17  21:13:53  lindner
 * Fix for pid load limiting
 *
 * Revision 3.28  1994/03/08  20:03:44  lindner
 * Fix for aix function strangeness
 *
 * Revision 3.27  1994/03/08  15:55:28  lindner
 * gcc -Wall fixes
 *
 * Revision 3.26  1994/03/08  03:56:33  lindner
 * Fix for return types
 *
 * Revision 3.25  1994/02/20  16:51:24  lindner
 * Add capability to compile out ftp routines
 *
 * Revision 3.24  1994/01/25  05:31:06  lindner
 * Skip . and .. file in ftp listing..
 *
 * Revision 3.23  1994/01/20  06:40:38  lindner
 * Fix for incorrect number of parameters to FTPerrorMessage
 *
 * Revision 3.22  1993/12/30  04:48:45  lindner
 * mods for Plan 9 ftp servers
 *
 * Revision 3.21  1993/12/30  04:07:14  lindner
 * Additional fix for VM servers
 *
 * Revision 3.20  1993/12/16  11:34:39  lindner
 * Fixes to work with VM ftp servers
 *
 * Revision 3.19  1993/12/09  20:46:53  lindner
 * More robust FTP Implementation
 *
 * Revision 3.18  1993/11/29  01:04:01  lindner
 * Fix for vms version numbers, don't strip any numbers..
 *
 * Revision 3.17  1993/09/27  20:05:13  lindner
 * Fix for broken Unix list
 *
 * Revision 3.16  1993/09/20  16:52:16  lindner
 * Fix for big ftp directories with lots of links
 *
 * Revision 3.15  1993/09/18  03:25:18  lindner
 * Fix for Mac FTPd and ftp gateway
 *
 * Revision 3.14  1993/09/18  02:21:58  lindner
 * Fix for Novell ftp servers
 *
 * Revision 3.13  1993/09/17  14:47:42  lindner
 * Totally new ftp code, eliminates tmp files
 *
 * Revision 3.12  1993/08/23  21:43:21  lindner
 * Fix for bug with symlinks
 *
 * Revision 3.11  1993/08/23  20:51:48  lindner
 * Multiple fixes from Matti Aarnio
 *
 * Revision 3.10  1993/08/11  14:41:48  lindner
 * Fix for error logging and uninitialized gs
 *
 * Revision 3.9  1993/08/11  14:08:39  lindner
 * Fix for hanging ftp gateway connections
 *
 * Revision 3.8  1993/08/03  20:43:54  lindner
 * Fix for sites that have @ -> for symbolic links
 *
 * Revision 3.7  1993/08/03  06:40:11  lindner
 * none
 *
 * Revision 3.6  1993/08/03  06:14:12  lindner
 * Fix for extra slashes
 *
 * Revision 3.5  1993/07/30  19:21:03  lindner
 * Removed debug stuff, fix for extra slashes
 *
 * Revision 3.4  1993/07/30  18:38:59  lindner
 * Move 3.3.1 to main trunk
 *
 * Revision 3.3.1.7  1993/07/29  21:42:21  lindner
 * Fixes for Symbolic links, plus removed excess variables
 *
 * Revision 3.3.1.6  1993/07/27  05:27:42  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.3.1.5  1993/07/26  17:18:55  lindner
 * Removed extraneous abort printf
 *
 * Revision 3.3.1.4  1993/07/26  15:34:21  lindner
 * Use tmpnam() and bzero(), plus ugly fixes..
 *
 * Revision 3.3.1.3  1993/07/23  03:12:29  lindner
 * Small fix for getreply, reformatting..
 *
 * Revision 3.3.1.2  1993/07/07  19:39:48  lindner
 * Much prettification, unproto-ed, and use Sockets.c routines
 *
 * Revision 3.3.1.1  1993/06/22  20:53:21  lindner
 * Bob's ftp stuff
 *
 * Revision 3.3  1993/04/15  04:48:09  lindner
 * Debug code from Mitra
 *
 * Revision 3.2  1993/03/24  20:15:06  lindner
 * FTP gateway now gets filetypes from gopherd.conf
 *
 * Revision 3.1.1.1  1993/02/11  18:02:51  lindner
 * Gopher+1.2beta release
 *
 *********************************************************************/

/* -------------------------------------------------
 *     g2fd.c          Gopher to FTP gateway daemon.
 *     Version 0.3 Hacked up: April 1992.  Farhad Anklesaria.
 *     Based on a Perl story by John Ladwig.
 *     Based on a Perl story by Farhad Anklesaria.
 *
 *      Modified by Greg Smith, Bucknell University, 24 Nov 1992
 *      to handle multiline status replies.
 *
 ---------------------------------------------------- */

#ifndef NO_FTP

#include "gopherd.h"
#include "ftp.h"
#include "url.h"
#include <signal.h>

#include <stdio.h>
#include "ext.h"
#include "Malloc.h"
#include "Sockets.h"
#include "command.h"

#include "Debug.h"

/** Global socket value **/
static int Gsockfd = -1;  

/** Global Gplus value **/
static int GFTPplus;  

/** track whether we sent a Gplus header already **/
static int SentGPHeader;

static boolean GETDIR = FALSE;

/*** Some forward declarations ***/
boolean NotText(char *);

boolean FTPconnect(FTP *ftp, char *host);

FTP *
FTPnew(char *host)
{
     FTP *temp;

     if (host == NULL || *host == '\0') {
	  Debugmsg("FTPnew, bad param..\n");
	  return(NULL);
     }

     temp = (FTP*)malloc(sizeof(FTP));

     temp->control_sock = -1;
     temp->data_sock    = -1;
     temp->mode         = 'A';
     temp->Ftptype      = FTP_UNKNOWN;

     if (FTPconnect(temp, host))
	  return(temp);
     else {
	  Debug("FTPnew: FTPconnect failed to %s\n",host);
	  free(temp);
	  return(NULL);
     }
}


/*
 * Close connection, destroy data structures..
 */

void
FTPdestroy(FTP *ftp)
{
     FTPcloseData(ftp);

     if (FTPgetControl(ftp) != -1) {
	  FTPbyebye(ftp);
	  close(FTPgetControl(ftp));
     }

     free(ftp);
}


/* 
  Establish connection, validate DNS name,
  connect and return control pointer
  Error returns ErrSocket as defined in Socket.h
*/

boolean
FTPconnect(FTP *ftp, char *host)
{
     int newcontrol;
     char message[256];
     void (*fptr_FTPcleanup)() = FTPcleanup;

     /** Get ready for some cleanup action **/
     signal(SIGPIPE, fptr_FTPcleanup);
     signal(SIGINT, fptr_FTPcleanup);
     signal(SIGALRM, fptr_FTPcleanup);
     
     newcontrol = SOCKconnect(host,21);

     if (newcontrol < 0) {
		 if (GFTPplus && (SentGPHeader == FALSE)) {
			 GSsendHeader(Gsockfd, -1);
			 SentGPHeader = TRUE;
		 }
		 snprintf(message, sizeof(message), 
			  "failed to connect to %s %s\n",
			  host, sys_err_str());
		 FTPerrorMessage(ftp, message);
		 return(FALSE);
	 }

     FTPsetControl(ftp,newcontrol);

     if (FTPgetReply(ftp, 299, message, sizeof(message)) <0)
	  return(FALSE);

     if (strcasestr(message, "NetWare"))
	  FTPsetType(ftp, FTP_NOVELL);
     else if (strcasestr(message, "SunOS"))
	  FTPsetType(ftp, FTP_UNIX);
     else if (strcasestr(message, "ultrix"))
	  FTPsetType(ftp, FTP_UNIX);
     else if (strcasestr(message, "MultiNet FTP"))
	  FTPsetType(ftp, FTP_VMS);
     else if (strcasestr(message, "Windows NT FTP"))
	  FTPsetType(ftp, FTP_WINNT);
     else if (strcasestr(message, "Peter's Macintosh FTP daemon"))
	  FTPsetType(ftp, FTP_MACOS);
     else if (strcasestr(message, "unix"))
	  FTPsetType(ftp, FTP_UNIX);
     else if (strcasestr(message, "Version 4.129"))
 	  FTPsetType(ftp, FTP_UNIX);
     else if (strcasestr(message, "DG/UX"))
 	  FTPsetType(ftp, FTP_UNIX_L8);
     else if (strcasestr(message, "Version 4.105"))
 	  FTPsetType(ftp, FTP_MICRO_VAX);
     else if (strcasestr(message, "OS/2"))
	  FTPsetType(ftp, FTP_OS2);


     FTPfindType(ftp);

     return(TRUE);
}


char *
FTPpwd(FTP *ftp)
{
     static char pwd[256];
     char message[256];
     int ftpnum;

     FTPsend(ftp, "PWD");
     ftpnum = FTPgetReply(ftp, 999, message, sizeof(message));
     
     if (ftpnum != 257)
	  return(NULL);
     
     sscanf(message,"%*[^\"]%*c%[^\"]%*s",pwd);
     
     return(pwd);
}


void
FTPfindType(FTP *ftp)
{
     int ftpnum;
     char message[256];

     if (FTPgetType(ftp) != FTP_UNKNOWN)
	  return;

     FTPsend(ftp, "SYST");
     ftpnum = FTPgetReply(ftp, 999, message, sizeof(message));
     
     if (ftpnum == 215) {
	  Debugmsg("Analyzing ftp syst output\n");
	  if (strncmp(message+4, "MTS", 3) == 0 ||
	      strncmp(message+4, "VMS", 3) == 0) {
	       FTPsetType(ftp, FTP_VMS);
	  }
	  else if (strncmp(message+4, "UNIX Type: L8", 13)==0)
	       FTPsetType(ftp, FTP_UNIX_L8);
	  else if (strncmp(message+4, "UNIX", 4)==0 ||
		   strncmp(message+4, "Plan 9", 6)==0)
	       FTPsetType(ftp, FTP_UNIX);
	  else if (strncmp(message+4, "MACOS", 5)==0)
	       FTPsetType(ftp, FTP_MACOS);
	  else if (strncmp(message+4, "MAC OS", 5)==0)
	       FTPsetType(ftp, FTP_MACOS);
	  else if (strncmp(message+4, "AIX-PS/2", 8)==0)
	       FTPsetType(ftp, FTP_UNIX_L8);
	  else if (strncmp(message+4, "VM", 2)==0)
	       FTPsetType(ftp, FTP_VM);
     }
}

/* Send a string across the control channel 
   Returns true if an error, false otherwise
*/
boolean
FTPsend(FTP  *ftp, char *command)
{
     Debug("--> %s\n", command);

     if (writestring(FTPgetControl(ftp), command) <0)
	  return(TRUE);

     if (writestring(FTPgetControl(ftp), "\r\n") <0)
	  return(TRUE);

     return(FALSE);
}


/* Find the numeric value of the message */

int
FTPfindNum(FTP *ftp, char *message)
{
     int ftpnum;

     ftpnum = (int) strtol(message, (char **)NULL, 10);
     return(ftpnum);
}


boolean
FTPisContinuation(FTP *ftp, char *message)
{
	if (message[0] == '\0')
		return TRUE;
	if (isdigit(message[0]) && isdigit(message[1]) &&
		isdigit(message[2]) && message[3] == '-')
		return TRUE;
	return (isspace(message[0]));
}


/*
 * Get a reply from the FTP control channel.
 */

int
FTPgetReply(FTP  *ftp, int  errorlevel, char *message, int  maxlen)
{
     int ftpnum;

     ftpnum = FTPgetReplyline(ftp, message, maxlen);

     if (ftpnum > errorlevel) {
	  /*** Had an error ***/
	  if (GFTPplus && (SentGPHeader == FALSE)) {
		  GSsendHeader(Gsockfd, -1);
		  SentGPHeader = TRUE;
	  }
	  LOGGopher(-1, message);
	  if (FTPerrorMessage(ftp, message) >= 0)
	   while (FTPisContinuation(ftp, message)) {
		 if ((ftpnum = FTPgetReplyline(ftp, message, maxlen)) < 0)
		  break;
		 if (FTPerrorMessage(ftp, message) < 0)
		  break;
	   }
	  FTPabort(ftp);
	  gopherd_exit(-1);
     } else if (ftpnum <0) {
	  /** Really bad error **/
	  FTPabort(ftp);
	  gopherd_exit(-1);
     }

     if (FTPisContinuation(ftp, message)) {
	  if (GFTPplus && (SentGPHeader == FALSE)) {
		  GSsendHeader(Gsockfd, -1);
		  SentGPHeader = TRUE;
	  }
	  do {
		  if (FTPinfoMessage(ftp, message) < 0)
			return(-1);
		  if ((ftpnum = FTPgetReplyline(ftp, message, maxlen)) < 0)
			break;
      } while (FTPisContinuation(ftp, message));
     }
     return(ftpnum);
}

/*
 * Gets a reply on the control socket.. fills message with what it
 * got back and returns the integer value of the return code.
 */

int
FTPgetReplyline(FTP  *ftp, char *message, int  maxlen)
{
     int ftpnum, i;

     for (i=0; i<maxlen; i++)
	  message[i] = '\n';

     i = readline(FTPgetControl(ftp), message, maxlen);
     
     Debug("<-- %s\n",message);

     if (i <= 0)
	  return(-1);
     
     ftpnum = FTPfindNum(ftp, message);
     
     return(ftpnum);
}


/*
 * Execute a specific ftp command (an sprintf style thing)
 *    replace %s with parameter
 *    fail when above errorlevel
 */

int
FTPcommand(FTP  *ftp, char *command, char *parameter, int  errorlevel)
{
     char commandline[512];
     char message[256];
     
     snprintf(commandline, sizeof(commandline), command, parameter);
     if (FTPsend(ftp, commandline))
	  return(-1); /** Error condition **/

     return(FTPgetReply(ftp, errorlevel, message, sizeof(message)));
}
    

/*
 * Send USER and PASS
 */

int
FTPlogin(FTP  *ftp, char *username, char *password)
{
     int result;

     /*** Send username ***/
     result = FTPcommand(ftp, "USER %s", username, 599);

     if (result >399 || result < 0) {
	  FTPabort(ftp);
	  return (-1);
     }
     if (result >300)
	  FTPcommand(ftp, "PASS %s", password, 399);
     return (0);
}



void
FTPbyebye(FTP *ftp)
{
     FTPcommand(ftp, "QUIT", NULL, 399);
}


/** Open a data connection **/
void
FTPopenData(FTP *ftp, char *command, char *file, int errorlevel)
{
     struct sockaddr_in we;
     char   theline[512];
     int ftp_dataport, ftp_data;

     if ((ftp_dataport = SOCKlisten(&we)) < 0)
	  return;
     
     snprintf(theline, sizeof(theline),
	      "PORT %d,%d,%d,%d,%d,%d",
	     (htonl(we.sin_addr.s_addr) >> 24) & 0xFF,
	     (htonl(we.sin_addr.s_addr) >> 16) & 0xFF,
	     (htonl(we.sin_addr.s_addr) >>  8) & 0xFF,
	     (htonl(we.sin_addr.s_addr)      ) & 0xFF,
	     (htons(we.sin_port)        >>  8) & 0xFF,
	     (htons(we.sin_port)             ) & 0xFF);
     
     FTPsend(ftp, theline);
     FTPgetReply(ftp, 201, theline, sizeof(theline));
     

     FTPcommand(ftp, command, file, errorlevel);

     ftp_data = SOCKaccept(ftp_dataport, we);

     if (ftp_data < -1)
	  return;

     FTPsetData(ftp, ftp_data);

}


void
FTPcloseData(FTP *ftp)
{
     if (FTPgetData(ftp) != -1)
	  close(FTPgetData(ftp));
}

int
FTPread(FTP *ftp, char *buf, int bufsize)
{
     int len;

     len = read(FTPgetData(ftp), buf, bufsize);

     return(len);
}


/*--------------------------------*/
/* Used in xLateText below --------*/
boolean
NotText(char *buf)
{
     int max;   char *c;
     
     if ((max = strlen(buf)) >= (BUFSIZ - 50)) max = BUFSIZ - 50;
     for (c = buf; c < (buf + max); c++) {
	  if (*c > '~') return(TRUE);
     }
     return(FALSE);
}


static char *textnames[] =
{ "README", "READ.ME", "MIRROR.LOG", "WELCOME", "INDEX", ".TXT", 0, };

/*--------------------------------*/
/* return false if extension indicates text, true if binary */

boolean
IsBinaryType(char *path)
{
     char Gtype;
     Extobj *ext;
     char *slashp, **textn, *cp;
     
     ext = EXnew();


     if (GDCViewExtension(Config, path, &ext)) {
	  Gtype = EXgetObjtype(ext);
	  
	  switch (Gtype) {

	  case A_FILE:
	  case A_MACHEX:
	  case A_CSO:
	  case A_INDEX:
	  case A_TELNET:
	  case A_TN3270:
	       EXdestroy(ext);
	       return FALSE;

	  }
     }
     if (EXAcasedSearch(Config->Extensions, ext, path, EXT_DECODER)) {
	  EXdestroy(ext);
	  return TRUE;
     }

     if ((slashp = strrchr(path, '/')) == NULL)
	  slashp = path;
     else
	  return(TRUE);

     for (textn = textnames; *textn; textn++)
	  if (strcasestr(slashp, *textn))
	       return FALSE;

     for (cp = slashp; *cp && isdigit(*cp); cp++)
	  ;
     if (cp > slashp && *cp)
	  for (textn = textnames; *textn; textn++)
	       if (strcasecmp(slashp, *textn)==0)
		    return FALSE;
     return(TRUE);
}


/*--------------------------------*/
void
TrimEnd(char *bufptr)
{
     int last;
     for (last = strlen(bufptr) - 1; isspace(bufptr[last]) ; bufptr[last--] = '\0')
	  ;
}



int
Vaxinate(char *bufptr)
{
     int last;
     char *cp;
     
     last = strlen(bufptr) - 1;

     /* strip off VMS version numbers */
     if (isdigit(bufptr[last]) && (cp=strrchr(bufptr, ';')) != NULL) {
          *cp = '\0';
	  last = strlen(bufptr) - 1;
     }
     
     /* if bufptr ends in ".dir", it's a directory, replace ".dir" with "/" */
     if((last > 3) && ((strncmp(bufptr + last - 3, ".dir", 4) == 0) ||
		       (strncmp(bufptr + last - 3, ".DIR", 4) == 0)))
     {
	  last -= 3;
	  bufptr[last] = '/';
	  bufptr[last+1]  = '\0';
	  return(last);
     }

     /* for files, uppercase terminal .z's or _z's */
     if ((int)strlen(bufptr) > 1) {
          if (bufptr[last] == 'z' &&
	     (bufptr[last-1] == '.' || bufptr[last-1] == '_'))
               bufptr[last] = 'Z';
     }

     return(last);
}

void
FTPchdir(FTP *ftp, char *path)
{
     int result;
     char *ptr;
     
     if ((result = FTPcommand(ftp, "CWD %s", path, 599)) < 300)
	  return;
     switch (result) {
     case 450:
     case 550:
     case 553:
	  if (strcmp(path, "/") && (ptr = strrchr(path, '/'))) {
	       if (ptr == path)
		    ptr++;
	       *ptr = '\0';
	       FTPchdir(ftp, path);
	       return;
	  }
	  /* else fall through */
     default:
	  FTPabort(ftp);
	  gopherd_exit(-1);
	  break;
     }
     return;
}


char *
FTPwalkchdir(FTP  *ftp, char *path)
{
     char *beg, *end, *cp;
     char vmspath[256], tmppath[256];
     int notascii;
     
     /* path looks like '/dir/dir.../dir/' now. Because Wollongong
      * wants "CWD dir/dir/dir" and Multinet wants "CWD dir.dir.dir"
      * so we do the CWD in a stepwise fashion. Oh well...
      */
     
     beg = path+1;
     for (end = beg, notascii = 0; (*end != '\0') && (*end != '/'); ++end)
	  if (!isascii(*end) || *end == '?' || *end == '*')
	       notascii++;
     
     while (*end != '\0')
     {
	  bzero(vmspath, 256);
	  strncpy(vmspath, beg, end-beg);
	  
	  if (notascii)
	       for (cp = vmspath; *cp; cp++)
		    if (!isascii(*cp) || *cp == ' ')
			 *cp = '?';

 	  if (!notascii && strchr(vmspath, ' ')) {
	       snprintf(tmppath, sizeof(tmppath),
			"\"%s\"", vmspath);
	       FTPchdir(ftp, tmppath);
 	  } else
	       FTPchdir(ftp, vmspath);
	  
	  beg=end+1; /* Skip slash */

 	  for (end = beg, notascii = 0; (*end != '\0') && (*end != '/'); ++end)
	       if (!isascii(*end) || *end == '?' || *end == '*')
		    notascii++;
     }
     
     return(beg);
}


/*--------------------------------*/

void
FTPcleanup()
{
     ;
}

int
FTPerrorMessage(FTP *ftp, char *message)
{
     char errmsg[256];
	 char *cp;
	 int writerr = 0;
     
     if (message == NULL || Gsockfd < 0)
	  return(0);

     for (cp = message; *cp; cp++)
	  if (*cp == '\t')
	       *cp = ' ';

     ZapCRLF(message);

     snprintf(errmsg, sizeof(errmsg), "3%s\t\terror.host\t1\r\n", message);
     if (GFTPplus)
	  writerr = writestring(Gsockfd, "+INFO: ");
     if (writerr || writestring(Gsockfd, errmsg)) {
	  Debug("FTPerrorMessage gave errno %d", errno);
	  Gsockfd = -1;
	  return(-1);
     }
     return(0);
}

int
FTPinfoMessage(FTP *ftp, char *message)
{
     char errmsg[512];
     char *cp;
     int writerr = 0;

     if (message == NULL || Gsockfd < 0)
	  return(0);

     for (cp = message; *cp; cp++)
	  if (*cp == '\t')
	       *cp = ' ';
     
     ZapCRLF(message);

     if (GETDIR) {
	  if ((int)strlen(message) > 3 && message[3] == '-')
	       snprintf(errmsg, sizeof(errmsg),
			"i%s\t\terror.host\t1\r\n", &message[4]);
	  else
	       snprintf(errmsg, sizeof(errmsg),
			"i%s\t\terror.host\t1\r\n", message);
	  
	  if (GFTPplus)
		 writerr = writestring(Gsockfd, "+INFO: ");
	  if (writerr || writestring(Gsockfd, errmsg)) {
		  Debug("FTPinfoMessage gave errno %d", errno);
		  Gsockfd = -1;
		  return(-1);
	  }
	  return(0);
     }
     return(0);

}

void
FTPabort(FTP *ftp)
{
     writestring(Gsockfd, ".\r\n");
     FTPdestroy(ftp);
}


Url *ftpurl;

/*
 * Implement ftp--> gopher gateway
 */

int
GopherFTPgw(int sockfd, char *ftpstr, CMDobj *cmd)
{
     FTP    *ftp;
     char   *cp;
     char   ftp_info_fname[256];
     char   *ftphost, *ftpuser=NULL, ftppass[256], *ftppath;
     char buf[8192];
     int blen;
     char lastchar;
     static GopherObj   *Gopherstow = NULL;
     GopherObj          *gs;
     GopherDirObj       *gd;

     Gsockfd = sockfd;
     
     /* Find @ and parse out */
     if (strncmp(ftpstr, "//", 2) == 0) {
	  char *mystr;
	  /** Must be a url stylee string... **/
	  mystr  = (char*)malloc(strlen(ftpstr) + 5);
	  strcpy(mystr, "ftp:");
	  strcat(mystr, ftpstr);
	  ftpurl = URLnew();
	  URLset(ftpurl, mystr);

	  if (URLgetUser(ftpurl) != NULL)
	       ftpuser = strdup(URLgetUser(ftpurl));
	  if (URLgetPass(ftpurl) != NULL)
	       strcpy(ftppass, URLgetPass(ftpurl));
	  ftppath = strdup(URLgetPath(ftpurl));
	  ftphost = strdup(URLgetHost(ftpurl));

	  if (strlen(ftppath) == 0) {
	       ftppath[0] = '/';
	       ftppath[1] = '\0';
	  }
	  free(mystr);
	  
     } else {
	  cp = strchr(ftpstr, '@');
	  if (!cp)
	       return(-1);

	  *cp = '\0';
	  
	  ftppath = cp+1;
	  ftphost = ftpstr;
     }

     if (!ftpuser)
	  ftpuser = "anonymous";

     Gsockfd = sockfd;
/*     GFTPplus = CMDisGplus(cmd);*/
     GFTPplus = FALSE;
     SentGPHeader = FALSE;

     if (*ftppath == '/') 
	  ftppath++;
 
     if (*ftppath == '\0') {
	  *ftppath = '/';
 	  *(ftppath+1) = '\0';
     }
 


     if (*ftppass == '\0') {
	  snprintf(ftppass, sizeof(ftppass), 
		   "-gopher@%s", Zehostname);
     }
     if (Gopherstow == NULL)
	  Gopherstow = GSnew();

     gs = Gopherstow;
     GSsetGplus(gs, TRUE);

     if (GFTPplus && GSgplusInited(gs) == FALSE)
	  GSplusnew(gs);

     if (GFTPplus && *CMDgetCommand(cmd) == '!') {
	  char *lastslash;
	  char tmpstr[256];
	  
	  lastslash = strrchr(ftppath, '/');
	  if (lastslash == NULL)
	       Die(sockfd, 500, "Can't parse %s", ftppath);

	  GSsetHost(gs, Zehostname);
	  GSsetPort(gs, GopherPort);
	  if (strcmp(ftpuser, "anonymous") != 0) {
 	       snprintf(tmpstr, sizeof(tmpstr),
			"%s%s", URLget(ftpurl), ftppath);
	  } else {
	       snprintf(tmpstr, sizeof(tmpstr),
			"ftp:%s@%s", ftphost, ftppath);
	  }
	  GSsetPath(gs, tmpstr);
	  GSsetTitle(gs, tmpstr);

	  if (*(lastslash+1) == '\0') {
	       /** Item is a directory **/
	       GSsetType(gs, '1');
	       AddDefaultView(gs, 1, NULL);

	       GSsendHeader(sockfd, -1);

	       GSplustoNet(gs, sockfd, NULL, "");
	       return(0);
	  } else {
	       *lastslash = '/';
	       *(lastslash +1) = '\0';
	       strcpy(ftp_info_fname, tmpstr);
	  }
     }	       
     gd = GDnew(32);

     /** find the last character **/
     lastchar = ftppath[strlen(ftppath)-1];
     if (lastchar == '/')
	  GETDIR = TRUE;
     else
	  GETDIR = FALSE;

     /***  here we go...!***/
     ftp = FTPnew(ftphost);
     if (ftp == NULL || ftp->control_sock < 0)
	  return(-1);

     if (FTPlogin(ftp, ftpuser, ftppass) < 0)
	  return(-1);

     if (lastchar == '/') {
	  char thename[256];

	  /*** Do the directory ***/
	  ftppath[strlen(ftppath)-1] = '\0';

	  if ((int)strlen(ftppath) > 1) {	  
	       if (FTPgetType(ftp) == FTP_VMS 
		   || FTPgetType(ftp) == FTP_NOVELL
		   || FTPgetType(ftp) == FTP_MACOS
		   || FTPgetType(ftp) == FTP_OS2
		   || FTPgetType(ftp) == FTP_VM) {
		    int len = strlen(ftppath);
		    
		    ftppath[len] =  '/'; /*** Ick!***/
		    ftppath[len+1] = '\0';
		    
		    FTPwalkchdir(ftp, ftppath);
		    ftppath[len] = '\0';
	       }
 	       else {
		    for (cp = ftppath; *cp; cp++)
			 if (!isascii(*cp))
			      *cp = '?';
		    if (strchr(ftppath, ' ')) {
			 snprintf (ftppass, sizeof(ftppass), 
				   "\"%s\"", ftppath);
			 FTPchdir(ftp, ftppass);
		    } else
			 FTPchdir(ftp, ftppath);
	       }
	       
	  }

	  switch(FTPgetType(ftp)) {

	  case FTP_UNKNOWN:
	  case FTP_UNIX:
	  case FTP_WINNT:
	       FTPopenData(ftp, "LIST -LF", NULL, 299);
	       break;


	  case FTP_UNIX_L8:
	       FTPopenData(ftp, "LIST -F", NULL, 299);
	       break;

	  case FTP_NOVELL:
	  case FTP_MICRO_VAX:
	  case FTP_VMS:
	  case FTP_VM:
	  case FTP_MACOS:
	  case FTP_OS2:
	       FTPopenData(ftp, "LIST", NULL, 299);
	       break;


	  default:
	       FTPopenData(ftp, "NLST", NULL, 299);
	       break;
	  }

	  if (GFTPplus && (SentGPHeader == FALSE)) {
		  GSsendHeader(Gsockfd, -1);
		  SentGPHeader = TRUE;
	  }
	  while (readline(FTPgetData(ftp), buf, sizeof(buf)) > 0) {

	       ZapCRLF(buf);
	       GSinit(gs);
	       GSsetGplus(gs, FALSE);
	       GSsetHost(gs, ftphost);
	       GSsetPath(gs, ftppath);
	       GSsetPort(gs, GopherPort);
	       GSsetTTL(gs, GDCgetCachetime(Config));
	       if (GFTPplus)
		    GSsetGplus(gs, TRUE);
	       if (GopherList(ftp, buf, thename, gs, ftpuser, ftppass) == -1)
		    continue;
	       GSsetHost(gs, Zehostname);
	       GSgetURL(gs,"");
	       GDaddGS(gd, gs);
	  }

	  if (GFTPplus) {
	       if (*CMDgetCommand(cmd) == '!') {
		    int fnum = GDSearch(gd, ftp_info_fname);
		    if (fnum >= 0)
			 GSplustoNet(GDgetEntry(gd, fnum), sockfd, NULL, "");
	       } else
	       if (strncasecmp(CMDgetView(cmd), "text/html", 9) == 0) {
		    GDtoNet(gd, sockfd, GSFORM_HTML, "", NULL);
	       }
	       else
		    GDplustoNet(gd, sockfd, NULL, "");
	  } else
	       GDtoNet(gd, sockfd, GSFORM_G0, "", NULL);
	  writestring(sockfd, ".\r\n");
     }
     else {
	  char *fname;
	  if (FTPgetType(ftp) == FTP_VMS 
	      || FTPgetType(ftp) == FTP_NOVELL
	      || FTPgetType(ftp) == FTP_MACOS
	      || FTPgetType(ftp) == FTP_OS2
	      || FTPgetType(ftp) == FTP_VM)
	       fname = FTPwalkchdir(ftp, ftppath);
	  else {
	       for (cp = ftppath; *cp; cp++)
		    if (!isascii(*cp))
			 *cp = '?';
	       if (strchr(ftppath, ' ')) {
		    snprintf (ftppass, sizeof(ftppass), "\"%s\"", ftppath);
		    fname = ftppass;
	       } else
		    fname = ftppath;
 	  }
	  
	  if (IsBinaryType(ftppath)) {
	       Debug("Getting binary %s\n", ftppath);

	       FTPbinary(ftp);
	       FTPopenData(ftp, "RETR %s", fname, 299);

	       if (GFTPplus && (SentGPHeader == FALSE)) {
		    GSsendHeader(Gsockfd, -2);
		    SentGPHeader = TRUE;
	       }
	       while ((blen = FTPread(ftp, buf, sizeof(buf)))>0)
		      writen(sockfd, buf, blen);
	  }
	  else {
	       Debug("Getting ascii %s\n", ftppath);

	       FTPascii(ftp);
	       FTPopenData(ftp, "RETR %s", fname, 199);

	       if (GFTPplus && (SentGPHeader == FALSE)) {
		    GSsendHeader(Gsockfd, -1);
		    SentGPHeader = TRUE;
	       }
	       blen = FTPread(ftp, buf, sizeof(buf));
	       while (blen > 0) {
		    writen(sockfd, buf, blen);
		    blen = FTPread(ftp, buf, sizeof(buf));
	       }
	       writestring(sockfd, ".\r\n");
	  }

     }
     FTPcloseData(ftp);
     FTPdestroy(ftp);

     return(0);
}


/*************************************************************
 Takes the LIST output and 
 truncates it to just the name
 Handles special cases for different
 formats of LIST output
******************************/

int
GopherList(FTP *ftp, char *bufptr, char *theName, GopherObj *gs,
	   char *ftpuser, char *ftppass)
{
     char Gzerotype;
     static char *IntName = NULL;
     
     if (IntName == NULL)
	  IntName = (char *)malloc(BUFSIZ);
     
     *IntName = '\0';

     /* Skip 'total' line */
     if (strncmp(bufptr, "total", 5) == 0)
	  return (-1); 
     if (strncmp(bufptr, "Total of", 8) == 0)
	  return (-1); 

     /* Trim whitespaces */
     TrimEnd(bufptr); 
     
     switch (FTPgetType(ftp))
     {
     case FTP_MACOS:
	  Debugmsg("Parsing MACOS List\n");
	  Gzerotype = ParseUnixList(ftp, bufptr, IntName, theName, 6, gs,
				    ftpuser, ftppass);
	  break;

     case FTP_VMS:
	  Debugmsg("Parsing VMS List\n");
	  Gzerotype = ParseVMSList(ftp, bufptr, IntName, theName, gs,
				   ftpuser, ftppass);
	  break;

     case FTP_NOVELL:
	  Debugmsg("Parsing Novell List\n");
	  Gzerotype = ParseUnixList(ftp, bufptr, IntName, theName, 6, gs, 
				    ftpuser, ftppass);
	  break;

     case FTP_UNIX:
     case FTP_UNIX_L8:
     case FTP_WINNT:
	  Debugmsg("Parsing Unix List\n");
	  Gzerotype = ParseUnixList(ftp, bufptr, IntName, theName, 7, gs,
				    ftpuser, ftppass);
	  break;

     case FTP_OS2:
	  Debugmsg("Parsing OS/2 List\n");
 	  Gzerotype = ParseOS2List(ftp, bufptr, IntName, theName, gs, 
				   ftpuser, ftppass);
 	  break;


     case FTP_VM:
     case FTP_UNKNOWN:
     default:
	  Debugmsg("Parsing Unknown List\n");
	  Gzerotype = ParseUnixList(ftp, bufptr, IntName, theName, 7, gs,
				    ftpuser, ftppass);
	  break;
     }


     return(Gzerotype);
}

/* NOTE: 
 * arg 8 has been changed from char **ftppass to char *ftppass given its
 * similarity to ParseVMSList and ParseOS2List.  But what gives?  It isn't
 * even used in the function body.  
 */
int
ParseUnixList(FTP *ftp, char *bufptr, char *IntName, char *theName,
	      int cols, GopherObj *gs, char *ftpuser, char *ftppass)
{
     int i=0, end=0;
     int gap=0, objsize=0;
     char *dirname, *alias, *group=NULL, *size=NULL, *month=NULL, 
          *novellsize=NULL;
     char tmpstr[64], sizestr[16], datestr[32];
     char *cp1=NULL;

     end = strlen(bufptr);
     while (isspace(bufptr[end-1]))
	  end--;

     /*** Skip the permissions bits.. ***/
     i = 10;
     while (bufptr[i] == ' ' || bufptr[i] == '-' || bufptr[i] == ']')
	  i++;
     gap =1;

     for ( ; (gap < cols) && (i < end); gap++)
     {
	  switch (gap) {
	  case 2: 
	       novellsize = &bufptr[i]; 
	       break;

	  case 3: 
	       group = &bufptr[i];
	       break;

	  case 4: 
	       size = &bufptr[i]; 
	       break;

	  case 5: 
	       month = &bufptr[i]; 
	       break;
	  }

	  /* Skip chars to white */
	  for (;(!isspace(bufptr[i])) && (i < end); i++);
	  
	  if (i >= end) 
	       FTPerrorMessage(ftp, "said Unix but wasn't");
	  
	  /* Skip white to chars */
	  for (;isspace(bufptr[i]) && (i < end); i++);
	  
	  if (i >= end) 
	       FTPerrorMessage(ftp, "said Unix but wasn't");
     }
     
     if (gap < cols) 
	  FTPerrorMessage(ftp, "said Unix but wasn't (short cols)");

     if (isdigit(bufptr[i])) {
	  if (isdigit(bufptr[i+1])) {
	       if (isdigit(bufptr[i+2]) && isdigit(bufptr[i+3]) &&
		   isspace(bufptr[i+4])) { /* probably the YEAR */
		    for (i+=4;isspace(bufptr[i]); i++);
	       } else if ((bufptr[i+2] == ':') && isdigit(bufptr[i+3]) &&
			  isdigit(bufptr[i+4]) && isspace(bufptr[i+5])) {
		    /* probably the HOUR:MINUTE */
		    for (i+=5;isspace(bufptr[i]); i++);
	       }
	  } else if ((bufptr[i+1] == ':') && isdigit(bufptr[i+2]) &&
		     isdigit(bufptr[i+3])) {
	       /* probably the HOUR:MINUTE */
	       for (i+=4;isspace(bufptr[i]); i++);
	  }
	  
     }     
     /* Point at supposed start-of-fileIntName */
     dirname = alias = &bufptr[i]; 
     
     
     /* Skip . and .. */
     if (!strcmp(dirname, ".") || !strcmp(dirname, ".."))
	  return (-1);

     if (GFTPplus) {
	  snprintf(tmpstr, sizeof(tmpstr), "%s <%s>", 
		  GDCgetAdmin(Config), GDCgetAdminEmail(Config));
	  GSsetAdmin(gs, tmpstr);
     }
     
     switch (FTPgetType(ftp)) {
     case FTP_MACOS:
	  switch(bufptr[0]) {
	  case 'd':
	       month = group;
	       break;
	  case '-':
	       month = size;
	       break;
	  }
	  size = novellsize;
	  break;

     case FTP_NOVELL:
	  month = group;
	  size = novellsize;
	  break;
          /* Extra unused cases added to clarify assumptions and so the 
           * compiler won't complain.
           */
     case FTP_UNKNOWN:
     case FTP_VMS:
     case FTP_UNIX:
     case FTP_MTS:
     case FTP_WINNT:
     case FTP_VM:
     case FTP_UNIX_L8:
     case FTP_MICRO_VAX:
     case FTP_OS2:
     }

 try_size_again:
     if (isdigit(*size)) {
	  if (isupper(*month) && isalpha(*(month+1)) &&
	      isalpha(*(month+2)) && isspace(*(month+3))) {
	       objsize = (int) strtol(size, (char **)NULL, 10);
	       if (objsize >= 1024)
		    snprintf(sizestr, sizeof(sizestr), "%dk", objsize/1024);
	       else
		    snprintf(sizestr, sizeof(sizestr), 
			     ".%dk", (objsize*10)/1024);
	       strncpy (datestr, month, alias-month-1);
	       datestr[alias-month-1] = '\0';
	  } else {
	       LOGGopher(Gsockfd, "Couldn't parse size of %s", bufptr);
	       LOGGopher(Gsockfd, "size is %s", size);
	       LOGGopher(Gsockfd, "month is %s", month);
	  }
     } else if (isupper(*size) && isalpha(*(size+1)) &&
		isalpha(*(size+2)) && isspace(*(size+3))) {
	  month = size;
	  size = group;
	  goto try_size_again;
     } else {
	  LOGGopher(Gsockfd, "Couldn't parse size of %s", bufptr);
	  LOGGopher(Gsockfd, "size is %s", size);
     }

     switch(bufptr[0])
     {
     case 'l': /* Link? Skip to REAL IntName! */
	  /* [mea] Or do you ? Handle the symlink semantics ??
	     Is it really a directory, or a file ?       */
	  
 	  /* Data is of   foobar@ -> barfoo format, that is, separator is
 	     5 characters "@ -> " */
	  
	  
	  for (dirname = alias ; (*dirname != '\0') && (dirname != NULL);
	       ++dirname) {
	       if (strncmp(dirname, " -> ",4) == 0) {
		    gap = 4;
		    break;
	       }
	       if (strncmp(dirname, "@ -> ",5) == 0) {
		    gap = 5;
		    break;
	       }
	  }
	  if (dirname == NULL || *dirname == '\0' || dirname[gap] == '\0')
	       return(-1); /* No real DirName?  Hm.  Oh well */
	  
	  end = strlen(dirname);
	  while (isspace(dirname[end-1]))
	       end--;
	  
	  /*Internal name in 'IntName' */
 	  strncpy(IntName,alias,dirname-alias);
 	  IntName[dirname-alias] = 0; /* Make sure it terminates */
	  if (IntName[strlen(IntName)-1] == '@')
	       IntName[strlen(IntName)-1] = '\0';

	  if (dirname[end-1] == '/'){
	       /* Display name in theName */
	       sprintf(theName, "%c%s", A_DIRECTORY, IntName );
	       
	       /* Tag slash on end */
	       sprintf(bufptr, "%s/", IntName);
	       
	       GSsetType(gs, A_DIRECTORY);
	       snprintf (tmpstr, sizeof(tmpstr), "%s [%s]", IntName, datestr);
	       GSsetTitle(gs, tmpstr);

	       if (strcmp(ftpuser, "anonymous") != 0)
 		    snprintf(tmpstr, sizeof(tmpstr),
			     "%s%s/", URLget(ftpurl), IntName);
	       else
		    snprintf (tmpstr, sizeof(tmpstr), 
			      "ftp:%s@%s/%s/", GSgetHost(gs),
			     GSgetPath(gs), IntName);

	       GSsetPath(gs, tmpstr);
	       if (GFTPplus) {
		    GSsetModDate(gs, datestr);
		    AddDefaultView(gs, objsize, NULL);
	       }
	       return(A_DIRECTORY);
	  } else {
	       /* Determine type of file */

	       i = GopherType(ftp, dirname+gap, theName);
	       strcpy(theName+1, IntName); 
	       snprintf(tmpstr, sizeof(tmpstr),
			"%s@%s", dirname+gap, theName + 1);
	       strcpy(bufptr, IntName);
	       GSsetType(gs, theName[0]);
	       snprintf(tmpstr, sizeof(tmpstr),
			"%s (%s) [%s]", IntName, sizestr, datestr);
	       GSsetTitle(gs, tmpstr);
	       if (strcmp(ftpuser, "anonymous") != 0)
 		    snprintf(tmpstr, sizeof(tmpstr),
			     "%s%s",URLget(ftpurl), theName+1);
	       else
		    snprintf(tmpstr, sizeof(tmpstr),
			     "ftp:%s@%s/%s", GSgetHost(gs),
			     GSgetPath(gs), theName+1);

	       GSsetPath(gs, tmpstr);
	       if (GFTPplus) {
		    GSsetModDate(gs, datestr);
		    AddDefaultView(gs, objsize, NULL);
	       }
	       return(i);
	  }
	  break;
	  
     case 'd': /* Now treat as regular directory */
 	  if (dirname[strlen(dirname)-1] == '/')
	       dirname[strlen(dirname)-1] = '\0';
      

	  /* Display name in theName */
	  sprintf(theName, "%c%s", A_DIRECTORY, dirname);

	  /*Internal name in 'IntName' */
	  strcpy(IntName,dirname); 

	  /* Tag slash on end */
	  sprintf(bufptr, "%s/", IntName);

	  GSsetType(gs, A_DIRECTORY);
	  snprintf (tmpstr, sizeof(tmpstr), "%s [%s]", IntName, datestr);
	  GSsetTitle(gs, tmpstr);
 	  if ((cp1 = strchr(IntName, '/')))
	       for ( ; *cp1; cp1++ ) {
		    if (*cp1 == '/')
			 *cp1 = '*';
	       }
	  
	  if (strcmp(ftpuser, "anonymous") != 0) 
 	       snprintf (tmpstr, sizeof(tmpstr),
			 "%s%s/", URLget(ftpurl), IntName);
	  else
	       snprintf (tmpstr, sizeof(tmpstr),
			 "ftp:%s@%s/%s/", GSgetHost(gs), GSgetPath(gs), 
			IntName);
	  GSsetPath(gs, tmpstr);
	  if (GFTPplus) {
		  GSsetModDate(gs, datestr);
		  AddDefaultView(gs, objsize, NULL);
	  }
		  
	  return(A_DIRECTORY);
	  break;
	  
     default: 
	  /* Determine type of file */
	  strcpy(IntName, dirname); 
	  strcpy(bufptr, theName+1);
	  GopherType(ftp, IntName, theName);
	  GSsetType(gs, theName[0]);
	  snprintf (tmpstr, sizeof(tmpstr),
		    "%s (%s) [%s]", theName+1, sizestr, datestr);
	  GSsetTitle(gs, tmpstr);
	  if (strcmp(ftpuser, "anonymous") != 0)
 	       snprintf(tmpstr, sizeof(tmpstr),
			"%s%s", URLget(ftpurl), theName+1);
	  else
	       snprintf(tmpstr, sizeof(tmpstr),
			"ftp:%s@%s/%s", GSgetHost(gs),
		       GSgetPath(gs), theName+1);

	  GSsetPath(gs, tmpstr);
	  if (GFTPplus) {
		  GSsetModDate(gs, datestr);
		  AddDefaultView(gs, objsize, NULL);
	  }
	  break;
     }
     return(i);
}

static char vmsprevline[64];

int
ParseVMSList(FTP  *ftp, char *bufptr, char *IntName, char *theName,
	     GopherObj *gs, char *ftpuser, char *ftppass)
{
     int i, j;
     int objsize;
     char tmpstr[64], sizestr[16], datestr[32];
     
     if (bufptr[0] == '\0')
	  return (-1);

     j = strlen(bufptr);
     while (isspace(bufptr[j-1]))
	  j--;
     for (i = 0; bufptr[i] && !isspace(bufptr[i]); i++);
     if (bufptr[i] == '\0') {
	  if (i)  strcpy (vmsprevline, bufptr);
	  return (-1);
     }
     if (i == 0 && vmsprevline[0]) {
	  strcpy(IntName,vmsprevline);
	  vmsprevline[0] = '\0';
     } else {
	  strncpy(IntName,bufptr,i);
	  IntName[i] = '\0'; /* Make sure it terminates */
     }
     GopherType(ftp, IntName, theName);
     GSsetType(gs, theName[0]);
     if (GFTPplus) {
	  snprintf(tmpstr, sizeof(tmpstr), "%s <%s>", 
		  GDCgetAdmin(Config), GDCgetAdminEmail(Config));
	  GSsetAdmin(gs, tmpstr);
     }
     for ( ; bufptr[i] && isspace(bufptr[i]); i++);
     if (isdigit(bufptr[i])) {
	  objsize = (int) strtol(&bufptr[i], (char **)NULL, 10);
	  snprintf(sizestr, sizeof(sizestr), "%dk", objsize);
     } else if (!strncmp(&bufptr[i], "%RMS-E-PRV", strlen("%RMS-E-PRV")))
	  return (-1);
     else {
	  LOGGopher(Gsockfd, "Couldn't parse %s", bufptr);
	  return (-1);
     }
     for ( ; bufptr[i] && !isspace(bufptr[i]); i++);
     for ( ; bufptr[i] &&  isspace(bufptr[i]); i++);
     for (j = i; bufptr[j] && bufptr[j] != '['; j++);
     j--;
     strncpy (datestr, &bufptr[i], j-i);
     datestr[j-i] = '\0';
     
     if (GFTPplus) {
	  GSsetModDate(gs, datestr);
	  AddDefaultView(gs, objsize*1024, NULL);
     }
     switch (i = GSgetType(gs)) {
     case A_DIRECTORY:
	  if (strcmp(ftpuser, "anonymous") != 0)
 	       snprintf(tmpstr, sizeof(tmpstr),
			"%s%s/", URLget(ftpurl), theName+1);
	  else
	       snprintf(tmpstr, sizeof(tmpstr), "ftp:%s@%s/%s/", GSgetHost(gs),
		       GSgetPath(gs), theName+1);

	  GSsetPath(gs, tmpstr);
	  snprintf(tmpstr, sizeof(tmpstr), "%s [%s]", theName+1, datestr);
	  GSsetTitle(gs, tmpstr);
	  return(A_DIRECTORY);
	  break;
     default: 
	  
	  if (strcmp(ftpuser, "anonymous") != 0)
 	       snprintf(tmpstr, sizeof(tmpstr),
			"%s%s", URLget(ftpurl), theName+1);
	  else
	       snprintf(tmpstr, sizeof(tmpstr),
			"ftp:%s@%s/%s", GSgetHost(gs),
			GSgetPath(gs), theName+1);
	  GSsetPath(gs, tmpstr);
	  snprintf (tmpstr, sizeof(tmpstr),
		    "%s (%s) [%s]", theName+1, sizestr, datestr);
	  GSsetTitle(gs, tmpstr);
	  
	  break;
     }
     return(i);
}


int
ParseOS2List(FTP *ftp, char *bufptr, char *IntName, char *theName,
	     GopherObj *gs, char *ftpuser, char *ftppass)
{
     int i;
     int objsize;
     char tmpstr[64], sizestr[16], datestr[32];
     
     if (bufptr[0] == '\0')
	  return (-1);
     for (i = 0; bufptr[i] && isspace(bufptr[i]); i++);
     if (isdigit(bufptr[i])) {
	  objsize = (int) strtol(&bufptr[i], (char **)NULL, 10);
	  if (objsize >= 1024)
	       snprintf(sizestr, sizeof(sizestr), "%dk", objsize/1024);
	  else
	       snprintf(sizestr, sizeof(sizestr), ".%dk", (objsize*10)/1024);
     } else {
	  LOGGopher(Gsockfd, "Couldn't parse %s", bufptr);
	  return (-1);
     }
     for ( ; bufptr[i] && !isspace(bufptr[i]); i++);
     for ( ; bufptr[i] &&  isspace(bufptr[i]); i++);
     if (!strncmp(&bufptr[i], "DIR ", 4)) {
	  GSsetType(gs, A_DIRECTORY);
	  i += 4;
     } else if (!strncmp(&bufptr[i], "A ", 2)) {
	  GSsetType(gs, A_FILE);
	  i += 2;
     } else if (isdigit(bufptr[i])) {
	  GSsetType(gs, A_FILE);
     } else {
	  LOGGopher(Gsockfd, "Couldn't parse %s", bufptr);
	  return (-1);
     }
     for ( ; bufptr[i] && isspace(bufptr[i]); i++);
     if (isdigit(bufptr[i])) {
	  strncpy (datestr, &bufptr[i], 16);
	  datestr[16] = '\0';
	  i += 16;
     } else {
	  LOGGopher(Gsockfd, "Couldn't parse %s", bufptr);
	  return (-1);
     }
     for ( ; bufptr[i] && isspace(bufptr[i]); i++);
     if (!strcmp(&bufptr[i], ".") || !strcmp(&bufptr[i], ".."))
	  return (-1);
	 
     if (GFTPplus) {
	  GSsetModDate(gs, datestr);
	  AddDefaultView(gs, objsize*1024, NULL);
	  snprintf(tmpstr, sizeof(tmpstr), "%s <%s>", 
		   GDCgetAdmin(Config), GDCgetAdminEmail(Config));
	  GSsetAdmin(gs, tmpstr);
     }
     GopherType(ftp, &bufptr[i], theName);

     switch (i = GSgetType(gs)) {
     case A_DIRECTORY:
	  if (strcmp(ftpuser, "anonymous") != 0)
 	       snprintf(tmpstr, sizeof(tmpstr), 
			"%s%s/", URLget(ftpurl), theName+1);
	  else
	       snprintf(tmpstr, sizeof(tmpstr), "ftp:%s@%s/%s/", GSgetHost(gs),
			GSgetPath(gs), theName+1);

	  GSsetPath(gs, tmpstr);
	  snprintf (tmpstr, sizeof(tmpstr), "%s [%s]", theName+1, datestr);
	  GSsetTitle(gs, tmpstr);
	  return(A_DIRECTORY);
	  break;

     default: 
	  i = GSsetType(gs, theName[0]);
	  if (strcmp(ftpuser, "anonymous") != 0)
 	       snprintf(tmpstr, sizeof(tmpstr), 
			"%s%s", URLget(ftpurl), theName+1);
	  else
	       snprintf(tmpstr, sizeof(tmpstr), "ftp:%s@%s/%s", GSgetHost(gs),
			GSgetPath(gs), theName+1);
	  GSsetPath(gs, tmpstr);
	  snprintf (tmpstr, sizeof(tmpstr),
		    "%s (%s) [%s]", theName+1, sizestr, datestr);
	  GSsetTitle(gs, tmpstr);
	  break;
     }
     return(i);
}




int
GopherType(FTP  *ftp, char *bufptr, char *theName)
{
     int last;
     
     if (FTPgetType(ftp) == FTP_VMS) 
	  last = Vaxinate(bufptr);
     else
	  last = strlen(bufptr)-1;
     
     
     if (bufptr[last] == '/')
     {
	  sprintf(theName, "%c%s", A_DIRECTORY, bufptr);
	  theName[strlen(theName)-1] = '\0';
	  return(A_DIRECTORY);
     }
     
     if ((bufptr[last] == '*') || (bufptr[last] == '@'))     /* Hack out * and @ */
	  bufptr[last] = '\0';
     
     
     return(GopherFile(ftp, bufptr, theName));
}



/* At this point we're looking at a file */

int
GopherFile(FTP  *ftp, char *buf, char *theName)
{
     char	Gtype;
     int	last;
     char	tmpName[256];	
     Extobj     *ext;
     char *slashp, **textn;
     
     last = strlen(buf) -1;
     
     strcpy(tmpName, buf);
     if (buf[last] == '/') {
	  tmpName[last] = '\0';
	  sprintf(theName, "%c%s", A_DIRECTORY, tmpName);
	  return(A_DIRECTORY);
     }
     if ((buf[last] == '*') || (buf[last] == '@')) {	/* Hack out * and @ */
	  buf[last] = '\0';
	  tmpName[last] = '\0';
     }
     
     ext = EXnew();
     /* At this point we're looking at a file */
     if (GDCViewExtension(Config, buf, &ext)) {  
	  Gtype = EXgetObjtype(ext);
	  
	  sprintf(theName, "%c%s", Gtype, tmpName);
	  EXdestroy(ext);
	  return(Gtype);
     }

     if (EXAcasedSearch(Config->Extensions, ext, buf, EXT_DECODER)) {
	  EXdestroy(ext);
	  sprintf(theName, "%c%s", A_UNIXBIN, tmpName);
	  return(A_UNIXBIN);
     }

     /** Hack for some notable looking text files **/
     
     if ((slashp = strrchr(buf, '/')) == NULL)
	  slashp = buf;
     else
	  slashp++;
     for (textn = textnames; *textn; textn++)
	  if (strcasestr(slashp, *textn)==0) {
	       sprintf(theName, "%c%s", A_FILE, tmpName);
	       return(A_FILE);	/* text file */
	  }
     sprintf(theName, "%c%s", A_UNIXBIN, tmpName);
     return(A_UNIXBIN);	/* Some other and hopefully text file */
}

#else
#include "command.h"

GopherFTPgw(int sockfd, char *ftpstr, CMDobj *cmd)
{
     Die(sockfd, 501, "Sorry, this server does not have ftp capabilities");
}

   
#endif  /** NO_FTP **/
