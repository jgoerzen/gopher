/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.6 $
 * $Date: 2002/01/10 17:41:40 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/ourutils.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992,1993 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: ourutils.c
 * stuff that doesn't really fit anywhere else.
 *********************************************************************
 * Revision History:
 * $Log: ourutils.c,v $
 * Revision 1.6  2002/01/10 17:41:40  jgoerzen
 * Made the text file handling better.
 *
 * Revision 1.5  2002/01/10 17:23:11  jgoerzen
 *   * gopher: Removed processing of . at end of text files.  It would stop
 *     downloading when it would see ".\n" in a file.  Will need to modify
 *     gopher/ourutils.c:937 area to deal with this properly -- ignore
 *     .\n if it's at the end of the file, otherwise preserve it.
 *     Reported by Ashok Aiyar <aiyar@ebv.mimnet.nwu.edu>
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
 * Revision 1.3  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.2  2001/01/03 22:31:08  s2mdalle
 * Inclusion of new header file.
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.77  1995/11/03  21:18:21  lindner
 * ANSIfication
 *
 * Revision 3.76  1995/11/03  20:50:51  lindner
 * Coen: fixes..
 *
 * Revision 3.75  1995/04/15  07:10:13  lindner
 * New control C methods, CTRL-X mods
 *
 * Revision 3.74  1995/03/14  16:08:41  lindner
 * TPU fix
 *
 * Revision 3.73  1995/03/09  05:27:23  lindner
 * F.Macrides 08-Mar-1995: Use strncasecmp() for getting the AskLs value
 * in AskBlock().
 *
 * F.Macrides 02-Mar-1995: Allow %s to be at the end of 'builtin' when
 * defining a viewer.
 *
 * Revision 3.72  1995/01/25  23:10:07  lindner
 * Remove hard limit of 5 AskL items per form
 *
 * Revision 3.71  1994/12/03  01:55:43  lindner
 * JL Wilkinson 16-Jul-1994 Merged in TGV's syslog() functions to allow
 * the CLIENT_LOGGER and TELNET_TRACE functionality to work with
 * Multinet.  Not tested against other VMS TCP/IP agents, and *NOT
 * SUPPORTED* thru TGV -- please don't anybody ask TGV for help in using
 * this.  Somebody who wants to use it can Email me or the
 * VMSGopher-L@trln.lib.unc.edu group and I'll try to assist them for
 * MULTINET only (JLW@psulias.psu.edu).  Basic documentation is in
 *
 * Added Lance's post-pager RETURN synonyms.
 *
 * Indicate 'D'ownload as an option on exit from the pager in SecureMode
 * and NoShellMode.
 *
 * Revision 3.70  1994/10/24  22:14:38  lindner
 * Add pdf type
 *
 * Revision 3.69  1994/10/13  05:30:38  lindner
 * Compiler complaint fixes
 *
 * Revision 3.68  1994/09/29  19:25:57  lindner
 * More restrictive Unix file name filter
 *
 * Revision 3.67  1994/08/19  16:53:29  lindner
 * Add casts for malloc calls
 *
 * Revision 3.66  1994/07/25  02:55:17  lindner
 * Secure mode mods
 *
 * Revision 3.65  1994/07/19  20:18:36  lindner
 * Remove CLIENT_LOGGER, Add default language choice
 *
 * Revision 3.64  1994/07/03  23:11:26  lindner
 * Add internal download feature
 *
 * Revision 3.63  1994/06/29  07:04:41  lindner
 * Mods for A_APP and builtin upload
 *
 * Revision 3.62  1994/06/09  04:11:07  lindner
 * Added Gerd.Boehm@physik.uni-regensburg.de patch for off-by-one line
 * counts in AskL blocks.
 *
 * Revision 3.61  1994/06/03  06:11:46  lindner
 * Fix for big bad cso dashes
 *
 * Revision 3.60  1994/05/19  14:08:04  lindner
 * use fast malloc on VMS VAXC
 *
 * Revision 3.59  1994/05/17  05:48:03  lindner
 * Massive internationalization change
 *
 * Revision 3.58  1994/05/14  04:13:46  lindner
 * Internationalization...
 *
 * Revision 3.57  1994/04/29  15:13:17  lindner
 * Fix for items with multiple AskL items
 *
 * Revision 3.56  1994/04/25  18:40:05  lindner
 * Fix for uninited variable i
 *
 * Revision 3.55  1994/04/25  03:37:43  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.54  1994/04/13  19:14:03  lindner
 * AskL modifications
 *
 * Revision 3.53  1994/03/08  20:38:28  lindner
 * Fix for VMS compilation
 *
 * Revision 3.52  1994/03/08  15:55:16  lindner
 * gcc -Wall fixes
 *
 * Revision 3.51  1994/03/08  03:25:58  lindner
 * Fix for big bad telnet bug, additions for secure process i/o
 *
 * Revision 3.50  1994/03/04  23:31:32  lindner
 * Fix for askblock from beckett
 *
 * Revision 3.49  1994/02/20  16:32:58  lindner
 * Modify routines to use fileio routines for reading text files
 *
 * Revision 3.48  1993/12/28  17:33:00  lindner
 * filter out more VMS characters
 *
 * Revision 3.47  1993/11/29  01:12:26  lindner
 * In Save_file(), disallow saving of <CSO> type since this is fetched as a
 * set of indexed fields instead of as a savable file.  Also disallow
 * saving of info and error types since there's nothing to save.  (Wolfe,
 * Beckett, Macrides)
 *
 * In Choose_View(), add case for A_MOVIE.  (Macrides)
 *
 * Revision 3.46  1993/11/03  03:50:01  lindner
 * More fixes for VMSrecords
 *
 * Revision 3.45  1993/11/02  21:17:44  lindner
 * Better client side logging code
 *
 * Revision 3.44  1993/10/27  18:52:17  lindner
 * Simplify VMS fixed/variable file code
 *
 * Revision 3.43  1993/10/26  18:22:43  lindner
 * Mucho changes for VMS variable length records
 *
 * Revision 3.42  1993/10/22  20:33:17  lindner
 * Fixed bad code in Save_file() for choosing view (Fote)
 *
 * Revision 3.41  1993/10/22  20:06:46  lindner
 * Add optional client logging
 *
 * Revision 3.40  1993/10/11  17:20:43  lindner
 * Fix for data/time stripping, vms multidot files, no case comparision for builtin
 *
 * Revision 3.39  1993/10/07  05:08:22  lindner
 * In Choose_View(), remove unreachable statement.
 *
 * In AskBlock(), allocate memory for the form based on screen width.
 *
 * In Save_file(), choose a view BEFORE opening the file -- user may
 * change mind and cancel, which left a zero-length file previously.
 *
 * In Save_file(), check to see if a view has already been chosen.  This
 * change requires that Save_file() have a third argument.  Any call to
 * Save_file() will now need a third argument.  The third argument should
 * be a pointer to the chosen view or NULL to indicate that one has yet to
 * be chosen.
 *
 * In GSdisplay(), add "view" as the third argument to the two "new"
 * Save_file() calls to prevent Save_file() from asking the user to choose
 * a view.  It is redundant in both cases.
 *
 * Revision 3.38  1993/09/29  22:47:21  lindner
 * Fix for AskBlock memory cruft
 *
 * Revision 3.37  1993/09/29  21:30:29  lindner
 * Fix for memory probs with ask blocks
 *
 * Revision 3.36  1993/09/29  20:49:42  lindner
 * Remove dead code, fix for vms mail
 *
 * Revision 3.35  1993/09/26  09:20:54  lindner
 * Change to use GSmail()
 *
 * Revision 3.34  1993/09/22  19:57:52  lindner
 * fix for off by one and Select:
 *
 * Revision 3.33  1993/09/21  01:46:02  lindner
 * Implement all remaining ASK block items..
 *
 * Revision 3.32  1993/09/18  04:44:23  lindner
 * Additions to fix caching of Multiple view items
 *
 * Revision 3.31  1993/09/18  03:29:02  lindner
 * Mucho fixes for ask block parsing..
 *
 * Revision 3.30  1993/09/11  05:12:41  lindner
 * Many ask block fixes
 *
 * Revision 3.29  1993/09/08  05:23:19  lindner
 * Implement Builtin pager..
 *
 * Revision 3.28  1993/09/08  01:12:33  lindner
 * Add support for HTML and MIME on the menu displays
 *
 * Revision 3.27  1993/09/01  21:47:37  lindner
 * Fix for compiler error on Alpha
 *
 * Revision 3.26  1993/08/19  20:51:52  lindner
 * secure patch from mitra
 *
 * Revision 3.25  1993/08/19  20:30:39  lindner
 * Fix problem with selecting ask item more than once
 *
 * Revision 3.24  1993/08/19  20:22:56  lindner
 * Mitra's Debug patch
 *
 * Revision 3.23  1993/08/16  18:01:10  lindner
 * Fix for VMSfile, mods for DEC alpha VMS
 *
 * Revision 3.22  1993/08/16  17:58:24  lindner
 * Removed REMOTEUSER ifdefs
 *
 * Revision 3.21  1993/08/06  14:39:57  lindner
 * One more small security patch
 *
 * Revision 3.20  1993/08/05  20:40:43  lindner
 * Added warning message..
 *
 * Revision 3.19  1993/08/04  22:07:23  lindner
 * Use /bin/mail instead of ucbmail
 *
 * Revision 3.18  1993/08/03  04:43:59  lindner
 * Fix for VMS unresolved variables
 *
 * Revision 3.17  1993/07/30  17:31:56  lindner
 * Mods to support AskP:, AskL:
 *
 * Revision 3.16  1993/07/29  17:20:19  lindner
 * eliminate non-used variables
 *
 * Revision 3.15  1993/07/27  06:14:17  lindner
 * Bug fix, use readrecvbuf, not readn
 *
 * Revision 3.14  1993/07/27  05:28:57  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.13  1993/07/27  02:04:13  lindner
 * More comments
 *
 * Revision 3.12  1993/07/26  20:30:05  lindner
 * fix memory usage
 *
 * Revision 3.11  1993/07/26  15:35:28  lindner
 * Fix for Unixfile to remove parens
 *
 * Revision 3.10  1993/07/23  04:38:43  lindner
 * Mods to make client view selection pretty
 *
 * Revision 3.9  1993/07/20  23:14:59  lindner
 * Mods to cache askdata..
 *
 * Revision 3.8  1993/07/07  19:43:42  lindner
 * Added TPU support for pager in VMS
 *
 * Revision 3.7  1993/06/29  06:18:52  lindner
 * recasting MIME types
 *
 * Revision 3.6  1993/04/15  21:27:26  lindner
 * NOMAIL option, Remote user stuff, and bug in Ask
 *
 * Revision 3.5  1993/03/26  19:44:36  lindner
 * Fix for CSO stuff and saving binary files
 *
 * Revision 3.4  1993/03/24  17:01:27  lindner
 * Major changes to the way things are displayed/saved
 *
 * Revision 3.3  1993/03/18  23:32:41  lindner
 * Added Note: support and default values to AskBlock
 *
 * Revision 3.2  1993/02/19  21:12:16  lindner
 * Generalized on gopher+ stuff, strip long filenames to reasonable length
 *
 * Revision 3.1.1.1  1993/02/11  18:02:58  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1993/02/09  22:37:50  lindner
 * Much new in the way of display routines.
 *
 *********************************************************************/

