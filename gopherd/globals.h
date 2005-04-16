/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/globals.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: globals.h
 * Global variables for gopher server
 *********************************************************************
 * Revision History:
 * $Log: globals.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.15  1995/09/25  22:06:57  lindner
 * Ansification bugs
 *
 * Revision 3.14  1995/09/25  05:02:34  lindner
 * Convert to ANSI C
 *
 * Revision 3.13  1995/02/11  06:20:55  lindner
 * Add variables to do common statistics
 *
 * Revision 3.12  1995/02/06  22:27:54  lindner
 * Use dynamic space for Data_Dir, remove RunLS
 *
 * Revision 3.11  1994/06/29  05:26:17  lindner
 * Add Gticket and CurrentUser
 *
 * Revision 3.10  1994/04/01  04:58:47  lindner
 * Fix for gopher- client error messages
 *
 * Revision 3.9  1994/03/31  22:45:49  lindner
 * Generate gopher- error responses for gopher- clients
 *
 * Revision 3.8  1994/03/17  04:26:40  lindner
 * Add errorfile fd
 *
 * Revision 3.7  1993/10/04  06:41:10  lindner
 * Removed gindexd functionality
 *
 * Revision 3.6  1993/08/04  22:12:37  lindner
 * Mods to use Gpopen
 *
 * Revision 3.5  1993/07/27  05:27:44  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.4  1993/07/26  17:23:43  lindner
 * Mods for send_binary
 *
 * Revision 3.3  1993/07/20  23:56:21  lindner
 * Added Argv vars, peername, etc.
 *
 * Revision 3.2  1993/07/07  19:38:01  lindner
 * none
 *
 * Revision 3.1.1.1  1993/02/11  18:02:51  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.3  1993/01/30  23:57:44  lindner
 * New global "ASKfile
 *
 * Revision 1.2  1992/12/16  05:07:23  lindner
 * Removed SortDir, moved it into listdir() where it belongs.
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

/*
 * This is some funky defines that assures that global variables are
 * declared only once.  (when globals.c includes this file with EXTERN
 * defined.
 */

#ifndef EXTERN
#define EXTERN extern
#define INIT(x)
#else
#define EXTERN
#define INIT(x) = (x)
#endif

#include "boolean.h"

/**** Defines ****/
#define MAXLINE 512

/**** Globals.  ****/

EXTERN GDCobj    *Config;
EXTERN boolean   RunFromInetd INIT(FALSE);
EXTERN boolean   Caching INIT(TRUE);
EXTERN boolean   UsingHTML INIT(FALSE);
EXTERN int       LOGFileDesc INIT(-1);
EXTERN int       ERRORFileDesc INIT(-1);
EXTERN char      *Data_Dir INIT(NULL);
EXTERN char      *pname INIT(NULL);

EXTERN int       dochroot INIT(TRUE);   /*** Should we use chroot?? ***/
EXTERN char      *Zehostname INIT(NULL);      /** Holds name and domain **/
EXTERN int       GopherPort INIT(GOPHER_PORT);
EXTERN char      *EXECargs INIT(NULL);   /** used with exec type **/

/** For statistics **/
#include <sys/types.h>
#include <sys/time.h>

EXTERN unsigned long Connections INIT(0);
EXTERN int       ActiveSessions INIT(0);
EXTERN time_t    ServerStarted INIT(0);

/*** What's being run ***/
EXTERN boolean   RunServer     INIT(TRUE);  /** Run server as default **/

EXTERN boolean    IsGplus       INIT(FALSE);

EXTERN char **   Argv;
EXTERN char *   LastArgv;

/*** Incoming data from the client ***/
EXTERN char *ASKfile           INIT(NULL);

/*** Who's querying us.. ***/
EXTERN char    CurrentPeerName[256];
EXTERN char    CurrentPeerIP[16];
EXTERN char    *CurrentUser INIT(NULL);

/*** The ticket value.. ****/
EXTERN char    *Gticket INIT(NULL);

/*** Prototypes n' externals ****/
#include "special.h"
#include "serverutil.h"
int           Process_Side(FILE *sidefile, GopherObj *Gopherp);

