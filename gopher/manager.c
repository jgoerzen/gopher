/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.4 $
 * $Date: 2002/02/12 21:20:14 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/manager.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: manager.c
 * Procedures to manage the screen.
 *********************************************************************
 * Revision History:
 * $Log: manager.c,v $
 * Revision 1.4  2002/02/12 21:20:14  jgoerzen
 * Made files using strcasecmp() include util.h
 *
 * Revision 1.3  2001/01/03 22:30:43  s2mdalle
 * Inclusion of new header file for this module.
 *
 * Revision 1.2  2000/12/27 21:26:22  s2mdalle
 * Added #include for CUR* functions
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.31  1995/11/03  21:18:20  lindner
 * ANSIfication
 *
 * Revision 3.30  1995/11/03  20:50:50  lindner
 * Coen: fixes..
 *
 * Revision 3.29  1995/03/09  05:21:23  lindner
 * + on last page goes back to beginning
 *
 * Revision 3.28  1995/02/27  17:09:51  lindner
 * Add Pacbell NOBANNER patch
 *
 * Revision 3.27  1994/10/24  22:14:56  lindner
 * Add pdf type
 *
 * Revision 3.26  1994/07/25  02:54:45  lindner
 * Skip over type 'i' items
 *
 * Revision 3.25  1994/06/29  07:11:03  lindner
 * change pointer item for info or error itme to a line
 *
 * Revision 3.24  1994/05/17  05:48:01  lindner
 * Massive internationalization change
 *
 * Revision 3.23  1994/05/14  04:13:45  lindner
 * Internationalization...
 *
 * Revision 3.22  1994/04/25  03:37:42  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.21  1994/03/31  21:26:15  lindner
 * Make DEBUG toggleable
 *
 * Revision 3.20  1994/03/08  15:55:13  lindner
 * gcc -Wall fixes
 *
 * Revision 3.19  1994/03/04  23:33:56  lindner
 * better screen size calculations
 *
 * Revision 3.18  1994/02/20  16:31:43  lindner
 * Patch around ANSI C trigraph sequence
 *
 * Revision 3.17  1993/12/28  17:31:40  lindner
 * Prettier Info items in gopher directories, removed dot from the end of files
 *
 * Revision 3.16  1993/11/29  01:11:46  lindner
 * In DisplayTitle(), <Movie> menu items now fit properly when the line is
 * too long for the screen.  (Beckett)
 *
 * Revision 3.15  1993/10/26  18:43:20  lindner
 * Fix for multiply defined cases
 *
 * Revision 3.14  1993/10/22  20:30:34  lindner
 * Fixes for menu drawing (Beckett)
 *
 * Revision 3.13  1993/10/22  20:05:20  lindner
 * Add Movie (;) and Info (i) type support
 *
 * Revision 3.12  1993/09/08  01:22:30  lindner
 * Add support for HTML and MIME on the menu displays
 *
 * Revision 3.11  1993/08/23  02:32:48  lindner
 * Fix for arrow updating
 *
 * Revision 3.10  1993/08/19  20:22:54  lindner
 * Mitra's Debug patch
 *
 * Revision 3.9  1993/08/16  18:11:29  lindner
 * fix for VMS Alpha systems
 *
 * Revision 3.8  1993/07/30  14:19:34  lindner
 * Mitra autoexit patch
 *
 * Revision 3.7  1993/07/27  05:28:56  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.6  1993/07/27  02:02:45  lindner
 * More comments
 *
 * Revision 3.5  1993/07/20  23:12:32  lindner
 * Mods to use patchlevel.h
 *
 * Revision 3.4  1993/04/23  20:14:13  lindner
 * Fix for munged characters in Draw_Status
 *
 * Revision 3.3  1993/04/15  21:17:00  lindner
 * none
 *
 * Revision 3.2  1993/03/26  19:43:46  lindner
 * Fix for repainting when selecting an item bu number
 *
 * Revision 3.1.1.1  1993/02/11  18:02:58  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1993/02/09  22:36:56  lindner
 * Now shows ask items, provision for auto size display in the future
 *
 * Revision 1.4  1993/01/31  00:08:12  lindner
 * New fcn DisplayTitle()
 *
 * Revision 1.3  1993/01/06  21:01:40  lindner
 * Improved behaviour when executing an item by typing it's number
 * The arrow and screen are updated to reflect the item being
 * retrieved.
 *
 * Revision 1.2  1993/01/05  22:31:57  lindner
 * Fixed display problems with directory title searching.
 *
 * Revision 1.1  1992/12/10  23:32:16  lindner
 * gopher 1.1 release
 *
 * Revision 1.1  1992/12/10  06:16:51  lindner
 * Initial revision
 *
 *
 *********************************************************************/

