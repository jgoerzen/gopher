/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.10 $
 * $Date: 2002/04/26 14:59:54 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/gopher.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: gopher.c
 * Main functions for the gopher client
 *********************************************************************
 * Revision History:
 * $Log: gopher.c,v $
 * Revision 1.10  2002/04/26 14:59:54  jgoerzen
 * Preparations for 3.0.5.
 *
 * Revision 1.9  2002/01/11 04:29:30  jgoerzen
 * Another attempt at the strerror fix -- maybe #elsif isn't so hot an idea...
 *
 * Revision 1.8  2002/01/10 14:44:03  jgoerzen
 * Updated
 *
 * Revision 1.7  2002/01/08 21:35:49  jgoerzen
 * Many changes:
 *  Revved the version number
 *  updated greeting
 *  updated copyright notices
 *
 * Revision 1.6  2002/01/08 20:57:26  jgoerzen
 * Small logic error fixed
 *
 * Revision 1.5  2002/01/08 20:30:35  jgoerzen
 *     * gopher.c: Modified to use HAVE_STRERROR
 *
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
 * Revision 1.3  2000/12/27 21:22:46  s2mdalle
 * Added a few #include's to get at prototypes for implicitly defined
 * functions
 *
 * Revision 1.2  2000/08/19 01:18:16  jgoerzen
 * fix sys_errlist
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.112  1996/01/04  18:28:53  lindner
 * Updates for autoconf
 *
 * Revision 3.111  1995/11/03  21:18:18  lindner
 * ANSIfication
 *
 * Revision 3.110  1995/11/03  20:50:48  lindner
 * Coen: fixes..
 *
 * Revision 3.109  1995/06/08  05:21:47  lindner
 * Fixed conflict for message 221, fixed problem with DECC
 *
 * Revision 3.108  1995/04/15  07:09:02  lindner
 * New control C methods, CTRL-X mods
 *
 * Revision 3.107  1995/02/27  17:09:49  lindner
 * Add Pacbell NOBANNER patch
 *
 * Revision 3.106  1995/01/20  04:52:24  lindner
 * Modifications for SOCKS
 * Use VARIABLE length records for text on VMS
 * Don't add the default gopher server when using the -b option.
 *
 * Revision 3.105  1994/12/07  07:43:22  lindner
 * Fix for URLs on the command line from F.Macrdes
 *
 * Revision 3.104  1994/11/25  17:40:35  lindner
 * Fix for folks with non gopher port of 70 in conf.h
 *
 * Revision 3.103  1994/11/16  18:52:52  lindner
 * Fix for adding a text file to bookmarks
 *
 * Revision 3.102  1994/10/21  04:40:34  lindner
 * IETF url format, plus ANSI printer
 *
 * Revision 3.101  1994/10/13  05:30:17  lindner
 * Compiler complaint fixes
 *
 * Revision 3.100  1994/08/19  16:52:38  lindner
 * Fixes for many gripe problems...
 *
 * Revision 3.99  1994/08/03  03:25:12  lindner
 * Fix for osf
 *
 * Revision 3.98  1994/08/01  21:55:27  lindner
 * Add back garbled flag
 *
 * Revision 3.97  1994/07/25  02:53:30  lindner
 * Skip over type 'i' items, secure mode mods
 *
 * Revision 3.96  1994/07/19  20:20:00  lindner
 * Gripe options added, telnet tracer etc.
 *
 * Revision 3.95  1994/07/08  05:55:28  lindner
 * Fix to allow urls in place of hosts..
 *
 * Revision 3.94  1994/07/07  01:51:49  lindner
 * Fix bug
 *
 * Revision 3.93  1994/07/06  15:36:27  lindner
 * These changes are from F. Macrides:
 *
 * Moved declaration and initialization of setlocale_LangDir to
 * globals.h.
 *
 * Note that setlocale() doesn't yet work with VMS Alphas, so it
 * presently just returns without doing anything and the "hardcoded"
 * English text is used.  It all works fine with VAXen.
 *
 * Added code to delete the temporary file when gopher is called with the
 * -p or the new -u switch.
 *
 * Added J. Lance Wilkinson's (JLW@psulias.psu.edu) internationalization
 * port to VMS and: 'L' operation for changing language on the fly.
 *
 * Pause after telnet/tn3270 to permit reading of any error messages.
 * Added telnet and tn3270 to prompt for a URL via the 'w' command.
 *
 * Modified Alan's dialog box title and prompt for the 'w' command.
 *
 * NEVERSETOPTIONS compilation symbol disables ability to set options via
 * SetOptions() and issues an appropriate message.
 *
 * NEVERSPAWN compilation symbol disables the spawn command and issues an
 * appropriate message.
 *
 * On VMS, the NEVERSETOPTIONS and NEVERSPAWN symbols can be used to
 * create a gopher image for CAPTIVE accounts which still permits
 * functions that would be disabled by the -s or -S switch, but issues an
 * appropriate message (rather than a DCL error message) on spawn attempt
 * and precludes "creative" modifications of the display software and
 * print command mappings.
 *
 * Added missing ! for NoShellMode under binary and and encoded save to
 * disk if().
 *
 * Revision 3.92  1994/07/03  23:11:24  lindner
 * Add internal download feature
 *
 * Revision 3.91  1994/06/29  05:20:37  lindner
 * Check for new gopher.rc files, add support for A_APP items
 *
 * Revision 3.90  1994/06/09  22:13:37  lindner
 * More language conversions
 *
 * Revision 3.89  1994/06/09  04:16:28  lindner
 * Added option to allow 'd'elete only for bookmarks via a
 * DELETE_BOOKMARKS_ONLY compilation symbol.
 *
 * Revision 3.88  1994/06/09  03:45:02  lindner
 * More attempts to use sunos 4.1.3 setlocale
 *
 * Revision 3.87  1994/06/03  05:58:58  lindner
 * Use LC_MESSAGES variable instead of LANG
 *
 * Revision 3.86  1994/05/27  18:08:51  lindner
 * Fix for SetOptions() on VMS
 *
 * Revision 3.85  1994/05/27  02:22:23  lindner
 * One more fix for International defs
 *
 * Revision 3.84  1994/05/26  19:36:45  lindner
 * Fix bugs for ginternational
 *
 * Revision 3.83  1994/05/26  19:15:22  lindner
 * Add GINTERNATIONAL stuff
 *
 * Revision 3.82  1994/05/25  21:46:35  lindner
 * Fix for suck_sound
 *
 * Revision 3.81  1994/05/25  21:38:17  lindner
 * Fix for mem prob..
 *
 * Revision 3.80  1994/05/25  20:59:42  lindner
 * Fix for play command forking
 *
 * Revision 3.79  1994/05/24  07:00:38  lindner
 * Don't allow users with rsh to specify a shell as an app..
 *
 * Revision 3.78  1994/05/24  05:50:10  lindner
 * Fix for bad free() in SetOptions()
 *
 * Revision 3.77  1994/05/19  15:53:34  lindner
 * Fix for vms
 *
 * Revision 3.76  1994/05/19  14:07:25  lindner
 * use fast malloc on VMS VAXC
 *
 * Revision 3.75  1994/05/18  03:59:25  lindner
 * Change to FIOsystem() for VMS
 *
 * Revision 3.74  1994/05/17  05:47:57  lindner
 * Massive internationalization change
 *
 * Revision 3.73  1994/05/14  04:13:41  lindner
 * Internationalization...
 *
 * Revision 3.72  1994/04/25  04:05:24  lindner
 * FIOsystem() return values were reversed for VMS.
 *
 * Added code for mapping telnet and/or tn3270 commands to "- none -" for
 * disabling them and having the client issue "Sorry, not implemented"
 * messages on efforts to use Type 8 or T tuples.
 *
 * Added FIOsystem() failure messages for both VMS and Unix. (all from
 * F.Macrides)
 *
 * Revision 3.71  1994/04/25  03:37:38  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.70  1994/04/13  19:12:50  lindner
 * Fix for ASK block bug
 *
 * Revision 3.69  1994/04/12  21:02:51  lindner
 * Move file declaration inside
 *
 * Revision 3.68  1994/03/30  20:32:58  lindner
 * Fix for deleting last item in a directory
 *
 * Revision 3.67  1994/03/08  15:55:09  lindner
 * gcc -Wall fixes
 *
 * Revision 3.66  1994/03/08  03:25:55  lindner
 * Fix for big bad telnet bug, additions for secure process i/o
 *
 * Revision 3.65  1994/03/04  23:41:17  lindner
 * Better gripe, more signal handling, better forms
 *
 * Revision 3.64  1994/02/20  16:34:22  lindner
 * Patch for weird gopher+ servers, add anon-ftp key, and localtime key
 *
 * Revision 3.63  1994/01/10  03:29:49  lindner
 * fix for changes in GDdeleteGS
 *
 * Revision 3.62  1993/11/29  01:11:25  lindner
 * 'v' no longer does anything if user is already viewing bookmarks.
 * (Beckett)
 *
 * Add new routine, do_movie(), which will eventually allow viewing of
 * movies.  For now, just display an error message.  In process_request(),
 * add a switch case for the A_MOVIE type.  (Wolfe, Macrides)
 *
 * Fix the 'm' command so that it deletes the current menu's cache files.
 * Also prevent 'm' from destroying bookmarks. (Beckett)
 *
 * In describe_gopher(), add TRUE argument to GStoLink() so that the Admin
 * and ModDate information are displayed ('=' and '^').  (Macrides)
 *
 * Prevent describe_gopher() from destroying the cache file for the current
 * item.  (Beckett, Wolfe)
 *
 * Revision 3.61  1993/11/03  03:55:12  lindner
 * More stable file caching, subject line chopping in Gripe
 *
 * Revision 3.60  1993/11/02  21:17:36  lindner
 * Better client side logging code
 *
 * Revision 3.59  1993/10/27  18:52:14  lindner
 * Simplify VMS fixed/variable file code
 *
 * Revision 3.58  1993/10/26  18:45:09  lindner
 * Move screen refresh stuff to CURwgetch()
 *
 * Revision 3.57  1993/10/26  18:01:01  lindner
 * Allow saving to variable length record VMS files, remove Interrupted msg
 *
 * Revision 3.56  1993/10/22  20:24:14  lindner
 * Fixes for VMS tempnam() (Fote)
 *
 * Revision 3.55  1993/10/22  20:06:37  lindner
 * Add optional client logging
 *
 * Revision 3.54  1993/10/20  03:24:55  lindner
 * Cleanup on a SIGHUP signal
 *
 * Revision 3.53  1993/10/13  16:47:23  lindner
 * Fixes for empty bookmark addition problem
 *
 * Revision 3.52  1993/10/11  17:16:15  lindner
 * Fote's mods for display applications
 *
 * Revision 3.51  1993/10/11  04:54:36  lindner
 * Fix for segvs when exiting early
 *
 * Revision 3.50  1993/10/07  05:09:28  lindner
 * Don't redraw the screen if we attempt to go up a directory level from
 * the root level.
 *
 * In the 'o' command, allocate memory for the form based on screen width.
 * Fix memory leak.
 *
 * In Gripe(), allocate memory for the form based on screen width.  Fix
 * email parsing under VMS.  Fix problem where gopher did not delete mail
 * temp files under VMS.
 *
 * In process_request(), make the status "Receiving xxxx..." a bit more
 * descriptive for images, MIME files, and other files.
 *
 * Add NULL as third argument to Save_File in a couple of places
 *
 * Revision 3.49  1993/09/30  22:42:04  lindner
 * Add option for bolding of searched words
 *
 * Revision 3.48  1993/09/29  20:50:57  lindner
 * Always cleanupandexit instead of exit, allow deleting of last bookmark
 *
 * Revision 3.47  1993/09/26  09:19:44  lindner
 * Changed errormsg wording
 *
 * Revision 3.46  1993/09/22  04:14:22  lindner
 * Fix core dumps when exiting from bookmark screen, various cleanups
 *
 * Revision 3.45  1993/09/22  03:56:47  lindner
 * Add support for tn3270 on oddball ports..
 *
 * Revision 3.44  1993/09/22  03:09:59  lindner
 * Add command-line searching
 *
 * Revision 3.43  1993/09/22  01:15:49  lindner
 * Add support for DEC HELP key/KEY_HELP
 *
 * Revision 3.42  1993/09/21  01:50:43  lindner
 * Fix for caching of alternate views
 *
 * Revision 3.41  1993/09/18  04:40:47  lindner
 * Additions to fix caching of Multiple view items
 *
 * Revision 3.40  1993/09/18  03:28:30  lindner
 * Remove extra CURexit()
 *
 * Revision 3.39  1993/09/11  04:46:45  lindner
 * Don't allow null applications to be added with the options code, Beep on network error
 *
 * Revision 3.38  1993/09/08  05:22:30  lindner
 * Lobotomized the bolding code
 *
 * Revision 3.37  1993/09/08  01:11:51  lindner
 * Added URL stuff to describe_gopher
 *
 * Revision 3.36  1993/09/03  03:33:45  lindner
 * Inform user about mal-configuration if view has Unix piping on VMS.
 *
 * Deal with the problem of the gopher+ code setting view to NULL or ""
 * here and there as if they were equivalent.
 *
 * Added code for mailing gripes from VMS systems, and fixed memory leak
 * in the Unix code.
 *
 * Added v1.12b 'S' command for saving titles in a directory.
 *
 * Added titles to CURGetOneOption() calls.
 *
 * Revision 3.35  1993/08/25  02:57:52  lindner
 * Fix for Fote mod to 'o' command
 *
 * Revision 3.34  1993/08/23  02:32:28  lindner
 * Don't connect if nothing typed in 'o'
 *
 * Revision 3.33  1993/08/19  20:31:10  lindner
 * remove excess variable
 *
 * Revision 3.32  1993/08/19  20:22:49  lindner
 * Mitra's Debug patch
 *
 * Revision 3.31  1993/08/16  18:10:19  lindner
 * Temporary code to fix DEC Alphas screen clearing, exit handler for VMS
 *
 * Revision 3.30  1993/08/16  17:58:20  lindner
 * Removed REMOTEUSER ifdefs
 *
 * Revision 3.29  1993/08/09  20:28:04  lindner
 * Mods for VMS for telnet dialog, argv[0]
 *
 * Revision 3.28  1993/08/09  20:17:59  lindner
 * Fixes for CMULIB and NETLIB for VMS
 *
 * Revision 3.27  1993/08/05  03:24:21  lindner
 * Fix for control-c on startup
 *
 * Revision 3.26  1993/08/04  22:08:47  lindner
 * Fix for problems with '=' and '?' and /bin/mail Gripe mods
 *
 * Revision 3.25  1993/08/03  20:48:27  lindner
 * Audio file fix from jqj
 *
 * Revision 3.24  1993/08/03  20:26:50  lindner
 * Don't allow securemode types to use o
 *
 * Revision 3.23  1993/08/03  20:24:18  lindner
 * Bigger Better Badder Options, inspired by jqj
 *
 * Revision 3.22  1993/08/03  04:43:56  lindner
 * Fix for VMS unresolved variables
 *
 * Revision 3.21  1993/07/30  17:37:24  lindner
 * SecureMode fix from Mitra
 *
 * Revision 3.20  1993/07/30  14:19:29  lindner
 * Mitra autoexit patch
 *
 * Revision 3.19  1993/07/30  14:12:18  lindner
 * Allow non-gplus stuff to cache
 *
 * Revision 3.18  1993/07/29  17:24:53  lindner
 * Removed dead variable, $ and ! are synonomous now
 *
 * Revision 3.17  1993/07/27  05:28:48  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.16  1993/07/27  02:02:22  lindner
 * More comments, GDdelete method
 *
 * Revision 3.15  1993/07/26  20:29:40  lindner
 * fix memory usage
 *
 * Revision 3.14  1993/07/26  15:35:56  lindner
 * fix for longjmp params
 *
 * Revision 3.13  1993/07/23  04:42:58  lindner
 * error checking and longjmp'ing
 *
 * Revision 3.12  1993/07/20  23:15:35  lindner
 * Mods to cache askdata..
 *
 * Revision 3.11  1993/07/07  19:42:59  lindner
 * fix for SGIs
 *
 * Revision 3.10  1993/06/29  06:18:09  lindner
 * Prettier error msg for VMS
 *
 * Revision 3.9  1993/06/22  06:13:28  lindner
 * took back fix for perror.h etc.
 *
 * Revision 3.8  1993/06/11  16:25:43  lindner
 * Many bug fixes, open new gopher, shell escape, etc.
 *
 * Revision 3.7  1993/04/30  16:04:48  lindner
 * Cleared gripe memory, fixed bug for long names in  check_sock
 *
 * Revision 3.6  1993/04/13  04:56:54  lindner
 * New code for REMOTEUSERS from Mitra
 * Better error messages for socket connections from jqj
 *
 * Revision 3.5  1993/03/24  16:59:54  lindner
 * Major changes to the way things are displayed
 *
 * Revision 3.4  1993/03/18  23:32:07  lindner
 * commented out forkoff stuff for now...
 *
 * Revision 3.3  1993/02/19  21:08:04  lindner
 * Updated most routines to get defaults from gopher+ attribute types,
 * Allow commands to be pipelines and almost have forking working :-)
 *
 * Fixed problems with bookmarks.  Still need to work on g+ bookmarks
 *
 * Revision 3.2  1993/02/11  18:25:45  lindner
 * Fixed helpfile display.
 *
 *********************************************************************/