#include "gopher.h"
#include "Stdlib.h"
#include "util.h"
#ifndef VMS
#include <pwd.h>
#else
#include <descrip.h>
#endif
#ifdef mips
char *getenv();
#endif
#include "Debug.h"
#include "fileio.h"
#include "Malloc.h"
#include "Locale.h"
#include "String.h"
#include "ourutils.h"
#include <curses.h>
#ifdef HAVE_TERM_H
#include <term.h>
#endif

void GSprompter(GopherObj *gs, char *view, char *command);

int 
outchar(int c)
{
        /** output the given character.  From tputs... **/
        /** Note: this CANNOT be a macro!              **/

        putc(c, stdout);
	return(c);
}


/*
 * Function that twirls on the screen..
 *
 */

int
twirl(void)
{
     static int twirlnum = 0;
     static char *twirls = "-/|\\";

     addch('\b');
     addch(*(twirls + (twirlnum++ % 4 )));
     refresh();

     return(0);
}



/*
** This procedure sends a file through the mail.
*/

void
GSmail(GopherObj *gs)
{
     char        *Filename = GSgetLocalFile(gs);
     char        *Realname = GSgetTitle(gs);

     static char *SaveName = NULL;
     char         command[512];
     FileIO      *infile, *mailit;
     char         buf[512];

#ifdef VMS
     char         MailAddress[512];
#endif

     
     if (SaveName==NULL) {
	  if ((SaveName = (char *) malloc(sizeof(char)*256)) == NULL)
	       perror("Out of memory!");
	  *SaveName = '\0';
     }

     if (CURGetOneOption(CursesScreen, Realname, 
			 Gtxt("Mail current document to:",105),
     			 SaveName)<0)
	  return;
     
     if (*SaveName == '\0')
	  return;

/*
 * You should be very careful when you add characters to this list..
 * 
 * It could create a nasty security hole!
 */
     
#define ACHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789%.@!-_"
		    
     if (SecureMode || NoShellMode)
	  if ( strspn(SaveName, ACHARS) != strlen(SaveName)||
	      *SaveName == '-') {
	       Debugmsg("mail_file - beep - bad char")
	       puts(CURgetBell(CursesScreen));
	       fflush(stdout);
	       Draw_Status(Gtxt("Address syntax rejected...",65));
	       refresh();
	       return;
	  }

     if (strchr(Realname, '\"') != NULL)
	  Realname = Gtxt("Gopher File",91);
     

     Draw_Status(Gtxt("Mailing File...",106));
     refresh();
     
#ifdef VMS
     /* encase any address with '@' in double-quotes and add transport */
     /* if the user hasn't done it (based on code from Earl Fogel)     */
     if ((strchr(SaveName, '@') != NULL) && (strchr(SaveName, '\"') == NULL))
	   sprintf(MailAddress, MAIL_ADRS, SaveName);
     else
	   strcpy(MailAddress, SaveName);

     sprintf(command, "%s/subject=\"%.70s\" %s %s",
           MAIL_COMMAND, Realname, Filename, MailAddress);

     CURexit(CursesScreen);
     printf(Gtxt("Mailing file to %s...",107), MailAddress);
     FIOsystem(command);
     CURenter(CursesScreen);
#else
     sprintf(command, "%s %s", 
	     MAIL_COMMAND, SaveName);
     
     infile = FIOopenUFS(Filename, O_RDONLY, 0);
     mailit = FIOopenCmdline(command, "w");
     
     if (mailit == NULL || infile == NULL) {
	  CursesErrorMsg(Gtxt("System problem, unable to mail",173));
	  return;
     }

     if (strlen(Realname) > 70)
	  Realname[70]  = '\0';

     FIOwritestring(mailit, "Subject: ");
     FIOwritestring(mailit, Realname);
     FIOwritestring(mailit, "\n\n");

     while (FIOreadline(infile, buf, sizeof(buf))) {
	  twirl();
	  FIOwritestring(mailit, buf);
     }

     FIOclose(mailit);
     FIOclose(infile);

#endif

}

/*
 *  This fcn allows one to choose a view out of a unlimited size list
 *  returns false if fails
 */

#ifndef DEFAULT_LANG	/* should be set in conf.h, but just in case */
#define DEFAULT_LANG    "En_US"     /* English (US) */
#endif


