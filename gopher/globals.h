/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.4 $
 * $Date: 2002/01/08 17:36:14 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/globals.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: globals.h
 * Global variables and #defines
 *********************************************************************
 * Revision History:
 * $Log: globals.h,v $
 * Revision 1.4  2002/01/08 17:36:14  jgoerzen
 * Finally builds!
 *
 * Changes:
 *
 *   * config.h.in: rebuilt by autoheader
 *
 *   * configure: rebuilt by autoconf
 *
 *   * configure.in:
 *     * Added REGEXLIBS test for NetBSD -- look for re_comp in -lcompat
 *     * Added checkes for term.h, re_comp.h, regex.h
 *
 *   * gopher/ourutils.c, gopher/CURcurses.c: Use term.h check
 *
 *   * gopher/Makefile.in, gopherd/Makefile.in, gophfilt/Makefile.in:
 *     * Use REGEXLIBS
 *
 *   * gopher/globals.h, gopher/gopher.c: Remove sys_errlist
 *
 *   * object/GSgopherobj.c: Removed <regex.h> #include, now use
 *     "Regex.h" that has proper regex determining logic
 *
 *   * object/Regex.h: Moved regex.h include to here.  Make it conditional
 *     based on configure test.  Add conditional re_comp.h include.
 *
 * Revision 1.3  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.2  2000/08/19 00:43:59  jgoerzen
 * Fix to use linux standard sys_errlist
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.20  1995/11/03  21:18:17  lindner
 * ANSIfication
 *
 * Revision 3.19  1995/05/01  05:41:30  lindner
 * compatibility fixes
 *
 * Revision 3.18  1995/05/01  03:42:08  lindner
 * Fixes for NetBSD
 *
 * Revision 3.17  1995/04/15  07:08:17  lindner
 * none
 *
 * Revision 3.16  1994/07/06  15:38:54  lindner
 * Add setlocale_langdir
 *
 * Revision 3.15  1994/06/29  07:07:56  lindner
 * moved gcatd to Locale.[ch] (Coopersmith)
 *
 * Revision 3.14  1994/05/17  05:47:55  lindner
 * Massive internationalization change
 *
 * Revision 3.13  1994/03/04  23:36:33  lindner
 * remove MAXRESP define
 *
 * Revision 3.12  1993/09/29  20:51:44  lindner
 * add declaration of CleanupandExit()
 *
 * Revision 3.11  1993/08/16  18:19:32  lindner
 * VMS special interrupt and subprocess handling vars
 *
 * Revision 3.10  1993/08/16  17:58:19  lindner
 * Removed REMOTEUSER ifdefs
 *
 * Revision 3.9  1993/08/03  20:48:21  lindner
 * Audio file fix from jqj
 *
 * Revision 3.8  1993/08/03  20:24:16  lindner
 * Bigger Better Badder Options, inspired by jqj
 *
 * Revision 3.7  1993/07/27  05:28:46  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.6  1993/07/23  04:36:59  lindner
 * Added a Jmpenv variable for longjmp
 *
 * Revision 3.5  1993/07/20  23:11:56  lindner
 * Got rid of versionline
 *
 * Revision 3.4  1993/06/08  06:33:07  lindner
 * Updated version number
 *
 * Revision 3.3  1993/04/15  21:28:27  lindner
 * Mods for remote access
 *
 * Revision 3.2  1993/03/24  16:57:51  lindner
 * New version#
 *
 * Revision 3.1.1.1  1993/02/11  18:02:57  lindner
 * Gopher+1.2beta release
 *
 *********************************************************************/

#include <setjmp.h>

#define WHOLELINE 80                   /* Used in ourutil.c */

/*
 * These are some funky defines that assures that global variables are
 * declared only once.  (when globals.c includes this file with EXTERN
 * defined.
 */

#ifndef EXTERN
#define EXTERN extern
#define INIT(x)
#else
#define EXTERN
#define INIT(x) =(x)
#endif

/*** Global variables ***/
EXTERN RCobj     *GlobalRC;

EXTERN boolean   ChangedDefs INIT(FALSE);

EXTERN char      *Searchstring INIT(NULL);
EXTERN int       iLevel INIT(0);
EXTERN BOOLEAN   SecureMode INIT(FALSE);
EXTERN BOOLEAN	 NoShellMode INIT(FALSE);
EXTERN BOOLEAN   RemoteUser INIT(FALSE);

EXTERN GopherDirObj *CurrentDir INIT(NULL);
EXTERN GopherDirObj *OldDirs[30];  /** Should be a stack... **/

EXTERN GopherDirObj *BookmarkDir INIT(NULL);

EXTERN char      USERCAP[WHOLELINE];    /* The validated user capability */
EXTERN int       SOUNDCHILD INIT(0);     /* The pid of the sound player child. */
EXTERN CursesObj *CursesScreen;

EXTERN char *Gopenfile INIT(NULL);

#if defined(VMS) && defined(A_LANGUAGE)
EXTERN        GopherDirObj    *setlocale_LangDir INIT(NULL);
#endif

#ifndef VMS
extern int errno;
#endif

/*** Externals ***/

#ifndef VMS
extern char **environ;                  /* User environment array */
#endif

/*** VMS needs special interrupt and subprocess handling ***/

#ifdef VMS
void (*VMSsignal(/* int, void (*) () */)) (int);
#define signal(a,b) VMSsignal(a,b)
EXTERN boolean HadVMSInt INIT(FALSE);
int DCLsystem(/* char* */);
#define system(a) DCLsystem(a)
#endif

/*** Prototypes and forward declarations ***/

/*** Ourutils.c ***/
void display_file(/* char *Filename */);
void ZapCRLF(char *);
int  outchar(int c);        
void CursesErrorMsg(char *Message);
void GetOneOption(/* char*, char* */);

int  process_request(GopherObj *ZeGopher);
int  Load_Dir(GopherObj *ZeGopher);
int  Load_Index(GopherObj *ZeGopher);
int  Load_Index_or_Dir(GopherObj *ZeGopher, char *Searchmungestr);
void GetOneOption(/* */);
void check_sock(int sockfd, char *host, int port);
char *Choose_View(GopherObj *gs);
char **AskBlock(GopherObj *gs);
void CleanupandExit(int exitval);