#include "gopher.h"
#if defined(VMS) && defined(A_LANGUAGE)
static	CursesObj	*LangScreen = NULL;
#endif
#include "Stdlib.h"
#include "Debug.h"

#include <errno.h>
#include "fileio.h"
#include "Malloc.h"
#include "patchlevel.h"

#ifdef HAVE_TIME_H
#  include <time.h>
#endif /* HAVE_TIME_H */

void describe_gopher(char *banner, GopherStruct *ZeGopher, FILE *gripefile);
extern int  twirl(void);


/*
** Open a connection to another host using telnet or tn3270
*/

void
do_tel_3270(GopherStruct *ZeGopher)
{
     char *Dialogmess[9];

     char sMessage1[128];
     char sMessage2[128];

     char sTelCmd[128];
     
     char *cp;

     /* retrieve the gopher information for the telnet command*/

     clear();
#ifdef VMS
     refresh();
#endif
     Dialogmess[0] = Gtxt("Warning!!!!!, you are about to leave the Internet",34);
     Dialogmess[1] = Gtxt("Gopher program and connect to another host. If",35);
     Dialogmess[2] = Gtxt("you get stuck press the control key and the",36);
#if defined(VMS) && defined(MULTINET)
     Dialogmess[3] = Gtxt("^ key, and then type q.",38);
#else
     Dialogmess[3] = Gtxt("] key, and then type quit",37);
#endif
     Dialogmess[4] = "";
     
     if (GSgetPort(ZeGopher) != 0)
	  sprintf(sMessage1,Gtxt("Connecting to %.40s, port %d using %s.",39), 
		  GSgetHost(ZeGopher),GSgetPort(ZeGopher), 
		  (GSgetType(ZeGopher) == A_TN3270) ? "tn3270" : "telnet");
     else
	  sprintf(sMessage1, Gtxt("Connecting to %.40s using %s.",40),
		  GSgetHost(ZeGopher),
		  (GSgetType(ZeGopher) == A_TN3270) ? "tn3270" : "telnet");

     Dialogmess[5] = sMessage1;

     cp = GSgetPath(ZeGopher);
     if (*cp != '\0')
	  sprintf(sMessage2,
		  Gtxt("Use the account name \"%.40s\" to log in",41),
		  GSgetPath(ZeGopher));
     else
	  sMessage2[0] = '\0';

     Dialogmess[6] = "";
     Dialogmess[7] = sMessage2;
     Dialogmess[8] = NULL;

     if (CURDialog(CursesScreen, GSgetTitle(ZeGopher), Dialogmess) <0)
	  return;

     CURexit(CursesScreen);

     if (GSgetType(ZeGopher) == 'T') {
	  /**** A TN3270 connection ****/
	  RCdisplayCommand(GlobalRC, "Terminal/tn3270", GSgetHost(ZeGopher),
			   sTelCmd);

     } else {

	  RCdisplayCommand(GlobalRC, "Terminal/telnet", GSgetHost(ZeGopher), 
			   sTelCmd);
     }

     if (GSgetPort(ZeGopher) != 0 && GSgetPort(ZeGopher) != 23) 
#if defined(VMS) && (defined(MULTINET) || defined(CMUIP))
	  sprintf(sTelCmd+strlen(sTelCmd),
		  "/PORT=%d", GSgetPort(ZeGopher));
#else
     sprintf(sTelCmd+strlen(sTelCmd), " %d", GSgetPort(ZeGopher));
#endif
     
     CURexit(CursesScreen);

#ifdef Telnet_Trace
     Telnet_Trace(sTelCmd, ZeGopher);
#endif
     FIOsystem(sTelCmd);
     printf("\n\n");
     printf(Gtxt("\nPress <RETURN> to continue",120));
     fflush(stdout);
     fflush(stdin);
     fflush(stdin);
     (void) getchar();
     CURenter(CursesScreen);
     return;
}


#if defined(VMS) && defined(TELNET_TRACE)

/*
** Some sites are really paranoid about attempts to connect from their site 
** to specific telnet hosts (e.g., MUDDs, etc.) -- so provide a log facility 
** to track specific connectsions just to see how clients are finding them.
*/
#include <descrip.h>
#include <lnmdef.h>
#include <ssdef.h>
#include "syslog.h"

/*
**  Like logrequest() but should use a different log file just for this...
*/

int
trace_telnet(msg, gs)
  char *msg;
  GopherObj *gs;
{
     static boolean inited = FALSE;
     char *url = "";
     char *name = "";

     if (!gs)
	return(0);

     if (inited == FALSE) {
	  openlog("gopher", (LOG_PID), LOG_LOCAL7); /* MULTINET allows LOCAL7 */
	  setlogmask(LOG_UPTO(LOG_INFO));
	  inited = TRUE;
     }
     if (gs != NULL) {
	  url = GSgetURL(gs, "");
	  name = GSgetTitle(gs);
     }

     syslog(LOG_INFO, "%s %s%s%s%s", msg, url, strlen(name)?" \"":"",
						    name, strlen(name)?"\"":"");

     return(0);
}

int
Telnet_Trace(char *sTelCmd, GopherStruct *ZeGopher)
{
	char	text[255];
        int	l;
static	int	buf_len;
static	int	i;
static	int	junk;
static	int	max_len;

	int	no_case = LNM$M_CASE_BLIND;
static	$DESCRIPTOR(lname_desc,"GOPHER_TELNET_LOG");
static	$DESCRIPTOR(tabnam,"LNM$SYSTEM_TABLE");
struct itmlst
    {
	short int buf_len;
	short int item_code;
	char *buffer;
	int *buf_length;
    };
static
    struct itmlst
		item_max[] = { {4,LNM$_MAX_INDEX,(char *)&max_len,&buf_len},
					{0,0,0,0}};
static	
    struct itmlst
		item_str[] = { {4,LNM$_INDEX,(char *)&i,&junk},
					{0,LNM$_STRING,0,&buf_len},
					{0,0,0,0}};

  if (SS$_NORMAL != SYS$TRNLNM(&no_case,&tabnam,&lname_desc,0,&item_max))
	return;
  item_str[1].buffer = text;
  for (i=0; i<=max_len; i++) {
    item_str[1].buf_len = 255;
    if (SS$_NORMAL==SYS$TRNLNM(&no_case,&tabnam,&lname_desc,0,&item_str)) {
	text[buf_len] = '\0';
	if (strcmp(text,GSgetHost(ZeGopher))==0) {
	    /* Log this session */
	    text[sprintf(text," *-%.*s>",iLevel,
			"-------------------------------")]='\0';
	    trace_telnet(text,ZeGopher);
	    if(GDgetLocation(CurrentDir) != NULL) {
		text[sprintf(text," + %.*s ",iLevel,
			    "                               ")]='\0';
		trace_telnet(text,GDgetLocation(CurrentDir));
	    }
	    for (l = iLevel; l>0; l--) {
		text[sprintf(text," +%.*s>",l,
			"-------------------------------")]='\0';
		trace_telnet(text, GDgetEntry(OldDirs[l-1], 
				    GDgetCurrentItem(OldDirs[l-1])-1));
	    }
	    return;
	}
    }
  }
    return;
}
#endif