char *
Choose_View(GopherObj *gs)
{
     int viewno;
     static char viewnlang[64];
     char *view;

     if (GSisGplus(gs)) {

	  GSgetginfo(gs, TRUE);
	  
	  if (GSgetNumViews(gs) >1) {
	       int choice, default_choice = -1;
	       int langChoice = -1, viewChoice = -1;
	       char *default_lang =  Gtxt(DEFAULT_LANG, 222);
	       char **views = (char **) malloc((1 + GSgetNumViews(gs)) * sizeof(char *));
	       char **choices = (char **) malloc((1 + GSgetNumViews(gs)) * sizeof(char *));

	       if ( (views == NULL) || (choices == NULL) )
		    return NULL;

	       for (viewno=0; viewno <GSgetNumViews(gs); viewno++) {
		    VIewobj	 *thisView = GSgetView(gs, viewno);

		    views[viewno] = strdup(VIgetViewnLang(thisView, viewnlang));
		    choices[viewno] = strdup(VIgetPrettyView(thisView,viewnlang));

		    if (default_choice < 0) {
			 int langMatch = !(strcasecmp(VIgetLang(thisView), default_lang));
			 int viewMatch = !(strcasecmp(VIgetType(thisView), "text/plain"));

			 if (langMatch && viewMatch)
			      default_choice = viewno;
			 else {
			      if (langMatch && (langChoice < 1))
				   langChoice = viewno;
			      if (viewMatch && (viewChoice < 1))
				   viewChoice = viewno;
			 }
		    }
	       }

	       if (default_choice < 0) {
		    if (viewChoice >= 0) 
			 default_choice = viewChoice;
		    else if (langChoice >= 0)
			 default_choice = langChoice;
		    else
			 default_choice = 0;
	       }

	       choices[viewno] = NULL;
	       choice = CURChoice(CursesScreen, GSgetTitle(gs), choices,
				  Gtxt("Choose a document type",73), 
				  default_choice);
	       Debug("Choice was %d\n", choice);
	       if (choice == -1)
		    return(NULL);

	       sprintf(viewnlang, "%s", views[choice]);

	       for (viewno = 0; viewno <GSgetNumViews(gs); viewno++)  {
		    free(views[viewno]);

		    free(choices[viewno]);

	       }
	       free(views);
	       free(choices);
	       return(viewnlang);
	  }
	  else if (GSgetNumViews(gs) == 1)
	       return(VIgetViewnLang(GSgetView(gs, 0), viewnlang));
	  else
	       return(NULL);
     }
     else {
	  switch (GSgetType(gs)) {
	  case A_FILE:
	  case A_CSO:
	       view = "text/plain";
	       break;
	  case A_MIME:
	       view = "message/rfc822";
	       break;
	  case A_MACHEX:
	       view = "application/mac-binhex40";
	       break;
	  case A_HTML:
	       view = "text/html";
	       break;
	  case A_GIF:
	       view = "image/gif";
	       break;
	  case A_IMAGE:
	       view = "image";
	       break;
	  case A_MOVIE:
	       view = "video";
	       break;
	  case A_SOUND:
	       view = "audio/basic";
	       break;
	  case A_TN3270:
	       view = "terminal/tn3270";
	       break;
	  case A_TELNET:
	       view = "terminal/telnet";
	       break;
	  case A_UNIXBIN:
	  case A_PCBIN:
	       view = "application/octet-stream";
	       break;
	  case A_PDF:
	       view = "application/pdf";
	       break;
	  default:
	       return(NULL);
	  }

	  return(view);
     }

}

Requestitem *
REQitemnew(void)
{
     Requestitem *temp;

     temp = (Requestitem*) (malloc(sizeof(Requestitem)));
     if (temp == NULL)
	  return(NULL);

     temp->stowage = (char *) malloc(sizeof(char) * COLS);
     temp->stowage[0] = '\0';
     temp->choices = NULL;
     temp->prompt = NULL;
     
     return(temp);
}

void
REQitemdestroy(Requestitem *req)
{
     int j;

     if (req->stowage != (char*) NULL)
	  free(req->stowage);
     
     if (req->prompt != (char*) NULL) 
	  free(req->prompt);
     
     if (req->choices != (char**) NULL) {
	  for (j=0; req->choices[j] != (char*) NULL; j++)
	       free(req->choices[j]);
	  free(req->choices);
     }
     free(req);

}

/*
 * Crude implementation of the ASK block stuff.
 */

char **
AskBlock(GopherObj *gs)
{
     int Asknum, reqnum, AskLs = 0;
     Blockobj *bl;
     Requestitem **items;
     char **asktypes;
     char **responses = NULL;
     char askline[1024];
     char *defaultval;
     int i;

     GSgetginfo(gs, TRUE);

     bl = GSfindBlock(gs, "ASK");

     if (bl == NULL)
	  return(NULL);

     /** Find out how many ASKL items we have... **/
     for (Asknum=0; Asknum < BLgetNumLines(bl); Asknum++) {
	  if (strncasecmp(BLgetLine(bl, Asknum), "AskL:", 5) == 0)
	       AskLs++;
     }

     /** Can't guess how many askl things we'll have.. **/
     items = (Requestitem **) malloc(sizeof(Requestitem*)* 
				     (BLgetNumLines(bl) + (AskLs * 10) + 1));

     asktypes = (char **) malloc(sizeof(char*) * 
				 (BLgetNumLines(bl) + (AskLs * 10) + 1));

     reqnum = 0;

     for (Asknum=0; Asknum <BLgetNumLines(bl); Asknum++, reqnum++) {
	  char *askprompt, *cp;

	  strncpy(askline, BLgetLine(bl, Asknum), sizeof(askline));
	  askline[sizeof(askline)-1] = '\0';
	  items[reqnum] = REQitemnew();

	  /*** find the type of question ***/
	  askprompt = strchr(askline, ':');
	  if (askprompt == NULL) {
	       /* Empty line crashes CURRequester unless we do this*/
	       items[reqnum]->prompt = strdup("");
	       items[reqnum]->thing = CUR_LABEL;
	       asktypes[reqnum] = strdup("Note:");
	       continue;
	  }
	  *(askprompt+1) = '\0';
	  askprompt+=2;
	  
	  /*** Zap the tabs, and load it up.. ***/
	  cp = strchr(askprompt, '\t');
	  if (cp != NULL) {
	       defaultval = cp+1;
	       *cp = '\0';
	  } else
	       defaultval = NULL;

	  asktypes[reqnum] = strdup(askline);
	  items[reqnum]->prompt = strdup(askprompt);
	  
	  if (strncasecmp(askline, "Note:", 5) == 0)
	       items[reqnum]->thing = CUR_LABEL;
	  else if (strncasecmp(askline, "Choose:", 7) == 0) {
	       int cnum = 0;

	       items[reqnum]->thing = CUR_CHOICE;
	       items[reqnum]->chooseitem = 0;
	       
	       /*** CURChoice() has a limit of 99 choices ***/
	       items[reqnum]->choices = (char**) malloc(sizeof(char*)*100);

	       /*** add list of choices to struct **/
	       while ((cp = strchr(defaultval, '\t')) != NULL) {
		    *cp = '\0';
		    items[reqnum]->choices[cnum++] = strdup(defaultval);
		    defaultval = cp+1;
	       }
	       if (defaultval != NULL)
		    items[reqnum]->choices[cnum++] = strdup(defaultval);

	       items[reqnum]->choices[cnum] = NULL;

	  }
	  else if (strncasecmp(askline, "Select:", 7) == 0) {
	       items[reqnum]->thing = CUR_CHOICE;
	       items[reqnum]->chooseitem = 0;
	       items[reqnum]->choices = (char**) malloc(sizeof(char*)*3);

	       cp = strrchr(items[reqnum]->prompt, ':');
	       if (cp != NULL) {
		    *cp = '\0';
		    cp++;
		    if (*cp == '1')
			 items[reqnum]->chooseitem = 1;
	       }
	       
	       items[reqnum]->choices[0] = strdup(Gtxt("No",111));
	       items[reqnum]->choices[1] = strdup(Gtxt("Yes",180));
	       items[reqnum]->choices[2] = NULL;
	  }
	  else if (strncasecmp(askline, "AskP:", 5)==0)
	       items[reqnum]->thing = CUR_PASSWD;
	  else if (strncasecmp(askline, "Choosef:", 8) == 0)
	       items[reqnum]->thing = CUR_FNAME;
	  else if (strncasecmp(askline, "AskL:", 5)==0){
	       int x;

	       items[reqnum]->thing = CUR_LABEL;
	       
	       /** Add a bunch more... **/

	       for (x=0; x < 10; x++) {
		    reqnum++;
		    items[reqnum] = REQitemnew();
		    items[reqnum]->prompt = strdup("");
		    items[reqnum]->thing = CUR_ASKL;
		    asktypes[reqnum] = NULL;
	       }

	  } 
	  else
	       items[reqnum]->thing = CUR_PROMPT;

	  if (defaultval != NULL)
	       strncat(items[reqnum]->stowage, defaultval, COLS-1);
     }
     
     items[reqnum] = NULL;
     
     if (!CURrequester(CursesScreen, GSgetTitle(gs), items)) {
	  int respnum = 0;

	  responses = (char**) malloc(sizeof(char*)*(10+reqnum));

	  for (i=0; i <reqnum; i++) {
	       if (asktypes[i] && strcasecmp(asktypes[i], "AskL:")==0) {
		    int alnum, extra;
		    char numlinestr[16];

		    /** Okay, figure out how many lines were typed **/
		    for (alnum=9; alnum >= 0; alnum--) {
			 if (*(items[alnum+i]->stowage) != '\0')
			      break;
		    }
		    alnum++;
		    sprintf(numlinestr, "%d", alnum);

		    responses[respnum++] = strdup(numlinestr);

		    extra = 10 - alnum;
		    while (alnum) {
			 responses[respnum++] = strdup(items[++i]->stowage);
			 alnum--;
		    }
		    i += extra;

	       } else if (asktypes[i] && strcasecmp(asktypes[i], "Select:")==0) {
		    if (items[i]->chooseitem == 0)
			 responses[respnum++] = strdup("0");
		    else
			 responses[respnum++] = strdup("1");

	       } else if (items[i]->thing == CUR_CHOICE) {
		    responses[respnum++] = strdup(items[i]->choices[items[i]->chooseitem]);
	       }
	       else if (items[i]->thing == CUR_PROMPT ||
			  items[i]->thing == CUR_PASSWD)
		    responses[respnum++] = strdup(items[i]->stowage);
	  }
	  responses[respnum++] = NULL;
     }
     /*** Free memory ***/
     for (i=0; i<Asknum; i++) {

	  REQitemdestroy(items[i]);
	  items[i] = NULL;
	  if (asktypes[i]!=NULL)
	       free(asktypes[i]);
     }

     free(items);
     free(asktypes);

     return(responses);
}

