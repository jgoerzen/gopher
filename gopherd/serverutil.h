/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/serverutil.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: serverutil.h
 * declarations of utility functions
 *********************************************************************
 * Revision History:
 * $Log: serverutil.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.7  1995/10/04  21:05:20  lindner
 * NO_AUTH mod
 *
 * Revision 3.6  1995/09/28  21:11:31  lindner
 * More fixes...
 *
 * Revision 3.5  1995/09/26  04:59:48  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.4  1995/09/25  05:02:39  lindner
 * Convert to ANSI C
 *
 * Revision 3.3  1995/02/07  08:37:38  lindner
 * Rewrite of mailfile/multifile parsing
 *
 * Revision 3.2  1994/12/20  17:20:56  lindner
 * Put environment variable setter here..
 *
 * Revision 3.1  1993/06/22  07:06:58  lindner
 * New header file..
 *
 *
 *********************************************************************/

#ifndef G_SERVERUTIL_H
#define G_SERVERUTIL_H

typedef int Splittype;
#define SPLIT_MAIL -1
#define SPLIT_UNKNOWN -2

#include <stdarg.h>
#include <stdio.h>
#include "boolean.h"

void      GplusError(int sockfd, int errclass, char *text, char **moretext);
void      Die(int sockfd, int errorlevel, char *fmt, ...);
void      Warn(int sockfd, int errorlevel, char *fmt, ...);
void      LOGGopher(int sockfd, const char *fmt, ...);

boolean   Setuid_username(char *);

int       is_mail_from_line(char *);
Splittype is_multipartfile(char *);

char      *mtm_basename( char *);
boolean   Cachetimedout(char *cache, int secs, char *dir);

boolean   isadir(char *);
void      SetEnvironmentVariable(char *, char *);
FILE*     Gpopen(int sockfd, char *cmd, char *rw);


#include "gopherdconf.h"

#ifdef NO_AUTHENTICATION
#  define CheckAccess(a,b) 
#else
   void      CheckAccess(int sockfd, Accesslevel);
#endif

#endif
