/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.6 $
 * $Date: 2002/04/26 13:33:33 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/CURcurses.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: CURcurses.h
 * Header file access methods for CURcurses.c
 *********************************************************************
 * Revision History:
 * $Log: CURcurses.h,v $
 * Revision 1.6  2002/04/26 13:33:33  jgoerzen
 * Removed glib MIME stuff.  We'll move to Pygopherd first.
 *
 * Revision 1.4.2.1  2002/03/21 15:24:56  jgoerzen
 * Fixes for Solaris.  Also commited on main branch.
 *
 * Revision 1.4  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.3  2001/01/03 22:26:27  s2mdalle
 * Code cleanups, addition of a few prototypes from CURcurses.c
 *
 * Revision 1.2  2000/12/27 21:24:04  s2mdalle
 * Added many function prototypes from CURcurses.c
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.24  1996/01/04  18:28:52  lindner
 * Updates for autoconf
 *
 * Revision 3.23  1995/11/03  21:18:15  lindner
 * ANSIfication
 *
 * Revision 3.22  1995/05/01  05:41:29  lindner
 * compatibility fixes
 *
 * Revision 3.21  1995/02/27  17:44:25  lindner
 * Change code to use enums
 *
 * Revision 3.20  1995/02/17  18:31:08  lindner
 * Fix for solaris curses routines
 *
 * Revision 3.19  1994/06/29  05:21:12  lindner
 * Start of AskF:
 *
 * Revision 3.18  1994/05/02  07:40:16  lindner
 * Mods to use setlocale()
 *
 * Revision 3.17  1994/04/13  19:14:31  lindner
 * AskL modifications
 *
 * Revision 3.16  1994/04/07  17:25:54  lindner
 * Fix for pyramids
 *
 * Revision 3.15  1994/03/04  23:39:48  lindner
 * Fix for log entries
 *
 * Revision 3.14  1994/03/04  23:36:30  lindner
 * figure out whether the terminal can do alternate character set.
 *
 * Revision 3.13  1994/02/20  21:45:20  lindner
 * Better sanity checks for box drawing characters
 *
 * Revision 3.12  1994/02/20  16:49:20  lindner
 * Fix bug in gcc for Solaris 2.x for curses routines
 *
 * Revision 3.11  1993/12/28  17:28:52  lindner
 * Better method of drawing box characters
 *
 * Revision 3.10  1993/11/29  01:09:46  lindner
 * Don't use the alternate character set for character graphics under AIX.
 * They don't work very well in xterms or aixterms.  [However, it does work
 * if I use Mac NCSA Telnet to connect to our RS/6000....]  (Beckett)
 *
 * Revision 3.9  1993/11/02  06:21:30  lindner
 * Fix for osf curses
 *
 * Revision 3.8  1993/09/26  09:19:21  lindner
 * Add bold #defines
 *
 * Revision 3.7  1993/09/22  15:44:36  lindner
 * Fix for sysv systems without KEY_HELP
 *
 * Revision 3.6  1993/09/22  01:15:47  lindner
 * Add support for DEC HELP key/KEY_HELP
 *
 * Revision 3.5  1993/09/21  01:46:08  lindner
 * Implement all remaining ASK block items..
 *
 * Revision 3.4  1993/09/08  05:20:02  lindner
 * Fix for multiple CURcurses.h includes
 *
 * Revision 3.3  1993/07/30  17:31:39  lindner
 * Mods to support AskP:
 *
 * Revision 3.2  1993/04/15  21:25:20  lindner
 * Fixes for CURbox() definitions
 *
 * Revision 3.1.1.1  1993/02/11  18:02:56  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.2  1992/12/31  05:57:38  lindner
 * Mods for VMS
 *
 * Revision 1.1  1992/12/10  23:32:16  lindner
 * gopher 1.1 release
 *
 * Revision 1.1  1992/12/10  06:16:51  lindner
 * Initial revision
 *
 *
 *********************************************************************/

#ifndef CURCURSES_H
#define CURCURSES_H

#include "Locale.h"
#include <ctype.h>

#ifdef __svr4__
  /* Fix bug in gcc for Solaris 2.x */
#  define SYSV
#endif

#if defined(ultrix)
#  include <cursesX.h>
#else
#  include <curses.h>
#endif

#include "boolean.h"
#include "STRstring.h"

#ifdef KEY_RIGHT
#  define SYSVCURSES
#endif

struct CursesStruct {
     WINDOW *Screen;
     String *Termtype;

     /** Termcap/terminfo stuff **/
     String *Clearscreen;
     String *AudibleBell;
     String *Highlighton;
     String *Highlightoff;
     
     boolean canUseACS;
     boolean inCurses;
     
     int COLS;
     int ROWS;
     
     int Box_ul;
     int Box_ur;
     int Box_ll;
     int Box_lr;
     int Box_vline;
     int Box_hline;
     int Box_tt;
     int Box_bt;
     int Box_lt;
     int Box_rt;
     int Box_ct;

     void (*sigtstp)();
     void (*sigwinch)();
};

typedef struct CursesStruct CursesObj;
typedef struct CursesStruct *CursesObjp;

#ifndef SYSVCURSES
#  define KEY_DOWN        0402           /* The four arrow keys ...*/
#  define KEY_UP          0403
#  define KEY_LEFT        0404
#  define KEY_RIGHT       0405           
#  define KEY_NPAGE       0522           /* Next page */
#  define KEY_PPAGE       0523           /* Previous page */
#  define KEY_ENTER       0527
#  define KEY_BACKSPACE   0407
#  define KEY_HELP        0553            /* help key */
#endif