/*
 * Handle a gplus error
 */
#define GERRMSGSIZE 25

void
Gplus_Error(int sockfd)
{
     char inputline[256];
     char *errmsg[GERRMSGSIZE];
     int i;
     
     Debug("Gplus_Error on %d...", sockfd);
     for (i=0; i < GERRMSGSIZE;i++) {
	  char *cp;

	  if (readline(sockfd, inputline, sizeof(inputline)) <=0)
	       break;
	  ZapCRLF(inputline);
	  if (strcmp(inputline, ".")==0)
	       break;
	  
	  cp = inputline;
	  while ((cp = strchr(cp, '\t')) != NULL)
	       *cp = ' ';

	  errmsg[i] = strdup(inputline);
     }
     
     errmsg[i] = NULL;
     
     CURDialog(CursesScreen, Gtxt("Gopher Transmission Error",95), errmsg);
     for (i=0; ; i++) {
	  if (errmsg[i] == NULL)
	       break;
	  else
	       free(errmsg[i]);
     }

}



/*
 * This fcn transfers a file from a gopher server into f.
 *
 */

boolean
GStoFile(GopherObj *gs, FILE *f, char *view, int (*twirlfn) (/* ??? */))
{
     int numread, sockfd;
     char buf[1024], newbuf[1024];
     int bytemethod;
     int line = 0, i;

     /*** Check for gopher+ and multiple views ***/

     Debug("GStoFile, view %s",view);
     if ((sockfd = GSconnect(gs)) <0) {
	  check_sock(sockfd, GSgetHost(gs), GSgetPort(gs));
	  return(FALSE);
     }

     /** Send out the request **/
     GStransmit(gs, sockfd, NULL, "+", view);
     bytemethod = GSrecvHeader(gs, sockfd);

     if (bytemethod == 0) {
	  Gplus_Error(sockfd);
	  
	  return(FALSE);
     }
	  

     Draw_Status(Gtxt("Receiving File...",126));
     refresh();
     

     if (GSisText(gs, view)) {
	  char cso_click = '\0';

	  while (readline(sockfd, newbuf, sizeof(newbuf)) > 0 && 
		 !ControlCpressed) {
	      ZapCRLF(newbuf);
	       if (line && !(GSgetType(gs) == A_CSO)) {
  		   /* Print out the line from last time through the loop */
		   fputs(buf, f);
		   /* fprintf(f, "%d %s", line, buf); */
		   /** Don't cut long lines... **/
		   if (strlen(buf) < sizeof(buf))  
		       putc('\n', f);
	       }
	       line++;
	       
	       strcpy(buf, newbuf);
	       if (GSgetType(gs) == A_CSO) {
		    if (*buf == '2')
			 break;
		    
		    if ((*buf >= '3') && (*buf <= '9'))  {
			 fprintf(f, "%s\n", GSgetPath(gs));
			 fprintf(f, "%s\n", buf+4);
			 break;
		    }
		    if (*buf == '-') {


			 if (buf[1] >= '3' && buf[1] <= '9') {
			      fprintf(f, "%s\n", GSgetPath(gs));
			      fprintf(f, "%s\n", buf+5);
			 }
			 else {
			      char *colonpos = strchr(buf+5,':');
			      i = '\0';
			      if (colonpos != NULL && *(colonpos-1) != cso_click) {
				   fprintf(f, "-------------------------------------------------------\n");
				   cso_click = *(colonpos-1);
			      }
			      fputs((colonpos ? colonpos+1 : buf+6), f);
			      fputc('\n', f);
			 }
		    }
	       } /* CSO */
	       if ((line % 25) == 0)
		    twirlfn();
	  } /*** readline ***/
	  if (ControlCpressed) {
	       CursesErrorMsg("Interrupted by User");
	       ControlCpressed = FALSE;
	  }
	  if (line && !(GSgetType(gs) == A_CSO) && 
	      !(*buf == '.' && *(buf+1) == '\0')) {
	      /* Print out the line from last time through the loop */
	      fputs(buf, f);
	      /* fprintf(f, "EOF %d: '%s'", line, buf); */
	      /** Don't cut long lines... **/
	      if (strlen(buf) < sizeof(buf))  
		  putc('\n', f);
	  }
	      
     }
     else {
	       
#if defined(VMS) && defined(VMSRecords)
          while (((numread = readrecvbuf(sockfd, buf, 512)) > 0) &&
		 !ControlCpressed) {
	       if (numread < 512)
		   for (i = numread; i < 512; i++)  buf[i] = 0;
               if (fwrite(buf, 512, 1, f) == 0) {
#else
	  while (((numread = readrecvbuf(sockfd, buf, sizeof buf)) > 0) &&
		 !ControlCpressed)
	  {
	       if (fwrite(buf, numread, 1, f) == 0) {
#endif
		    CursesErrorMsg(Gtxt("Problems Writing",123));
		    closenet(sockfd);
		    return(FALSE);
	       }
	       twirlfn();
	  }
 	  if (ControlCpressed) {
	       CursesErrorMsg("Interrupted by User");
	       ControlCpressed = FALSE;
	       closenet(sockfd);
	       return(FALSE);
	  }

     }

     if (GSgetType(gs) == A_CSO)
	  writestring(sockfd, "quit\r\n");

     closenet(sockfd);
     return(TRUE);
}


/*
 * This function makes a nice vms filename.
 */

void
VMSfile(char *fname)
{
#ifndef VMS
     return;
#else
     char    *cp, *dot, *end;
     int     j, k;

     /*** Trim off date-size suffix, if present ***/
     end = fname + strlen(fname);
     if ((*(end - 1) == ']') && ((cp = strrchr(fname, '[')) != NULL) &&
         (cp > fname) && *(--cp) == ' ')
	  *cp = '\0';

     /*** Replace illegal or problem characters ***/
     dot = fname + strlen(fname);
     for (cp = fname; cp < dot; cp++) {

	  /** Replace with underscores **/
	  if (*cp == ' ' || *cp == '/' || *cp == ':' ||
	      *cp == '[' || *cp == ']' || *cp == '{' ||
	      *cp == '}')
	       *cp = '_';

	  /** Replace with dashes **/
	  else if (*cp == '!' || *cp == '?' || *cp == '\'' || 
	           *cp == ',' || *cp == ':' || *cp == '\"' ||
	           *cp == '+' || *cp == '@' || *cp == '\\' ||
	           *cp == '(' || *cp == ')' || *cp == '=' ||
	           *cp == '<' || *cp == '>' || *cp == '#' ||
	           *cp == '%' || *cp == '*' || *cp == '`' ||
	           *cp == '~' || *cp == '^' || *cp == '|' ||
		   *cp == '/')
	       *cp = '-';
     }

     /** Collapse any serial underscores **/
     cp = fname + 1;
     j = 0;
     while (cp < dot) {
	  if (fname[j] == '_' && *cp == '_')
	       cp++;
	  else
	       fname[++j] = *cp++;
     }
     fname[++j] = '\0';

     /** Collapse any serial dashes **/
     dot = fname + (strlen(fname));
     cp = fname + 1;
     j = 0;
     while (cp < dot) {
          if (fname[j] == '-' && *cp == '-')
	       cp++;
	  else
	       fname[++j] = *cp++;
     }
     fname[++j] = '\0';

     /** Trim any trailing or leading **/
     /** underscrores or dashes       **/
     cp = fname + (strlen(fname)) - 1;
     while (*cp == '_' || *cp == '-')
          *cp-- = '\0';
     if (fname[0] == '_' || fname[0] == '-') {
          dot = fname + (strlen(fname));
          cp = fname;
          while ((*cp == '_' || *cp == '-') && cp < dot)
	       cp++;
	  j = 0;
          while (cp < dot)
	       fname[j++] = *cp++;
	  fname[j] = '\0';
     }

     /** Replace all but the last period with _'s, or second **/
     /** to last if last is followed by a terminal Z or z,   **/
     /** e.g., convert foo.tar.Z to                          **/
     /**               foo.tar_Z                             **/
     j = strlen(fname) - 1;
     if ((dot = strrchr(fname, '.')) != NULL) {
	  if (((fname[j] == 'Z' || fname[j] == 'z') && fname[j-1] == '.') &&
	      (((cp = strchr(fname, '.')) != NULL) && cp < dot)) {
	       *dot = '_';
	       dot = strrchr(fname, '.');
	  }
	  cp = fname;
	  while ((cp = strchr(cp, '.')) != NULL && cp < dot)
	       *cp = '_';

          /** But if the root is > 39 characters, move **/
          /** the period appropriately to the left     **/
	  while (dot - fname > 39) {
	       *dot = '\0';
	       if ((cp = strrchr(fname, '_')) != NULL) {
		    *cp  = '.';
		    *dot = '_';
	       } 
	       else if ((cp = strrchr(fname, '-')) != NULL) {
		    *cp  = '.';
		    *dot = '_';
	       }
	       else {
		    *dot = '_';
		    j = strlen(fname);
		    fname[j+1] = '\0';
		    while (j > 39)
			 fname[j--] = fname[j];
		    fname[j] = '.';
	       }
               dot = strrchr(fname, '.');
	  }

          /** Make sure the extension is < 40 characters **/
          if ((fname + strlen(fname) - dot) > 39)
	       *(dot+40) = '\0';

	  /** Trim trailing dashes or underscores **/
	  j = strlen(fname) - 1;
	  while (fname[j] == '_' || fname[j] == '-')
	       fname[j--] = '\0';
     }
     else {
	  /** No period, so put one on the end, or after   **/
	  /** the 39th character, trimming trailing dashes **/
	  /** or underscrores                              **/
	  if (strlen(fname) > 39)
	       fname[39] = '\0';
	  j = strlen(fname) - 1;
	  while ((fname[j] == '_') || (fname[j] == '-'))
	       j--;
	  fname[++j] = '.';
	  fname[++j] = '\0';
     }

     /** Make sure the rest of the original string in nulled, **/
     /** to avoid problems with the CURwgetstr() line editor  **/
     cp = fname + strlen(fname);
     while (cp < end)
          *cp++ = '\0';

	  
#endif
}
     


/*
 * This function makes a nice UNIX filename
 */

void
UNIXfile(char *fname)
{
     char *cp;

     for (cp = fname; *cp != '\0'; cp++) {
	  switch (*cp) {
	  case '\'':
	  case '\"':
	  case '/':
	  case ' ':
	  case '(':
	  case ')':
	  case '&':
	       *cp = '-';
	       break;

	  case '[':
	  case ']':
	  case '{':
	  case '}':
	  case '|':
	  case '!':
	  case '?':
	  case '*':
	  case '`':
	  case '\\':
	  case '<':
	  case '>':
	       *cp = '_';
	       break;


	  }
     }

     if (strlen(fname) >50) {
	  fname[50] = '\0';
     }

     /*** Don't allow '-' or '.' as first character of filename ***/
     if (fname[0] == '-' || fname[0] == '.')
	  fname[0] = '_';

}


/*
 * This procedure prompts the user for a name, checks for pipe characters
 * and ~ characters in the filename and saves the file to disk.
 * 
 * If infile is NULL then contact the server and get the file.
 * 
 * If saveto is non-NULL then don't prompt the user for a filename
 * to save into, just "do it".
 */

void
Save_file(GopherObj *gs, char *saveto, char *view)
{
     char    Title[128], Userfilename[128];
     char    *cp;
     char    buf[1024];
     boolean Openpipe = FALSE;
     boolean GS2FileSucceeded = TRUE;
     FILE    *f;

     switch (GSgetType(gs)) {
     case A_CSO:
     case A_DIRECTORY:
     case A_ERROR:
     case A_INDEX:
     case A_INFO:
     case A_TELNET:
     case A_TN3270:
	  if (saveto == NULL) {
	       CursesErrorMsg(Gtxt("Sorry, can't save that item to a file!",154));
	       return;
	  }

     }

     if (view == NULL)
          view = Choose_View(gs);

     if (view == NULL || view == "")
	  return;

     /*** Get the Title ***/
     strcpy(Title, GSgetTitle(gs));

     /*** Construct a nice default filename ***/
     
     if (saveto == NULL) {
  	/* It shouldnt ever come here if Secure, but I've noticed calls 
     	to this in the code */
  	if (NoShellMode || SecureMode) {
		CursesErrorMsg(Gtxt("Sorry, you are not allowed to do this", 64));
		return;
  	}
	  /*** Let them use the Title ***/
	  strcpy(Userfilename, Title);

#ifdef VMS
	  VMSfile(Userfilename);
#else
	  UNIXfile(Userfilename);
#endif

	  if (CURGetOneOption(CursesScreen, Title, Gtxt("Save in file:",136),
	  		      Userfilename)<0)
	       return;

	  saveto = Userfilename;
     }

     if (*saveto == '\0')
	  return;

     if (view == NULL)
	  view = Choose_View(gs);

     if (view == NULL || view == "")
	  return;

#ifndef VMS
     if (*saveto == '|') {
	  /** Open a pipe! **/
	  Openpipe = TRUE;
	  saveto++;
     }
     
     if (*saveto == '~') {
	  /*** Save in our home directory ***/
	  if (*(saveto + 1) == '/' && (cp = getenv("HOME")) != NULL) {
	       /*** Expand ~ to the home directory ***/
	       strcpy(buf,cp);
	       buf[strlen(cp)]='/';
	       strcpy(buf+strlen(cp) +1, saveto+2);
	       strcpy(saveto, buf);
	  } else {
	  /*** Save in someone else's home directory ***/
	       struct passwd *pass;

	       cp = strchr(saveto,'/');
	       *cp = '\0';
	       pass = getpwnam(saveto+1);
	       if (pass != NULL) {
		    /** align in prep for the home dir **/
		    strcpy(buf,pass->pw_dir);
		    buf[strlen(pass->pw_dir)]='/';
		    strcpy(buf+strlen(pass->pw_dir)+1,cp+1);
		    strcpy(saveto,buf);
		    
	       } else {
		    char tmpstr[256];
		    sprintf(tmpstr, Gtxt("No such user '%s'",114), saveto+1);
		    CursesErrorMsg(tmpstr);
		    return;
	       }
	  }
     }
#endif

#if defined(VMS) && defined(VMSRecords)
     if (GSisText(gs, view))
          /*** Save ASCII files as VARIABLE length records for VMS ***/
	  f = fopen_VAR(saveto, "w+");
     else
          /*** Save binary files as FIXED 512 records for VMS ***/
	  f = fopen_FIX(saveto, "w+");
#else
     if (Openpipe)
	  f = popen(saveto, "w+");
     else
	  f = fopen(saveto, "w+");
#endif

     if (f == NULL) {
	  char tempstr[128];

	  sprintf(tempstr, Gtxt("Couldn't create '%s'",80), saveto);
	  CursesErrorMsg(tempstr);
	  return;
     }

     if (GSgetLocalFile(gs) != NULL && strcmp(view, GSgetLocalView(gs))==0) {
	  /** We've already retrieved it, copy it over... **/
	  int oldfd, cc;

	  oldfd = open(GSgetLocalFile(gs), O_RDONLY, 0);
	  if (oldfd <0) {
	       CursesErrorMsg(Gtxt("Can't open old file..",68));
	       if (Openpipe)
		    pclose(f);
	       else
		    fclose(f);
	       return;
	  }
	  
	  while ( ((cc=read(oldfd, buf, sizeof buf)) > 0) && !ControlCpressed){
	       if (fwrite(buf, cc, 1, f) <= 0)
		    CursesErrorMsg(Gtxt("Problems Writing",123));
	       twirl();
	  }
	  if (ControlCpressed) {
	       CursesErrorMsg("Save Interrupted by user");
	       ControlCpressed = FALSE;
 	  }
 

	  if (Openpipe)
	       pclose(f);
	  else
	       fclose(f);

	 close(oldfd);
	 return;

     } else {
	  /** We don't have the file yet, let's get it... **/
	  
	  GS2FileSucceeded = GStoFile(gs, f, view, twirl); /* false if fails */

	  if (Openpipe)
	       pclose(f);
	  else
	       fclose(f);

	  if (!GS2FileSucceeded)
	       unlink(saveto);
     }
}
	       

/*
 * This procedure saves a directory (menu) list to a file.
 * It prompts the user for a filename, checks for pipe characters
 * and ~ characters in the filename and saves the directory name
 * and the list of items in the file to disk.
 * 
 * This is useful after searches for which the titles of hits
 * themselves contain the information one was seeking.
 */

void
Save_list(GopherDirObj *gd)
{
     char    Title[256], Userfilename[128];
     char    *cp, *saveto;
     int     numitems, j;
     char    buf[128];
     boolean Openpipe = FALSE;
     FILE    *f;

     *Userfilename = '\0';

     sprintf(Title,"%s", GDgetTitle(gd));
     if (CURGetOneOption(CursesScreen, Title, 
			 Gtxt("Save list of items in file:",137),
                         Userfilename)<0)
	  return;
     saveto = Userfilename;
     if (*saveto == '\0')
	  return;

#ifndef VMS
     if (*saveto == '|') {
	  /** Open a pipe! **/
	  Openpipe = TRUE;
	  saveto++;
     }
     
     if (*saveto == '~') {
	  /*** Save in our home directory ***/
	  if (*(saveto + 1) == '/' && (cp = getenv("HOME")) != NULL) {
	       /*** Expand ~ to the home directory ***/
	       strcpy(buf,cp);
	       buf[strlen(cp)]='/';
	       strcpy(buf+strlen(cp) +1, saveto+2);
	       strcpy(saveto, buf);
	  } else {
	  /*** Save in someone else's home directory ***/
	       struct passwd *pass;

	       cp = strchr(saveto,'/');
	       *cp = '\0';
	       pass = getpwnam(saveto+1);
	       if (pass != NULL) {
		    /** align in prep for the home dir **/
		    strcpy(buf,pass->pw_dir);
		    buf[strlen(pass->pw_dir)]='/';
		    strcpy(buf+strlen(pass->pw_dir)+1,cp+1);
		    strcpy(saveto,buf);
		    
	       } else {
		    char tmpstr[256];
		    sprintf(tmpstr, Gtxt("No such user '%s'",114), saveto+1);
		    CursesErrorMsg(tmpstr);
		    return;
	       }
	  }
     }
#endif

#if defined(VMS) && defined(VMSRecords)
     f = fopen_VAR(saveto, "w+");
#else
     if (Openpipe)
	  f = popen(saveto, "w+");
     else
	  f = fopen(saveto, "w+");
#endif

     if (f == NULL) {
	  char tempstr[128];

	  sprintf(tempstr, Gtxt("Couldn't create '%s'",80), saveto);
	  /** Should give better error messages here **/
	  CursesErrorMsg(tempstr);
	  return;
     }
     
     /*** Write the Title, and put the Searchstring, ***/
     /*** if present, on the next line.              ***/
     sprintf(buf,"  %s\n\n", GDgetTitle(gd));
     if (Searchstring != NULL)
          if ((cp = strstr(buf, Searchstring)) != NULL)
	       *(cp-1) = '\n';
     if (fwrite(buf, strlen(buf), 1, f) <= 0)
          CursesErrorMsg(Gtxt("Problems Writing",123));
     twirl();

     /*** Write the Items ***/
     numitems = GDgetNumitems(gd);
     for (j = 0; j < numitems; j++) {
          sprintf(buf, "%s\n", GSgetTitle(GDgetEntry(gd, j))); 
          if (fwrite(buf, strlen(buf), 1, f) <= 0) {
               CursesErrorMsg(Gtxt("Problems Writing",123));
          }
	  twirl();
     }

     if (Openpipe)
          pclose(f);
     else
          fclose(f);
}



/*
** This procedure exits out of the curses environment and
** displays the file indicated by pathname to the screen
** using a pager command of some sort 
*/

void
GSdisplay(GopherObj *gs)
{
     char command[MAXSTR];
     char *file = GSgetLocalFile(gs);
     char *view = GSgetLocalView(gs);

#ifdef VMS
     char *cp;
     int return_status;
     /** Keep DECC from complaining **/
     struct dsc$descriptor_s  command_desc;
     command_desc.dsc$b_class   = DSC$K_CLASS_S;
     command_desc.dsc$b_dtype   = DSC$K_DTYPE_T;
#endif


     /** Search for a helper application **/
     RCdisplayCommand(GlobalRC, view, file, command);

     Debug("Displaying view %s ", view);
     Debug("command %s ", command);
     Debug("file %s", file);
     Debug("/%s \n", GSgetLocalFile(gs));

     if (strlen(command) == 0) {
	  if (SecureMode || NoShellMode)
	       CursesErrorMsg(Gtxt("Sorry, no viewer for the file....",166));
	  else
	       Save_file(gs, NULL, view);

	  return;
     }


#ifdef VMS
     if (strncasecmp("TPU",command,3) == 0) {
	  
	  CURexit(CursesScreen);

	  /** Make sure we invoke TPU in /READ_ONLY/NOJOURNAL mode **/
	  for (cp = command; *cp; cp++)
               *cp = _toupper(*cp);
	  if (strstr(command, "/READ") == NULL)
	       strcat(command,"/READ");
	  if (strstr(command, "/NOJOU") == NULL)
	       strcat(command,"/NOJOU");

	  /** Load the descriptor and call TPU **/
          command_desc.dsc$w_length  = strlen(command);
          command_desc.dsc$a_pointer = command;
	  return_status = TPU$TPU(&command_desc);
	  if (!return_status&1) {
	       CURenter(CursesScreen);
	       CursesErrorMsg(Gtxt("Error invoking callable TPU as a pager!!!",85));
	       return;
	  }
	  if (GSgetType(gs) != A_APP) {
	       GSprompter(gs, view, command);
	  }
     } else
#endif
     if (strncasecmp(command, "builtin-upload", 14) == 0) {
	  BuiltinUploadfile();
	  return;
     }
     else if (strncasecmp(command, "builtin-download", 14) == 0) {
	  BuiltinDownload(getenv("HOME"));
	  return;
     }
     else if (strncasecmp(command, "builtin", 7) == 0) {
	  PagerBuiltin(CursesScreen, gs);
	  return;
     }
     else {
	  CURexit(CursesScreen);
	  FIOsystem(command);
	  if (GSgetType(gs) != A_APP) {
	       GSprompter(gs, view, command);
	  }

     }
}


/*
 * Display the prompt at the end of viewing a document
 */

void
GSprompter(GopherObj *gs, char *view, char *command)
{
     int ch;

#ifdef VMS
     printf(Gtxt("\nPress %sRETURN%s to continue",184),
     	    CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen));

     if (!SecureMode && !NoShellMode) {
          if (COLS < 78)
	       printf(",\n   ");
	  else
	       printf(", ");
	  printf(Gtxt("%sm%s to mail, %sD%s to download, %ss%s to save, or %sp%s to print:",61),
		 CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen),
		 CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen),
		 CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen),
		 CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen));
     }