void
Gripe(GopherObj *gs)
{
     char *gripeprompt[15];
     char *gripemess[15];
     char *cp = NULL;
     char  email[128], version[128];
     int   i, j;
     char *mailargv[3];
     FileIO *fio;

#ifdef VMS
     char mailcmd[256],
     FILE *f;
     char MailAddress[512], *tmpfilename;
#endif

     GSgetginfo(gs, TRUE);

     if (GSisGplus(gs) && (GSgetAdmin(gs) != NULL))
	  cp = strchr(GSgetAdmin(gs),'<');

     /* at this point cp is NULL if (and hopefully only if)
      * the item is not Gopher+, there is no Admin for the item,
      * or there is no e-mail address in the admin definition 
      */
     if (cp == NULL) {

#ifdef LOCAL_GRIPE_ADMINISTRATOR
# ifdef DOMAIN_FOR_LOCAL_GRIPES
	  if (strstr(GSgetHost(gs),DOMAIN_FOR_LOCAL_GRIPES) != NULL )
# endif
	       cp = LOCAL_GRIPE_ADMINISTRATOR;
#endif

	  /* if the local gripe admin won't take it, either let the user
	   * edit it, or put up an error message
	   */
	  if (cp == NULL) {
#ifdef MODIFIABLE_GRIPE_TO
	       cp = "<>";
#else
	       CursesErrorMsg(
		 Gtxt("Can't find an administrator for this item, sorry!",48));
	       return;
#endif
	  }
     }

     strncpy(email, cp+1, sizeof(email));
     cp = strrchr(email, '>');
     if (cp != NULL)
	  *cp = '\0';

#define ACHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789%.@!-_"
		    
     if (SecureMode || NoShellMode)
	  if ( strspn(email, ACHARS) != strlen(email)||
	      *email == '-') {
		  CursesErrorMsg("Can't find an administrator for this item, sorry!");
		  return;
	  }

     /** Empty out the array **/
     for (i=3; i< 15; i++) {
	  cp = (char *) malloc(sizeof(char) * COLS);
	  bzero(cp,COLS*sizeof(char));
	  gripeprompt[i] = "";
	  gripemess[i] = cp;
     }

     for (i=0; i < 3; i++)
	  gripemess[i] = NULL;

     

     gripeprompt[0] = Gtxt("Hit the Tab key at the end of each line you type.",49);
#ifdef CONTROLX
     gripeprompt[1] = Gtxt("Press Control-X to send your message.",50);
#else
     gripeprompt[1] = Gtxt("Hit the Enter key to send your message.",50);
#endif


#ifdef MODIFIABLE_GRIPE_TO
     gripeprompt[2] = Gtxt("To",230);
     gripemess[2] = email;
     gripeprompt[13] = Gtxt("<Last Line>",231);
#else
     gripeprompt[2] = "";
#endif
     gripeprompt[3] = Gtxt("Subject",51);
     gripeprompt[4] = Gtxt("Problem",52);
     gripeprompt[14] = NULL;

     if (CURRequest(CursesScreen, GSgetAdmin(gs), gripeprompt, gripemess)!=0) {
	  return;
     }
     if (strlen(gripemess[3]) == 0) {
	  CursesErrorMsg(Gtxt("Cannot send, no subject...",228));
	  return;
     }
     for (i=4, j=0; i<14; i++)
	  j += strlen(gripemess[i]);
     if (j < 20) {
	  CursesErrorMsg(Gtxt("Cannot send, message too small...",228));
	  return;
     }

#ifdef MODIFIABLE_GRIPE_TO
     if (strlen(strcpy(email,gripemess[2])) == 0) {
	CursesErrorMsg(Gtxt("Cannot send mail to null address...",228));
	return;
     }
#endif
#ifdef VMS
     if ((strchr(email, '@') != NULL) && (strchr(email, '\"') == NULL))
	   sprintf(MailAddress, MAIL_ADRS, email);
     else
	   strcpy(MailAddress, email);

     tmpfilename = tempnam(NULL,NULL);

     if ((f = fopen(tmpfilename, "w")) == NULL) {
	  CursesErrorMsg(Gtxt("Cannot send mail...",53));
	  return;
     }

     sprintf(mailcmd, "%s/subject=\"%.70s\" %s %s",
           MAIL_COMMAND, gripemess[3], tmpfilename, MailAddress);
     free(gripemess[3]);
#ifdef DESCRIBE_GOPHER_GRIPE
     fprintf(f, Gtxt("Regarding the following Gopher item:\n",229));
     describe_gopher(" ",
		    GDgetEntry(CurrentDir, GDgetCurrentItem(CurrentDir)-1),
			f);
     fprintf(f, "\n");
#endif
     for (i=14; i>3 && strlen(gripemess[i])==0; i--) {
	  free(gripemess[i]);
	  gripemess[i] = NULL;
     }

     for (i=4; (i< 15) && gripemess[i]; i++) {
	  fprintf(f, "%s\n", gripemess[i]);
	  free(gripemess[i]);
     }
     fprintf(f, "\n[Sent from within the ");
     fprintf(f, Gtxt("Internet Gopher Information Client v%s.%s.%d",102),
		GOPHER_MAJOR_VERSION, GOPHER_MINOR_VERSION, PATCHLEVEL);
     fprintf(f, "]\n");

     fclose(f);

     CURexit(CursesScreen);
     printf(Gtxt("Mailing gripe to %s...",54), MailAddress);
     FIOsystem(mailcmd);
     unlink(tmpfilename);
     free(tmpfilename);
     CURenter(CursesScreen);
#else

     mailargv[0] = MAIL_COMMAND;
     mailargv[1] = email;
     mailargv[2] = NULL;

     fio = FIOopenProcess(mailargv[0], mailargv, "w");

     if (fio == NULL) {
	  CursesErrorMsg(Gtxt("Cannot send mail...",53));
	  return;
     }

     *(gripemess[3] + 71) = '\0';
     FIOwritestring(fio, "Subject: ");
     FIOwritestring(fio, gripemess[3]);
     FIOwritestring(fio, "\n\n");

     free(gripemess[3]);

#ifdef DESCRIBE_GOPHER_GRIPE
    FIOwritestring(fio, Gtxt("Regarding the following Gopher item:\n",229));
    GStoLink(GDgetEntry(CurrentDir, GDgetCurrentItem(CurrentDir)-1),
	     FIOgetfd(fio), TRUE);
    FIOwritestring(fio, "\n");
#endif


     for (i=14; i>3 && strlen(gripemess[i])==0; i--) {
	    free(gripemess[i]);
	    gripemess[i] = NULL;
     }

     for (i=4; i< 15 && gripemess[i]; i++) {
	  FIOwritestring(fio, gripemess[i]);
	  FIOwritestring(fio, "\n");
	  free(gripemess[i]);
     }

     FIOwritestring(fio, "\n[Sent from within the ");
     sprintf(version, Gtxt("Internet Gopher Information Client v%s.%s.%d",102),
	     GOPHER_MAJOR_VERSION, GOPHER_MINOR_VERSION, PATCHLEVEL);
     FIOwritestring(fio, version);
     FIOwritestring(fio, "]\n");

     FIOclose(fio);
#endif
}

/*
** do_index gets keywords from the user to search for.  It returns
** it to the calling process.  This storage is volotile. Callers should
** make a copy if they want to call do_index multiple times.
*/

char* do_index(GopherObj *ZeGopher)
{
     static char *inputline = NULL;
     static char *prompt[2];
     static char *response[2];

     if (inputline == NULL) {
	  inputline = (char *) malloc(sizeof(char)*256);
	  if (inputline == NULL)
	       perror("Out of memory"), CleanupandExit(-1);
	  *inputline = '\0';
     }

     prompt[0] = Gtxt("Words to search for",55);
     prompt[1] = NULL;

     response[0] = inputline;
     response[1] = NULL;

     if (CURRequest(CursesScreen, GSgetTitle(ZeGopher),prompt, response) == -1 )
	  return(NULL);

     if (*inputline == '\0')
	  return(NULL);
     else
	  return(inputline);
}


/*
 * this procedure just retrieves binary data from the socket and
 * pumps it into a "play" process.
 */

#define BUFSIZE 1400  /* A pretty good value for ethernet */

#ifndef VMS
void
suck_sound(int sockfd)
{
     FileIO *Play;
     int j;
     char buf[BUFSIZE];
     char playCmd[BUFSIZE];

     if (!RCdisplayCommand(GlobalRC, "Audio/basic", "", playCmd) ||
         !strncasecmp(playCmd, "- none -", 8) ||
	 playCmd == NULL || playCmd[0] == '\0') {
	  /*** Hey! no play command, bummer ***/
	  /** We're forked at this point, and we can't display anything.. **/
	  /*	  CursesErrorMsg(Gtxt("Sorry, this machine doesn't support sounds",159));*/

	  return;
     }
     
     Play = FIOopenCmdline(playCmd, "w");

     
     while(1) {
          j = read(sockfd, buf, BUFSIZE);
	  
	  if (j == 0)
	       break;
	  
	  FIOwriten(Play, buf, j);
     }
}
#endif

/*
 * fork off a sound process to siphon the data across the net.
 * So the user can listen to tunage while browsing the directories.
 */

void
do_sound(GopherStruct *ZeGopher)
{
#ifdef VMS
     CursesErrorMsg(Gtxt("Sorry, this machine doesn't support sounds",159));
#else
     int sockfd;
     BOOLEAN Waitforchld = FALSE;

     if ((sockfd = GSconnect(ZeGopher)) <0) {
	  check_sock(sockfd, GSgetHost(ZeGopher), GSgetPort(ZeGopher));
	  return;
     }

     /** Send out the request **/
     GStransmit(ZeGopher, sockfd, NULL, NULL, NULL);
     
     /** Okay, it's cool, we can fork off **/

     if (SOUNDCHILD != 0)
	  Waitforchld = TRUE;

     if ( (SOUNDCHILD = fork()) < 0)
	  ;/* Fork Error */
     
     else if (SOUNDCHILD == 0) {  /* Child Process */
	  suck_sound(sockfd);
	  exit(0);
     }
     
     /* Parent Process */
     
     closenet(sockfd);
     return;
#endif  /* not VMS */
}




/**
*** Show file takes a gopher text thing, writes it to a file
*** and passes it to your favorite pager.
**/

void
showfile(GopherObj *ZeGopher)
{
     char    *tmpfilename = NULL;
     FILE    *tmpfile;
     char    inputline[512];
     char    *view = NULL;
     char    mkstemp_template[] = "/tmp/gopher-XXXXXX";
     int     mkstemp_fd = -1;
     boolean WritePipe = FALSE,
             ForkOff   = FALSE;
     boolean GS2FileSucceeded = TRUE;
     int     Child;

     DebugGSplusPrint(ZeGopher, "showfile:start");

     view = Choose_View(ZeGopher);

     Debug("showfile:Choose_View returned view=%s\n",view);

     if ((view == NULL) || (*view == '\0'))
	  return;

     if (!RCdisplayCommand(GlobalRC, view, "", inputline) ||
         !strncasecmp(inputline, "- none -", 8) ||
	 inputline == NULL || inputline[0] == '\0') {
          CursesErrorMsg(Gtxt("No display command is mapped to this view!",113));
	  return;
     }
     Debug("showfile:inputline=%s\n",inputline);

     if (GSgetLocalFile(ZeGopher) == NULL ||
	 GSgetLocalView(ZeGopher) == NULL ||
	 strcmp(GSgetLocalView(ZeGopher), view)!= 0) {
	  /*** We need to retrieve the file ***/

	  if (*inputline == '|') {
#ifdef VMS
	       CursesErrorMsg(Gtxt("Unix piping requested.  Check your configuration!",176));
	       return;
#endif
	       if ((tmpfile = popen(inputline+1, "w")) == NULL) {
                   char buf[75];
                   snprintf(buf, sizeof(buf) - 1,
                            Gtxt("Coultn't execute %s", 81), inputline);
                   CursesErrorMsg(buf);
                   return;
               }
	       WritePipe = TRUE;
	       CURexit(CursesScreen);
	  } else {
	       /** Open a temporary file **/
#ifdef VMS
	       tmpfilename = tempnam(NULL,NULL);
#else /* !VMS */
#ifdef HAVE_MKSTEMP
               mkstemp_fd = mkstemp(mkstemp_template);
               tmpfilename = strdup(mkstemp_template);
               tmpfile = fdopen(mkstemp_fd, "w");
#else  /* ! HAVE_MKSTEMP */
	       tmpfilename = tempnam("/tmp","gopher");
               tmpfile = fopen(tmpfilename, "w");
#endif /* HAVE_MKSTEMP */
#endif /* VMS */

#if defined(VMS)  && defined(VMSRecords)
	       if (GSisText(ZeGopher, view))
		    /*** Use VARIABLE length records for text on VMS ***/
		    tmpfile = fopen_VAR(tmpfilename, "w");
	       else
		    /*** Use FIXED 512 records for binaries on VMS ***/
	            tmpfile = fopen_FIX(tmpfilename, "w");
#endif      /* VMS */
	       if (tmpfile == NULL) {
		    CURexit(CursesScreen);
		    fprintf(stderr, Gtxt("Couldn't make a tmp file!\n",83)), 
		    CleanupandExit(-1);
	       }
	       if (GSgetLocalFile(ZeGopher) != NULL &&
		   GSgetLocalView(ZeGopher) != NULL &&
		   strcmp(GSgetLocalView(ZeGopher), view) != 0)
		    unlink(GSgetLocalFile(ZeGopher));
	       GSsetLocalFile(ZeGopher, tmpfilename);
	  } /* | */
	  
	  if (inputline[strlen(inputline)-1] == '&') {
	       inputline[strlen(inputline)-1] = '\0';
	       ForkOff = TRUE;
	  }

#ifndef VMS
	  /* This is the child process, so exit.. */
	  if (ForkOff) {
	       if ((Child = fork()) < 0) {
		    ;/** Fork Error ***/
		    CursesErrorMsg("Fork Error!");
	       }
	       else if (Child >0) {
		    /*** Parent Process ***/
		    ;
		    CURenter(CursesScreen);
		    return;
	       }
	  }  /* Forkoff */
#endif
	  
 	  Debug("\n\rshowfile: view=%s ",view);
 	  Debug("\n\rcommand=%s ",inputline);
 	  Debug("\n\rfile=%s",tmpfilename);
 	  Debug("\n\r/%s\n",GSgetLocalFile(ZeGopher));
	  
	  GS2FileSucceeded = GStoFile(ZeGopher, tmpfile, view, twirl);

	  if (!GS2FileSucceeded) {
	       GSsetLocalFile(ZeGopher, NULL);
	       GSsetLocalView(ZeGopher, NULL);
	  }
	  else
	       GSsetLocalView(ZeGopher, view);


	  if (WritePipe)
	       pclose(tmpfile);  /* data went down pipe - displayed there */
	  else
	       fclose(tmpfile);  /* data is in tmpfile, will display below */

     } /* GopehrPluss || LocalFile = NULL */

     logrequest("ARRIVED AT", ZeGopher);

     if (!WritePipe && GS2FileSucceeded)
	  GSdisplay(ZeGopher);
     if (!GS2FileSucceeded)
          unlink(tmpfilename);

/*     if (!ForkOff) {
	  printf("\nPress <RETURN> to continue: ");
	  fflush(stdin);
	  fflush(stdin);
	  getchar();
	  CURenter(CursesScreen);
     }*/


     if (tmpfilename!=NULL) free(tmpfilename);

     if (ForkOff)
	  exit(-1);

     CURenter(CursesScreen);
     return;
}