#include "CURcurses.h"
#include "gopher.h"
#include "patchlevel.h"
#include "Debug.h"
#include "manager.h"
#include "util.h"

#ifdef NOBANNER
#  define MENULINE(x)   (x)+1
#else
#  define MENULINE(x)   (x)+3
#endif


/* If any gophers to display (screen can be blank), count the number
   of pages.  If there is a remainder greater than zero, add one page */
#define PAGECALC(x,y) (y) ? (x/y) + ((x%y)>0) : 1



/*
** Draw the title on the top
*/

void
Draw_Banner(void)
{
#ifndef NOBANNER
     char line[80];

     sprintf(line, Gtxt("Internet Gopher Information Client v%s.%s.%d",102),
	     GOPHER_MAJOR_VERSION, GOPHER_MINOR_VERSION, PATCHLEVEL);
     CURcenterline(CursesScreen, stdscr, line, 0, COLS, TRUE);
#endif
}     


/*
** Draw the status line
*/

void
Draw_Status(char *textline)
{
     mvaddstr(LINES-1, 0, Gtxt("Press ",119));
     standout();
     addstr("?");
     standend();

     addstr(Gtxt(" for Help, ",57));

     standout();
     addstr("q");
     standend();
     addstr(Gtxt(" to Quit",58));
     
#ifndef AUTOEXITONU
     if (iLevel != 0) {
	  addstr(", ");
	  standout();
	  addstr("u");
	  standend();
	  addstr(Gtxt(" to go up a menu",59));
     }
#endif
     clrtoeol();

     mvaddch(LINES-1,  COLS-strlen(textline)-4, ' ');
     addstr(textline);
}

void
DisplayTitle(GopherObj *gs, int maxlength, boolean dogplus)
{
     char type;
     char *c, *d;
     char *size;
     int  m,n;
     char temp[1024];

     type = GSgetType(gs);
     d = GSgetTitle(gs);

     if (GSisGplus(gs) && GSgplusInited(gs)) {

	  size = VIgetSize(GSgetView(gs, 0));
     }

     if (GSisAsk(gs))
	  maxlength -= 5;

     switch(type)
     {
     case A_DIRECTORY:
	  maxlength--;
	  break;
     case A_SOUND:
	  maxlength -= 3;
	  break;
     case A_INDEX:
	  maxlength -=4;
	  break;
     case A_PDF:
     case A_CSO:
     case A_TELNET:
     case A_UNIXBIN:
     case A_MACHEX:
	  maxlength -=6;
	  break;
     case A_TN3270:
     case A_MIME:
     case A_HTML:
	  maxlength -= 7;
	  break;
     case A_MOVIE:
	  maxlength -= 8;
	  break;
     case A_PCBIN:
	  maxlength -=9;
	  break;
     case A_GIF:
     case A_IMAGE:
	  maxlength -= 10;
     }
     
     if (d==NULL)
	  d = "error";
     
     if((m = strlen(d)) <= maxlength)
	  printw(" %s", d);
     else {
	  /*** Cut out the middle bits **/
	  if ((c = strchr(d, '/'))!=NULL && (maxlength > (c-d))) {
	       n = c - d;
	       strncpy(temp, d, n);
	       strcpy(temp + n, "..");
	       strcat(temp, d + (m + n - maxlength));
	       printw(" %s", temp);
	  } else {
	       /** Trunc it.. **/
	       strcpy(temp, d);
	       temp[maxlength] ='\0';
	       printw(" %s..", temp);
	  }
     }
     
     switch(type)
     {
     case A_DIRECTORY:
	  addch('/');
	  break;
     case A_CSO:
	  addstr(" <CSO>");
	  break;
     case A_TN3270:
	  addstr(" <3270>");
	  break;
     case A_TELNET:
	  addstr(" <TEL>");
	  break;
     case A_INDEX:
	  addstr(" <?>");
	  break;
     case A_SOUND:
	  addstr(" <)");  /** It's supposed to look like a speaker! **/
	  break;
     case A_PCBIN:
	  addstr(" <PC Bin>");
	  break;
     case A_UNIXBIN:
	  addstr(" <Bin>");
	  break;
     case A_IMAGE:
     case A_GIF:
	  addstr(" <Picture>");
	  break;
     case A_MACHEX:
	  addstr(" <HQX>");
	  break;
     case A_MIME:
	  addstr(" <MIME>");
	  break;
     case A_HTML:
	  addstr(" <HTML>");
	  break;
     case A_MOVIE:
	  addstr(" <Movie>");
	  break;
     case A_PDF:
	  addstr(" <PDF>");
	  break;
     }

     if (GSisAsk(gs)) {
	  /* Add string in two parts to avoid ANSI C turning "??>" into a
	       trigraph for '}' */
	  addstr(" <?");
	  addstr("?>");
     }
}