#ifndef NOMAIL
     else {
	  printf(Gtxt(", %sm%s to mail, %sD%s to download:",62),
		 CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen),
		 CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen));
     }
#else
     else {
	  printf(Gtxt(", %sD%s to download:",237),
		 CURgetHighon(CursesScreen), CURgetHighoff(CursesScreen));
     }
#endif
#else /*** Unix: ****/
     printf(Gtxt("\nPress <RETURN> to continue",185));

     if (!SecureMode && !NoShellMode)
	  printf(Gtxt(",\n   <m> to mail, <D> to download, <s> to save, or <p> to print:",64));
#ifndef NOMAIL
     else
	  printf(Gtxt(", <m> to mail, <D> to download:",63));
#else
     else
	  printf(Gtxt(", <D> to download:",238));
#endif
#endif
     fflush(stdin);
     fflush(stdout);
     noecho();
     cbreak();

     ch = 0;
     while (ch == 0) {
	  ch=getchar();
#ifdef VMS
	  if (HadVMSInt)
	       break;
#endif

	  if (SecureMode || NoShellMode) {
	       switch (ch) {
	       case '\n':
	       case '\r':
	       case KEY_ENTER:
	       case ' ':
	       case 'q' :
	       case 'Q' :
#ifdef VMS
	       case '\032': /* ^Z */
#endif
		    break;
#ifndef NOMAIL
	       case 'm':
#ifdef VMS
                    (void) getchar();
#endif
		    CURenter(CursesScreen);
		    GSmail(gs);
		    break;
#endif
	       case 'D':
#ifdef VMS
		    (void) getchar();
#endif
		    CURenter(CursesScreen);
		    Download_file(gs);
		    break;

	       default:
		    puts(CURgetBell(CursesScreen));
		    fflush(stdout);
		    ch=0;
		    break;
	       }
	  }
	  else {

	       switch(ch) {
	       case '\n':
	       case '\r':
	       case ' ' :
	       case 'q' :
	       case 'Q' :
#ifdef VMS
	       case '\032': /* ^Z */
#endif
		    break;
	       case 's':
#ifdef VMS
                    (void) getchar();
#endif
		    CURenter(CursesScreen);
		    Draw_Status(Gtxt("Saving File...",138));
		    refresh();

 		    if (!(SecureMode || NoShellMode))
			 Save_file(gs,NULL, view);
		    break;

	       case 'p':
#ifdef VMS
                    (void) getchar();
#endif
		    if (!RCprintCommand(GlobalRC, view, GSgetLocalFile(gs), command)) {
			 CursesErrorMsg(Gtxt("Sorry, cannot print this document",155));
			 return;
		    }
		    FIOsystem(command);
		    break;
		    
	       case 'm':
#ifdef VMS
                    (void) getchar();
#endif
                    CURenter(CursesScreen);
		    GSmail(gs);
		    break;

	       case 'D':
#ifdef VMS
		    (void) getchar();
#endif
		    CURenter(CursesScreen);
		    Download_file(gs);
		    break;
		    
	       default:
		    Debugmsg("GSdisplayAfter - beep\r\n");
		    puts(CURgetBell(CursesScreen));
		    fflush(stdout);
		    ch=0;
		    break;
	       }
	  }
     }
     
     tputs(CURgetCLS(CursesScreen),1,outchar);
     fflush(stdout);
}



