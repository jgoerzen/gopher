/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/gopherd.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: gopherd.h
 * Header file for gopher server.
 *********************************************************************
 * Revision History:
 * $Log: gopherd.h,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.2  2000/11/18 23:21:50 mdallen
 * Prototyped all non-static functions from gopherd.c 
 * at the bottom of the file, added more relevant header #includes.
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.10  1994/09/29  19:58:50  lindner
 * Fix for including Locale.h
 *
 * Revision 3.9  1994/05/14  04:19:15  lindner
 * Use new locale.h
 *
 * Revision 3.8  1994/05/02  07:41:14  lindner
 * Mods to use setlocale()
 *
 * Revision 3.7  1994/04/14  18:00:38  lindner
 * Fix for AIX
 *
 * Revision 3.6  1994/04/08  21:09:50  lindner
 * Remove extraneous stat include
 *
 * Revision 3.5  1994/03/17  04:30:15  lindner
 * VMS fixes gopherd.h
 *
 * Revision 3.4  1993/06/14  22:23:02  lindner
 * status-->state:
 *
 * Revision 3.3  1993/04/15  21:59:08  lindner
 * Fix for newer hpux systems
 *
 * Revision 3.2  1993/02/19  21:21:32  lindner
 * Fixed problems with signals
 *
 * Revision 3.1.1.1  1993/02/11  18:02:51  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#include "conf.h"

#include "Locale.h"
#include <ctype.h>
#include <stdio.h>

#ifdef VMS
#include <perror.h>
#endif

#ifndef VMS
#  include <sys/types.h>
#  include <pwd.h>
#endif

#include <errno.h>

#include <signal.h>

/** For logfile locking... **/
#ifndef VMS
#if !defined(NeXT) && !defined(mips) && !defined(UMAX43) && !defined(sequent) && !defined(sony_news)
#  include <unistd.h>
#  include <fcntl.h>
#else
#  include <fcntl.h>
#  include <sys/file.h>
#  ifndef SEEK_END
#    define SEEK_END L_XTND
#    define SEEK_SET L_SET
#  endif
#endif
#endif
extern int errno;

#include "String.h"

#include <time.h>


#include "GDgopherdir.h"
#include "Dirent.h"

/*
 * Make sure we don't accidentally use a library routine instead of our
 * private restricted version.  Need to avoid the stat in struct stat
 * getting clobbered by the #define for the routine.
 */
typedef struct stat STATSTR;
#include "openers.h"
#define open	barf_ropen
#define fopen	barf_rfopen
#define stat	barf_rstat
#define opendir	barf_ropendir
#define chdir	barf_rchdir

#include "compatible.h"
#include "util.h"
#include "gopherdconf.h"

/**  Make sure that we can find out the max pathname size **/
#ifndef MAXPATHLEN
#  if !defined(VMS)
#    include <sys/param.h>
#  endif
#  ifndef MAXPATHLEN
#    define MAXPATHLEN (256)
#  endif
#endif

#include "daemon.h"
#include "error.h"
#include "kernutils.h"
#include "ftp.h"
#include "gopherdconf.h"
#include "command.h"
#include "ext.h"
#include "serverutil.h"
#include "index.h"

/*** This one must be last ***/
#include "globals.h"

/* Prototypes */
RETSIGTYPE read_timeout                        (int sig);
void gopherd_exit                              (int val);
RETSIGTYPE sigabnormalexit                     (void);
void OkHTTPresponse                            (int sockfd);

#ifndef NO_AUTHENTICATION
void OutputAuthForm                            (int sockfd, char *pathname, 
                                                char *host, int port, 
                                                CMDprotocol p);
#endif /* NO_AUTHENTICATION */

void HandleHTTPpost                            (CMDobj *cmd);
void AddDefaultView                            (GopherObj *gs, int size, 
                                                char *dirname);
void GSrunScripts                              (GopherObj *gs, char *filename);
GopherDirObj *GDfromUFS                        (char *pathname, int sockfd, 
                                                boolean isGplus);
void GSicontoNet                               (GopherObj *gs, int  sockfd);
int Process_Side                               (FILE *sidefile, 
                                                GopherObj *Gopherp);
boolean IsAskfile                              (char *pathname);