/*
** Man is this ugly.
*/

void
Display_Dir_Page(GopherDirObj *gopherdir, 
		 int iNewLine, 
		 int nNewPage, 
		 int nMaxPages, 
		 int iPageLen, 
		 int iLastPageLen)
{
     int i, iLoop, iOffset;
     boolean dogplus = FALSE;
     GopherObj *gs;
     static char *MenuStyle = NULL;
     static int MenuBytePad;
     int j;


     /*** Clear the screen and redraw the top line **/
     clear();
     Draw_Banner();
     if (MenuStyle == NULL) {
	  if ((MenuStyle = getenv("GOPHERMENUSTYLE")) == NULL)
	       MenuStyle = "[%d]";
	  MenuBytePad = strlen(MenuStyle)-2;
     }


     /** Draw the menu **/
     iLoop = (nNewPage == nMaxPages) && iLastPageLen ? iLastPageLen : iPageLen;

     /** Look at the first item in the directory to decide whether to
         use a gopher+ display of the menu ***/


     for (i= 0, iOffset = (nNewPage-1) * iPageLen; i <iLoop; i++, iOffset++) {
	  gs = GDgetEntry(gopherdir, iOffset);

	  move(MENULINE(i+1), 6);

	  if (GSgetType(gs) == A_INFO) {
	       for (j=0; j <= MenuBytePad; j++)
		    addch(' ');

	       if (iOffset > 9)
		    addch(' ');
	       if (iOffset >99)
		    addch(' ');
	  } else {
	       printw(MenuStyle, iOffset +1);
	  }
	  if (iOffset + 1 < 10)
	       addch(' ');

	  dogplus = GSisGplus(GDgetEntry(gopherdir, iOffset));
	  DisplayTitle(GDgetEntry(gopherdir, iOffset), COLS-13, dogplus);
     }
}


/* scline - Screen line relocator.
 *          Returns the line resulting from choice */