/*
 * This displays a single line message using CUR routines
 */
     
void
CursesMsg(char *Message, char *title)
{
     char *mess[2];
     
     mess[0] = Message;
     mess[1] = NULL;

     Debugmsg("CursesMsg - beep\r\n")
     CURBeep(CursesScreen);

     CURDialog(CursesScreen, title,mess);

     return;

}


/*
 * This does the same as above, but puts a window title on the mess 
 */

void
CursesErrorMsg(char *Message)
{
     CursesMsg(Message, Gtxt("Gopher Error",90));
}



#ifdef CLIENT_LOGGER
#if defined(VMS) && defined(MULTINET)
#include "syslog.h"
#else
#include <syslog.h>
#endif
#endif

int
logrequest(char *msg, GopherObj *gs)
{
#ifdef CLIENT_LOGGER
     static boolean inited = FALSE;
     char *url = "";

     if (inited == FALSE) {
	  openlog("gopher", (LOG_PID), LOG_DAEMON);
	  setlogmask(LOG_UPTO(LOG_INFO));
	  inited = TRUE;
     }
     if (gs != NULL)
	  url = GSgetURL(gs, "");

     syslog(LOG_INFO, "%s %s", msg, url ? url : "");

#endif
     return(0);
}


#if defined(VMS) && defined(MULTINET) && ( defined(CLIENT_LOGGER) || defined(TELNET_TRACE) )

