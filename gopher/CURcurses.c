/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.5 $
 * $Date: 2002/01/08 17:36:14 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/CURcurses.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: CURcurses.c
 * Abstraction of all Curses Functions
 *********************************************************************
 * Revision History:
 * $Log: CURcurses.c,v $
 * Revision 1.5  2002/01/08 17:36:14  jgoerzen
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
 * Revision 1.4  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.3  2001/01/03 22:25:57  s2mdalle
 * Code cleanups, fixes for copious compiler warnings.
 *
 * Revision 1.2  2000/12/27 21:25:01  s2mdalle
 * Added #include "gopher.h"
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.61  1996/01/04  18:28:51  lindner
 * Updates for autoconf
 *
 * Revision 3.60  1995/11/03  21:18:15  lindner
 * ANSIfication
 *
 * Revision 3.59  1995/11/03  20:50:47  lindner
 * Coen: fixes..
 *
 * Revision 3.58  1995/05/02  06:06:57  lindner
 * Fix for message str..
 *
 * Revision 3.57  1995/04/15  07:09:46  lindner
 * New funky C-x stuff
 *
 * Revision 3.56  1995/03/09  05:24:39  lindner
 * F.Macrides 01-Mar-1995 Removed the CURsetScreen(cur, initscr()) that
 * someone #ifdef'ed for VMS in CURresize().
 *
 * Revision 3.55  1995/02/27  17:44:51  lindner
 * New pacbell fcn CURrightline
 *
 * Revision 3.54  1995/02/07  19:34:41  lindner
 * Fix for unix systems without SIGTSTP
 *
 * Revision 3.53  1995/01/25  23:09:24  lindner
 * Word wrapping in gopher forms, provided by Pacific Bell Knowledge Net
 *
 * Revision 3.52  1994/12/03  01:51:39  lindner
 * Fix prototype for DCLspawn_exception
 *
 * Revision 3.51  1994/10/21  04:41:07  lindner
 * Add ANSI attached printer code..
 *
 * Revision 3.50  1994/06/29  07:06:59  lindner
 * moved gcatd to Locale.[ch] (Coopersmith)
 *
 * Revision 3.49  1994/06/29  05:11:07  lindner
 * None..
 *
 * Revision 3.48  1994/06/12  05:28:17  lindner
 * Fix for locales
 *
 * Revision 3.47  1994/06/09  22:13:35  lindner
 * More language conversions
 *
 * Revision 3.46  1994/05/24  05:49:54  lindner
 * Fix for bad free() in CURrequest()
 *
 * Revision 3.45  1994/04/25  20:43:29  lindner
 * Remove ugly pointer debug statement
 *
 * Revision 3.44  1994/04/25  03:49:41  lindner
 * Fixed ^_ mislabel of the ^- help command.
 *
 * Added prompt for RETURN in VMSExit(), to enable reading of ACCVIO tracebacks.
 *
 * Changed return values of DCLSystem() to be compatible with the
 * for-Unix code.
 *
 * Fixed spawns to work with OpenVMS/AXP v6.1 (still doesn't have the
 * TRUSTED flag).
 *
 * Revision 3.43  1994/04/25  03:37:35  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.42  1994/04/13  19:13:20  lindner
 * AskL modifications
 *
 * Revision 3.41  1994/04/01  02:24:41  lindner
 * Remove debugging stuff
 *
 * Revision 3.40  1994/03/31  21:26:43  lindner
 * Mitra's debug routine
 *
 * Revision 3.39  1994/03/08  15:55:02  lindner
 * gcc -Wall fixes
 *
 * Revision 3.38  1994/03/04  23:39:45  lindner
 * Fix for log entries
 *
 * Revision 3.37  1994/03/04  23:36:28  lindner
 * Many, many fixes and enhancements for Beckett and Co.
 *
 * Revision 3.36  1994/02/21  15:47:34  lindner
 * Remove conditional compilation for DEC ALPHAS trusted flag on VMS
 *
 * Revision 3.35  1994/02/20  21:45:18  lindner
 * Better sanity checks for box drawing characters
 *
 * Revision 3.34  1994/01/20  06:46:15  lindner
 * Fix boundary condition
 *
 * Revision 3.33  1994/01/14  16:40:04  lindner
 * Added VMSVersion() for getting the VMS version number, and modified
 * spawn_DCLprocess() so that it includes the TRUSTED flag for VAX v6+
 * and AXP v2+.  If DEC changes AXP v2.0 to v6.1 (I've heard rumors to
 * that affect), remove the __ALPHA conditional compilation in
 * spawn_DCLprocess() so it just checks for v6+.
 *
 * Revision 3.32  1993/12/28  17:29:09  lindner
 * Better method of drawing box characters
 *
 * Revision 3.31  1993/11/04  02:06:50  lindner
 * Add Ben's AIS hack
 *
 * Revision 3.30  1993/10/26  18:24:31  lindner
 * Make sure we cleanup before exiting
 *
 * Revision 3.29  1993/10/26  18:12:56  lindner
 * Fixes for Input routines, screen redrawing, ASK fixes
 *
 * Revision 3.28  1993/10/22  20:22:12  lindner
 * Range checks for lots of things
 *
 * Revision 3.27  1993/10/07  05:10:12  lindner
 * Make TrimmedTitle[] (twice) a larger array (256 elements).  Makes
 * 132-column terminals and huge xterm windows happier.
 *
 * In CURwgetch(), fix vt100 "Help" escape sequence.  Add vt100 "Enter",
 * vt200 "Help", and vt200 "Do" escape sequences.
 *
 * Revision 3.26  1993/09/30  22:41:57  lindner
 * Add option for bolding of searched words
 *
 * Revision 3.25  1993/09/29  22:47:17  lindner
 * Fix for AskBlock memory cruft
 *
 * Revision 3.24  1993/09/29  20:52:47  lindner
 * Add [space to cycle] prompt earlier
 *
 * Revision 3.23  1993/09/26  09:19:19  lindner
 * Add bold #defines
 *
 * Revision 3.22  1993/09/22  19:58:15  lindner
 * Fix for extra cruft on screen from Select/Choose
 *
 * Revision 3.21  1993/09/22  04:13:44  lindner
 * none
 *
 * Revision 3.20  1993/09/22  01:15:44  lindner
 * Add support for DEC HELP key/KEY_HELP
 *
 * Revision 3.19  1993/09/21  02:42:18  lindner
 * Exit on error reading from terminal (kills spinning processes)
 *
 * Revision 3.18  1993/09/21  01:46:06  lindner
 * Implement all remaining ASK block items..
 *
 * Revision 3.17  1993/09/11  06:31:33  lindner
 * spelling correction
 *
 * Revision 3.16  1993/09/11  04:51:00  lindner
 * Fix to skip labels in CURrequest()
 *
 * Revision 3.15  1993/09/03  03:28:59  lindner
 * Modified CURGetOneOption() to have a Title field.  Serveral other
 * routines modified for improved line editing and to show all available
 * commands while answering prompts.  If dialog box has only one prompt,
 * as for search query entries, start user entry on new line so more of
 * the screen is available for the entry (useful for search terms with
 * booleans).
 *
 * Revision 3.14  1993/08/19  20:22:45  lindner
 * Mitra's Debug patch
 *
 * Revision 3.13  1993/08/16  18:17:01  lindner
 * Fixes from F.Macrides:
 *
 * Added temporary code to work around DECC/AXP's problems with screen
 * clearing and cursor homing (we'll get rid of that code if the problem
 * goes away in the next version of DECC/AXP).  It's bolding via
 * standout() or wstandout(win) still doesn't work, but that's not a
 * serious functional problem for gopher users on Alphas.
 *
 * Added exit block to ensure that the terminal characteristics are
 * retored and cleanups are done on VMS.
 *
 * Added code for getting terminal characteristics
 * from the terminal table on VMS.
 *
 * Replaced/modified Cruft for VMS with routines which handle both
 * Control C and Control Y, enable use of ReallyQuit(), and restore all
 * original terminal characteristics for spawns and intentional or
 * unintentional exits.  Did it in a way that should stay transparent to
 * the otherwise "for-Unix" code, and should hold up with future mods or
 * enhancements of that code.  Standout() doesn't work at all and
 * endwin() and delwin() are unreliable on Alphas (due to bugs in the
 * Alpha's Curses library).  Andrew Heyler is looking into workarounds,
 * should DEC not fix the bugs soon.  Code compiles with DECC on Alphas
 * without warnings or error messages, but still get lots of
 * "informational" messages due to incomplete prototyping (no problems or
 * compiler messages with VAXC).
 *
 * Revision 3.12  1993/08/12  06:32:06  lindner
 * Add needed variable
 *
 * Revision 3.11  1993/08/09  20:44:48  lindner
 * Fix for really long strings
 *
 * Revision 3.10  1993/08/09  20:29:50  lindner
 * Get rid of the beep during a ^G in CURwgetstr().
 *
 * Make CURChoice() delete its window when it exits.
 *
 * Revision 3.9  1993/07/30  17:31:34  lindner
 * Mods to support AskP:
 *
 * Revision 3.8  1993/07/27  02:02:47  lindner
 * More comments
 *
 * Revision 3.7  1993/07/23  04:33:48  lindner
 * Mods to curchoice for default
 *
 * Revision 3.6  1993/07/20  23:10:17  lindner
 * none
 *
 * Revision 3.5  1993/04/15  21:23:36  lindner
 * Removed extraneous wattron/wattroff
 *
 * Revision 3.4  1993/03/26  19:42:32  lindner
 * Fix for skip by two problem in CURrequest
 *
 * Revision 3.3  1993/03/18  23:15:24  lindner
 * Mods to support titles inside of a CURrequest.
 *
 * Revision 3.2  1993/02/16  23:26:02  lindner
 * Fixes for SIG_ERR (for Crays)
 *
 * Revision 3.1.1.1  1993/02/11  18:02:56  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.8  1993/01/11  20:25:31  lindner
 * Fixed weird wprintw error on EP/IX.
 *
 * Revision 1.8  1993/01/11  20:25:31  lindner
 * Fixed weird wprintw error on EP/IX.
 *
 * Revision 1.7  1993/01/09  02:16:21  lindner
 * Changed (void*)-1 constructs to SIG_ERR
 *
 * Revision 1.6  1993/01/09  01:28:11  lindner
 * Replaced hosed Log messages (Ooops!)
 *
 * Revision 1.5  1993/01/09  01:24:42  lindner
 * Added CURchoice(), let's you choose one option from [2-9] items.
 *
 * Revision 1.4  1993/01/09  00:49:16  lindner
 * More mods for VMS from jqj.  Looks like better ctrl-y and ctrl-c
 * processing.
 *
 * Revision 1.3  1993/01/06  17:05:46  lindner
 * Added nl() to CURexit() for EP/IX machines.
 *
 * Revision 1.2  1992/12/31  05:55:44  lindner
 * Mods for VMS
 *
 * Revision 1.1  1992/12/10  06:16:51  lindner
 * Initial revision
 *
 *
 *********************************************************************/

#include "CURcurses.h"
#include "gopher.h"
#include "Malloc.h"

#include <signal.h>
#ifndef SIG_ERR
#define SIG_ERR ((void *) -1)
#endif
#include "Stdlib.h"
#include "compatible.h"
#include "Debug.h"
#include "String.h"
#include "util.h"

#ifdef VMS
static int w_getch();
#undef wgetch
#define wgetch w_getch
void setterm_pas();
void resetterm();
void VMSCURinit(/* CursesObj* */);
void VMSinit();
int VMSVersion(/* char *, int */);
void VMSexit();
int spawn_DCLprocess();
unsigned int DCLspawn_exception();
#endif /* VMS */

#include <curses.h>
#ifdef HAVE_TERM_H
#include <term.h>
#endif

#ifdef CTRLCPROMPTS
#  define DIALOGCANCELSTR "Cancel - ^C"
#  define HELPCANCEL      "^C      : Cancel"
#  define HELP2CANCEL     "^C        : Cancel"
#  define FORMCANCEL      "[Help: ^-]  [Cancel: ^C] "
#else 
#  define DIALOGCANCELSTR "Cancel - ^G"
#  define HELPCANCEL      "^G      : Cancel"
#  define HELP2CANCEL     "^G        : Cancel"
#  define FORMCANCEL      "[Help: ^-]  [Cancel: ^G] "
#endif /*NEWCTRLC*/


#ifdef CONTROLX
#  define CXPROMPT1    "^X      : Accept"
#else
#  define CXPROMPT1    "Enter   : Accept"
#endif

/*
 * Do we use wordwrap?
 */

boolean wordwrap = 0;


/*
 * Initialize data space for various screen information
 */

CursesObj *
CURnew(void)
{
     CursesObj *cur;

     cur = (CursesObj *) malloc(sizeof(CursesObj));

     cur->Screen       = NULL;
     cur->Termtype     = STRnew();
     cur->Clearscreen  = STRnew();
     cur->AudibleBell  = STRnew();
     cur->Highlighton  = STRnew();
     cur->Highlightoff = STRnew();

     cur->canUseACS    = FALSE;
     cur->inCurses     = FALSE;
     cur->sigtstp      = SIG_ERR;
     cur->sigwinch     = SIG_ERR;

     CURinit(cur);
     
     return(cur);

}


/*
 * Initialize various strings and such. 
 */

void
CURinit(CursesObj *cur)
{
#ifdef VMS
     VMSCURinit(cur);
#else
     int err;
     static char terminal[1024];
     static char capabilities[1024];   /* String for cursor motion */
     static char *ptr = capabilities;  /* for buffering         */
     char *cp;


     /*** Set the terminal type ***/
     if (getenv("TERM") != NULL)
	  CURsetTerm(cur, getenv("TERM"));
     else 
	  CURsetTerm(cur, "unknown");

     err = tgetent(terminal, CURgetTerm(cur));
     
     if (err !=1)
	  CURsetTerm(cur, "unknown");

     /*** Get the clearscreen code ***/
     if ((cp = (char *)tgetstr("cl", &ptr)) != NULL)
	  CURsetCLS(cur, cp);
     else
	  CURsetCLS(cur, "");

     /*** Set the bell ***/
     if ((cp = (char *) tgetstr("bl", &ptr)) != NULL)
	  CURsetBell(cur, cp);
     else
	  CURsetBell(cur, "\007");

     /*** Set the highlight codes ***/
     if ((cp = (char *) tgetstr("so", &ptr)) != NULL) {
	  CURsetHighon(cur, cp);
	  if ((cp = (char *) tgetstr("se", &ptr)) != NULL)
	       CURsetHighoff(cur, cp);
     } else {
	  CURsetHighon(cur, "");
	  CURsetHighoff(cur, "");
     }

     /*** Does terminal support alternate character set? ***/
     if ((char *) tgetstr("as", &ptr) != NULL) 
	  cur->canUseACS = TRUE;

     CURsetScreen(cur,initscr());
#endif
     cur->inCurses = FALSE;

     CURsetGraphicsChars(cur, CURgetTerm(cur));
}

/*
 * Set the characters to output depending on the terminal type...
 * 
 * Most systems don't do it right..
 */

static char *CURgraphicChars[] = {
     "vtxxx",  "qxlkmjwvtun",
     "xterm",  "qxlkmjwvtun",
     "z29",    "aqfcedsuvtb",
     "wyse50", "zvrsqupwtyx",
     "isc",    "D3Z?@Y+++++",
     "sun-cmd","-|+++++++++",
     NULL,     NULL
};

void
CURsetGraphicsChars(CursesObj *cur, char *termtype)
{
     cur->Box_hline  = '-';
     cur->Box_vline  = '|';
     cur->Box_ul     = '+';
     cur->Box_ur     = '+';
     cur->Box_ll     = '+';
     cur->Box_lr     = '+';
     cur->Box_tt     = '+';
     cur->Box_bt     = '+';
     cur->Box_lt     = '+';
     cur->Box_rt     = '+';
     cur->Box_ct     = '+';

#if defined(A_ALTCHARSET) && !defined(ultrix) && !defined(__osf__)
     /** use ALTCHARSET if termcap or terminfo knows how **/
     if (cur->canUseACS) {

	  char *term, *chars;
	  char termname[20];
	  int count=0;

	  strcpy(termname, termtype);

	  /** VT100 terminals and above support alternate character set **/
	  if (strncasecmp(termname, "vt1", 3) == 0 ||
	      strncasecmp(termname, "vt2", 3) == 0 ||
	      strncasecmp(termname, "vt3", 3) == 0)
	       strcpy(termname, "vtxxx");  /* name in CURgraphicChars[] */

	  while ((term = CURgraphicChars[count++]) != NULL) {
	       chars = CURgraphicChars[count++];
	       if (strcmp(termname, term) == 0) {
#ifdef ACS_BSSB
		    cur->Box_hline  = ACS_HLINE;
		    cur->Box_vline  = ACS_VLINE;
		    cur->Box_ul     = ACS_BSSB;
		    cur->Box_ur     = ACS_BBSS;
		    cur->Box_ll     = ACS_SSBB;
		    cur->Box_lr     = ACS_SBBS;
		    cur->Box_tt     = ACS_TTEE;
		    cur->Box_bt     = ACS_BTEE;
		    cur->Box_lt     = ACS_LTEE;
		    cur->Box_rt     = ACS_RTEE;
		    cur->Box_ct     = ACS_PLUS;
#else /* ACS_BSSB */
		    cur->Box_hline  = A_ALTCHARSET|chars[0];
		    cur->Box_vline  = A_ALTCHARSET|chars[1];
		    cur->Box_ul     = A_ALTCHARSET|chars[2];
		    cur->Box_ur     = A_ALTCHARSET|chars[3];
		    cur->Box_ll     = A_ALTCHARSET|chars[4];
		    cur->Box_lr     = A_ALTCHARSET|chars[5];
		    cur->Box_tt     = A_ALTCHARSET|chars[6];
		    cur->Box_bt     = A_ALTCHARSET|chars[7];
		    cur->Box_lt     = A_ALTCHARSET|chars[8];
		    cur->Box_rt     = A_ALTCHARSET|chars[9];
		    cur->Box_ct     = A_ALTCHARSET|chars[10];
#endif /* ACS_BSSB */

	       }
	  }
     }
#endif
}



/* 
 * Given a properly "CURnew" cursesobj, initialize the screen..
 */

void
CURenter(CursesObj *cur)
{
     /* for safety */
     if (cur->inCurses == TRUE)
	  return;

#ifdef VMS
     (void)setterm_pas();
     CURsetScreen(cur,initscr());
#else
     tputs(CURgetCLS(cur),1,CURoutchar);
     fflush(stdout); 
#endif

     cur->inCurses = TRUE;

     CURwenter(cur,stdscr);

#ifdef SIGWINCH
     if (cur->sigwinch != SIG_ERR)
	  signal(SIGWINCH, cur->sigwinch);
#endif
#ifdef SIGTSTP
     if (cur->sigtstp != SIG_ERR)
	  signal(SIGTSTP, cur->sigtstp);
#endif
}

/*
 * Set up processing for the window (especially for system V curses!
 */

void
CURwenter(CursesObj *cur, WINDOW *win)
{
     cbreak();
     noecho();
     nonl();
#ifdef SYSVCURSES
     intrflush(win, FALSE);
     nodelay(win, FALSE);
#ifndef ultrix			/** Causes wgetch to dump core in ultrix **/
#ifndef _SEQUENT_     
     keypad(win, TRUE);
#endif
#endif
#endif
}     
/*
 * Exit curses system.
 */

void
CURexit(CursesObj *cur)
{
     
     if (!cur->inCurses)
	return;
     cur->inCurses = FALSE;
     echo();
     nl();
     endwin();

#ifdef SYSVCURSES
#ifndef  _SEQUENT_
     keypad(stdscr, FALSE);
#endif
#endif

#ifdef VMS
     (void)resetterm();
#endif

#ifdef SIGTSTP
     tputs(CURgetCLS(cur),1,CURoutchar);
     fflush(stdout);
 
     cur->sigtstp = signal(SIGTSTP, SIG_DFL);
#endif

#ifdef SIGWINCH
     cur->sigwinch = signal(SIGWINCH, SIG_DFL);
#endif

}



/*
 * send a character to stdout, not really curses, but we do use it...
 */

int
CURoutchar(int c)
{
     /** output the given character.  From tputs... **/
     /** Note: this CANNOT be a macro!              **/
     
     putc(c, stdout);
     return(c);
}

/*
 * Rightline, uses curses routines to center a line.
 */
void CURrightline(CursesObj *cur, char *theline, int yval)
{
     mvaddstr(yval, (COLS - strlen(theline) -1), theline);
}




/*
 * Centerline, uses curses routines to center a line.
 */
void CURcenterline(CursesObj *cur, WINDOW *win, char *theline, int yval, int windowidth, boolean bright)
{
     char TrimmedLine[256];

     strcpy(TrimmedLine, theline);

     /** Make sure theline fits in the window **/
     if (strlen(theline) > windowidth) {
	  TrimmedLine[windowidth-3] = '.';
	  TrimmedLine[windowidth-2] = '.';
	  TrimmedLine[windowidth-1] = '.';
	  TrimmedLine[windowidth] = '\0';
     }

     wmove(win, yval, (windowidth - strlen(TrimmedLine))/2);

     if (bright)
	  wstandout(win);

     waddstr(win, TrimmedLine);

     if (bright)
	  wstandend(win);
}


/*
 * CURwgetstr is a replacement of getstr that allows editing of the string
 * if the user types control codes we don't recognize, it's returned instead
 * We assume that the incoming string is shorter than the max..
 */


int
CURwgetstr(CursesObj *cur, WINDOW *win, char *inputline, int maxlength, boolean hidden)
{
     int pointer = 0;
     int curpointer = 0;
     int ch;
     int y,x;

     wstandout(win);

     /*** Check to see if there's something in the inputline already ***/
     while (inputline[pointer] != '\0') {
	  if (hidden)
	       waddch(win, '*');
	  else
	       waddch(win, inputline[pointer]);
	  pointer ++;
	  curpointer ++;
     }

     wrefresh(win);

     for (;;) {
	  ch = CURwgetch(cur,win);

	  switch (ch) {

	  case '\n':
#ifdef CONTROLX
	  case '\030':
#endif
	       inputline[pointer] = '\0';
	       wstandend(win);
	       return(ch);
	       break;

	  case '\b':  /*** backspace and delete ***/
	       if (curpointer > 0) {
		    char *cp;

		    getyx(win, y, x);
		    wmove(win, y, x-1);
		    
		    /* Update the string */

		    for (cp = inputline + curpointer - 1; *cp != '\0'; cp++) {
			 *cp = *(cp + sizeof(char));
			 if (*cp != '\0') {
			      if (hidden)
				   waddch(win, '*');
			      else
				   waddch(win, *cp);
			 }
		    }
		    *cp = '\0';
		    waddch(win, ' ');

		    pointer--;
		    curpointer--;

		    wmove(win, y, x-1);
		    wrefresh(win);
	       }
	       break;

	  case '\001':  /*** ^A go to beginning of line ***/
	       getyx(win, y, x);
	       wmove(win, y, x-curpointer);
	       curpointer = 0;
	       wrefresh(win);
	       break;

	  case '\005':  /*** ^E go to end of line ***/
	       getyx(win, y, x);
	       wmove(win, y, x+pointer-curpointer);
	       curpointer = pointer;
	       wrefresh(win);
	       break;

	  case '\007':  /*** ^G cancel... ***/
	       wstandend(win);
	       return(-1);
	       break;
       
	  case '\013':  /*** ^K delete to end of line ***/
	       getyx(win, y, x);

	       while (curpointer < pointer) {
		    waddch(win, ' ');
		    pointer--;
	       }
	       inputline[pointer] = '\0';
	       wmove(win, y, x);
	       wrefresh(win);
	       break;

	  case '\025':  /*** ^U erase whole line ***/
	       getyx(win, y, x);
	       wmove(win, y, x-curpointer);

	       while (pointer > 0) {
		    waddch(win, ' ');
		    pointer--;
	       }
	       inputline[pointer] = '\0';
	       wmove(win, y, x-curpointer);
	       curpointer = 0;
	       wrefresh(win);
	       break;

	  case KEY_LEFT:
	       if (curpointer > 0) {
		    curpointer--;
		    getyx(win, y, x);
		    wmove(win, y, x-1);
		    wrefresh(win);
	       }
	       break;

	  case KEY_RIGHT:
	       if (curpointer < pointer) {
		    curpointer++;
		    getyx(win, y, x);
		    wmove(win, y, x+1);
		    wrefresh(win);
	       }
	       break;

	  default:
	       if (ch < 256 && isprint(ch) && curpointer >= maxlength) {
		    /* Return at the end for word wrapping */
		    if (wordwrap) {
			 /*inputline[curpointer] = ch;*/
			 inputline[curpointer] = '\0';
			 wstandend(win);
			 wrefresh(win);
			 return(ch);
		    } else
			 CURBeep(cur);
	       }

	       else if (ch < 256 && isprint(ch)) {

		    inputline[curpointer++] = ch;

		    if (curpointer > pointer) {
			 pointer = curpointer;
			 inputline[curpointer+1] = '\0';
		    }
		    if (hidden)
			 waddch(win, '*');
		    else
			 waddch(win, ch);
		    wrefresh(win);
	       }
	       else {
		    wstandend(win);
		    return(ch);
	       }
	  } /* switch */
     } /* for */
}


/*
 * This stuff is stolen and modified from hytelnet  Thanks Earl!
 */

int
CURwgetch(CursesObj *cur, WINDOW *window)
{
     int a, b, c;

     if (ControlCpressed) {
	  return ('\007');  /** Control-C cancels us **/
     }
     
     if (signal(SIGINT, controlcJmp) == SIG_ERR)
	  perror("signal died:\n"), exit(-1);
     
     if (setjmp(Jmpenv)) {
	  /* Note controlcJmp will reprime controlc and set flag*/
	  Debug("interruptable_wgetch triggered\r\n",NULL);
	  ControlCpressed = FALSE;	/* Deal with return code, not flag*/
	  return('\007');
     }

     
     while (1) {

	  c = wgetch(window);
	  reprimeControlc();

	  switch (c) {

	  case -1:  /* error usually means connection was closed.. */
	       CleanupandExit(-1);
	       break;

	  case 12:  /* ^L */
	  case 18:  /* ^R */
	  case 23:  /* ^W */
	       /* redraw the screen */
#ifdef VMS
	       clearok(curscr, TRUE);
	       wrefresh(window);
#else
	       wrefresh(curscr);
#endif
	       /* don't return yet, get another keystroke */
	       break;

	  case 27:  /* handle escape sequence */
	       b = wgetch(window);
	       if (b == '[' || b == 'O')
		    a = wgetch(window);
	       else
		    a = b;

	       switch (a) {
	       case 'A': c = KEY_UP; break;
	       case 'B': c = KEY_DOWN; break;
	       case 'C': c = KEY_RIGHT; break;
	       case 'D': c = KEY_LEFT; break;

	       case 'M':                   /* vt100 enter */
		    if (b == 'O')
			c = KEY_ENTER;
		    break;

	       case 'Q':                   /* vt100 Help */
		    if (b == 'O')
			c = KEY_HELP;
		    break;

	       case '2':
		    if (b == '[')
			b = wgetch(window);
		    switch(b) {
		    case '8':             /* vt200 Help */
			if (wgetch(window) == '~')
			    c = KEY_HELP;
			break;
		    case '9':             /* vt200 Do */
			if (wgetch(window) == '~')
			    c = KEY_ENTER;
			break;
		    }
	       
	       case '5':                       /* vt 200 prev. screen */
		    if (b == '[' && wgetch(window) == '~')
			c = KEY_PPAGE;
		    break;
	       case '6':                       /* vt 200 next screen */
		    if (b == '[' && wgetch(window) == '~')
			c = KEY_NPAGE;
		    break;
	       }
	       /* fall through to default to reassign certain keys */

	  default:
	       
	  /* The many forms of the return key... */
	  if ((c == KEY_ENTER)|| (c=='\r')) 
	       c = '\n'; /** SYSV curses Gack! **/

	  /* The many forms of backspace */
	  if (c == '\010' || c == '\177' || c == KEY_BACKSPACE)
	       return('\b');

	  return(c);
	  break;

	  }  /* switch(c) */
     }  /* while(1) */
}

int
CURgetch(CursesObj *cur)
{
   return(CURwgetch(cur, stdscr));
}  

/*
 * Resets the screen when a size change has happened
 */

void
CURresize(CursesObj *cur)
{
     if (cur->inCurses) {
	  CURexit(cur);
	  CURenter(cur);
     }
}

/*
 * Get one option displays a message, and gets a response
 *
 * If the Response has something in it, it is displayed and editable
 * 
 * If the user wants to abort, GetOneOption returns a -1, otherwise it
 * returns a 0
 */

int
CURGetOneOption(CursesObj *cur, char *Title, char *OptionName, char *Response)
{
     int i;
     Requestitem *items[2], item;

     items[0] = &item;
     items[1] = NULL;

     item.prompt = OptionName;
     item.stowage = Response;
     item.thing = CUR_PROMPT;

     i = CURrequester(cur, Title, items);
     
     refresh();
     return(i);
}

/*
 * This is the old version of GetOneOption, for those times when the
 * garsh darn terminal is just too gadblam slow :-)
 */
int
CUROldGetOneOption(CursesObj *cur, char *OptionName, char *Response)
{
     int i;
     
     mvaddstr(LINES-1, 0, OptionName);
     standout();
     addstr("    ");
     standend();
     clrtoeol();
     move(LINES-1, strlen(OptionName));
     
     refresh();
     i = CURwgetstr(cur, stdscr, Response, 4, FALSE);
     
     return(i);
}



/*
 * Fills in the Response with either a lowercase 'y' or 'n'
 */

void
CURgetYesorNo(CursesObj *cur, char *OptionName, char *Response)
{
     int c;
     int posx, posy;

     mvaddstr(LINES-1, 0, OptionName);
     clrtoeol();
     getyx(cur->Screen, posy, posx);
     addch(' ');

     if (*Response == 'y')
	  mvaddstr(posy, posx+1, "y");
     else {
	  *Response = 'n';
	  mvaddstr(posy, posx+1, "n ");
     }
     move(posy, posx+1);

     refresh();

     while (1) {
	  c = CURgetch(cur);

	  if (c == 'y') {
	       mvaddstr(posy, posx+1, Gtxt("Yes",180));
	       move(posy, posx+1);
	       refresh();
	       *Response = 'y';
	       *(Response +1) = '\0';
	       return;
	  }
	  else if (c == 'n') {
	       mvaddstr(posy, posx+1, Gtxt("No ",111));
	       move(posy, posx+1);
	       refresh();
	       *Response = 'n';
	       *(Response +1) = '\0';
	       return;
	  }
	  
	  else if ((c == '\n')||(c=='\r')) {
	       return;
	  }
#ifdef VMS
	  else if ( c == '\032' ) {	/* control-Z */
		return;
	  }
#endif
	  else {
	       Debugmsg("CURgetYesorNo beep\r\n")
	       CURBeep(cur);
	  }
     }
}
	  
void 
CURBeep(CursesObj *cur)
{
	Debugmsg("CURBeep\r\n")
#ifdef SYSVCURSES
     beep();
#else
/*   CURcenterline(cur, stdscr, CURgetBell(cur), 1, COLS, FALSE); */
     tputs(CURgetBell(cur), 1, CURoutchar);
     fflush(stdout);
#endif
}


void
CURbox(CursesObj *cur, WINDOW *win, int height, int width)
{
     int i;

     wmove(win, 0, 0);
     waddch(win, CURgetBox_ul(cur));
     for (i=0; i<width-2; i++)
	  waddch(win, CURgetBox_hline(cur));
     waddch(win, CURgetBox_ur(cur));

     for (i=1; i<height-1; i++) {
	  wmove(win, i, 0);
	  waddch(win, CURgetBox_vline(cur));
	  wmove(win, i, width-1);
	  waddch(win, CURgetBox_vline(cur));
     }

     wmove(win, height-1, 0);
     waddch(win, CURgetBox_ll(cur));
     for (i=0; i<width-2; i++)
	  waddch(win, CURgetBox_hline(cur));
     waddch(win, CURgetBox_lr(cur));
}


void
CURbutton(CursesObj *cur, WINDOW *win, char *Label, boolean bright)
{
/*     wboldout(win);*/

     if (bright)
	  wstandout(win);

     waddstr(win, "[");
     waddstr(win, Label);
     waddstr(win, "]");

     if (bright)
	  wstandend(win);

/*     wboldend(win);*/
}     



int
CURDialog(CursesObj *cur, char *Wintitle, char **Message)
{
     int i;
     int messheight=0;
     int messlength=0;
     WINDOW *tempwin;
     int winwidth;

     while (Message[messheight] != NULL) {
	  if (strlen(Message[messheight]) > messlength)
	      messlength = strlen(Message[messheight]);
	  messheight++;
     }

     winwidth = max(31, messlength+6);
     winwidth = max(winwidth, strlen(Wintitle)+6);
     winwidth = min(winwidth, COLS-2);

     tempwin = newwin(5+messheight, winwidth, (LINES-(5+messheight))/2,
     			(COLS-winwidth)/2);

     if (tempwin == (WINDOW *)0)
	  return(-1);

     CURwenter(cur,tempwin);
     CURbox(cur, tempwin, 5+messheight, winwidth);

     /** Add the message **/
     for (i=0; i<messheight; i++) {
	  int len = strlen(Message[i]), j;

          wmove(tempwin, 2+i,(winwidth - messlength)/2);

	  if (len > messlength) {
	       for (j=0; j < messlength; j++)
		    waddch(tempwin, Message[i][j]);
	  }
	  else
	       waddstr(tempwin, Message[i]);

     }



     /** Add the window title, centered **/
     if (Wintitle != NULL)
	  CURcenterline(cur, tempwin, Wintitle, 0, winwidth, TRUE);

     /** Add the keyboard labels **/
     wmove(tempwin, 3+messheight, winwidth - 28);
     CURbutton(cur, tempwin, Gtxt(DIALOGCANCELSTR,211), FALSE);
     waddch(tempwin, ' ');
     CURbutton(cur, tempwin, Gtxt("OK: Enter",191), FALSE);

     wrefresh(tempwin);

     switch(CURwgetch(cur, tempwin)) {
     case -1:
     case '\007':
	  delwin(tempwin);
	  return(-1);
     default:
	  delwin(tempwin);
	  return(0);
     }
}

int
CURRequest(CursesObj *cur, char *Wintitle, char **Prompts, char **Stowages)
{
     Requestitem **items;
     int i, choice, numprompts = 0;

     while (Prompts[numprompts] != NULL)
	  numprompts++;

     items = (Requestitem **) malloc(sizeof(Requestitem *) * (numprompts+1));

     for (i = 0; i < numprompts; i++) {
	  items[i] = (Requestitem*) malloc(sizeof(Requestitem));
	  items[i]->prompt  = Prompts[i];
	  items[i]->stowage = Stowages[i];

	  if (Stowages[i] == NULL)
	       items[i]->thing = CUR_LABEL;
	  else
	       items[i]->thing = CUR_PROMPT;
     }
     items[i] = NULL;
     
     choice = CURrequester(cur, Wintitle, items);

     /** Free memory **/
     for (i=0; i <= numprompts; i++)
	  if (items[i])
	       free(items[i]);
     free(items);

     return(choice);
}

/* 
 * CURrequester takes an array of requestitems and does the right
 * things with it..  
 */

int
CURrequester(CursesObj *cur, char *Wintitle, Requestitem **items)
{
     int         BottomField;
     int         ch;
     Requestitem *curitem;
     int         currentfield = 0;
     int         FieldsPerPage;
     boolean     Found;
     int         i,j,k;
     int         maxlength;
     int         maxpromptwidth = 0;
     int         numprompts = 0;
     WINDOW      *tempwin;
     int         TopField;
     int         WinLines;
     int         WinWidth = COLS - 1;
     static char **RequestHelp = NULL;
     

     if (RequestHelp == NULL) {
	  RequestHelp = (char**) malloc(sizeof(char*) * 8);
	  RequestHelp[0] = Gtxt(HELPCANCEL,190);
	  RequestHelp[1] = Gtxt("Tab, ^N : Move to next field",192);
	  RequestHelp[2] = Gtxt("^P      : Move to previous field",193);
	  RequestHelp[3] = Gtxt("^F      : Display next page",194);
	  RequestHelp[4] = Gtxt("^B      : Display previous page",195);
	  RequestHelp[5] = Gtxt("^-      : Help (^/ or ^7 may work)",196);
	  RequestHelp[6] = Gtxt(CXPROMPT1,197);
	  RequestHelp[7] = NULL;
     }

     /*** Find the number of prompts... and the max width ***/
     while ((curitem = items[numprompts++]) != NULL) {

	  /*** Skip non editable prompts ***/
	  if (curitem->thing != CUR_LABEL) {
	       if (curitem->prompt && strlen(curitem->prompt) > maxpromptwidth)
		    maxpromptwidth = strlen(curitem->prompt);
	  } else {
	       if (currentfield == numprompts-1)
		    currentfield++;
	  }
     }

     numprompts--;
     if (numprompts == 0)
	  return(-1);

     TopField = 0;     
     FieldsPerPage = min(numprompts, LINES - 5); 
     BottomField = min(TopField + FieldsPerPage, numprompts) - 1;

     /** If only one prompt, offer an entire line for input **/
     if (numprompts == 1 && items[0]->thing == CUR_PROMPT) {
          WinLines = 8;
          maxlength = WinWidth - 4;
     }
     else {
          WinLines = FieldsPerPage + 5;
          maxlength = WinWidth - maxpromptwidth - 6;
     }

     /*** Create the new window ***/
     tempwin = newwin(WinLines, WinWidth, (LINES - WinLines) / 2, 0);
     CURwenter(cur, tempwin);
     CURbox(cur, tempwin, WinLines, WinWidth);
     
     /*** Add the window title, centered ***/
     if (Wintitle != NULL)
          CURcenterline(cur, tempwin, Wintitle, 0, WinWidth, TRUE);
     
     /*** Display the form and allow the user to update it ***/
     while (1) {
          /*** Add the labels ***/
          wmove(tempwin, WinLines - 2, 2);
          waddstr(tempwin, Gtxt(FORMCANCEL,198));
          if (items[currentfield]->thing == CUR_CHOICE)
               waddstr(tempwin, Gtxt("  [Cycle Values: Space]  [List Values: l]",199));
          else {
#ifdef CONTROLX
	       waddstr(tempwin, " [Accept: ^X] ");
#else
	       waddstr(tempwin, " [Accept: Enter] ");
#endif
	       waddstr(tempwin, " [Next field: TAB] ");
	       waddstr(tempwin, "           ");
	  }


          /** Add the prompts and typing area **/
          if (numprompts == 1 && items[0]->thing == CUR_PROMPT) {
	       wmove(tempwin, 2, 2);
               waddstr(tempwin, items[0]->prompt);

	       /** Trim the stowage if necessary **/
	       if ((j = strlen(items[0]->stowage)) > maxlength)
	            while (j > maxlength)
	                 (items[0])->stowage[--j] = '\0';

	       /** Add the stowage if it exists and its black space **/
	       wmove(tempwin, 4, 2);
	       wstandout(tempwin);
	       waddstr(tempwin, items[0]->stowage);
	       for (j=strlen(items[0]->stowage)+2; j < WinWidth-2; j++)
	            waddch(tempwin, ' ');
	       wstandend(tempwin);
          }
          else {
               i = 0;
               for (k = TopField; k <= BottomField ; k++) {
	            wmove(tempwin, 2 + i, 2);
		    if (items[k]->prompt) {
			 
			 waddstr(tempwin, items[k]->prompt);
	  
			 for (j = WinWidth-strlen(items[k]->prompt)-4; j>0;j--)
			      waddch(tempwin, ' ');
		    }

	            switch (items[k]->thing) {

	            case CUR_LABEL:
		         break;
		    
	            case CUR_CHOICE:
		         /** Add the default **/
	                 wmove(tempwin, 2 + i, maxpromptwidth + 4);
		         waddstr(tempwin, 
                              items[k]->choices[items[k]->chooseitem]);
		         break;
			 
		    case CUR_ASKL:
			 wordwrap = TRUE;
			 wmove(tempwin, 2 + i, 2);
	                 wstandout(tempwin);
			 waddstr(tempwin, items[k]->stowage);
			 for (j = WinWidth - strlen(items[k]->stowage) - 4
			      ; j > 0; j--)
		              waddch(tempwin, ' ');

	                 wstandend(tempwin);
			 break;

	            default:
	                 /** Add the black space for the stowage, **/
	                 /** and the stowage, if it exists        **/
			 wordwrap = FALSE;
	                 wmove(tempwin, 2 + i, maxpromptwidth + 4);
	                 wstandout(tempwin);

	                 if (items[k]->thing == CUR_PASSWD)
			      for (j = strlen(items[k]->stowage); j > 0; j--)
			           waddch(tempwin, '*');
	                 else if (items[k]->stowage)
		              waddstr(tempwin, items[k]->stowage);



			 for (j = WinWidth - 
			      (items[k]->stowage ? strlen(items[k]->stowage):0)
			      - maxpromptwidth - 6; j > 0; j--)
		              waddch(tempwin, ' ');

	                 wstandend(tempwin);
	            }
                    i++;
	       }
          }

          if (numprompts == 1)
	       wmove(tempwin, 4, 2);
	  else 
	       wmove(tempwin, 2 + currentfield - TopField, maxpromptwidth + 4);
	  
	  if (items[currentfield]->thing == CUR_ASKL)
	       wmove(tempwin, 2 + currentfield - TopField, 2);

	  wrefresh(tempwin);

	  if (items[currentfield]->thing == CUR_CHOICE) {
	       int choice = items[currentfield]->chooseitem;
	       boolean done = FALSE;

	       wmove(tempwin, 2 + currentfield - TopField, maxpromptwidth +
		    strlen(items[currentfield]->choices[choice]) + 4);
	       
	       wrefresh(tempwin);

	       while (!done) {
		    ch = CURwgetch(cur, tempwin);

		    wmove(tempwin, 2 + currentfield - TopField, 
                         maxpromptwidth + 4);
		    for (i=strlen(items[currentfield]->choices[choice]);
			 i>0; i--)
			 waddch(tempwin, ' ');

		    if (ch == ' ') {
		    
			 choice++;
			 
		    }
		    else if (ch == 'l') {
			 int tempx;
			 tempx = CURChoice(cur, items[currentfield]->prompt,
					   items[currentfield]->choices,
					   Gtxt("Select an item",200),
					   items[currentfield]->chooseitem);
			 touchwin(tempwin);
			 if (tempx != -1)
			      choice = tempx;
		    }

		    else
			 done = TRUE;

		    if (items[currentfield]->choices[choice] == NULL)
			 choice = 0;
		    
		    wmove(tempwin, 2 + currentfield - TopField, 
                         maxpromptwidth + 4);
		    
		    waddstr(tempwin, items[currentfield]->choices[choice]);
		    items[currentfield]->chooseitem = choice;
		    wrefresh(tempwin);
	       }
	  } 
          else {
	       int zemax = maxlength;

	       if (items[currentfield]->thing == CUR_ASKL)
		    zemax = COLS - 6;
		    
	       ch = CURwgetstr(cur,tempwin,items[currentfield]->stowage,
                    zemax, items[currentfield]->thing == CUR_PASSWD);
	  }

#ifdef CONTROLX
	  if (ch == '\n') {
	       if (numprompts > 1) {
		    ch = '\t';	   /* Treat as tab, next field */
	       } else {
		    ch = '\030';      /* Treat as control-X, accept */
	       }
	  }
#endif


	  switch (ch) {
          case '\016':
	  case '\t':
	       /*** Move to next non-label field (tab) ***/
	       do {
                    currentfield++;
                    if (currentfield > numprompts - 1) {
                         TopField = 0;
                         BottomField = min(TopField + FieldsPerPage, 
                                           numprompts) - 1;
                         currentfield = TopField;
                    }
                    else if (currentfield > BottomField) {
                         TopField++;
                         BottomField++;
                    }
	       } while (items[currentfield]->thing == CUR_LABEL);
	       break;

          case '\020':
               /*** Move to the previous non-label field (back tab) ***/
               do {
                    currentfield--;
                    if (currentfield < 0) {
                         currentfield = numprompts - 1;
                         TopField = max(0, numprompts - FieldsPerPage);
                         BottomField = currentfield;
                    }
                    else if (currentfield < TopField) {
                         TopField--;
                         BottomField--;
                    }
               } while (items[currentfield]->thing == CUR_LABEL);
               break;

          case KEY_DOWN:
               /*
                * Move to the next non-label field on the current
                * screen.  If there is no such field, scroll down
                * one line.  If a non-label field is found, move
                * to it, otherwise don't move. (Line down)
                */
               if (currentfield == BottomField) {
                    TopField++;
                    BottomField++;
                    if (BottomField > numprompts - 1) {
                        TopField = 0;
                        BottomField = min(TopField + FieldsPerPage,
                                          numprompts) - 1;
                        currentfield = TopField;
                        while (items[currentfield]->thing == CUR_LABEL)
                             currentfield++;
                    }
                    else if (items[BottomField]->thing != CUR_LABEL)
                        currentfield = BottomField;
               }
               else {
                    Found = FALSE;
                    for (i = currentfield + 1; i <= BottomField; i++)
                         if (items[i]->thing != CUR_LABEL) {
                              currentfield = i;
                              Found = TRUE;
                              break;
                         }
                    if (!Found) {
                         TopField++;
                         BottomField++;
                         if (BottomField > numprompts - 1) {
                              TopField = 0;
                              BottomField = min(TopField + FieldsPerPage,
                                                numprompts) - 1;
                              currentfield = TopField;
                              while (items[currentfield]->thing == CUR_LABEL)
                                   currentfield++;
                         }
                         else if (items[BottomField]->thing != CUR_LABEL)
                              currentfield = BottomField;
                    }
               }
               break;

	  case KEY_UP:
               /*
                * Move to the previous non-label field on the current
                * screen.  If there is no such field, scroll up
                * one line.  If a non-label field is found, move
                * to it, otherwise don't move. (Line up)
                */
               if (currentfield == TopField) {
                    TopField--;
                    BottomField--;
                    if (TopField < 0) {
                        TopField = max(0, numprompts - FieldsPerPage);
                        BottomField = numprompts - 1;
                        currentfield = BottomField;
                        while (items[currentfield]->thing == CUR_LABEL)
                             currentfield--;
                    }
                    else if (items[TopField]->thing != CUR_LABEL)
                        currentfield = TopField;
               }
               else {
                    Found = FALSE;
                    for (i = currentfield - 1; i >= TopField; i--)
                         if (items[i]->thing != CUR_LABEL) {
                              currentfield = i;
                              Found = TRUE;
                              break;
                         }
                    if (!Found) {
                         TopField--;
                         BottomField--;
                         if (TopField < 0) {
                              TopField = max(0, numprompts - FieldsPerPage);
                              BottomField = numprompts - 1;
                              currentfield = BottomField;
                              while (items[currentfield]->thing == CUR_LABEL)
                                   currentfield--;
                         }
                         else if (items[TopField]->thing != CUR_LABEL)
                              currentfield = TopField;
                    }
               }
	       break;

          case '\006':
          case KEY_NPAGE:
               /*** Move down a page ***/
               if (BottomField == numprompts - 1) {
                    currentfield = BottomField;
                    while (items[currentfield]->thing == CUR_LABEL)
                         currentfield--;
               }
               else {
                    BottomField = min(BottomField + FieldsPerPage, 
                                      numprompts) - 1;
                    TopField = max(0, BottomField - FieldsPerPage + 1);
                    if (currentfield < TopField) {
                         currentfield = TopField;
                         while (items[currentfield]->thing == CUR_LABEL)
                              currentfield++;
                    }
               }
               break;

          case '\002':
          case KEY_PPAGE:
               /*** Move up a page ***/
               if (TopField == 0) {
                    currentfield = 0;
                    while (items[currentfield]->thing == CUR_LABEL)
                         currentfield++;
               }
               else {
                    TopField = max(0, TopField - FieldsPerPage + 1);
                    BottomField = min(TopField + FieldsPerPage, numprompts) - 1;
                    if (currentfield > BottomField) {
                         currentfield = BottomField;
                         while (items[currentfield]->thing == CUR_LABEL)
                              currentfield--;
                    }
               }
               break;

          case '\037':
          case KEY_HELP:
               CURDialog(cur, "Form Help", RequestHelp);
               touchwin(tempwin);
               break;
	       
	  case '\007':
	  case -1:
	       /*** Cancel ***/
	       delwin(tempwin);
	       return(-1);
	       
#ifdef CONTROLX
	  case '\030':
#else
	  case '\n':
#endif
	       delwin(tempwin);
	       return(0);

          default:
	       if (wordwrap) {
		    char *p;
		    if ( currentfield == numprompts-1){
			 CURBeep(cur);
			 continue;
		    }
		    if (items[currentfield+1]->thing != CUR_ASKL ) {
			 CURBeep(cur);
			 continue;
		    }

		    /* we must go and find the last space in stowage and move
		       to next line */
		    p=strrchr(items[currentfield]->stowage,' ');

		    /* copy rest of string to next line */
		    if ( p != NULL ) {
			 char addit[2];
			 *addit = ch;
			 *(addit+1) = '\0';
			 strcat(p+1, addit);

			 strcpy(items[currentfield+1]->stowage,p+1);
			 
			 /* now blank out those characters */
			 *p='\0';
			 wmove(tempwin, 2 + currentfield - TopField, 2); 
			 wstandout(tempwin);
			 waddstr(tempwin, items[currentfield]->stowage);
			 for (j=strlen(items[currentfield]->stowage)+
			      maxpromptwidth+4; j < COLS-6; j++)
			      waddch(tempwin, ' ');
			 wstandend(tempwin);
		    }
		    currentfield++;	/* go to next ASKL field */

                    if (currentfield > BottomField) {
                         TopField++;
                         BottomField++;

                    }

	       }
	  }
     }
}


/*
 * CURChoice takes a bunch of titles, throws them on the screen,
 * and asks the user to choose one.
 *
 * Returns the number chosen, or -1 if the user cancels.
 *
 * Limitation: The number of choices must be less than 100.
 */


int
CURChoice(CursesObj *cur, char *WinTitle, char **Choices, char *Prompt, int DefaultChoice)
{
     int    BottomChoice;
     int    ch;
     int    ChoicesPerPage;
     int    CurrentChoice = 0;
     int    CurrentLine;
     int    CurrentPage;
     int    i;
     char   InBuf[3];
     int    j;
     int    k;
     int    MaxChoiceWidth = 0;
     int    NumChoices = 0;
     int    NumPages;
     WINDOW *TempWin;
     int    TopChoice;
     int    WinLines;
     int    WinWidth;
     static char ** ChoiceHelp = NULL;

     if (ChoiceHelp == NULL) {
	  ChoiceHelp = (char**) malloc(sizeof(char*) * 10);
	  ChoiceHelp[0] = Gtxt(HELP2CANCEL,202);
	  ChoiceHelp[1] = Gtxt("Down, ^N  : Move to next choice",203);
	  ChoiceHelp[2] = Gtxt("Up, ^P    : Move to previous choice",204);
	  ChoiceHelp[3] = Gtxt("Space, ^F : Display next page",205);
	  ChoiceHelp[4] = Gtxt("b, ^B     : Display previous page",206);
	  ChoiceHelp[5] = Gtxt("^         : Display first page",207);
	  ChoiceHelp[6] = Gtxt("$         : Display last page",208);
	  ChoiceHelp[7] = Gtxt("0-9       : Select a specific choice",209);
	  ChoiceHelp[8] = Gtxt("Enter     : Select current choice",210);
	  ChoiceHelp[9] = NULL;
     }


     /*** Determine number of choices and verify it's within limits ***/
     while (Choices[NumChoices] != NULL)
          NumChoices++;
     if (NumChoices == 0)
          return(-1);
     if (NumChoices > 99) {
          CursesErrorMsg("More than 99 choices!  Please gripe.");
          return(-1);
     }

     ChoicesPerPage = min(NumChoices, LINES - 6);
     NumPages = NumChoices / ChoicesPerPage + 1;
     WinLines = ChoicesPerPage + 6;

     CurrentChoice = (DefaultChoice > -1) ? DefaultChoice : 0;
     CurrentPage = CurrentChoice / ChoicesPerPage;
     BottomChoice = min(CurrentPage * ChoicesPerPage + ChoicesPerPage, 
                        NumChoices) - 1;
     TopChoice = max(0, BottomChoice - ChoicesPerPage + 1);
     CurrentLine = CurrentChoice - TopChoice + 2;

     /*** Determine dialog box width ***/
     for (i = 0; i < NumChoices; i++)
          if (i == DefaultChoice)
               MaxChoiceWidth = max(strlen(Choices[i]) + 10, MaxChoiceWidth);
          else
               MaxChoiceWidth = max(strlen(Choices[i]), MaxChoiceWidth);

     WinWidth = max(strlen(Prompt) + 17, MaxChoiceWidth + 15);
     WinWidth = max(strlen(WinTitle) + 8, WinWidth);
     WinWidth = max(29, WinWidth);
     WinWidth = min(COLS - 2, WinWidth);

     /*** Create the new window ***/
     TempWin  = newwin(WinLines, WinWidth, 
                       (LINES - WinLines) / 2, (COLS - WinWidth) / 2);
     CURwenter(cur, TempWin);
     CURbox(cur, TempWin, WinLines, WinWidth);

     /*** Add the window title, centered ***/
     if (WinTitle != NULL)
          CURcenterline(cur, TempWin, WinTitle, 0, WinWidth, TRUE);

     /*** Add the prompt ***/
     wmove(TempWin, ChoicesPerPage + 3, 3);
     wprintw(TempWin, "%s (1-%d): ", Prompt, NumChoices);

     /*** Add the labels ***/
     wmove(TempWin, ChoicesPerPage + 4, 3);
     CURbutton(cur, TempWin, Gtxt("Help: ?",96), FALSE);
     waddstr(TempWin, "  ");
     CURbutton(cur, TempWin, Gtxt(DIALOGCANCELSTR,211), FALSE);

     /*** Display the dialog box and allow the user to make a choice ***/
     while (1) {
          /*** Add the current page of choices ***/
          i = 0;
          for (j = TopChoice; j <= BottomChoice; j++) {
               wmove(TempWin, 2 + i, 8);
               for (k = 0; k < WinWidth - 10; k++)
                    waddch(TempWin, ' ');
               wmove(TempWin, 2 + i, 8);
               wprintw(TempWin, "%2d.", j + 1);
               wmove(TempWin, 2 + i, 12);
               wprintw(TempWin, "%s", Choices[j]);
               if (j == DefaultChoice)
                    waddstr(TempWin, Gtxt(" (default)",212));
               i++;
          }

          /*** Add the cursor ***/
          wmove(TempWin, CurrentLine, 3);
          waddstr(TempWin, "-->");

          /*** Update the screen ***/
          wrefresh(TempWin);

          /*** Let the user choose ***/
          ch = CURwgetch(cur, TempWin);
          switch(ch) {

          case '\016':
          case KEY_DOWN:
               /*** Move down a line ***/
               if (CurrentChoice == BottomChoice) {
                    TopChoice++;
                    BottomChoice++;
                    if (BottomChoice > NumChoices - 1) {
                         CurrentChoice = 0;
                         TopChoice = 0;
                         BottomChoice = min(TopChoice + ChoicesPerPage,
                                            NumChoices) - 1;
                    }
                    else
                         CurrentChoice++;
               }
               else
                    CurrentChoice++;
               break;

          case '\020':
          case KEY_UP:
               /*** Move up a line ***/
               if (CurrentChoice == TopChoice) {
                    TopChoice--;
                    BottomChoice--;
                    if (TopChoice < 0) {
                         CurrentChoice = NumChoices - 1;
                         TopChoice = max(0, NumChoices - ChoicesPerPage);
                         BottomChoice = CurrentChoice;
                    }
                    else
                         CurrentChoice--;
               }
               else
                    CurrentChoice--;
               break;

          case '\006':
          case ' ':
          case '+':
          case KEY_NPAGE:
               /*** Move down a page ***/
               if (BottomChoice == NumChoices - 1)
                    CurrentChoice = BottomChoice;
               else {
                    BottomChoice = min(BottomChoice + ChoicesPerPage, 
                                       NumChoices) - 1;
                    TopChoice = max(0, BottomChoice - ChoicesPerPage + 1);
                    CurrentChoice = max(TopChoice, CurrentChoice);
               }
               break;

          case '\002':
          case 'b':
          case '-':
          case KEY_PPAGE:
               /*** Move up a page ***/
               if (TopChoice == 0)
                    CurrentChoice = TopChoice;
               else {
                    TopChoice = max(0, TopChoice - ChoicesPerPage + 1);
                    BottomChoice = min(TopChoice + ChoicesPerPage,
                                       NumChoices) - 1;
                    CurrentChoice = min(CurrentChoice, BottomChoice);
               }
               break;

          case '^':
               /*** Move to top ***/
               TopChoice = 0;
               BottomChoice = min(TopChoice + ChoicesPerPage,
                                  NumChoices) - 1;
               CurrentChoice = TopChoice;
               break;

          case '$':
               /*** Move to bottom ***/
               BottomChoice = NumChoices - 1;
               TopChoice = max(0, BottomChoice - ChoicesPerPage + 1);
               CurrentChoice = BottomChoice;
               break;

          case '\037':
          case KEY_HELP:
          case 'h':
          case '?':
               /*** Help ***/
               CURDialog(cur, Gtxt("Choice Dialog Help",213), ChoiceHelp);
               touchwin(TempWin);
               break;

          case '1': case '2': case '3': case '4': case '5':
          case '6': case '7': case '8': case '9': case '0':
	       /*** Literal ***/
	       InBuf[0] = ch;
	       InBuf[1] = '\0';
	       InBuf[2] = '\0';

	       i = strlen(Prompt) + (NumChoices < 10 ? 11 : 12);
	       j = ChoicesPerPage + 3;
	       wmove(TempWin, j, i);

	       ch = CURwgetstr(cur, TempWin, InBuf, (NumChoices < 10 ? 1 : 2), FALSE);
	       k = atoi(InBuf);

	       if ((ch == '\n') && (0 < k) && (k <= NumChoices)) {
	            delwin(TempWin);
	            return (k - 1);
	       }
	       else {
		    wmove(TempWin, j, i);
		    waddstr(TempWin, "  ");
		    if (ch != -1)
			CURBeep(cur);
		    break;
	       }

          case '\007':
               /*** Cancel ***/
               delwin(TempWin);
               return (-1);

          case '\n':
	  case '\030':  /* Control X */
               /*** Accept ***/
               delwin(TempWin);
               return (CurrentChoice);

          default:
               CURBeep(cur);
               break;
          }
          wmove(TempWin, CurrentLine, 3);
          waddstr(TempWin, "   ");
          CurrentPage = CurrentChoice / ChoicesPerPage;
          CurrentLine = CurrentChoice - TopChoice + 2;
     }
}

/********************** Cruft for VMS follows ****************************/

#ifdef VMS
#include <descrip.h>
#include <iodef.h>
#include <ssdef.h>
#include <ttdef.h>
#include <tt2def.h>
#include <libclidef.h>
#include <smg$routines.h>
#include <smgdef.h>
#include <smgtrmptr.h>
#include <clidef.h>
#include <syidef.h>
#ifdef signal
#undef signal
#endif
#include <signal.h>
#ifdef system
#undef system
#endif
#include <processes.h>

#ifndef CLI$M_TRUSTED
#define CLI$M_TRUSTED 64 /* May not be in the compiler's clidef.h	*/
#endif
#define EFN	0				/* Event flag		*/
static	int	mask = LIB$M_CLI_CTRLY|LIB$M_CLI_CTRLT; /* ^Y and ^T	*/
static	int 	old_msk;			/* Saved control mask	*/
static	short	trap_flag = FALSE;		/* TRUE if AST is set	*/
static $DESCRIPTOR (term_name, "SYS$INPUT:");	/* For channel assigns	*/
short term_chan;				/* The channel		*/
static short first = TRUE;		/* Terminal initialization flag	*/
struct char_buffer_type {		/* Terminal characteristics	*/ 
			  char 		 class;
			  char		 type;
			  unsigned short size;
			  unsigned long  tchars;
			  unsigned long  tchars2;
} oldbuf;
static int in_pos, in_len;			/* For escape sequence	*/
static unsigned char buffer[20];		/*  handling in w_getch	*/
boolean DidCleanup = FALSE;			/* Exit handler flag	*/
static char VersionVMS[20];			/* Version of VMS	*/


/*
 * Define local replacement for wgetch that returns the characters without
 * having to set the terminal /pasthru, which screws up control-Y processing.
 */
static int w_getch(win)
     int win;
{
     int status;
     unsigned short iosb[4];
     
     if (in_pos < in_len)
          return(buffer[in_pos++]);

     status = sys$qiow (0, term_chan, IO$_READVBLK|IO$M_NOECHO|IO$M_NOFILTR,
     			&iosb, 0, 0, &buffer, 1, 0, 0, 0, 0);
     if ((status&1) == 1)
          status = iosb[0];
     if (status == SS$_PARTESCAPE) {
	  /* escape sequence in progress, fake a successful read */
	  status = 1;
     }
     if ((status&1) != 1)
          exit(status);
     in_pos = 1;
     in_len = iosb[1] + iosb[3];
     return (buffer[0]);
}

void
setterm_pas()
{
     int status;
     short iosb[4];

     if(first)
          VMSinit();
     else
          status = lib$disable_ctrl(&mask);

     in_pos = 0; in_len = 0;
}

void
resetterm()
{
     int status;
     
     status = sys$qiow(0,term_chan,IO$_SETMODE,0,0,0,
		       &oldbuf,12,0,0,0,0);
     status = lib$enable_ctrl(&old_msk);
}


/* VMS doesn't have termcap.  Unfortunately, the code in this */
/* module uses termcap just a little bit (it really shouldn't) */
/* rather than doing everything through curses */
 
/* The following simulates tputs, but does not support padding */
int
tputs(register char *cp, int affcnt, int (*outc)())
{
     while (*cp)
          outc(*(cp++));
     return(0);
}


/*
 *  VMSsignal -- F.Macrides 31-Jul-1993 (modification of my LYNX routine)
 *	Sets up AST for both Ctrl-C and Ctrl-Y, with system response to Ctrl-T
 *	 disabled.  If called with a sig other than SIGINT, it will use the C
 *	 library's signal(sig, func).
 *	VMSsignal(SIGINT, SIG_DFL) is treated as a call to resetterm().
 *      Call VMSsignal(SIGINT, SIG_IGN) before system() calls to enable Ctrl-C
 *	 and Ctrl-Y in the subprocess, and then call VMSsignal(SIG_INT, func)
 *	 on return from the subprocess.
 *	For func's which do not invoke an exit, the func should reassert itself.
 *	The VMS C signal() calls do not fully emulate the Unix calls, and
 *	 VMSsignal() is just a "helper", also not a full emulation.
 */

void
(*VMSsignal (sig, func)) (int)
int sig;
void (*func)();
{
     int status;
     short iosb[4];
     static int SIG_IGN_flag;

     /* pass all signals other than SIGINT to signal() */
     if (sig != SIGINT) {
          return(signal(sig, func));
     }

     /* if func is SIG_DFL, treat it as resetterm() */
     if (func == SIG_DFL) {
          resetterm();
	  return(SIG_DFL);
     }

     /* Clear any previous AST */
     if (trap_flag) {
          status = sys$dassgn (term_chan);
	  status = lib$enable_ctrl(&old_msk);
	  trap_flag = FALSE;
     }

     /* if func is SIG_IGN, leave the TT channel closed and the  */
     /* system response to interrupts enabled for system() calls */
     if (func == SIG_IGN)
          return(SIG_IGN);

     /* if we get to here, we have a SIGINT func, so set the AST */
     if(first)
          VMSinit();
     else {
	 status = sys$assign (&term_name, &term_chan, 0, 0);
         status = lib$disable_ctrl(&mask);
     }
     status = sys$qiow (EFN, term_chan,
			IO$_SETMODE|IO$M_CTRLCAST|IO$M_CTRLYAST,
			&iosb, 0, 0,
			func, SIGINT, 0, 0, 0, 0);
     trap_flag = TRUE;
     return(func);
}


/*
 *  VMSCURinit, VMSinit, VMSexit -- F.Macrides 12-Aug-1993
 *	Save termial characteristics at the time when gopher.c's Initialize()
 *	 is called.  Make sure they're retored and that cleanup is done if we
 *	 exit via exit(-1)'s or ACCVIO's.
 */

void
VMSCURinit(cur)
  CursesObj *cur;
{
     int status, i;
     short Type = 0;
     long buf_siz = 20, len;
     unsigned long Addr;
     char ch, Name[20], cl[20], bl[20], so[20], se[20];

     /** Keep DECC from complaining **/
     struct dsc$descriptor_s Name_desc;
     Name_desc.dsc$w_length  = 20;
     Name_desc.dsc$b_class   = DSC$K_CLASS_S;
     Name_desc.dsc$b_dtype   = DSC$K_DTYPE_T;
     Name_desc.dsc$a_pointer = Name;
     
     /*** Initialize the terminal, if we haven't already ***/
     if(first)
          VMSinit();

     /*** Can't handle "unknown" terminal type ***/
     if (oldbuf.type == 0) {
	  CURsetTerm(cur, "unknown");
	  return;
     }

     /*** Get the terminal table ready ***/
     Type = (short) oldbuf.type;
     status = smg$init_term_table_by_type (&Type, &Addr, &Name_desc);
     if (!(status&1))
     {
	  CURsetTerm(cur, "unknown");
	  return;
     }
     
     /*** Set the terminal name ***/
     i = 0;
     while ((Name[i++] != ' ') && i <= 20) ;
     Name[--i] = '\0';
     if (strlen(Name))
          CURsetTerm(cur, Name);
     else
          CURsetTerm(cur, "VMS");

     /*** Get the clearscreen code ***/
     status = smg$get_term_data(&Addr, &SMG$K_ERASE_WHOLE_DISPLAY,
                                &buf_siz, &len, cl);
     if (status&1) {
          cl[len] = '\0';
	  CURsetCLS(cur, cl);
     }
     else
     	  /*** Assume 7-bit ***/
          CURsetCLS(cur, "\033[2J");
     
     /*** Set the bell ***/
     CURsetBell(cur, "\007");

     /*** Set the highlight codes ***/
     status = smg$get_term_data (&Addr, &SMG$K_BEGIN_BOLD,
     				 &buf_siz, &len, so);
     if (status&1) {
          so[len] = '\0';
	  CURsetHighon(cur, so);
     }
     else
	  CURsetHighon(cur, "\033[1m");

     status = smg$get_term_data (&Addr, &SMG$K_BEGIN_NORMAL_RENDITION,
     				 &buf_siz, &len, se);
     if (status&1) {
          se[len] = '\0';
	  CURsetHighoff(cur, se);
     }
     else
	  CURsetHighoff(cur, "\033[m");

}

void
VMSinit()
{
     int status;
     static unsigned long condition;
     static struct _exit_block {
          unsigned long forward;
          unsigned long address;
          unsigned long zero;
          unsigned long condition;
     } exit_handler_block;

     if(first) {
          /* get a channel and save terminal parms if we haven't already */
	  status = sys$assign (&term_name, &term_chan, 0, 0 );
          status = sys$qiow(0,term_chan,IO$_SENSEMODE,0,0,0,
	  		    &oldbuf,12,0,0,0,0);
          first = FALSE;
          status = lib$disable_ctrl(&mask, &old_msk);

          exit_handler_block.forward   = 0;
          exit_handler_block.address   = (unsigned long) &VMSexit;
          exit_handler_block.zero      = 0;
          exit_handler_block.condition = (unsigned long) &condition;

	  /* declare the exit handler block */
          status = sys$dclexh(&exit_handler_block);
          if (status != SS$_NORMAL) {
               printf("exit_status: %d", status);
          }

	  /* Get the version of VMS */
	  if (VMSVersion(VersionVMS, 20) < 3)
	  	/* Load zeros on error */
	  	strcpy(VersionVMS, "V0.0-0");
     }
}

void
VMSexit()
{
    /*
     * If we didn't get here via CleanupandExit(), it was via an
     *  exit(-1) or ACCVIO, so make *sure* we reset the terminal,
     *  then attempt a cleanup.
     */
     if (!DidCleanup) {
          fprintf(stderr,"\nPress RETURN to clean up and exit: ");
	  (void) getchar();
          resetterm();
          CleanupandExit(-1);
     }
}


/*
 *  VMSVersion -- F.Macrides 09-Jan-1994
 *	Modification of UNZIP routine for getting VMS Version.
  */

int
VMSVersion(VerString, VerLen)
char *VerString;
int VerLen;
{
     int status, i = SYI$_VERSION, verlen = 0;
     struct dsc$descriptor version;
     char *m;

     version.dsc$a_pointer = VerString;
     version.dsc$w_length = VerLen - 1;
     version.dsc$b_dtype = DSC$K_DTYPE_B;
     version.dsc$b_class = DSC$K_CLASS_S;

     status = lib$getsyi(&i, 0, &version, &verlen, 0, 0);
     if (!(status&1) || verlen == 0)
	  return 0;

     /* Cut out trailing spaces */
     for (m=VerString+verlen, i=verlen-1; i > 0 && VerString[i] == ' '; --i)
	  *(--m) = '\0';

     return strlen(VerString)+1;	/* Transmit ending 0 too */
}


/*
 *  DCLsystem, spawn_DCLprocess, DCLspawn_exception -- F.Macrides 31-Jul-1993
 *	Exception-handler routines for enabling interrupts and Control-T
 *      during spawns when setjmp() has been established, and the parent
 *	passes a DCL CLI.
 *  F.Macrides 09-Jan-1994 -- spawn_DCLprocess includes TRUSTED flag for
 *	spawns with a command (but not spawns to DCL) if the version of
 *	OpenVMS/VAX requires it in captive accounts.
 */

int
DCLsystem(command)
char *command;
{
     int status;
     extern void controlc();
     
     VMSsignal(SIGINT, SIG_IGN);
     status = spawn_DCLprocess(command);
     VMSsignal(SIGINT, controlc);
     if ((status&1) != 1)
         return(-1);
     else
     	 return(0);
}

int
spawn_DCLprocess(command)
char *command;
{
     int status;
     /** Keep DECC from complaining **/
     struct dsc$descriptor_s  command_desc;
     command_desc.dsc$w_length  = strlen(command);
     command_desc.dsc$b_class   = DSC$K_CLASS_S;
     command_desc.dsc$b_dtype   = DSC$K_DTYPE_T;
     command_desc.dsc$a_pointer = command;

     VAXC$ESTABLISH(DCLspawn_exception);

     if (command == "")
          status = lib$spawn(0);
     else {
#ifndef __ALPHA /* OpenVMS/AXP v6.1 still doesn't have the TRUSTED flag ;( */
	  if(VersionVMS[1] >= '6')
	       /** Include TRUSTED flag **/
               status = lib$spawn(&command_desc,0,0,&CLI$M_TRUSTED);
	  else
#endif
	       status = lib$spawn(&command_desc);
     }

     return(status);
}

unsigned int
DCLspawn_exception(sigarr, mecharr)
void *sigarr, *mecharr;
{
     int status;
     
     status = lib$sig_to_ret(sigarr, mecharr);
     return(SS$_UNWIND);
}
#endif /* VMS */