int 
scline(int iOldGopher, int iNewGopher, GopherDirObj *gophersdir)
{
     int iPageLen, iLastPageLen;        /* Length of normal, final pages */
     int nMaxPages, nNewPage, nOldPage; /* Natural numbers */
     int iOldLine, iNewLine;            /* Screen locations */
     char sPagenum[40];
     int iMaxGophers;
	 GopherObj *tempGopher;
	 int Direction = 1;
     
     iMaxGophers = GDgetNumitems(gophersdir);

     if (iNewGopher==0)
	  iNewGopher = GDgetNumitems(gophersdir);

     if (iNewGopher > iMaxGophers)
	  iNewGopher = 1;
     
	 if (iNewGopher == iOldGopher - 1)
		 Direction = -1;
	 do {
	      if (iNewGopher <= 0)
		   break;

		 tempGopher = GDgetEntry(gophersdir, iNewGopher-1);
		 if (GSgetType(tempGopher) != A_INFO &&
			 GSgetType(tempGopher) != A_ERROR)
			 break;
		 iNewGopher += Direction;
		 if (iNewGopher > iMaxGophers) {
			 iNewGopher = 1;
			 if (iOldGopher < 1)
				 break;
		 }
		 if (iNewGopher < 1)
			 iNewGopher = iMaxGophers;
		 if (iOldGopher > iMaxGophers)
			 break;
	 } while (iNewGopher != iOldGopher);
     iPageLen = LINES-4 - TITLEPOS;/* Number of menu lines possible per page */
     
     nMaxPages = PAGECALC(iMaxGophers, iPageLen);    /* Total number of pages */
     nOldPage =  PAGECALC(iOldGopher, iPageLen); 
     nNewPage =  PAGECALC(iNewGopher, iPageLen);
     
     if ((nNewPage < 1) || (nNewPage > nMaxPages))   /* It won't work , make*/
	  return(iOldGopher);                          /* no changes */
     
    iLastPageLen = iMaxGophers % iPageLen;

    /* Lines on last page */

     iOldLine = iOldGopher - ((nOldPage-1)*iPageLen);/* Old Screen location */
     iNewLine = iNewGopher - ((nNewPage-1)*iPageLen);/* New Screen location */
     
     if ((iNewLine < 0) || (iNewLine > iPageLen))
	  return(iOldGopher);
     
     if (nOldPage != nNewPage)    {
	Display_Dir_Page(gophersdir,
                        iNewLine, nNewPage, nMaxPages, iPageLen, iLastPageLen);
	/*** Draw the title ***/
#ifdef NOBANNER
	standout();
#endif
	CURcenterline(CursesScreen, stdscr, GDgetTitle(gophersdir), TITLEPOS, COLS, FALSE);
#ifdef NOBANNER
	standend();
#endif
   }
	

     sprintf(sPagenum, Gtxt("  Page: %d/%d",56), nNewPage, nMaxPages);
     Draw_Status(sPagenum);
     mvaddstr(MENULINE(iOldLine), 1, "   ");
     if ( (GSgetType(GDgetEntry(gophersdir, iNewGopher-1)) == A_INFO) ||
	  (GSgetType(GDgetEntry(gophersdir, iNewGopher-1)) == A_ERROR) )
	  mvaddstr(MENULINE(iNewLine), 1, "---");
     else
	  mvaddstr(MENULINE(iNewLine), 1, "-->");
     refresh();

     return(iNewGopher);
}

/*
** This routine draws a numbered menu
** from a gopherdirobj
** 
** It returns the number that the user selected, or it returns
** zero if the user decided to cancel.
**
**       RETURN Code isnt used currently anywhere!
*/