/*
 *	Copyright (C) 1992	TGV, Incorporated
 */

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef lint
static char sccsid[] = "@(#)syslog.c    5.3 (Berkeley) 9/17/85";
#endif /* not lint */

/*
 * SYSLOG -- print message on log file
 *
 * This routine looks a lot like printf, except that it
 * outputs to the log file instead of the standard output.
 * Also:
 *	adds a timestamp,
 *	prints the module name in front of the message,
 *	has some other formatting types (or will sometime),
 *	adds a newline on the end of the message.
 *
 * The output of this routine is intended to be read by /etc/syslogd.
 *
 * Author: Eric Allman
 * Modified to use UNIX domain IPC by Ralph Campbell
 */

#include <stdio.h> /* JDB */
#include <lnmdef.h>  /* JLW */
#include <ssdef.h>   /* JLW */

#include <types.h>
#include <socket.h>
#include <file.h>
#include <signal.h>

#ifndef _sys_syslog_h
#include "syslog.h"	/*  was <syslog.h> -- JLW */
#endif
#include <netdb.h>
#ifdef __ALPHA
#include <varargs.h>
#endif /* __ALPHA */

#define MAXLINE 1024			/* max message size */

#define PRIMASK(p)	(1 << ((p) & LOG_PRIMASK))
#define PRIFAC(p)	(((p) & LOG_FACMASK) >> 3)
#define IMPORTANT	LOG_ERR