/*
 * Eventually this routine should allow viewing of the movie type by
 * forking off a movie process to siphon the data across the net.
 */

void
do_movie(GopherStruct *ZeGopher)
{
     showfile(ZeGopher);
}


/*
** Pushgopher takes a GopherThing pointer and adds it to it's stack.
**
** Ick this must be fixed!
*/

void
pushgopher(GopherDirObj *ZeDir)
{

     OldDirs[iLevel]= ZeDir;
     iLevel ++;
}

/*
** If the stack is empty, popgopher returns a -1
*/

int
popgopher(GopherDirObj **ZeDir)
{

     if (iLevel == 0)
	  return(-1);

     iLevel --;

     *ZeDir =  OldDirs[iLevel];

     return(0);
}


#ifdef VMS
#include <perror.h>
#else
extern int h_errno;
#ifndef HAVE_STRERROR
extern int sys_nerr;
extern char *sys_errlist[];
#endif
extern int  errno;
#endif

void check_sock(int sockfd, char *host, int port)
{
     char DispString[WHOLELINE];
     char DispString2[WHOLELINE];
     char *DispStrings[4];

     /* NULL DispStrings entries here, so can override below */
     DispStrings[3] = NULL;

     if (sockfd <0) {
	  sprintf(DispString, 
		  Gtxt("Cannot connect to host %.40s, port %d.",72), 
		  host, port);

	  switch (sockfd) {
	  case -2:
	       DispStrings[2] = Gtxt("Hostname is unknown.",100);
	       break;
	  case -3:
	       DispStrings[2] = Gtxt("Unable to allocate a socket.",175);
	       break;
	  case -4:
#if defined(VMS) && defined(MULTINET)
	       sprintf(DispString2, "%.78s.", vms_errno_string());
	       DispStrings[2] = DispString2;
#else
#ifdef __DECC
	       if (errno > 0 && errno <= 65) {
#else
#ifdef HAVE_STRERROR
               if (1) {
#else
	       if (errno > 0 && errno <= sys_nerr) {
#endif
#endif
		    sprintf(DispString2, Gtxt("Connection failed: %s.",78),
#if defined(VMS) || defined(HAVE_STRERROR)
			    strerror(errno));
#else
		    sys_errlist[errno]);
#endif
	            DispStrings[2] = DispString2;
	       } else
		    DispStrings[2] = 
			 Gtxt("Connection to remote host failed.",79);
#endif

	  default:
	       DispStrings[2] = Gtxt("Unknown error.",177);
	  }
	  DispStrings[0] = DispString;
	  DispStrings[1] = "";
          DispStrings[3] = NULL;
      
          CURBeep(CursesScreen);

	  CURDialog(CursesScreen, Gtxt("Network Error",110), DispStrings);
     }
}


BOOLEAN
ReallyQuit(void)
{
     char yesno[3];
	       
     yesno[0] = 'y';
     yesno[1] = '\0';
     
     CURgetYesorNo(CursesScreen, Gtxt("Really quit (y/n) ?",124), yesno);
     if (*yesno == 'y') {
	  return(TRUE);
     }
     
     return(FALSE);
}
     

RETSIGTYPE
CleanupandExit(int exitval)
{
     GopherDirObj *gd;
#ifdef VMS
     extern boolean DidCleanup;
#endif

#ifdef SIGWINCH
     signal(SIGWINCH, SIG_IGN);
#endif
     signal(SIGINT, SIG_IGN);
     signal(SIGQUIT, SIG_IGN);
     signal(SIGPIPE, SIG_IGN);
     signal(SIGTERM, SIG_IGN);
     signal(SIGHUP, SIG_IGN);


     if (CursesScreen)
	  CURexit(CursesScreen);

     if (ChangedDefs)
	  RCtoFile(GlobalRC);

     do {
	  if (CurrentDir != NULL && CurrentDir != RCgetBookmarkDir(GlobalRC))
	       GDdestroy(CurrentDir);
     }
     while (popgopher(&CurrentDir) != -1);
     
     if (GlobalRC) {
	  gd = RCgetBookmarkDir(GlobalRC);
	  if (gd != NULL)
	       GDdestroy(gd);
     }
     logrequest("EXIT", NULL);
#ifdef VMS
     DidCleanup = TRUE;
#endif
     exit(exitval);
}



/**************
** This bit of code catches control-c's, it cleans up the curses stuff.
*/

RETSIGTYPE
controlc_old(int sig)
{

#ifdef VMS
     if (!CursesScreen->inCurses) {
          /** Reprime the signal and set flag **/
          if (signal(SIGINT, controlc) == SIG_ERR)
	       perror("signal died:\n"), CleanupandExit(-1);
 	  HadVMSInt = TRUE;
          return;
     }
#endif

     if (CurrentDir == NULL || GDgetNumitems(CurrentDir) <= 0) {
          CURexit(CursesScreen);
	  fprintf(stderr,Gtxt("gopher: Nothing received for main menu, can't continue\n",60));
	  CleanupandExit(1);
     }

     if (ReallyQuit())
     {
	  CleanupandExit(0);
     }
     else {
	  CURresize(CursesScreen);
	  /* scline(-1, 1, CurrentDir); */
	  /** Interrupt search, go back a level?? **/
     }

     /*
      * Reprime the signals...
      */

     if (signal(SIGINT, controlc) == SIG_ERR)
	  perror("signal died:\n"), CleanupandExit(-1);


     /** Really should be siglongjmp **/
     longjmp(Jmpenv,1);
}


/**************
** This bit of code catches window size change signals
**/

RETSIGTYPE
sizechange(int sig)
{
     int lines, cols;
     
#ifdef  TIOCGWINSZ
     static struct      winsize zewinsize;        /* 4.3 BSD window sizing */
#endif

     lines = LINES;
     cols  = COLS;
     
#ifdef  TIOCGWINSZ
     if (ioctl(0, TIOCGWINSZ, (char *) &zewinsize) == 0) {
	  lines = zewinsize.ws_row;
	  cols  = zewinsize.ws_col;
     } else {
#endif
	  /* code here to use sizes from termcap/terminfo, not yet... */
	  ;
#ifdef  TIOCGWINSZ
     }

     if (lines != LINES || cols != COLS) {
	  LINES = lines;
	  COLS  = cols;
	  CURresize(CursesScreen);
     
	  scline(-1, 1, CurrentDir);
     }

     if (signal(SIGWINCH, sizechange)==SIG_ERR)
	  perror("signal died:\n"), CleanupandExit(-1);

	    
#endif

}



/**********
**
** Set up all the global variables.
**
***********/

void
Initialize(void)
{

     Debugmsg("Initialize\n")
     GlobalRC        = RCnew();

     /** get defaults from the rc file **/

     RCfromUser(GlobalRC);


     /*** Set up the curses environment ***/
     
     CursesScreen = CURnew();

     if (strcmp(CURgetTerm(CursesScreen), "unknown")==0)
	  fprintf(stderr, Gtxt("I don't understand your terminal type\n",101)), 
	  CleanupandExit(-1);


     /*** Make a signal handler for window size changes ***/

#ifdef SIGWINCH
     CURsetSIGWINCH(CursesScreen, sizechange);
     if (signal(SIGWINCH, sizechange)==SIG_ERR)
	  perror("signal died:\n"), 
	  CleanupandExit(-1);

#endif

     if (signal(SIGINT, controlc) == SIG_ERR)
	  perror("signal died:\n"), 
	  CleanupandExit(-1);

     if (signal(SIGQUIT, CleanupandExit) == SIG_ERR)
	  perror("signal died:\n"),
	  CleanupandExit(-1);

     if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	  perror("signal died:\n"), 
	  CleanupandExit(-1);

     if (signal(SIGTERM, CleanupandExit) == SIG_ERR)
	  perror("signal died:\n"),
	  CleanupandExit(-1);

     if (signal(SIGHUP, CleanupandExit) == SIG_ERR)
	  perror("signal died:\n"),
	  CleanupandExit(-1);

       

     /*** Init MainWindow ****/
     CURenter(CursesScreen);


     /*** Check for new global configuration file ***/
     if (RCisGlobalNew()) {
	  int result;
	  char *mess[2];
	  mess[0] =  Gtxt("A new configuration is available.  Load it?",218);
	  mess[1] = NULL;

	  /*** Ask if the user wants to reload values from globalrc ***/
	  result = CURDialog(CursesScreen, "", mess);

	  if (result == 0) {
	       /*** Do it...  ****/
	       RCreadGlobalRC(GlobalRC);
	  }
	  ChangedDefs = TRUE;
	       
     }
}



/*
 * This stuff will set the options in a nice way...
 */

void
SetOptions(void)
{
     int choice, restricted = FALSE;
     char     *shell;

     static char **OptionNewApp = NULL, **OptionsMenu = NULL;
     static char **noyes = NULL;
     static char **printchoice = NULL;

     if (OptionNewApp == NULL) {
	  OptionNewApp = (char**) malloc(sizeof(char*) * 4);
	  OptionNewApp[0] = Gtxt("Content-Type Name",27);
	  OptionNewApp[1] = Gtxt("Display Application",28);
	  OptionNewApp[2] = Gtxt("Printing Application",29),
	  OptionNewApp[3] = NULL;
     }

     if (OptionsMenu == NULL) {
	  OptionsMenu = (char **) malloc(sizeof(char*) * 5);
	  OptionsMenu[0] = Gtxt("General Options",30);
	  OptionsMenu[1] = Gtxt("Configure Display Applications",31);
	  OptionsMenu[2] = Gtxt("Configure Printing Applications",32);
	  OptionsMenu[3] = Gtxt("Define New Content-Type",33);
	  OptionsMenu[4] = NULL;
     }
     if (noyes == NULL) {
	  noyes = (char**) malloc(sizeof(char*) * 3);
	  noyes[0] = Gtxt("No",111);
	  noyes[1] = Gtxt("Yes",180);
	  noyes[2] = NULL;
     }
     if (printchoice == NULL) {
	  printchoice = (char**) malloc(sizeof(char*) * 3);
	  printchoice[0] = Gtxt("Default System Printer", 219);
	  printchoice[1] = Gtxt("ANSI attached Printer", 220);
	  printchoice[2] = NULL;
     }

     /*
      * If the user is running a restricted shell, don't allow him/her to
      * set applications, because he/she can specify a shell.  This is
      * independent of -s and -S because rsh users may have a shell.
      */
#ifndef VMS
     restricted = (shell = getenv("SHELL")) == NULL
	  || strcmp(shell+strlen(shell) - 3, "rsh") == 0;
     
#endif

     if (SecureMode || NoShellMode || restricted) {
	  CursesErrorMsg(Gtxt("Sorry, you are not allowed to set options in secure mode.",163));
	  return;
     }

     /** Display Configuration Menu and allow user to choose **/
     choice = CURChoice(CursesScreen, Gtxt("Gopher Options",94), OptionsMenu,
			Gtxt("Your Choice",181), -1);

     if (choice == -1)
	  return;

     switch (choice) {

     case 0:
     {
	  /** General Options **/
	  
	  Requestitem *items[3];
	  Requestitem bold;
	  Requestitem printer;
	  
	  bold.prompt  = Gtxt("Bold Search words in Builtin Pager",66);
	  bold.thing   = CUR_CHOICE;
	  bold.choices = noyes;
	  bold.chooseitem = RCsearchBolding(GlobalRC);

	  printer.prompt  = Gtxt("Print to",221);
	  printer.thing   = CUR_CHOICE;
	  printer.choices = printchoice;
	  printer.chooseitem = RCuseANSIprinter(GlobalRC);


	  items[0] = &bold;
	  items[1] = &printer;
	  items[2] = NULL;

	  if (CURrequester(CursesScreen, Gtxt("General Options",30), items)==0) {
	       RCsetSearchBolding(GlobalRC, items[0]->chooseitem);
	       RCsetANSIprinter(GlobalRC,  items[1]->chooseitem);
	       ChangedDefs = TRUE;
	  }

	  break;
     }

     case 1:
     case 2:
     {
	  /** Display/Print Applications **/

	  char     **gplusviews;
	  int      i;
	  int      numoptions;
	  RCMapObj *rcm;
	  char     **Responses;
	  char     *zetitle;

	  numoptions = RCMAgetNumEntries(GlobalRC->commands);
	  gplusviews = (char **) malloc(sizeof(char *) * (numoptions + 1));
	  Responses = (char **) malloc(sizeof(char *) * (numoptions + 1));

	  for (i = 0 ; i < numoptions; i++) {
	       rcm = RCMAgetEntry(GlobalRC->commands, i);
	       gplusviews[i] = RCMgetView(rcm);
	       Responses[i] = (char *) malloc(sizeof(char) * MAXSTR);
	       if (choice == 1)
		    strcpy(Responses[i], RCMgetDisplaycmd(rcm));
	       else
		    strcpy(Responses[i], RCMgetPrintcmd(rcm));
	  }
	  gplusviews[i] = NULL;
	  Responses[i] = NULL;

	  if (choice == 1)
	       zetitle = Gtxt("Configure Display Applications",31);
	  else
	       zetitle = Gtxt("Configure Printing Applications",32);

	  if (CURRequest(CursesScreen, zetitle, gplusviews, 
			 Responses) == 0) {
	       char tmpstr[512];
	       
	       while (i-- > 0) {
		    rcm = RCMAgetEntry(GlobalRC->commands, i);
		    if (choice == 1)
			 /** Display Applications **/
			 sprintf(tmpstr, "%s,%s,%s", gplusviews[i], 
				 Responses[i], RCMgetPrintcmd(rcm));
		    else
			 /** Print Applications **/
			 sprintf(tmpstr, "%s,%s,%s", gplusviews[i], 
				 RCMgetDisplaycmd(rcm), Responses[i]);
			 
		    RCMAfromLine(GlobalRC->commands, tmpstr);
	       }
	       ChangedDefs = TRUE;
	  }

	  for (i = 0 ; i <= numoptions; i++)
	       if (Responses[i])
		    free(Responses[i]);
	  free(Responses);
	  free(gplusviews);

	  break;
     }

     case 3:
     {
	  /** Add a new Content-Type and define **/
	  /** its Display and Printing applications **/
	  
	  int  i;
	  char *Responses[4];

	  for (i = 0; i < 3; i++) {
	       Responses[i] = (char *) malloc(sizeof(char) * MAXSTR);
	       Responses[i][0] = '\0';
	  }
	  Responses[3] = NULL;
	  
	  if ((CURRequest(CursesScreen, Gtxt("Define New Content-Type",33), 
	       OptionNewApp, Responses) == 0) && (*Responses[0] != '\0')) {
	       char tmpstr[512];
	       
	       sprintf(tmpstr, "%s,%s,%s", Responses[0], Responses[1],
		       Responses[2]);
	       RCMAfromLine(GlobalRC->commands, tmpstr);
	       ChangedDefs = TRUE;
	  }

	  for (i = 0; i < 3; i++)
	       free(Responses[i]);
          break;
     }

     }  /** End of switch on option type **/
}



static char *search_string = NULL;

int
main(int argc, char **argv)
{
     BOOLEAN bDone = FALSE;
     char sTmp[80];
     GopherStruct *RootGophers[2];
     int numhosts = 2;
     int TypedChar;
     /*** for getopt processing ***/
     int c;
     extern char *optarg;
     extern int optind;
     int errflag =0, i;
     int curitem;
     int Garbled = TRUE;
     boolean Bkmarksfirst = FALSE;
     boolean startAtHome = TRUE;
     

     /*** Initialize international languages ***/
     Gtxtlocale(LC_ALL, "");


#if defined(GINTERNATIONAL) && !defined(VMS)

#ifdef LC_CTYPE
     Gtxtlocale(LC_CTYPE, "iso_8859_1");
#endif

#ifdef LC_MESSAGES
     if (getenv("LC_MESSAGES") != NULL)
	 Gtxtlocale(LC_MESSAGES, getenv("LC_MESSAGES"));
#endif /* LC_MESSAGES */

#ifndef __osf__
     Gtxtopen("gopher", 0);  /** OSF seems to mess this up... **/
#endif

     if (Gcatd == (nl_catd) -1) {
	  /** Try finding it elsewhere.. **/
	  char fname[256];
	  if (getenv("LC_MESSAGES") != NULL) {
	       sprintf(fname, "%s/gophernls/%s.cat", GOPHERLIB, getenv("LC_MESSAGES"));
	       Gtxtopen(fname, 0);
	  }
     }
#endif	  /* GINTERNATIONAL & !VMS */

#ifdef VMS
     argv[0] = "gopher";
#endif

     for (i=0; i<2; i++) {
	  RootGophers[i] = GSnew();
	  GSsetType (RootGophers[i], A_DIRECTORY);   
	  GSsetPath (RootGophers[i],"");
	  GSsetGplus(RootGophers[i], TRUE);
     }
     
     /** Should generalize this to >2 hosts .... Sigh... ***/
     GSsetHost (RootGophers[0], CLIENT1_HOST);
     GSsetPath (RootGophers[0], CLIENT1_PATH);
     GSsetPort (RootGophers[0], CLIENT1_PORT);

     GSsetHost (RootGophers[1], CLIENT2_HOST);
     GSsetHost (RootGophers[1], CLIENT2_PATH);
     GSsetPort (RootGophers[1], CLIENT2_PORT);

     if (CLIENT2_PORT == 0)
	  numhosts = 1;

     sTmp[0] = '\0';


     while ((c = getopt(argc, argv, "DsSbrp:t:T:i:")) != -1)
	  switch (c) {
	  case 's':
	       SecureMode = TRUE;
	       break;
	  case 'S':	/* Similar to secure, but assumes own Unix account */
			/* No change in behaviour if this not set */
		NoShellMode = TRUE;
		break;
	  case 'i':
	       search_string = optarg;
	       break;

	  case 'p':
	       GSsetPath(RootGophers[0], optarg);
	       GSsetPath(RootGophers[1], optarg);
	       GSsetGplus(RootGophers[0], FALSE);
	       GSsetGplus(RootGophers[1], FALSE);

	       startAtHome = FALSE;
	       break;
	  case 'T':
	       GSsetType(RootGophers[0], *optarg);
	       GSsetType(RootGophers[1], *optarg);
	       if (optarg[1] == '+') {
		    GSsetGplus(RootGophers[0], TRUE);
		    GSsetGplus(RootGophers[1], TRUE);
	       } else if (optarg[1] == '?') {
		    GSsetAsk(RootGophers[0], TRUE);
		    GSsetAsk(RootGophers[1], TRUE);
	       }
	       startAtHome = FALSE;
	       break;
	  case 't':
	       GSsetTitle(RootGophers[0], optarg);
	       GSsetTitle(RootGophers[1], optarg);
	       break;
	  case 'D':
	       DEBUG = TRUE;
	       Debugmsg("gopher starting - debug on\n")
	       break;
	  case 'r':
	       RemoteUser = TRUE;
	       break;
	  case 'b':
	       Bkmarksfirst = TRUE;
	       break;
	  case '?':
	       errflag++;
	  }


     if (errflag) {
	  
	  fprintf(stderr, Gtxt("Usage: %s [-sSbDr] [-T type] [-p path] [-t title] [hostname port]+\n",9), argv[0]);
	  fprintf(stderr, 
	    Gtxt("     -s      secure mode, users without own account\n",10));
	  fprintf(stderr,
	    Gtxt("     -S      secure mode, users with own account\n",11));
	  fprintf(stderr,
	    Gtxt("     -p path specify path to initial item\n",12));
	  fprintf(stderr,
            Gtxt("     -T type Type of initial item\n",13));
	  fprintf(stderr,
            Gtxt("     -i      Search argument (for -T 7)\n",14));
	  fprintf(stderr, 
	    Gtxt("     -b      Bookmarks first\n",15));
	  fprintf(stderr,
            Gtxt("     -r      Remote user\n",16));
	  fprintf(stderr, 
	    Gtxt("     -D      Debug mode\n",17));
	  CleanupandExit(-1);
     }

     /**** Get host #1 from the command line ****/
     
     if (optind < argc) {

	  if (strchr(argv[optind], ':') != NULL) {
	       GopherObj *tempGopher = GSnew();
	       int doneflags = 0;

	       doneflags = GSfromURL(tempGopher, argv[optind],
	       			     AFTP_HOST, AFTP_PORT, doneflags);
	       GScpy(RootGophers[0], tempGopher);
	       GScpy(RootGophers[1], tempGopher);
	       GSdestroy(tempGopher);
	       startAtHome = FALSE;
	       optind++;
	  } else {
	       GSsetPort(RootGophers[0], 70);  /* Just in case the default */
	       GSsetPort(RootGophers[1], 70);  /* isn't 70 */
	       
               /* Restore blank path if a host was specified but -p was not */

	       if (startAtHome)
	           GSsetPath(RootGophers[0], "");

	       GSsetHost(RootGophers[0], argv[optind]);
	       GSsetHost(RootGophers[1], "");  /** Nuke the 2nd alternative host **/
	       GSsetPort(RootGophers[1], 0);
	       numhosts = 1;
	       startAtHome = FALSE;
	       optind++;
	  }
     }
     if (optind < argc) {
	  GSsetPort(RootGophers[0], atoi(argv[optind]));
	  optind++;
     }

     /*** Get host #2 from the command line... ***/
     if (optind < argc) {
	  GSsetHost(RootGophers[1], argv[optind]);
	  numhosts = 2;
	  optind++;
     }
     if (optind < argc) {
	  GSsetPort(RootGophers[1], atoi(argv[optind]));
	  optind++;
     }

     /*** If the title hasn't been set, then add a default title **/
     if (GSgetTitle(RootGophers[0]) == NULL) {
	  sprintf(sTmp, Gtxt("Home Gopher server: %.59s",97),
		  GSgetHost(RootGophers[0]));
	  GSsetTitle(RootGophers[0], sTmp);
	  sprintf(sTmp, Gtxt("Home Gopher server: %.59s",97),
		  GSgetHost(RootGophers[1]));
	  GSsetTitle(RootGophers[1], sTmp);
     }

     /*** Set up global variables, etc. ***/

#ifdef SOCKS
     SOCKSinit(argv[0]);
#endif

     Initialize();

     if (startAtHome && (RCgetHome(GlobalRC) != NULL)) {
	  GScpy(RootGophers[0], RCgetHome(GlobalRC));
	  GScpy(RootGophers[1], RCgetHome(GlobalRC));
     }

     if (Bkmarksfirst) {
	  CurrentDir = RCgetBookmarkDir(GlobalRC);
	  
     } else {
	  int rnum;

	  srand(time(NULL));
	  rnum = rand() % numhosts;
	  process_request(RootGophers[rnum]);

          /* just process the command line entry */
          if( GSgetType(RootGophers[0]) != A_DIRECTORY &&
	      GSgetType(RootGophers[0]) != A_INDEX) {
	       refresh();
	       if( GSgetLocalFile(RootGophers[0]) != NULL)
	           unlink(GSgetLocalFile(RootGophers[0]));
	       CleanupandExit(0);
          }
     }

#ifdef DEBUGGING
	if (CurrentDir == NULL)  {
	     Debugmsg("CurrentDir=NULL\n");
	}
	else
	     if (GDgetNumitems(CurrentDir) <=0) {
		  Debugmsg("GDgetNumitems<=0\n");
	     }
#endif

     if (CurrentDir == NULL || GDgetNumitems(CurrentDir) <= 0) {
	  /*
	   * We didn't get anything from that gopher server.  Either
	   * it is down, doesn't exist, or is empty or otherwise
	   * busted.
           *
           * Try any alternative hosts that are available..
	   */

	  while (optind < argc && CurrentDir == NULL) {
	       Debugmsg("Trying alternate site\n");
	       GSsetHost(RootGophers[0], argv[optind]);
	       optind++;
		   if (optind < argc) {
			   GSsetPort(RootGophers[0], atoi(argv[optind]));
			   optind++;
		   }
	       process_request(RootGophers[0]);
	  }
     }	  

     if (CurrentDir == NULL || GDgetNumitems(CurrentDir) <= 0) {
#ifdef DEBUGGING
	if (CurrentDir == NULL)  {
	     Debugmsg("B: CurrentDir=NULL\n");
	}
	else if (GDgetNumitems(CurrentDir) <=0) {
	     Debugmsg("B: GDgetNumitems<=0\n");
	}
#endif	

	CURexit(CursesScreen);
	fprintf(stderr,
		Gtxt("%s: Nothing received for main menu, can't continue\n",188)
		, argv[0]);
	CleanupandExit(1);
   }

     while (bDone == FALSE)
     {
	  GetMenu(CurrentDir, &TypedChar, Garbled);
	  Garbled = TRUE;
#ifdef DESCRIBE_GOPHER
#undef DESCRIBE_GOPHER
#endif
#ifdef DESCRIBE_GOPHER_GRIPE
#define	DESCRIBE_GOPHER(a,b)	describe_gopher((a),(b), NULL)
#else
#define	DESCRIBE_GOPHER(a,b)	describe_gopher((a),(b))
#endif

	  switch(TypedChar)
	  {
	  case '\r':
	  case '\n':
	       /*** Select the designated item ***/
		   if ((curitem = GDgetCurrentItem(CurrentDir)) <= 0)
			   curitem = 1;
	       if (process_request(GDgetEntry(CurrentDir, curitem-1))==1)
		    ;  /** Don't do anything if we failed, I guess **/
#if defined(VMS) && defined(A_LANGUAGE)
	       if (CurrentDir == setlocale_LangDir) {
		    sprintf(sTmp, Gtxt("Home Gopher server: %.59s",97),
						    GSgetHost(RootGophers[0]));
		    GDsetTitle(OldDirs[0], sTmp);
		    GSsetTitle(RootGophers[0], sTmp);
		    Garbled = TRUE;
		    goto Backup_Resetlocale;
	       }		    
#endif
	       break;
	       
	  case '\0':
	       /*** What the heck? ***/
	       CursesErrorMsg("Strange Error occurred!");
	       break;
	       
	  case 'u':
	  case 'U': 
#ifdef AUTOEXITONU
	  case 'q':
#endif
#if defined(VMS) && defined(A_LANGUAGE)
	  Backup_Resetlocale:
#endif
	  {
	       GopherDirObj *tempGdir;

	       logrequest("PREVIOUS DIRECTORY", NULL);

	       /*** Don't highlight texts which aren't search hits ***/
	       Searchstring = NULL;

	       /*** Go up a directory level ***/
	       tempGdir = CurrentDir;
	       /** Don't destroy root level directory, or bookmarks **/
	       if (popgopher(&CurrentDir)==0 && tempGdir != CurrentDir) {
		    if (tempGdir != RCgetBookmarkDir(GlobalRC))
#if defined(VMS) && defined(A_LANGUAGE)
		      if (tempGdir != setlocale_LangDir)
#endif
			 GDdestroy(tempGdir);
#ifdef AUTOEXITONU
	       } else {
	       		bDone = TRUE;
	       		CURexit(CursesScreen);
#else
	       } else {
		    Garbled = FALSE;
#endif
	       }
	  }
	       break;

	  case 'g':
	       /*** Gripe! ***/
#ifdef NOGRIPE_SECURE
	       if (SecureMode || NoShellMode) {
		    CursesErrorMsg(Gtxt
				   ("Sorry, can't submit gripes in securemode",
							227));
		    break;
	       }
#endif
	       Gripe(GDgetEntry(CurrentDir,GDgetCurrentItem(CurrentDir)-1));
	       break;

	  case 's':  /*** Save a file directly ***/
	       if (SecureMode || NoShellMode) {
		    CursesErrorMsg(Gtxt("Sorry, can't save files with this account",152));
		    break;
	       }
	       Save_file(GDgetEntry(CurrentDir,GDgetCurrentItem(CurrentDir)-1), NULL, NULL);
	       break;
	       
          case 'S':   /*** Save list of menu items (objects) to a file ***/
	       if (SecureMode || NoShellMode) {
		    CursesErrorMsg(Gtxt("Sorry, can't save files with this account",152));
		    break;
	       }
	       Save_list(CurrentDir);
	       break;
	       
	  case 'D':
	       Download_file(GDgetEntry(CurrentDir,GDgetCurrentItem(CurrentDir)-1));
	       break;

	  case 'o': /** Open a new session **/
	  {
	       GopherObj *tempgs = GSnew();
	       char *prompts[4];
	       char *responses[4];
	       int i;

	       if (SecureMode) {
		    CursesErrorMsg(Gtxt("Sorry, you're not allowed to do this",164));
		    break;
	       }

	       prompts[0]=Gtxt("Hostname",99);
	       prompts[1]=Gtxt("Port",118);
	       prompts[2]=Gtxt("Selector (Optional)",144);
	       prompts[3]=NULL;
	       
	       for (i=0; i<3; i++) {
		    responses[i] = (char*)malloc(COLS*sizeof(char));
		    *responses[i] = '\0';
	       }
	       strcpy(responses[1], "70");
		    
	       responses[3] = NULL;
	       
	       if ((CURRequest(CursesScreen, 
			       Gtxt("Connect to a new Gopher Server",75),
			       prompts, responses) != -1) 
		   && *responses[0]!='\0') {
		    
		    GSsetType(tempgs, A_DIRECTORY);
		    GSsetTitle(tempgs, responses[0]);
		    GSsetHost(tempgs, responses[0]);
		    GSsetPort(tempgs, atoi(responses[1]));
		    GSsetPath(tempgs, responses[2]);
	       
		    Load_Dir(tempgs);
	       }

	       GSdestroy(tempgs);
	       for (i=0; i<3; i++)
		    free(responses[i]);
	       break;
	  }


	  case 'w': /** Open a new session from a URL **/
	  {
	       GopherObj *tempgs = GSnew();
	       char *prompt[2];
	       char *response[2];
	       int doneflags = 0;

	       if (SecureMode) {
		    CursesErrorMsg("Sorry, you're not allowed to do this");
		    break;
	       }

	       prompt[0]=
	           "http, gopher, ftp, telnet or tn3270 URL to connect to:";
	       prompt[1]=NULL;
	       
	        response[0] = (char*)malloc(COLS*sizeof(char));
	       *response[0] = '\0';
		response[1] = NULL;
	       
	       if ((CURRequest(CursesScreen, "Connect to a new server",
			  prompt, response) != -1) && *response[0]!='\0') {
		    
		    doneflags = GSfromURL(tempgs, response[0],
		    			  AFTP_HOST, AFTP_PORT, doneflags);
		    if (doneflags & G_HOST) {
			 if (!(doneflags & G_PATH))
			      GSsetPath(tempgs, "");
			 if (!(doneflags & G_TYPE))
			      GSsetType(tempgs, A_DIRECTORY);
			 if (!(doneflags & G_NAME))
			      GSsetTitle(tempgs, response[0]);

			 process_request(tempgs);
		    }
	       }

	       GSdestroy(tempgs);
	       free(response[0]);
	       break;
	  }

	  case 'f': /** Open an aFTP session **/
	  {
	       GopherObj *tempgs = GSnew();
	       char *prompts[3];
	       char *responses[3];
	       char FTPpath[256];
	       int i;

	       if (SecureMode) {
		    CursesErrorMsg(Gtxt("Sorry, you're not allowed to do this",164));
		    break;
	       }

	       prompts[0]=Gtxt("anonymous FTP Host",187);
	       prompts[1]=Gtxt("Selector (Optional)",144);
	       prompts[2]=NULL;
	       
	       for (i=0; i<2; i++) {
		    responses[i] = (char *) malloc(sizeof(char)*COLS);
		    *responses[i] = '\0';
	       }
		    
	       responses[2] = NULL;
	       
	       if ((CURRequest(CursesScreen,
		   Gtxt("Connect to an anonymous FTP Server via the Gopher gateway",76),
			   prompts, responses) == -1) || *responses[0] == '\0')
		    break;

	       GSsetType(tempgs, A_DIRECTORY);
	       GSsetTitle(tempgs, responses[0]);
	       GSsetHost(tempgs, AFTP_HOST);
	       GSsetPort(tempgs, AFTP_PORT);
	       if (responses[1][strlen(responses[1])-1] != '/')
	            strcat(responses[1], "/");
	       sprintf(FTPpath, "ftp:%s@%s%s", responses[0],
		              (*responses[1] == '/') ? "" : "/", responses[1]);
	       GSsetPath(tempgs, FTPpath);
	       
	       Load_Dir(tempgs);

	       GSdestroy(tempgs);
	       for (i=0; i<2; i++)
		    free(responses[i]);
	       break;
	  }

	  case 'r':  /** Go to root menu of current item **/
	  {
	       char GSType;
	       GopherObj *tempgs;

	       if (SecureMode) {
		    CursesErrorMsg(Gtxt("Sorry, you're not allowed to do this",164));
		    break;
	       }

		   if ((curitem = GDgetCurrentItem(CurrentDir)) <= 0)
			   curitem = 1;
	       GSType = GSgetType(GDgetEntry(CurrentDir, curitem-1));

	       tempgs = GSnew();

	       /** These Gopher objects don't necessarily point 
		   to machines running Gopher servers **/
	       if (GSType == A_CSO || GSType == A_TELNET || 
		   GSType == A_TN3270 || GSType == A_ERROR ||
		   GSType == A_INFO) {

		    /** Try the parent instead **/
		    if(GDgetLocation(CurrentDir) != NULL)
			 GScpy(tempgs, GDgetLocation(CurrentDir));
		    else
			 GScpy(tempgs, GDgetEntry(OldDirs[iLevel-1], 
				     GDgetCurrentItem(OldDirs[iLevel-1])-1));
	       }
	       else
		    if ((curitem = GDgetCurrentItem(OldDirs[iLevel-1])) <= 0)
			 curitem = 1;
		    GScpy(tempgs, GDgetEntry(CurrentDir, 
			  GDgetCurrentItem(CurrentDir)-1));

	       GSsetType(tempgs, A_DIRECTORY);
	       GSsetTitle(tempgs, Gtxt("Root menu: ",135));
	       STRcat(tempgs->Title, GSgetHost(tempgs));
	       GSsetPath(tempgs, NULL);

	       Load_Dir(tempgs);
	       GSdestroy(tempgs);
	       break;
	  }

	  case 'R':  /** Go to root menu of current menu **/
	  {
	       GopherObj *tempgs;

	       if (SecureMode) {
		    CursesErrorMsg(Gtxt("Sorry, you're not allowed to do this",164));
		    break;
	       }

	       if (iLevel == 0) {
		    Garbled = FALSE;
		    break;
	       }

	       tempgs = GSnew();

	       if (GDgetLocation(CurrentDir) != NULL)
		    GScpy(tempgs, GDgetLocation(CurrentDir));
	       else
		    GScpy(tempgs, GDgetEntry(OldDirs[iLevel-1], 
				   GDgetCurrentItem(OldDirs[iLevel-1])-1));

	       GSsetType(tempgs, A_DIRECTORY);
	       GSsetTitle(tempgs, Gtxt("Root menu: ",135));
	       STRcat(tempgs->Title, GSgetHost(tempgs));
	       GSsetPath(tempgs, NULL);

	       Load_Dir(tempgs);
	       GSdestroy(tempgs);
	       break;
	  }

	  case 'v':  /** View bookmark list **/
	  {
	       logrequest("VIEW BOOKMARKS", NULL);

	       if (RCgetBookmarkDir(GlobalRC) == NULL) {
		    CursesErrorMsg(Gtxt("No bookmarks are defined",112));
		    break;
	       }

	       /** Don't do anything if we are already viewing bookmarks **/
	       if (CurrentDir == RCgetBookmarkDir(GlobalRC)) {
		    Garbled = FALSE;
		    break;
	       }

	       /** Don't push an empty gopher directory... **/
	       if (CurrentDir != NULL)
		    pushgopher(CurrentDir); 
	       
	       CurrentDir = RCgetBookmarkDir(GlobalRC);

	       break;
	  }	       

	  case 'a': /** add current item as a bookmark **/
	  {
	       GopherObj *tmpgs;
	       char newtitle[256];
	       
	       if (RCgetBookmarkDir(GlobalRC) == NULL) {
 		    RCsetBookmarkDir(GlobalRC,GDnew(32));
		    GDsetTitle(RCgetBookmarkDir(GlobalRC), Gtxt("Bookmarks",67));
	       }

	       tmpgs = GSnew();
	       GScpy(tmpgs, GDgetEntry(CurrentDir, GDgetCurrentItem(CurrentDir)-1));
	       
	       strcpy(newtitle, GSgetTitle(tmpgs));
	       if (CURGetOneOption(CursesScreen, newtitle,
	       		   Gtxt("Name for this bookmark:",109), newtitle)==0) {
		    if (*newtitle != '\0') {
			 GSsetTitle(tmpgs, newtitle);
			 GDaddGS(RCgetBookmarkDir(GlobalRC), tmpgs);
			 ChangedDefs = TRUE;
		    }
	       }

	       logrequest("ADD BOOKMARK", tmpgs);

               /* Clear the local file name so the file isn't deleted */
               GSsetLocalFile(tmpgs, NULL);

	       GSdestroy(tmpgs);

	       break;
	  }

	  case 'A': /*** Add current directory/search as a bookmark **/
	  {
	       GopherObj *tmpgs;
	       char newtitle[256];
		   char *tmps;

	       if (RCgetBookmarkDir(GlobalRC) == NULL) {
		    RCsetBookmarkDir(GlobalRC,GDnew(32));
		    GDsetTitle(RCgetBookmarkDir(GlobalRC), Gtxt("Bookmarks",67));
	       }

	       if (CurrentDir == RCgetBookmarkDir(GlobalRC)) {
		    CursesErrorMsg(Gtxt("Sorry, can't make a bookmark of bookmarks",151));
		    break;
	       }
	       
	       tmpgs = GSnew();
	       if (GDgetLocation(CurrentDir) != NULL)
		    GScpy(tmpgs, GDgetLocation(CurrentDir));
	       else if (iLevel == 0)
		    GScpy(tmpgs, RootGophers[0]);
	       else
		    GScpy(tmpgs, GDgetEntry(OldDirs[iLevel-1], 
					    GDgetCurrentItem(OldDirs[iLevel-1])-1));
	       
		   if ((GSgetType(tmpgs) == '7') &&
			   ((tmps = GSgetPath(tmpgs)) == NULL ||
				*tmps == '\0' ||
				Searchstring == NULL ||
				*Searchstring== '\0')) {
			   CursesErrorMsg("Sorry, can't make a bookmark of this search");
			   GSdestroy(tmpgs);
			   break;
           }
	       strcpy(newtitle, GDgetTitle(CurrentDir));
	       if (CURGetOneOption(CursesScreen, newtitle,
	       		Gtxt("Name for this bookmark:",109), newtitle)==0) {

		    if (*newtitle != '\0') {
			 GSsetTitle(tmpgs, newtitle);

			 /*** Freeze the search, if there was one. ***/
			 if (GSgetType(tmpgs) == '7') {
			      char ickypath[512];
			      strcpy(ickypath, GSgetPath(tmpgs));
			      strcat(ickypath, "\t");
			      strcat(ickypath, Searchstring);
			      GSsetPath(tmpgs, ickypath);
			      GSsetType(tmpgs, '1');
			 }
			 
			 GDaddGS(RCgetBookmarkDir(GlobalRC), tmpgs);
			 ChangedDefs = TRUE;
		    }
	       }
	       logrequest("ADD BOOKMARK", tmpgs);

	       GSdestroy(tmpgs);
	       break;
	  }

	  case 'd':  /*** Delete a bookmark ***/
	  {
	       GopherDirObj *tempgd;


#ifdef DELETE_BOOKMARKS_ONLY
              if (CurrentDir != RCgetBookmarkDir(GlobalRC)) {
                   CursesErrorMsg(Gtxt("The 'd'elete command is only for bookmarks.",189));
                  break;
             }
#endif
 	       if (CurrentDir != RCgetBookmarkDir(GlobalRC)) {
		    Download_file(GDgetEntry(CurrentDir,GDgetCurrentItem(CurrentDir)-1));
		    break;
	       }


	       logrequest("DELETE BOOKMARK", GDgetEntry(CurrentDir, GDgetCurrentItem(CurrentDir)-1));
	       
	       if (GDgetNumitems(CurrentDir) == 1)
		    tempgd = NULL;
	       else
		    tempgd=GDdeleteGS(CurrentDir,(GDgetCurrentItem(CurrentDir)-1));

	       if (CurrentDir == RCgetBookmarkDir(GlobalRC))
		    RCsetBookmarkDir(GlobalRC, tempgd);

	       if (tempgd == NULL) {
		    Debugmsg("Last item - pip up a directory")
		    tempgd = CurrentDir;
		    if (popgopher(&CurrentDir)==0 && tempgd != CurrentDir)
			 GDdestroy(tempgd);
		    else {
			 CursesErrorMsg(Gtxt("Sorry, can't delete top level directory.",149));
			 scline(-1, 1, CurrentDir);
		    }
		    ChangedDefs = TRUE;
	       } else {
		     CurrentDir = tempgd;
		     
		     ChangedDefs = TRUE;
	       }
	       break;
	  }

#if defined(VMS) && defined(A_LANGUAGE)
	  case 'L':  /** add language change	**/
	       if (CurrentDir == setlocale_LangDir)
	            break;
	       if (setlocale_LangDir== NULL) {
 		    CursesErrorMsg(Gtxt("Sorry, no language choice available",
					239));
	       }
	       else
	       {
		   /** Don't push an empty gopher directory... **/
		   if (CurrentDir != NULL)
			pushgopher(CurrentDir); 
		   
		   CurrentDir = setlocale_LangDir;
	       }
	       break;
#endif

	  case '$':
	  case '!':
#ifdef NEVERSPAWN
	       CursesErrorMsg(Gtxt("Sorry, can't spawn in with this account",153));
	       break;
#else
	       if (SecureMode || NoShellMode) {
		    CursesErrorMsg(Gtxt("Sorry, can't spawn in with this account",153));
		    break;
	       }
	       /*** Spawn to shell or DCL ***/
	       CURexit(CursesScreen);
#ifdef VMS
	       printf(Gtxt("Spawning DCL subprocess.  Logout to return to Gopher.\n",169));
	       fflush(stdout);
	       i = FIOsystem("");
#else
	       printf(Gtxt("Spawning your default shell.  Type 'exit' to return to Gopher.\n\n",170));
	       fflush(stdout);
	       i = FIOsystem(getenv("SHELL"));
#endif
	       CURenter(CursesScreen);
	       if (i < 0)
#ifdef VMS
		    CursesErrorMsg(Gtxt("Spawn to DCL failed!",167));
#else
		    CursesErrorMsg(Gtxt("Spawn to default shell failed!",168));
#endif
#endif	  /* NEVERSPAWN */
	       break;
	       

	       /*** Pop back to the main menu ***/
	  case 'M':
	  case 'm': 
	  {
	       GopherDirObj *tempGdir;

	       tempGdir = CurrentDir;
	       while (popgopher(&CurrentDir) != -1) {
		    if (tempGdir != NULL && tempGdir != RCgetBookmarkDir(GlobalRC))
			 GDdestroy(tempGdir);
		    tempGdir = CurrentDir;
	       }

	  }

	       break;

#ifndef AUTOEXITONU
	  case 'q':
	       /*** Quit the program ***/
	       if (TRUE == ReallyQuit()) {
		    bDone = TRUE;
		    CURexit(CursesScreen);
		    break;
	       }

	       break;
#endif /*AUTOEXITONU*/
#ifdef VMS
	  case '\032': /* ^Z */
#endif
	  case 'Q':
	       /*** Quit the program, don't ask ***/
	       bDone = TRUE;
	       CURexit(CursesScreen);
	       break;
	       
	  case 'O':
	       /*** Change various program things ***/
#ifdef NEVERSETOPTIONS
	       CursesErrorMsg(Gtxt("Sorry, you're not allowed to do this",164));
#else
	       SetOptions();
#endif
	       break;
		      
	  case '=':
	       DESCRIBE_GOPHER(Gtxt("Gopher Item Information",93), 
			       GDgetEntry(CurrentDir, GDgetCurrentItem(CurrentDir)-1));
	       break;

	  case '^':
	  {
	       if (GDgetLocation(CurrentDir) != NULL)
		    DESCRIBE_GOPHER(Gtxt("Gopher Directory Information",89),
				    GDgetLocation(CurrentDir));
	       else if (iLevel == 0)
		    DESCRIBE_GOPHER(Gtxt("Gopher Directory Information",89),
				    RootGophers[0]);
	       else
		    DESCRIBE_GOPHER(Gtxt("Gopher Directory Information",89),
				    GDgetEntry(OldDirs[iLevel-1],
					       GDgetCurrentItem(OldDirs[iLevel-1])-1));
	       break;
	  }
			       

	  case '?':
	  case KEY_HELP:
	  {
	       /*** Display help file ***/
	       GopherObj *tmpgs;
	       char *tmpurl = (char *)malloc(17+255);
	       char line[80];
	       
	       tmpgs = GSnew();
	       GSsetType(tmpgs, A_FILE);
	       sprintf(line, Gtxt("Internet Gopher Information Client v%s.%s.%d",102),
		       GOPHER_MAJOR_VERSION, GOPHER_MINOR_VERSION, PATCHLEVEL);

/*	       GSsetTitle(tmpgs, Gtxt("Gopher Help File",92));*/
	       GSsetTitle(tmpgs, line);
	       
#ifdef VMS
	       GSsetLocalFile(tmpgs, Gtxt(GOPHERHELP,223));
#else
	       GSsetLocalFile(tmpgs, Gtxt(GOPHERHELP,225));
#endif
#ifdef GOPHERHELP_SECURE
	       if (SecureMode)
#ifdef VMS
		  GSsetLocalFile(tmpgs, Gtxt(GOPHERHELP_SECURE,224));
#else
		  GSsetLocalFile(tmpgs, Gtxt(GOPHERHELP_SECURE,226));
#endif
#endif
	       GSsetLocalView(tmpgs, "text/plain");

	       if (tmpurl != NULL) {
		    /* set a URL for logrequest() in showfile() */
		    strcpy(tmpurl, "file://localhost");
#ifdef VMS
		    strcat(tmpurl, Gtxt(GOPHERHELP,223));
#else
		    strcat(tmpurl, Gtxt(GOPHERHELP,225));
#endif
#ifdef GOPHERHELP_SECURE
		    if (SecureMode)
#ifdef VMS
			strcat(tmpurl, Gtxt(GOPHERHELP_SECURE,224));
#else
			strcat(tmpurl, Gtxt(GOPHERHELP_SECURE,226));
#endif
#endif
		    GSsetURL(tmpgs, tmpurl);
		    free(tmpurl);
	       }
	       
	       showfile(tmpgs);

	       GSsetLocalFile(tmpgs, NULL);
	       GSdestroy(tmpgs);
	       break;
	  }

	  case 20:	/* Control-T */
	  {
	       /*** Show host's local date and time ***/	
	       GopherObj *tempgs = GSnew();
	       char *prompts[2];
	       char *responses[2];

	       prompts[0]=Gtxt("Host to query:",98);
	       prompts[1]=NULL;
	       
	       /*** Offer current gopher host as default ***/
	       responses[0] = (char *) malloc(sizeof(char)*COLS);
	       if (GDgetLocation(CurrentDir) != NULL)
		    strcpy(responses[0], GSgetHost(GDgetLocation(CurrentDir)));
	       else if (iLevel == 0)
		    strcpy(responses[0], GSgetHost(RootGophers[0]));
	       else
		    strcpy(responses[0],
		           GSgetHost(GDgetEntry(OldDirs[iLevel-1], 
			             GDgetCurrentItem(OldDirs[iLevel-1])-1)));
	       responses[1] = NULL;
	       
	       if ((CURRequest(CursesScreen,
			       Gtxt("Show host's local date and time",145),
			   prompts, responses) == -1) || *responses[0] == '\0')
		    break;

	       GSsetType(tempgs, A_FILE);
	       GSsetTitle(tempgs, Gtxt("Local Date and Time",104));
	       GSsetHost(tempgs, responses[0]);
	       GSsetPort(tempgs, 13);
	       GSsetPath(tempgs, NULL);
	       
	       showfile(tempgs);

	       GSdestroy(tempgs);
	       free(responses[0]);
	       break;
	  }

	  default:
	       Debug("main beep %d\r\n",TypedChar)
	       CURBeep(CursesScreen);
	       Garbled = FALSE;
	       break;

	  } /* switch */

#ifdef DESCRIBE_GOPHER
#undef DESCRIBE_GOPHER
#endif

     }      /* while */

     CleanupandExit(0);

     exit(0); /* Never get here... */
}


#if defined(VMS) && defined(A_LANGUAGE)
/*
 *  This procedure displays the available language menu and reports the
 *	user's language selection
 */

int
setlocale_screen()
{
    int status;

    if (!LangScreen)
	LangScreen = CURnew();
    CURenter(LangScreen);
    GetMenu(setlocale_LangDir, &status, 1);
    status = GDgetCurrentItem(setlocale_LangDir)-1;   
    CURexit(LangScreen);
    return(status);
}
#endif


int
Load_Index(GopherObj *ZeGopher)
{
     Draw_Status(Gtxt("Searching...",143));
     refresh();

     return(Load_Index_or_Dir(ZeGopher, Searchstring));
}

int
Load_Dir(GopherObj *ZeGopher)
{
     Searchstring= NULL;
     return(Load_Index_or_Dir(ZeGopher, NULL));
}


int
Load_Index_or_Dir(GopherObj *ZeGopher, char *Searchmungestr)
{
     int failed = 0;
     int sockfd;
     int i, numbytes;
     static char DirTitle[512];
     GopherDirObj *NewDir = NULL;

     DebugGSplusPrint(ZeGopher,"Load_Index_or_Dir");
     NewDir = GDnew(32);


     Draw_Status(Gtxt("Connecting...",77)); refresh();


     if ((sockfd = GSconnect(ZeGopher)) <0) {

	  check_sock(sockfd, GSgetHost(ZeGopher), GSgetPort(ZeGopher));
	  failed = 1;
     }
     else {
	  if (GSgetType(ZeGopher) == A_DIRECTORY) {
	       Draw_Status(Gtxt("Retrieving Directory...",133)); refresh();
	       GStransmit(ZeGopher, sockfd, NULL, "$", NULL);
	  }
	  else if (GSgetType(ZeGopher) == A_INDEX) {
	       Draw_Status(Gtxt("Searching...",143)); refresh();
	       GStransmit(ZeGopher, sockfd, Searchmungestr, "$", NULL);
	  }

	  numbytes = GSrecvHeader(ZeGopher, sockfd);

	  if (numbytes == 0) {

	       Gplus_Error(sockfd);
	       failed = 1;
	       return(failed);
	  }

	  if (GSisGplus(ZeGopher))
	       GDplusfromNet(NewDir, sockfd, twirl);
	  else
	       GDfromNet(NewDir, sockfd, twirl);

	  logrequest("ARRIVED AT", ZeGopher);


	  if (GDgetNumitems(NewDir) <= 0) {
	       CursesErrorMsg(Gtxt("Nothing available.", 214));
	       failed = 1;
	       GDdestroy(NewDir);
	       closenet(sockfd);
	       return(failed);
	  }

	  if (GSgetType(ZeGopher) == A_INDEX) {
	       sprintf(DirTitle, "%s: %s", GSgetTitle(ZeGopher), Searchmungestr);
	       GDsetTitle(NewDir, DirTitle);
	  }
          else
	       GDsetTitle(NewDir, GSgetTitle(ZeGopher));

	  GDsetLocation(NewDir, ZeGopher);

	  /** Don't push an empty gopher directory... **/
	  if (CurrentDir != NULL)
	       pushgopher(CurrentDir); 

	  CurrentDir = NewDir;
	  
     }
     i = closenet(sockfd);
     return(failed);
}

/*
 * This dispatches the appropriate fcns depending on what the object is.. 
 */

int
process_request(GopherObj *ZeGopher)
{
     int failed=0;
     char ImageCmd[128];
     char MovieCmd[128];

     if (GSisAsk(ZeGopher)) {
	  char **askdata = AskBlock(ZeGopher);
	  
	  if (askdata == NULL)
	       return(1);
	  GSsetAskdata(ZeGopher, askdata);
     }

     /** Decide what to do with it.. **/

     Debug("process_request type=%c\n",GSgetType(ZeGopher));
     logrequest("GOING TO", ZeGopher);

     switch(GSgetType(ZeGopher)) {
     case -1:
	  break;

#if defined(VMS) && defined(A_LANGUAGE)
     case A_LANGUAGE:
	  rsetlocale(GSgetPort(ZeGopher));
	  break;
#endif

     case A_INFO:
	  /** Do nothing **/
	  break;

     case A_FILE:
	  Draw_Status(Gtxt("Receiving Information...",128));
	  refresh();
	  showfile(ZeGopher);
	  break;

     case A_GIF:
     case A_IMAGE:
          if (!RCdisplayCommand(GlobalRC, "Image", "", ImageCmd) ||
	      !strncasecmp(ImageCmd, "- none -", 8) ||
	      ImageCmd == NULL || ImageCmd == '\0') {
	       CursesErrorMsg(Gtxt("Sorry, this machine doesn't support images",158));
	       break;
	  }
	  Draw_Status(Gtxt("Receiving Image...",132));
	  refresh();
	  if (RemoteUser)
		Download_file(ZeGopher);
	  else
		showfile(ZeGopher);
	  break;

     case A_MIME:
	  if (!SecureMode) {
	       Draw_Status(Gtxt("Receiving MIME File...",129));
	       refresh();
	       showfile(ZeGopher);
	  }
	  else
	       CursesErrorMsg(Gtxt("Sorry, cannot display this file",148));
	  break;

     case A_MACHEX:
     case A_PCBIN:
     case A_UNIXBIN:
	if (RemoteUser || SecureMode || NoShellMode) {
		Draw_Status(Gtxt("Receiving File...",126));
		refresh();
		Download_file(ZeGopher);
	}
	else
	       Save_file(ZeGopher, NULL, NULL);
	  break;
	  
     case A_DIRECTORY:
	  Draw_Status(Gtxt("Receiving Directory...",125));
	  refresh();
	  failed = Load_Dir(ZeGopher);
	  break;
		    
     case A_TELNET:
     case A_TN3270:
          {
	      char type, buf[512];

	      if ((type=GSgetType(ZeGopher)) == A_TELNET &&
	          (!RCdisplayCommand(GlobalRC, "Terminal/telnet", "", buf) ||
		   !strncasecmp(buf, "- none -", 8) ||
	 	   buf == NULL || buf[0] == '\0')) {
	          CursesErrorMsg(Gtxt("Sorry, telnet is not available.",157));
		  break;
	      }
	      else if (type == A_TN3270 &&
	          (!RCdisplayCommand(GlobalRC, "Terminal/tn3270", "", buf) ||
		   !strncasecmp(buf, "- none -", 8) ||
	 	   buf == NULL || buf[0] == '\0')) {
	          CursesErrorMsg(Gtxt("Sorry, tn3270 is not available.",160));
		  break;
	      }
	      else
	          do_tel_3270(ZeGopher);
	  }
	  break;

     case A_INDEX:
	  refresh();
	  if (search_string) { /* search_string in commandline */
	       Searchstring = search_string;
	       search_string = NULL;
	  } else
	       Searchstring = do_index(ZeGopher);

	  Draw_Status(Gtxt("Searching Text...",142));
	  if (Searchstring != NULL)
	       failed=Load_Index(ZeGopher);
	  else
	       failed = 1;
	  break;
	  
     case A_CSO:
	  do_cso(ZeGopher);
	  break;

     case A_SOUND:
	  Draw_Status(Gtxt("Receiving Sound...",131));
	  refresh();
	  do_sound(ZeGopher);
	  break;

      case A_MOVIE:
	  if (!RCdisplayCommand(GlobalRC, "video", "", MovieCmd) ||
	      !strncasecmp(MovieCmd, "- none -", 8) ||
	      MovieCmd == NULL || MovieCmd == '\0') {
		CursesErrorMsg(Gtxt("Sorry, this machine doesn't support movies",165));
		break;
	  }
	  Draw_Status(Gtxt("Receiving Movie...",130));
	  refresh();
	  if (RemoteUser)
	        Download_file(ZeGopher);
	  else
	        do_movie(ZeGopher);
	  break;

     case A_HTML:
	  Draw_Status(Gtxt("Receiving HTML page...",127));
	  refresh();
	  do_html(ZeGopher);
	  break;
     }
     if (GSisAsk(ZeGopher) && GSgetLocalFile(ZeGopher) != NULL) {
	  unlink(GSgetLocalFile(ZeGopher));
	  GSsetLocalFile(ZeGopher, NULL);
     }
     (void)GSsetAskdata(ZeGopher, NULL);
     return(failed);
}


/*
 * Provide information about a gopher item...
 * Should use curses...  But....
 */

void
#ifdef DESCRIBE_GOPHER_GRIPE
describe_gopher(char *banner, GopherStruct *ZeGopher, FILE *gripefile)
                  
#else
                                 
#endif
               
                         
{
     char *tmpfilename;
     char mkstemp_template[] = "/tmp/gopher-XXXXXX";
     int  mkstemp_fd = -1;
     FILE *tmpfile;
     Blockobj *bl;
     int i,j, views;
     GopherObj *infogs;

     infogs = GSnew();
     GSsetType(infogs, A_FILE);
     GSsetTitle(infogs, Gtxt("Link Info",103));

     GSgetginfo(ZeGopher, TRUE);

#ifdef DESCRIBE_GOPHER_GRIPE
     if (!gripefile)
	goto normal_describe;
     tmpfile = gripefile;
     GSdestroy(infogs);
     goto format_description;

normal_describe:
#endif
#ifdef VMS
     Gopenfile = tmpfilename = tempnam(NULL,NULL);
     tmpfile = fopen(tmpfilename, "w");
#else  /* ! VMS */
#ifdef HAVE_MKSTEMP
     mkstemp_fd = mkstemp(mkstemp_template);
     tmpfilename = strdup(mkstemp_template);
     Gopenfile = tmpfilename;
     tmpfile = fdopen(mkstemp_fd, "w");
#else /* ! HAVE_MKSTEMP */
     Gopenfile = tmpfilename = tempnam("/tmp", "gopher");
     tmpfile = fopen(tmpfilename, "w");
#endif /* HAVE_MKSTEMP */
#endif /* VMS */
     GSsetLocalFile(infogs, tmpfilename);
     GSsetLocalView(infogs, "text/plain");
     if (tmpfile == NULL) {
	  CURexit(CursesScreen);
	  fprintf(stderr, Gtxt("Couldn't make a tmp file!\n",83)), 
	  CleanupandExit(-1);
     }
     free(tmpfilename);

#ifdef DESCRIBE_GOPHER_GRIPE
format_description:
#endif

     GStoLink(ZeGopher, fileno(tmpfile), TRUE);

     fprintf(tmpfile, "<URL:%s>\n\n", GSgetURL(ZeGopher,""));
     fflush(tmpfile);

     if (GSisGplus(ZeGopher)) {
	  GopherObj *server;

	  server = GSnew();
	  GScpy(server, ZeGopher);
	  GSsetPath(server, "");
	  GSsetLocalFile(server, "");

	  fputc('\n', tmpfile);
	  /** Search out for specific blocks **/
	  for (i=0; i<GSgetNumBlocks(ZeGopher); i++) {
	       if (strcmp(BLgetName(GSgetBlock(ZeGopher, i)),"ABSTRACT")==0){
		    bl = GSgetBlock(ZeGopher, i);
		    fprintf(tmpfile, "%s\n---------\n\n",BLgetName(bl));
		    for (j=0; j < BLgetNumLines(bl); j++) {
			 fprintf(tmpfile, "%s\n", BLgetLine(bl, j));
		    }
		    fprintf(tmpfile, "\n");
		    break;
	       }
	  }

	  fprintf(tmpfile, Gtxt("Size       Language      Document Type\n",146));
	  fprintf(tmpfile, Gtxt("---------- ------------- ----------------------------\n",147));
	  
	  for (views=0; views< GSgetNumViews(ZeGopher); views++) {
	       char *cp; 
	       VIewobj *ZeView = GSgetView(ZeGopher, views);

	       if (ZeView == NULL)
		    continue;

	       cp = VIgetSize(ZeView);
	       if (cp != NULL)
		    fprintf(tmpfile, "%s", cp);
	       else
		    cp = "";

	       for (i=strlen(cp); i<11; i++)
		    fputc(' ', tmpfile);

	       
	       cp = VIprettyLang(ZeView, "En_US"); 
	       if (cp != NULL)
		    fprintf(tmpfile, "%s", cp);
	       else
		    cp = VIgetLang(ZeView);
	       for (i=(cp?strlen(cp):0); i<14; i++)
		    fputc(' ', tmpfile);

	       cp = VIgetType(ZeView);
	       fprintf(tmpfile, "%s\n", cp);
	  }


	  for (i=0; i<GSgetNumBlocks(ZeGopher); i++) {
	       bl = GSgetBlock(ZeGopher, i);
	       if (strcmp(BLgetName(bl),"ADMIN")==0) {
		    fprintf(tmpfile, Gtxt("\n\nServer Information\n",186));
		    fprintf(tmpfile, "------------------\n");
		    for (j=0; j < BLgetNumLines(bl); j++) {
			 char *cp = BLgetLine(bl, j);

			 if ( (strncmp(cp, "Admin:", 5) != 0) &&
			      (strncmp(cp, "Mod-Date:", 9) != 0) &&
			      (strncmp(cp, "TTL:", 4) != 0) ) {
			      fprintf(tmpfile, "%s\n", cp);
			 }
		    }
		    fprintf(tmpfile, "\n");
		    break;
	       }
	  }
	  GSdestroy(server);
     }

#ifdef DESCRIBE_GOPHER_GRIPE
     if (gripefile)
	return;
#endif
     fclose(tmpfile);

     showfile(infogs);

     CURenter(CursesScreen); /* do this after unlink fails */

     GSdestroy(infogs);
     return;
}