int 
GetMenu(GopherDirObj *gd, int *typedchar, boolean redisplay)
{
     int ch;                /* Input character */
     int iItem;             /* Display line */
     static int iNewItem=1;
     char sLinenum[5];      /* Used when going to a specific line */
     int numitems;
     /** variables for searching **/
     char search1[100];
     char *search2;
     int sfound;
     int i;

     search1[0] = '\0'; /* search string will be remembered so init now */

     numitems = GDgetNumitems(gd);
     iItem = -1;
     iNewItem = GDgetCurrentItem(gd);

     if (redisplay == TRUE) {

	  /*** Draw the title ***/
#ifdef NOBANNER
	  standout();
#endif
	  CURcenterline(CursesScreen, stdscr, GDgetTitle(gd), TITLEPOS, COLS, FALSE);
#ifdef NOBANNER
	  standend();
#endif

	  /* Move to the last line that we were sitting on */
 	  /* scline is the bit that actually draws the menu */
	  iItem = scline(iItem, iNewItem, gd);
     } else
	  iItem = GDgetCurrentItem(gd);


     while (1) {
#ifdef VMS
	  if (HadVMSInt) {
	       HadVMSInt = FALSE;
	       ch = 'q';
	  }
	  else
	       ch = CURgetch(CursesScreen);
#else
	  ch = CURgetch(CursesScreen);
#endif

 	  /* Note letters not in here are picked up by default and passed back
 		to caller for processing */
	  switch(ch)
	  {
	  case '\004':
	       DEBUG = 1 - DEBUG;
	       break;
	  case 'j':
	  case '\016':
	  case KEY_DOWN:

	       iNewItem = iItem + 1; /* Advance down the page */
	       break;
	       
	  case 'k':
	  case '\020':   /*** For those emacs dudes **/
	  case KEY_UP:

	       iNewItem = iItem - 1; /* Back up */
 	       break;


	  case '+':  /** Like in elm **/
	  case '>':  /** Like in nn  **/
	  case ' ':  /** Like in the pager ***/
	  case KEY_NPAGE:
	       /*** Go down a page ***/
	       if (iItem == numitems) { /* Already on last item */
		    iNewItem = 1;
	       } else {
		    iNewItem = iItem + (LINES -4 - TITLEPOS);
		    if (iNewItem > numitems)
			 iNewItem = numitems;
	       }
	       break;

	       
	  case '-':
	  case '<':
	  case KEY_PPAGE:
	  case 'b':      /*** Like in the pager ***/
	       /*** Go up a page ***/

	       iNewItem = iItem - (LINES - 4 - TITLEPOS);
	       if ( iNewItem < 0 )
		    iNewItem = 1;
               break;

	  case '1': case '2': case '3': case '4': case '5':
	  case '6': case '7': case '8': case '9': case '0':
	       
	       sLinenum[0] = (char) ch;
	       sLinenum[1] = '\0';
	       
	       if (CUROldGetOneOption(CursesScreen, 
			     Gtxt("View item number: ",179),
				      sLinenum) <0) {
		    scline(iItem, iNewItem, gd);
		    break;
	       }
	       
	       if (atoi(sLinenum) <= numitems)
		    iNewItem = atoi(sLinenum); /* Jump */
	       else {
		    CURBeep(CursesScreen);
		    break;
	       }

	       iItem = scline(iItem, iNewItem, gd);

	       if (iNewItem > 0 && iNewItem <= GDgetNumitems(gd)) {
		    *typedchar = '\n';
		    GDsetCurrentItem(gd, iNewItem);
		    return(iItem);
	       }


	       break;
	       
	  case '/': 
	  case 'n':
	       sfound = 0;

	       if (search1[0] == '\0' && ch == 'n') {
		    CursesErrorMsg(Gtxt("Use '/' to define a search first...",178));
		    iItem = scline(-1,iItem, gd);
		    break;
	       }

	       if (search1[0] == '\0' || ch == '/')
		    if (CURGetOneOption(CursesScreen, GDgetTitle(gd),
		              Gtxt("Search directory titles for:",139)
			      , search1)<0) {
			 iItem = scline(-1,iItem, gd);
			 break;
		    }
	       if (strlen(search1) == 0) {
		    iItem = scline(-1, iItem, gd);
		    break;
	       }
	       
	       /*
		* Start searching from next item
		*/

	       for (i=iItem; i < numitems && sfound==0; i++) {
		    search2 = GSgetTitle(GDgetEntry(gd, i));
		    if (strcasestr(search2, search1) != NULL ) {
			 iNewItem = i+1;
			 sfound = 1;
		    }
	       }
	       /* if it wasn't found after the current line start
		  from the beginning again
	       */

               for ( i= 0 ; i < iItem && sfound==0;i++)  {
                       search2 = GSgetTitle(GDgetEntry(gd,i));
                       if (strcasestr(search2,search1) != NULL ) {
                               iNewItem = i+1;
                               sfound = 1;
                       }
               }
	       if (sfound == 0) {
		    search1[0] = '\0';
		    Debugmsg("GetMenu beep search\r\n")
		    CURBeep(CursesScreen);
		    CursesErrorMsg(Gtxt("Search failed...",140));
	       }

	       if (ch != 'n')
		    iItem = scline(-1, iNewItem, gd);
               break;

	  
	  case '\0':
	       break;

	  default:
	       if (ch == KEY_LEFT  || ch == 'h' || ch == '\002')
		    ch = 'u';
	       if (ch == KEY_RIGHT || ch == 'l' || ch == '\006') 
		    ch = '\n';
	       *typedchar = ch;
	       GDsetCurrentItem(gd, iItem);
	       return(iItem);
	  }

	  iItem = scline(iItem, iNewItem, gd);
     
	  refresh();
     
     }

}