static char	logname[] = "/dev/log";
static char	ctty[] = "/dev/console";
static int	LogFile = -1;		/* fd for log */
static int	LogStat = 0;		/* status bits, set by openlog() */
static char	*LogTag = "syslog";	/* string to tag the entry with */
static int	LogMask = 0xff;		/* mask of priorities to be logged */
static int	LogFacility = LOG_USER; /* default facility code */

static struct sockaddr SyslogAddr;	/* AF_UNIX address of local logger */

#ifdef ORIG
extern int errno;
extern	int sys_nerr;
extern	char *sys_errlist[];
#endif

#ifndef VMS$TrnLNM	/*  JLW -- use SYS$TRNLNM() instead of VMS$TrnLNM() */
static
    int	i, junk, buf_len;
int	no_case = LNM$M_CASE_BLIND;
static	$DESCRIPTOR(lname_desc,"MULTINET_SYSLOG_DESTINATION");
static	$DESCRIPTOR(tabnam,"LNM$SYSTEM_TABLE");
char	syslogging_status[256];
struct itmlst
{
	short int buf_len;
	short int item_code;
	char *buffer;
	int *buf_length;
};
static	
    struct itmlst
		item_str[] = { {4,LNM$_INDEX,(char *)&i,&junk},
					{0,LNM$_STRING,0,&buf_len},
					{0,0,0,0}};
#endif

char	*vms_errno_string();

#ifdef __ALPHA
syslog(va_alist)
va_dcl
#else  /* __ALPHA */
syslog(pri, fmt, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9)
	int pri;
	char *fmt;
#endif /* __ALPHA */
{
	char buf[MAXLINE + 1], outline[MAXLINE + 1];
	register char *b, *f, *o;
	register int c;
	long now;
	int pid;
#ifdef ORIG
	int pid, olderrno = errno;
#endif
#ifdef __ALPHA
	int pri, numargs;
	char *fmt;
	__int64 p0,p1,p2,p3,p4,p5,p6,p7,p8,p9;
	va_list ap;

	va_start(ap);
	va_count(numargs);

	pri = va_arg(ap, int);
	fmt = va_arg(ap, char *);

	if (numargs > 2) p0 = va_arg(ap, __int64);
	if (numargs > 3) p1 = va_arg(ap, __int64);
	if (numargs > 4) p2 = va_arg(ap, __int64);
	if (numargs > 5) p3 = va_arg(ap, __int64);
	if (numargs > 6) p4 = va_arg(ap, __int64);
	if (numargs > 7) p5 = va_arg(ap, __int64);
	if (numargs > 8) p6 = va_arg(ap, __int64);
	if (numargs > 9) p7 = va_arg(ap, __int64);
	if (numargs > 10) p8 = va_arg(ap, __int64);
	if (numargs > 11) p9 = va_arg(ap, __int64);

#endif /* __ALPHA */


	/* see if we should just throw out this message */
	if (pri <= 0 || PRIFAC(pri) >= LOG_NFACILITIES || (PRIMASK(pri) & LogMask) == 0)
		return;
	if (LogFile < 0)
		openlog(LogTag, LogStat & ~LOG_ODELAY, 0);

	/* set default facility if none specified */
	if ((pri & LOG_FACMASK) == 0)
		pri |= LogFacility;

	/* build the message */
	o = outline;
	sprintf(o, "<%d>", pri);
	o += strlen(o);
	time(&now);
	sprintf(o, "%.15s ", ctime(&now) + 4);
	o += strlen(o);
	if (LogTag && strcmp(LogTag, "dprintf")) {
		strcpy(o, LogTag);
		o += strlen(o);
	}
	if (LogStat & LOG_PID) {
		sprintf(o, "[Pid 0x%x]", getpid());
		o += strlen(o);
	}
	if (LogTag  &&	strcmp(LogTag, "dprintf")) {
		strcpy(o, ": ");
		o += 2;
	}

	b = buf;
	f = fmt;
	while ((c = *f++) != '\0' && c != '\n' && b < &buf[MAXLINE]) {
		if (c != '%') {
			*b++ = c;
			continue;
		}
		if ((c = *f++) != 'm') {
			*b++ = '%';
			*b++ = c;
			continue;
		}
		sprintf(b, "%s", vms_errno_string() );
		b += strlen(b);
	}
	*b++ = '\n';
	*b = '\0';
	sprintf(o, buf, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	c = strlen(outline);
	if (c > MAXLINE)
		c = MAXLINE;

	/* output the message to the local logger */
#ifndef VMS$TrnLNM
	item_str[1].buf_len = 255;
	item_str[1].buffer = syslogging_status;
	i = 0;
	if (SS$_NORMAL == 
		    SYS$TRNLNM(&no_case,&tabnam,&lname_desc,0,&item_str)) {
#else
	if (VMS$TrnLNM("MULTINET_SYSLOG_DESTINATION",0)) { 
#endif
	    if (sendto(LogFile, outline, c, 0, &SyslogAddr, sizeof SyslogAddr) >= 0) {
		    return 1;
	    }
	} 
#ifdef Send_Oper
	if (!(LogStat & LOG_CONS) && (pri & LOG_PRIMASK) <= LOG_ERR)
		return -1;

	 /* Output the message to the console, add the LogTag so it looks
	  * like syslog(), even if syslog is disabled and we're using OPCOM.
	  * make sure there is a newline.
	  */

	o = outline;
	if (LogTag && strcmp(LogTag, "dprintf"))  {
		strcpy(o, LogTag);
		o += strlen(o);
		*o++ = ':';
		*o++ = ' ';
	}
	sprintf(o, fmt, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	c = strlen(outline);
	if (c > MAXLINE)
		c = MAXLINE-1;
	if ( outline[c-1] != '\n' ) {
		outline[c]  = '\n';
		outline[c+1]  = '\0';
	}
	Send_Oper(outline);
#endif
	return 1;
}

/*
 * OPENLOG -- open system log
 */

openlog(ident, logstat, logfac)
	char *ident;
	int logstat, logfac;
{
	if (ident != NULL)
		LogTag = ident;
	LogStat = logstat;
	if (logfac != 0)
		LogFacility = logfac & LOG_FACMASK;
	if (LogFile >= 0)
		return;
	SyslogAddr.sa_family = AF_UNIX;
	strncpy(SyslogAddr.sa_data, logname, sizeof SyslogAddr.sa_data);
	if (!(LogStat & LOG_ODELAY)) {
		LogFile = socket(AF_UNIX, SOCK_DGRAM, 0);
#ifdef ORIG
		fcntl(LogFile, F_SETFD, 1);
#endif
	}
}

/*
 * CLOSELOG -- close the system log
 */
closelog()
{

	(void) close(LogFile);
	LogFile = -1;
}

/*
 * SETLOGMASK -- set the log mask level
 */
setlogmask(pmask)
	int pmask;
{
	int omask;

	omask = LogMask;
	if (pmask != 0)
		LogMask = pmask;
	return (omask);
}

#endif