/*
 * Definitions for character graphics
 */

#if defined(SYSVCURSES) && !defined(ultrix) && !defined(hpux) && !defined(_AUX_SOURCE) && !defined(__osf__) && !defined(_AIX)
#  define CUR_FANCY_BOXES
#endif

#if defined(VMS) || defined(__convex__) || defined(sequent) || defined(pyr)
#if !defined(cbreak)
#define cbreak crmode
#endif
#endif

#if defined(__svr4__)
#  define CURS_MACROS
#endif

#ifndef KEY_HELP
#define KEY_HELP        0553            /* help key */
#endif

/*
 * Request types....  More to be added later..
 */

enum curtype {
     CUR_LABEL  =1,
     CUR_PROMPT =2,
     CUR_PASSWD =3,
     CUR_CHOICE =4,
     CUR_ASKL   =5,
     CUR_FNAME  =6
};
typedef enum curtype Curreqtype;

struct Requestitem_struct {
     char       *prompt;
     char       *stowage;
     Curreqtype thing;
     int        chooseitem;
     char       **choices;
};

typedef struct Requestitem_struct Requestitem;

#define max(A, B) ((A) > (B) ? (A) : (B))
#define min(A, B) ((A) < (B) ? (A) : (B))

/*
 * access functions
 */

#define CURgetScreen(a)    ((a)->Screen)
#define CURgetTerm(a)      (STRget((a)->Termtype))
#define CURgetCLS(a)       (STRget((a)->Clearscreen))
#define CURgetBell(a)      (STRget((a)->AudibleBell))
#define CURgetHighon(a)    (STRget((a)->Highlighton))
#define CURgetHighoff(a)   (STRget((a)->Highlightoff))

#ifdef VMS
#define CURsetScreen(a,b)  ((b),(a)->Screen=stdscr)
#else
#define CURsetScreen(a,b)  ((a)->Screen=(b))
#endif
#define CURsetTerm(a,b)    (STRset((a)->Termtype, (b)))
#define CURsetCLS(a,b)     (STRset((a)->Clearscreen,(b)))
#define CURsetBell(a,b)    (STRset((a)->AudibleBell,(b)))
#define CURsetHighon(a,b)  (STRset((a)->Highlighton,(b)))
#define CURsetHighoff(a,b) (STRset((a)->Highlightoff,(b)))
#define CURsetSIGTSTP(a,b) ((a)->sigtstp=(b))
#define CURsetSIGWINCH(a,b) ((a)->sigwinch=(b))

#ifdef HAVE_WSTANDOUT
#define wboldout(win)       (wstandout(win))
#define wboldend(win)       (wstandend(win))
#else
#define wboldout(win)       (wattron(win, A_BOLD))
#define wboldend(win)       (wattroff(win, A_BOLD))
#endif

#define CURgetBox_ul(a)  ((a)->Box_ul)
#define CURgetBox_ur(a)  ((a)->Box_ur)
#define CURgetBox_ll(a)  ((a)->Box_ll)
#define CURgetBox_lr(a)     ((a)->Box_lr)
#define CURgetBox_vline(a)  ((a)->Box_vline)
#define CURgetBox_hline(a)  ((a)->Box_hline)
#define CURgetBox_tt(a)     ((a)->Box_tt)
#define CURgetBox_bt(a)     ((a)->Box_bt)
#define CURgetBox_lt(a)     ((a)->Box_lt)
#define CURgetBox_rt(a)     ((a)->Box_rt)
#define CURgetBox_ct(a)     ((a)->Box_cr)


CursesObj *CURnew(void);
void      CURinit(CursesObj *cur);
void      CURcenterline(CursesObj *cur, WINDOW *win, char *theline, 
			int yval, int windowidth, boolean bright);
void      CURrightline(CursesObj *cur, char *theline, int yval);
void      CURenter(CursesObj *cur);
void      CURexit(CursesObj *cur);
int       CURgetstr();
int       CURgetch(CursesObj *cur);
void      CURresize(CursesObj *cur);
int       CURoutchar(int c);
int       CURGetOneOption(CursesObj *cur, char *Title, char *OptionName,
			  char *Response);
void      CURBeep(CursesObj *cur);
void      CURwenter(CursesObj *cur, WINDOW *win);
void      CURsetGraphicsChars(CursesObj *cur, char *termtype);
int       CUROldGetOneOption(CursesObj *cur, char *OptionName, char *Response);
int       CURwgetch(CursesObj *cur, WINDOW *window);
int       CURrequester(CursesObj *cur, char *Wintitle, Requestitem **items);
int       CURDialog(CursesObj *cur, char *Wintitle, char **Message);
void      CURgetYesorNo(CursesObj *cur, char *OptionName, char *Response);
int       CURRequest(CursesObj *cur, char *Wintitle, char **Prompts, 
                     char **Stowages);

int       CURChoice(CursesObj *cur, char *WinTitle, char **Choices, 
                    char *Prompt, int DefaultChoice);
void      CURbutton(CursesObj *cur, WINDOW *win, char *Label, boolean bright);
void      CURbox(CursesObj *cur, WINDOW *win, int height, int width);
int       CURwgetstr(CursesObj *cur, WINDOW *win, char *inputline, 
                     int maxlength, boolean hidden);
#endif  /* CURCURSES_H */

