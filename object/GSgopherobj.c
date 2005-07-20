/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.8 $
 * $Date: 2002/03/19 20:07:16 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/GSgopherobj.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: GSgopherobj.c
 * Implement gopher directory functions.
 *********************************************************************
 * Revision History:
 * $Log: GSgopherobj.c,v $
 * Revision 1.8  2002/03/19 20:07:16  jgoerzen
 * GSgopherobj.c: Moved Regex.h include up.
 *
 * Regex.c, Regex.h: continuing full rewrites.
 *
 * Revision 1.7  2002/01/08 17:36:14  jgoerzen
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
 * Revision 1.6  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.5  2001/01/17 21:16:35  jgoerzen
 * More psinrtf -> snprintf changes
 *
 * Revision 1.4  2000/12/26 22:48:12  s2mdalle
 *
 * Changed GSsendHeader(int, int) => GSsendHeader(int, long) because the
 * second argument is often the file size, and an int may not be
 * sufficient depending on platform.  Most calls use canned values of -1
 * and -2 anyway.
 *
 * Revision 1.3  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.2  2000/08/23 00:18:41  jgoerzen
 * Various bugfixes
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.85  1995/11/10  22:26:26  lindner
 * Fix for wierd relative gopher URLs
 *
 * Revision 3.84  1995/11/03  18:00:45  lindner
 * Coen: changes
 *
 * Revision 3.83  1995/11/03  05:37:13  lindner
 * Convert ------ to <HR>
 *
 * Revision 3.82  1995/10/04  22:30:45  lindner
 * Add <BR> to the end of info lines
 *
 * Revision 3.81  1995/09/28  22:59:25  lindner
 * Move nullword
 *
 * Revision 3.80  1995/09/25  22:07:18  lindner
 * Ansification
 *
 * Revision 3.79  1995/08/29  07:13:06  lindner
 * Add tickets to urls
 *
 * Revision 3.78  1995/06/12  15:33:41  lindner
 * Add onlyhtml support
 *
 * Revision 3.77  1995/05/02  04:03:51  lindner
 * Remove unused variable
 *
 * Revision 3.76  1995/04/15  07:07:19  lindner
 * Fix for fcn prototype, check for error
 *
 * Revision 3.75  1995/02/22  05:30:25  lindner
 * reintroduce gsinit to gsnew
 *
 * Revision 3.74  1995/02/17  18:29:37  lindner
 * Abstract display support
 *
 * Revision 3.73  1995/02/16  22:32:44  lindner
 * HTML icon support
 *
 * Revision 3.72  1995/02/07  07:12:55  lindner
 * oops!
 *
 * Revision 3.71  1995/02/07  07:06:58  lindner
 * performance fixes
 *
 * Revision 3.70  1995/02/06  22:13:52  lindner
 * Performance fixes
 *
 * Revision 3.69  1995/02/02  17:14:46  lindner
 * Fix for memory leaks and accesses
 *
 * Revision 3.68  1995/02/01  22:08:02  lindner
 * Put back GSaddmerge
 *
 * Revision 3.67  1995/02/01  21:44:41  lindner
 * Remove GSmerge fcns, Add message/rfc822 and application/gopher to GSisText
 *
 * Revision 3.66  1994/12/15  17:30:49  lindner
 * Allow multi-line abstracts in link files, Fix for ftp URL generation
 *
 * Revision 3.65  1994/12/05  22:39:55  lindner
 * Fix Name and Path settings in GSfromURL
 *
 * Revision 3.64  1994/11/17  06:33:58  lindner
 * Fixes for VMS internationalization
 *
 * Revision 3.63  1994/11/13  06:30:48  lindner
 * Better GSfromURL
 *
 * Revision 3.62  1994/10/24  22:15:53  lindner
 * Add PDF type
 *
 * Revision 3.61  1994/10/18  21:37:03  lindner
 * Make sure buf is set
 *
 * Revision 3.60  1994/10/13  05:27:18  lindner
 * Compiler complaint fixes
 *
 * Revision 3.59  1994/09/29  19:26:45  lindner
 * Fix for debug messages and NULL values
 *
 * Revision 3.58  1994/08/19  16:13:09  lindner
 * mktime() etc, fixes from Alan Coopersmith
 *
 * Revision 3.57  1994/08/03  20:03:44  lindner
 * Fix for adding ask stuff
 *
 * Revision 3.56  1994/07/31  04:57:20  lindner
 * Fix for .names messup...
 *
 * Revision 3.55  1994/07/21  22:29:18  lindner
 * misc hacks
 *
 * Revision 3.54  1994/07/06  19:21:34  lindner
 * use strftime to get localized date and time
 *
 * Revision 3.53  1994/06/29  06:51:21  lindner
 * ifdef GINTERNATIONAL, use strftime to localize the date string
 * returned by GSgetModDate()
 *
 * in GSplusfromNet(), if there's anything in the ADMIN block other than
 * Admin, ModDate, or TTL entries, save it for the client's showinfo()
 * Server Information
 *
 * Move GSfromURL() return code definitions to header so other files can
 * call it
 *
 * Fix GStoNetURL() & HTML display of directories
 *
 * Round sizes in GSaddView
 *
 * Make GSfromURL() lie and say all http: is HTML so the client
 * passes it off to a http-speaking program anyway (assuming
 * all HTML viewers speak http, that is)  (Coopersmith)
 *
 * Revision 3.52  1994/06/29  05:45:56  lindner
 * Mods to pump tickets to the net
 *
 * Revision 3.51  1994/05/25  20:57:46  lindner
 * Remove unused var
 *
 * Revision 3.50  1994/05/06  02:29:46  lindner
 * Fix from Robert Beckett for binhex files
 *
 * Revision 3.49  1994/04/25  03:36:58  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.48  1994/04/25  02:26:30  lindner
 * Fix for url problems
 *
 * Revision 3.47  1994/04/25  02:16:31  lindner
 * Fix for |= mistype
 *
 * Revision 3.46  1994/04/22  06:41:35  lindner
 * More pacbell hacks for Domain=
 *
 * Revision 3.45  1994/04/22  03:26:15  lindner
 * pacbell hack
 *
 * Revision 3.44  1994/04/19  14:32:29  lindner
 * Change GD and GSfromLink routines to use FIO
 *
 * Revision 3.43  1994/04/13  19:16:17  lindner
 * Fix for ASK block bug
 *
 * Revision 3.42  1994/04/08  20:05:52  lindner
 * gcc -Wall fixes
 *
 * Revision 3.41  1994/04/07  17:27:09  lindner
 * Fix for pyramids
 *
 * Revision 3.40  1994/04/01  04:38:06  lindner
 * Fix for conditional macros
 *
 * Revision 3.39  1994/03/31  22:48:55  lindner
 * fix for Hgopher and ask requests
 *
 * Revision 3.38  1994/03/31  21:05:11  lindner
 * More debug code, and fix for Socket return values
 *
 * Revision 3.37  1994/03/17  04:37:41  lindner
 * Fix for spinning clients
 *
 * Revision 3.36  1994/03/08  15:56:17  lindner
 * gcc -Wall fixes
 *
 * Revision 3.35  1994/03/04  17:59:16  lindner
 * More URL fixes from A.C.
 *
 * Revision 3.34  1994/02/20  16:27:44  lindner
 * Remove dead code for GDtoNetHTML
 *
 * Revision 3.33  1994/01/25  06:51:21  lindner
 * Many additions for URL/HTML support, removed insidious dot at end of host..
 *
 * Revision 3.32  1994/01/10  03:28:12  lindner
 * Allow bangs in Domain= lines, to negate classes of hosts
 *
 * Revision 3.31  1994/01/06  06:10:53  lindner
 * fix for cvs entries
 *
 *
 * Revision 3.30  1994/01/06  05:43:46 lindner
 * Fix for type=1 and ask blocks.
 *
 * Revision 3.29  1993/12/27  16:25:39 lindner
 * Add fix for appending a period to domain names
 *
 * Revision 3.28 1993/11/29  01:07:23 lindner
 * In GSfromLink(), disable handling of "Domain_pat:" for VMS to keep the
 * compiler from complaining about the lack of re_comp() and re_exec().
 * "Domain_pat:" handling isn't needed in the client anyway.  (Wolfe)
 *
 * Add "AddInfo" argument to GStoLink().  This option decides whether to
 * add the Admin and ModDate fields to the basic information about the
 * gopher object.  Bookmarks should not include them, but requested
 * technical information ('=' and '^') should.  This change requires
 * changes to gopher/gopher.c and object/GDgopherdir.c; see below.
 * (Macrides)
 *
 * Revision 3.27  1993/11/05  07:24:46 lindner
 * Allow Type=1? etc.. in .link files
 *
 * Revision 3.26  1993/11/04  04:50:50 lindner
 * Add quotes around HREFs
 *
 * Revision 3.25  1993/11/04  02:10:47 lindner
 * Added Domain_pat= line to check for a regexp domains
 *
 * Revision 3.24  1993/11/03  15:35:40 lindner
 * Fix problems with bookmarks of gopher+ items
 *
 * Revision 3.23  1993/11/02  06:15:24 lindner
 * HTML additions
 *
 * Revision 3.22  1993/10/26  17:49:50 lindner
 * Fix for NULL view in GSisText()
 *
 * Revision 3.21  1993/10/22  20:02:33 lindner
 * Add Movie (;) and Info (i) type support
 *
 * Revision 3.20  1993/09/18  04:44:41 lindner
 * Additions to fix caching of Multiple view items
 *
 * Revision 3.19  1993/09/11  06:41:08 lindner
 * Fix for picky compilers
 *
 * Revision 3.18  1993/09/11  06:32:52  lindner
 * URL support
 *
 * Revision 3.17  1993/09/01  21:51:59  lindner
 * Remove GSnewSet, better initialization in GSnew()
 *
 * Revision 3.16  1993/08/19  20:24:11  lindner
 * Mitra's Debug patch
 *
 * Revision 3.15  1993/07/29  20:02:55  lindner
 * Removed dead variables
 *
 * Revision 3.14  1993/07/27  20:17:25  lindner
 * Fix improper bracketed debug output
 *
 * Revision 3.13  1993/07/27  05:30:23  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.12  1993/07/27  00:30:09  lindner
 * plus patch from Mitra
 *
 * Revision 3.11  1993/07/23  04:50:17  lindner
 * Mods to allow abstract/admin setting in .names files
 *
 * Revision 3.10  1993/07/21  03:31:09  lindner
 * Askdata can be stored locally, plus GSfromLink doesn't core dump with 
 * mangled items
 *
 * Revision 3.9  1993/07/14  20:37:11  lindner
 * Negative numbering patches
 *
 * Revision 3.8  1993/07/07  19:30:12  lindner
 * Split off socket based fcns to Sockets.c
 *
 * Revision 3.7  1993/07/06  20:22:40  lindner
 * Added listener and accept fcns
 *
 * Revision 3.6  1993/06/22  06:07:14  lindner
 * Added Domain= hacks..
 *
 * Revision 3.5  1993/04/15  17:44:52  lindner
 * Fixed link processing, mods from Mitra
 *
 * Revision 3.4  1993/03/26  19:50:46  lindner
 * Mitra fixes for better/clearer fromNet code
 *
 * Revision 3.3  1993/03/24  17:05:33  lindner
 * Additions for Localfile for each GopherObj
 *
 * Revision 3.2  1993/03/18  22:15:50  lindner
 * filtering, memory leaks fixed, GSmerge problems
 *
 *********************************************************************/

#include "GSgopherobj.h"

#if defined(mips) && defined(ultrix)   /*** Gross hack, yuck! ***/
#define _SIZE_T
#endif

#include "String.h"
#include "STRstring.h"
#include "Regex.h"
#include <stdio.h>
#include "compatible.h"
#include <errno.h>
#include "Malloc.h"
#include "Sockets.h"
#include "util.h"
#include "Debug.h"
#include "fileio.h"
#include <time.h>

#ifdef pyr
unsigned long errno;
#endif

/*
 * Make a new gopherobj...  Should reuse destroyed GopherObjs...
 */

GopherObj *
GSnew()
{
     GopherObj *temp;

     temp = (GopherObj *) malloc(sizeof(GopherObj));

     temp->Selstr    = STRnew();
     temp->Title     = STRnew();
     temp->Host      = STRnew();
     temp->Localfile = STRnew();
     temp->Localview = STRnew();
     temp->gplus     = NULL;
     temp->isask     = FALSE;
     temp->url       = NULL;

     GSinit(temp);

     return(temp);
}


/*
 * Initialize the gopherplus components of the object
 * (Only called for a gplus item)
 */

void
GSplusnew(GopherObj *gs)
{
     if (gs->gplus == NULL) {
	  gs->gplus = (GplusObj *) malloc(sizeof(GplusObj));
     }
     
     gs->gplus->Admin    = STRnew();
     gs->gplus->ModDate  = STRnew();

     gs->gplus->Views    = VIAnew(10);
     gs->gplus->OtherBlocks = BLAnew(5);
     gs->gplus->Askdata  = NULL;
}


/*** Destroy gopher object ***/

void
GSdestroy(GopherObj *gs)
{
     STRdestroy(gs->Selstr);
     STRdestroy(gs->Title);
     STRdestroy(gs->Host);
     if (GSgetLocalFile(gs) != NULL)
	  unlink(GSgetLocalFile(gs));
     STRdestroy(gs->Localfile);
     STRdestroy(gs->Localview);

     GSplusdestroy(gs);
     if (gs->url != NULL)
	  URLdestroy(gs->url);
     free(gs);
}
     

/*** Destroy Gopher+ attributes ***/

void
GSplusdestroy(GopherObj *gs)
{
     if (gs->gplus != NULL) {
	  STRdestroy(gs->gplus->Admin);
	  STRdestroy(gs->gplus->ModDate);
	  VIAdestroy(gs->gplus->Views);
	  BLAdestroy(gs->gplus->OtherBlocks);

	  (void)GSsetAskdata(gs, NULL);

	  free(gs->gplus);
	  gs->gplus = NULL;
     }
}


/*
 * Clear out all the crud 
 */

void
GSinit(GopherObj *gs)
{
     GSsetType(gs, '\0');
     
     STRinit(gs->Title);
     STRinit(gs->Selstr);
     STRinit(gs->Host);
     if (GSgetLocalFile(gs) != NULL)
	  unlink(GSgetLocalFile(gs));
     STRinit(gs->Localfile);
     STRinit(gs->Localview);

     gs->ttl = -1;
     gs->iPort = 0;
     GSsetNum(gs, 0);
     GSsetWeight(gs, 0);

     GSsetGplus(gs,FALSE);  /** Default is no gplus **/
     GSsetAsk(gs, FALSE);

     if (gs->url != NULL) 
	  URLdestroy(gs->url);
     gs->url = NULL;
     GSplusInit(gs);
}


/*
 * Clear out gopher+ crud if it exists
 */

void
GSplusInit(GopherObj *gs)
{
     if (gs->gplus != NULL) {
	  STRinit(gs->gplus->Admin);
	  STRinit(gs->gplus->ModDate);
	  VIAinit(gs->gplus->Views);
	  STAinit(gs->gplus->OtherBlocks);

     }
}


/* 
 * Set a URL for the gopherobject..
 */

void
GSsetURL(gs, url)
  GopherObj *gs;
  char      *url;
{
     if (gs->url == NULL)
	  gs->url = URLnew();
     
     URLset(gs->url, url);
}

int
GSgetNumBlocks(GopherObj *gs)
{
     if (gs->gplus == NULL)
	  return(-1);

     return(BLAgetTop(gs->gplus->OtherBlocks));
}

Blockobj*
GSgetBlock(GopherObj *gs, int bnum)
{
     if (gs->gplus == NULL || bnum < 0)
	  return(NULL);

     return(BLAgetEntry(gs->gplus->OtherBlocks, bnum));
}

/*
 * Return the number of views if item is gopher+
 */

int
GSgetNumViews(GopherObj *gs)
{
     if (gs->gplus == NULL)
	  return(0);
     
     return(VIAgetTop(gs->gplus->Views));
}

/*
 * Find out what a specific view is
 */

VIewobj *
GSgetView(GopherObj *gs, int viewnum)
{
     if (gs->gplus == NULL)
	  return(NULL);
     
     return(VIAgetEntry(gs->gplus->Views, viewnum));
}


char *
GSgetURL(GopherObj *gs, char *ticket)
{
     if (gs->url == NULL) {
	  gs->url = URLnew();
	  URLfromGS(gs->url, gs, ticket);
     }
     
     return(URLget(gs->url));
}


char *
GSgetURLhtml(GopherObj *gs, char *ticket)
{
     char *cp = GSgetURL(gs, ticket);
     char *cp2;
     int   views;

     if ((cp != NULL) && (strncmp(cp, "gopher://", 9) == 0)) {
	  if ( GSgplusInited(gs) ) {
	       for (views=0; views< GSgetNumViews(gs); views++) {
		    if (!(strncasecmp(VIgetType(GSgetView(gs,views)),
				      "text/html", 9))) {

			 cp = strdup(cp);

			 /** find the type character **/
			 cp2 = strchr(cp+10, '/');
			 if (cp2 != NULL) {
			      *(++cp2) = 'h';
			      cp2++;
			      if (strncmp(cp2, "%2a",3)==0) {
				   /** validated item, skip forward... **/
				   cp2 = strstr(cp2, "%20");
				   if (cp2 != NULL) {
					cp2 +=3;
					cp2 = strstr(cp2, "%20");
					if (cp2 != NULL) {
					     cp2+=3;
					     *cp2 = 'h';
					}
				   }
			       } else
				    *cp2 = 'h';
     			 }
		     }
	       }
	  } 
	  if (strstr(cp, "/1validate%201") != NULL) {
	       cp = strdup(cp);
	       cp2 = strchr(cp+10, '/');
	       if (cp2 != NULL) {
		    *(++cp2) = 'h';
		    *(++cp2) = 'h';
		    *(cp2+11) = 'h';
	       }
	  }
	  else if (strstr(cp, "/hhalidate%201") != NULL) {
	       cp = strdup(cp);
	       cp2 = strchr(cp+10, '/');
	       *(cp2+13) = 'h';
	  } else if (*(GSgetPath(gs)) == '*') {
	       /** This is a validated item.. **/
	       ;
	  }
     }

     return cp;

}

static char *
GStoNetURL(GopherObj *gs, char *url,  char *ticket)
{
     char *path, *ftphost, *ftppath;

     *url = '\0';
     path  = GSgetPath(gs);

     /** This is a server specific hack for now..  **/
     if (path == NULL)
	  return(NULL);

     if ((path != NULL) && (strncmp(path, "ftp://", 6) == 0)) {
	  strcpy(url, path);
	  return(url);
     }
     else if ((path != NULL) && (strncmp(path, "ftp:", 4) == 0) ) {
	  ftphost = path+4;
	  ftppath = strchr(ftphost, '@');

	  if (ftppath != NULL)
	  {
	       sprintf(url, "ftp://%.*s/", (int) (ftppath - ftphost), ftphost);
	       
	       ftppath++;

	       /*** The rest is the file/path ***/
	       if (*ftppath == '/')
		    ftppath++;

	       strcat(url, ftppath);
	       return(url);
	  }
     }
     strcpy(url,GSgetURLhtml(gs,ticket));
     return(url);
}


Blockobj*
GSfindBlock(GopherObj *gs, char *blockname)
{
     int x;
     
     x = BLAsearch(GSgetOtherBlocks(gs), blockname);

     Debug("GSfind num is %d\n", x);
     if (x < 0)
	  return(NULL);
     else
	  return(GSgetBlock(gs, x));
}

void
GSsetBlock(GopherObj *gs, char *blockname, char *text, boolean appendIfPoss)
{
     Blockobj *bl = NULL;
     boolean newbl = FALSE;

     Debug("GSsetBlock:%s;",blockname);
     Debug("%s\r\n", text);

     if (appendIfPoss)
          bl = GSfindBlock(gs, blockname);

     if (bl == NULL) {
          newbl = TRUE;
          bl = BLnew();
     }

     BLsetName(bl, blockname);
     BLaddText(bl, text);
     
#ifdef DEBUGGING
     if (DEBUG)
	  BLtoNet(bl, fileno(stderr), TRUE);
#endif
     if (gs->gplus == NULL)
	  GSplusnew(gs);
     
     if (newbl) {
          BLApush(gs->gplus->OtherBlocks, bl);
          BLdestroy(bl);
     }

}

/*
 * Set the Askdata, destroy if necessary..
 */

char **
GSsetAskdata(GopherObj *gs, char **askdata)
{
     if (!GSgplusInited(gs))
	  return(NULL);

     /** Destroy data if necessary **/
     if (gs->gplus->Askdata != NULL) {
	  int i=0;
	  char **Askmoo = gs->gplus->Askdata;
	  
	  while (Askmoo[i] != NULL) {
	       free(Askmoo[i++]);
	  }
	  free(Askmoo);
     }

     gs->gplus->Askdata = askdata;

     return(askdata);

}


/*
 * Set the administrator line as defined in Gopher+ protocol
 * Name <email>
 */


/* Converts the gopher+ moddate from <YYYYMMDDHHMMSS> to a struct tm ptr */
struct tm *
GSgetModDateTM(GopherObj *gs)
{
     static struct tm time;
     char *cp;
#ifndef NO_MKTIME
     time_t converted;
#endif

     if ( (gs->gplus == NULL) || (STRget(gs->gplus->ModDate) == NULL) )
          return NULL;

     cp = strchr(STRget(gs->gplus->ModDate), '<');
     if (cp == NULL)
          return NULL;

#define ASCII_TO_INT(a)     (a - '0')
     /* Should really do some sanity checking on the input here */
     time.tm_year = ((ASCII_TO_INT(cp[1]) * 1000) + (ASCII_TO_INT(cp[2]) * 100)
                   + (ASCII_TO_INT(cp[3]) * 10) + ASCII_TO_INT(cp[4]) ) - 1900;
     time.tm_mon  = (ASCII_TO_INT(cp[5])  * 10) + ASCII_TO_INT(cp[6]) - 1;
     time.tm_mday = (ASCII_TO_INT(cp[7])  * 10) + ASCII_TO_INT(cp[8]);
     time.tm_hour = (ASCII_TO_INT(cp[9])  * 10) + ASCII_TO_INT(cp[10]);
     time.tm_min  = (ASCII_TO_INT(cp[11]) * 10) + ASCII_TO_INT(cp[12]);
     time.tm_sec  = (ASCII_TO_INT(cp[13]) * 10) + ASCII_TO_INT(cp[14]);
     time.tm_isdst = -1;      /* make the system figure it out */

#ifndef NO_MKTIME
     /* if mktime() is present, let it do sanity checking and day of
	  week setting for us */
     converted = mktime(&time);
     if (converted != -1)
	  return (localtime(&converted));
#endif
     return (&time);

}


#if defined(VMS) || (defined(GINTERNATIONAL) && !defined(NO_STRFTIME))
/* In internationalized environments, convert the ModDate string to the
 *  native language when it's requested.
 * If i18n is not on, the macro from GSgopherobj.h is used.
 *  (The i18n definition's are turned on or off in Locale.h)
 */
char *
GSgetModDate(GopherObj *gs)
{
     char	    dateBuf[256];
     char	    *datetime;
     struct tm	    *modTime;

     if ( (gs->gplus == NULL) || (STRget(gs->gplus->ModDate) == NULL) )
          return NULL;

     modTime = GSgetModDateTM(gs);

     if ( (modTime != NULL) &&
	  strftime(dateBuf, sizeof(dateBuf) - 16, "%c ", modTime) )
     {
          datetime = strchr(STRget(gs->gplus->ModDate), '<');
          if (datetime != NULL)
               strncat(dateBuf, datetime, 16);

          STRset(gs->gplus->ModDate, dateBuf);
     }

     return STRget(gs->gplus->ModDate);
}
#endif /* GINTERNATIONAL */



void
GSsetAdmin(GopherObj *gs, char *admin)
{
     if (gs->gplus == NULL)
	  GSplusnew(gs);

     STRset(gs->gplus->Admin, admin);
}     



void
GSsetModDate(GopherObj *gs, char *str)
{
     if (gs->gplus == NULL) GSplusnew(gs);
     STRset(gs->gplus->ModDate, str);
}     



/** Add a view for a specific gopherobj **/

void
GSaddView(GopherObj *gs, char *view, char *language, int estsize)
{
     char tmpstr[64];
     VIewobj *temp;

     if (estsize > 960)
	  snprintf(tmpstr, sizeof(tmpstr), "%dk", (estsize + 512)/1024);
     else
      	  snprintf(tmpstr, sizeof(tmpstr), ".%dk", ((estsize+64)*10)/1024);

     temp = VInew();

     VIsetType(temp, view);
     VIsetLang(temp, language);
     VIsetSize(temp, tmpstr);

     VIApush(gs->gplus->Views, temp);
     VIdestroy(temp);
}

void
GSdelTopView(GopherObj *gs)
{
#define VIApop(a) (DApop((DynArray*)(a)))
     VIApop(gs->gplus->Views);
}

void
GSaddBlock(GopherObj *gs, char *Blockname, char *file)
{
     Blockobj *bl;

     bl = BLnew();
     BLsetName(bl, Blockname);
     BLsetFile(bl, file);
     ;
     if (gs->gplus == NULL)
	  GSplusnew(gs);

     BLApush(gs->gplus->OtherBlocks, bl);

     BLdestroy(bl);
}



/*
 * Send a gopher reference into an fd
 */

static char *nullword = "(NULL)";

void
GStoNet(GopherObj *gs, int sockfd, GSformat fmt, char *ticket)
{
     char buf[1024];
     int i;
     Blockobj *bl;

     if (ticket == NULL)
	  ticket = "";

     buf[0] = GSgetType(gs);
     
     if (buf[0] == '\0')	/* For example a .names with no Type */
	buf[0] = '3'; 

     if (fmt == GSFORM_G0) {
	  for (i=0; i< GSgetNumBlocks(gs); i++) {
	       bl = GSgetBlock(gs, i);
	       if (strcasecmp(BLgetName(bl), "ONLYHTML")==0)
		    return;
	  }
	  snprintf(buf + 1, 
		   sizeof(buf) - 1,
		   "%s\t%s%s\t%s\t%d",
		   GSgetTitle(gs) ? GSgetTitle(gs) : nullword,
		   ticket         ? ticket : nullword,
		   GSgetPath(gs)  ? GSgetPath(gs) : nullword,
		   GSgetHost(gs)  ? GSgetHost(gs) : nullword,
		   GSgetPort(gs));
	  
	  if (GSisAsk(gs))
	       strcat(buf, "\t?\r\n");
	  else if (GSisGplus(gs))
	       strcat(buf, "\t+\r\n");
	  else
	       strcat(buf, "\r\n");
	  
	  writestring(sockfd, buf);
	  Debug("GStoNet:%s", buf);
     } 
     else if (fmt == GSFORM_GPLUS) {
	  ;
     }
     else if (fmt == GSFORM_HTML) {
	  char url[256];
	  for (i=0; i < GSgetNumBlocks(gs); i++) {
	       bl = GSgetBlock(gs, i);
	       if (strcmp(BLgetName(bl), "NOHTML") == 0)
		    return;
	  }

	  if (GSgetType(gs) == A_INFO) {
	       writestring(sockfd, "<DT>");
	       if (strncmp(GSgetTitle(gs), "----------------",16)==0) {
		    writestring(sockfd, "<HR>");
	       } else
		    writestring(sockfd, GSgetTitle(gs));
	       writestring(sockfd, "<BR>");
	       return;
	  }
	  GStoNetURL(gs, url, ticket);
	  if (url[0] == '\0')
	       return;
	  snprintf(buf, sizeof(buf),
		   "<A HREF=\"%s\">%s</A>\r\n", url, GSgetTitle(gs));
	  writestring(sockfd, buf);

	  if (GSgplusInited(gs)) {
	       Blockobj *bl;

	       bl = GSfindBlock(gs, "ABSTRACT");
	       if (bl != NULL) {
		    writestring(sockfd, "<DD>");
		    BLtoNet(bl, sockfd, FALSE);
		    writestring(sockfd, "\r\n");
	       }
	  }

	  if (GSgetWeight(gs) != 0) {
	       snprintf(buf, sizeof(buf),
			"<DD>Score: %d\r\n", GSgetWeight(gs));
	       writestring(sockfd, buf);
	  }
     }
	  
}


/*
 * Send a long gopher data descriptor
 *
 * filter is a character array of blocks to send
 */

void
GSplustoNet(GopherObj *gs, int sockfd, char **filter, char *ticket)
{
     int      i;
     char     tmpstr[256];
     boolean  sendviews, sendadmin, sendothers;

     for (i=0; i < GSgetNumBlocks(gs); i++) {
	  if (strcmp(BLgetName(GSgetBlock(gs, i)), "ONLYHTML") == 0)
	       return;
     }

     if (filter == NULL)
	  sendviews = sendadmin = sendothers = TRUE;
     else {
	  sendviews = sendadmin = sendothers = FALSE;
	  for (i=0; filter[i] != NULL ;i++) {
	       if (strcasecmp(filter[i], "VIEWS")==0)
		    sendviews = TRUE;
	       else if (strcasecmp(filter[i], "ADMIN")==0)
		    sendadmin = TRUE;
	       else 
		    sendothers = TRUE;
	  }
     }    
     
     
     /** Send out the old style INFO stuff **/
     writestring(sockfd, "+INFO: ");
     GStoNet(gs,sockfd, GSFORM_G0, ticket);


     /** Only write out "interesting" URLs **/

     if (GSgetURL(gs,ticket) != NULL) {
	  if (strncmp(GSgetURL(gs,ticket), "gopher:", 7) != 0) {
	       writestring(sockfd, "+URL:\r\n ");
	       writestring(sockfd, GSgetURL(gs,ticket));
	       writestring(sockfd, "\r\n");
	  }
     }


     if (GSgplusInited(gs)) {
	  /*** Should special case for local filename.... ***/
	  
	  if (GSgetAdmin(gs) != NULL && GSgetModDate(gs) != NULL && sendadmin){
	       writestring(sockfd, "+ADMIN:\r\n Admin: ");
	       writestring(sockfd, GSgetAdmin(gs));
	       writestring(sockfd, "\r\n Mod-Date: ");
	       writestring(sockfd, GSgetModDate(gs));
	       if (GSgetTTL(gs) > -1) {
		    writestring(sockfd, "\r\n TTL: ");
		    snprintf(tmpstr, sizeof(tmpstr), "%d", GSgetTTL(gs));
		    writestring(sockfd, tmpstr);
	       }
	       writestring(sockfd, "\r\n");
	  }
	  if (GSgetNumViews(gs) > 0 && sendviews) {
	       writestring(sockfd, "+VIEWS:\r\n");
	       for (i=0 ; i< GSgetNumViews(gs); i++) {
		    VItoLine(GSgetView(gs, i), tmpstr);
		    writestring(sockfd, tmpstr);
		    writestring(sockfd, "\r\n");
	       }
	  }
	  
	  if (sendothers) {

	       for (i=0; i< GSgetNumBlocks(gs); i++)
		    BLtoNet(GSgetBlock(gs, i),sockfd, TRUE);
	  }
     }
}


/*** GSplusfromnet() assumes that the leading +INFO text has been read from
     the file descriptor.

     It returns 1  if there's more data (and another INFO block)
                0  if there's EOF
		SOFTERROR caller can keep reading
		HARDERROR caller should stop reading
 ***/

int
GSplusfromNet(GopherObj *gs, int fd)
{
     int result,readok,i;
     boolean nextinfo = FALSE;
     char plusfield;
     Blockobj *bl;
     char inputline[512];
     
     if (gs->gplus == NULL)
	  GSplusnew(gs);

     bl = BLnew();

     /** get the gopher-data descriptor **/
     readok = GSfromNet(gs, fd);

     if (readok == HARDERROR)
	  return(HARDERROR);

     /** If readok is softerror we still need to look for blocks to throw
         away any data before next item **/

     /** Now start looking for blocks.  Process blocks if we know how.  **/
     /** State: _GotGREF_ **/

     if ((result = readrecvbuf(fd, &plusfield, 1))<0)
	  return(HARDERROR);

     while (!nextinfo) {
	  if (result == 0) { 	       /*** We're done ***/
	       BLdestroy(bl);

	       if (readok == SOFTERROR)
		    return(SOFTERROR);
	       else
		    return(FOUNDEOF);
	  }
	  switch (plusfield) {

	  case '.':
	       readline(fd, inputline, sizeof(inputline));
	       result = FOUNDEOF;
	       break;

	  case '+':
	       /*** State _NewBlock_ ***/

	       if (readtoken(fd,inputline, sizeof(inputline), ':') <= 0) 
		    return(HARDERROR);

	       if (strcasecmp(inputline, "INFO")==0) {
		    /** Get rid of the space. **/
		    if (readrecvbuf(fd, &plusfield,1) <= 0)
			 return(HARDERROR);
		    BLdestroy(bl);
		    if (readok == SOFTERROR)
			 return(SOFTERROR);
		    else
			 return(MORECOMING);
	       }

	       /** Specialized fromNets here **/

	       BLinit(bl);
	       if ((result=BLfromNet(bl, fd, inputline)) <0) {
		    /** Bad read **/
		    BLdestroy(bl);
		    return(HARDERROR);
	       }
	       else if (result == FOUNDEOF)   /** EOF, block read ok **/
		    nextinfo = TRUE;
	       else 
		    nextinfo = FALSE;


	       /*** See what the block is. Transform it if necessary ***/
	       if (strcasecmp(BLgetName(bl), "VIEWS")==0) {
		    VIAfromBL(gs->gplus->Views, bl);
	       } else if (strcasecmp(BLgetName(bl), "ADMIN")==0) {
		    int	 saveAdminBlock = 0;

		    for (i=0; i<BLgetNumLines(bl); i++) {
			 char *cp;
			 
			 cp = BLgetLine(bl, i);
			 
			 if (strncasecmp(cp, "Admin: ",7)==0)
			      GSsetAdmin(gs, cp+7);
			 else if (strncasecmp(cp, "Mod-Date: ", 10)==0)
			      GSsetModDate(gs, cp+10);
			 else if (strncasecmp(cp, "TTL: ", 5) == 0)
			      GSsetTTL(gs, atoi(cp+5));
			 else
			      saveAdminBlock = 1;
		    }
		    if (saveAdminBlock)
			 BLApush(gs->gplus->OtherBlocks, bl);
	       
	       } else if (strcasecmp(BLgetName(bl), "URL")==0) {
		    char *cp;

		    cp = BLgetLine(bl, 0);
		    if (cp != NULL)
			 GSsetURL(gs, cp);
	       }
	       else
		    BLApush(gs->gplus->OtherBlocks, bl);

	       break;  /** '+' **/

	  default: /*** Hmmm plusfield wasn't a plus or '.' **/
	       return(HARDERROR);

	  } /** switch plusfield **/

     }   /** While **/

     BLdestroy(bl);

     return(FOUNDEOF);
}



/* GSfromNet - no comments on the original, so this is my (Mitra's) guess
   GSfromNet reads a gopher style line, it is called from:
   GDfromNet - in which case the gopher line is the whole item or;
   GSplusfromNet - in which case it is the part after the +INFO.
   It should return after reading the whole line, however in gopher+1.2b2
   this is not always the case, especially if it sees a Type=3
   returns:
       1      blank line (I think?)
       0      ok
      -1 HARDERROR    readfield etc error - give up
      -2 SOFTERROR    unrecognized or unhandleable type - skip on to next one
*/




extern int readfield();
extern int readline();

#if 0
static void
GSprocessObject(GopherObj *gs)
{
 	if (DEBUG)
		return;
#ifdef GOPHERTOFTP_HOST
    if (strncmp(GSgetPath(gs),"ftp:",4) == 0) {
	GSsetPort(gs,GOPHERTOFTP_PORT);
 	GSsetHost(gs,GOPHERTOFTP_HOST);
     }
#endif
}
#endif /* 0 */



int
GSfromNet(GopherObj *gs, int sockfd)
{
     char foo[1024], *cp;
     
     if (readtoken(sockfd, foo, 1024, '\t')<= 0) {
	  /* EOF or error */
	  return(HARDERROR);
     }

     GSsetType(gs, foo[0]);

     /** Get the kind of file from the first character **/
     /** Filter out files that we can't deal with **/

     switch (GSgetType(gs)) {
       case A_PDF:
       case A_FILE:
       case A_DIRECTORY:
       case A_MACHEX:
       case A_PCBIN:
       case A_CSO:
       case A_INDEX:
       case A_TELNET:
       case A_SOUND:
       case A_UNIXBIN:
       case A_GIF:
       case A_HTML:
       case A_TN3270:
       case A_MIME:
       case A_IMAGE:
       case A_INFO:
       case A_MOVIE:
       case A_APP:
	  break;
     case A_ERROR:
	  GSsetPath(gs, "");
	  GSsetHost(gs, "");
	  GSsetGplus(gs, FALSE);
	  ZapCRLF(foo+1);
	  cp = foo + strlen(foo+1);
	  while (*cp == '.' && (*(cp-1) == '\n' || *(cp-1) == '\r')) {
	       *cp = '\0';
	       ZapCRLF(foo+1);
	  }
	  break;
     case A_EOI:
	  if (foo[1] == '\r' && foo[2] == '\n')
	       return(1);
     default:
	  /** Can't handle this type **/
	  readline(sockfd, foo, 1024);/** Cleanup **/
	  return(SOFTERROR);
     }

     /** Suck off the User Displayable name **/
     cp = foo+1;
     while (*cp == '\n' || *cp == '\r')
	    cp++;
     GSsetTitle(gs, cp);
     
     /** Suck off the Pathname **/
     if (readtoken(sockfd, foo, 1024, '\t') <= 0)
	  return(GSgetType(gs)==A_ERROR?0:HARDERROR);
     GSsetPath(gs, foo);

     /** Suck off the hostname **/
     if (readtoken(sockfd, foo, 1024, '\t') <= 0)
	  return(GSgetType(gs)==A_ERROR?0:HARDERROR);

     GSsetHost(gs, foo);

     if (readline(sockfd, foo, 1024)<=0)
	  return(GSgetType(gs)==A_ERROR?0:HARDERROR);

     GSsetPort(gs, 0);

     /** Get the port number **/
     if ((cp = strchr(foo, '\t')) != NULL) {
	  *cp = '\0';
	  switch (*(cp+1)) {
	  case '?':
	       GSsetAsk(gs, TRUE);
	  case '+':
	       GSsetGplus(gs, TRUE);
	       break;
	  }
     }

     GSsetPort(gs, atoi(foo));
     DebugGSplusPrint(gs,"GSfromNet end:");

     return(0);
}


/** Copy a GopherObj ***/

void
GScpy(GopherObj *dest, GopherObj *orig)
{
     dest->sFileType = orig->sFileType;
     dest->iPort     = orig->iPort;
     dest->Itemnum   = orig->Itemnum;

     GSsetTitle(dest, GSgetTitle(orig));
     GSsetPath(dest, GSgetPath(orig));
     GSsetHost(dest, GSgetHost(orig));
     
     GSsetGplus(dest, GSisGplus(orig));
     GSsetAsk(dest, GSisAsk(orig));
     if (orig->url != NULL)
	  GSsetURL(dest, GSgetURL(orig, ""));
     GSsetTTL(dest, GSgetTTL(orig));

     STRinit(dest->Localfile);
     STRinit(dest->Localview);
     GSpluscpy(dest, orig);
     
}

void
GSpluscpy(GopherObj *dest, GopherObj *orig)
{
     if (GSgplusInited(orig)) {
	  if (!GSgplusInited(dest))
	       GSplusnew(dest);
	  GSsetAdmin(dest, GSgetAdmin(orig));
	  GSsetModDate(dest, GSgetModDate(orig));
	  VIAcpy(dest->gplus->Views, orig->gplus->Views);
	  BLAcpy(dest->gplus->OtherBlocks, orig->gplus->OtherBlocks);
	  (void)GSsetAskdata(dest, GSgetAskdata(orig));
     }
}

/** GSmerge combines 2 gopher objects overwriting fields defined in overlay **/
/* Called by GDaddGSmerge to merge gs into directory, that is only called 
   by function to do this from a link */
void
GSmerge(GopherObj *gs, GopherObj *overlay)
{
     char *tempstr;
     char oldFileType;
     
     DebugGSplusPrint(gs,"GSmerge: Original");
     DebugGSplusPrint(overlay,"GSmerge: Overlay");
     
     if (GSgetHost(overlay) != NULL) {
	  oldFileType = GSgetType(gs);

	  if (GSgetType(overlay) != '\0') {
	       /* Just setting Type wont work, since they interelated with Path
	        * so set first char of path as well */
	       GSsetType(gs, GSgetType(overlay));
	       tempstr = GSgetPath(gs);
	       
	       if (*tempstr != A_DIRECTORY) {
		    tempstr[0] = GSgetType(overlay);
		    GSsetPath(gs, tempstr);
	       }
	       if (GSisAsk(overlay) && GSgetType(overlay) == A_DIRECTORY)  
		    GSaddView(gs, "application/gopher+-menu", "En_US", 0); 
	       /* lang==GDCgetLang(Config), 0 == dummy size here */
	       /* need this patch for proper protocol & Hgopher, 
		  which ignores gopher0 type in favor of +VIEW */
	  }

	  if (GSgetTitle(overlay) != NULL)
	       GSsetTitle(gs, GSgetTitle(overlay));
	  /* Don't set path - that is the key to the merge, and in the overlay
	     most probably has the first char set to ' '  ????*/
	  if (GSgetHost(overlay) != NULL)
	       GSsetHost(gs, GSgetHost(overlay));
	  if (GSgetPort(overlay) != 0)
	       GSsetPort(gs, GSgetPort(overlay));
	  if (GSgetNum(overlay) != -1)
	       GSsetNum(gs, GSgetNum(overlay));
	  if (GSgetWeight(overlay) != 0)
	       GSsetWeight(gs, GSgetWeight(overlay));

	  if (GSgplusInited(overlay)) {
	       if (!GSgplusInited(gs))
		    GSplusnew(gs);
	       BLAcpy(gs->gplus->OtherBlocks,
		      overlay->gplus->OtherBlocks);
	  }

	  if (GSgetAdmin(overlay) != NULL)
               GSsetAdmin(gs, GSgetAdmin(overlay));


 	  if (GSgetModDate(overlay) != NULL)
	       GSsetModDate(gs, GSgetModDate(overlay));
 	  if (GSgetNumViews(gs) && oldFileType == A_FILE
	      && GSgetType(gs) != A_FILE) {
	       char *vs, vsize[64], *vc, vcomments[256];
	       VIewobj *temp;
	       if ((vs = VIgetSize(GSgetView(gs, 0))))
		    strcpy(vsize, vs);
	       if ((vc = VIgetComments(GSgetView(gs, 0))))
		    strcpy(vcomments, vc);
	       VIAdestroy(gs->gplus->Views);
	       gs->gplus->Views    = VIAnew(10);
	       temp = VInew();
	       VIsetType(temp, "");
	       VIsetLang(temp, "");
	       VIsetSize(temp, vsize);
	       VIsetComments(temp, vcomments);
	       VIApush(gs->gplus->Views, temp);
	       VIdestroy(temp);
 	  }
 	  if (GSgetNumViews(overlay))
	       BLAcpy(gs->gplus->Views, overlay->gplus->Views);
     }
     
     DebugGSplusPrint(gs,"GSmerge: Result");
}


/** Compare two GopherObjs ***/

#if 0 /* unused */
static int
GScmp(GopherObj *gs1, GopherObj *gs2)
{
     if (GSgetTitle(gs1) == NULL)
	  return(1);
     if (GSgetTitle(gs2) == NULL)
	  return(-1);

     return(strcmp(GSgetTitle(gs1), GSgetTitle(gs2)));
}
#endif /* 0 */

/*********** The following functions implement the gopher/gopher+
  protocol, mostly
  GSconnect(), then GStransmit(), GSsendHeader() GSrecvHeader();
************/



/* GSconnect performs a connection to socket 'service' on host
 * 'host'.  Host can be a hostname or ip-address.  If 'host' is null, the
 * local host is assumed.   The parameter full_hostname will, on return,
 * contain the expanded hostname (if possible).  Note that full_hostname is a
 * pointer to a char *, and is allocated by connect_to_gopher()
 *
 * returns Errors : ErrSocket* defined in Sockets.h or socket
 *
 */

int
GSconnect(GopherObj *gs)
{
     int sockfd; 

     Debug("GSconnect: Host=%s",GSgetHost(gs));
     Debug("Port=%d\r\n",GSgetPort(gs));
     
     sockfd = SOCKconnect(GSgetHost(gs), GSgetPort(gs));
     
     return(sockfd);
}


/*
 * GStransmit sends the request from the client to the server
 *
 * All parameters are optional except for gs and sockfd.
 * 
 * the rest pertain to gopher+ transmission.
 */

void 
GStransmit(GopherObj *gs, int sockfd, char *search, char *command, char *view)
{
     char *cp;
     char **ask = GSgetAskdata(gs);
     int i;

     writestring(sockfd, GSgetPath(gs));
     if (search != NULL) {
	  writestring(sockfd, "\t");
	  writestring(sockfd, search);
     }

     /** Only send if gplus **/
     if  (!GSisGplus(gs)) {
	  writestring(sockfd, "\r\n");
	  return;
     }

     if (command != NULL) {
	  writestring(sockfd, "\t");
	  writestring(sockfd, command);
     }	  

     if (view != NULL) {
	  writestring(sockfd, view);
     }
     if (ask == NULL) 
	  writestring(sockfd, "\r\n");
     else {
	  writestring(sockfd, "\t1\r\n");

	  GSsendHeader(sockfd, -1);
	  
	  for (i=0; ;i++) {
	       cp = ask[i];

	       if (cp == NULL)
		    break;

	       writestring(sockfd, cp);
	       writestring(sockfd, "\r\n");
	  }
	  writestring(sockfd, ".\r\n");
     }
}



/*
 * GSsendHeader generates an appropriate header on sockfd
 *
 */
void
GSsendHeader(int sockfd, long size)
{
     char sizestr[64];

     snprintf(sizestr, sizeof(sizestr), "+%ld\r\n", size);
     writestring(sockfd, sizestr);
}


/** GSsendErrorHeader sends an error message header/message to the client **/
void
GSsendErrorHeader(GopherObj *gs, int sockfd, int errortype, char *errormsg)
{
     char tmpstr[512];

     snprintf(tmpstr, sizeof(tmpstr), "-%d %s\r\n", errortype, errormsg);
     writestring(sockfd, tmpstr);
}



/*
 * GSrecvHeader will retrieve a gopher+ header, if it exists
 *
 * It returns the expected number of bytes that are headed our
 * way, if it can.
 * 
 * Otherwise it returns -1 to indicate to read until \r\n.\r\n
 * or -2 to indicate to read to EOF
 *
 * If it encounters an error, it returns 0 and sets errno to the
 * gopher error class.
 */

int
GSrecvHeader(GopherObj *gs, int sockfd)
{
     char headerline[256];

     Debugmsg("GSrecvHeader\n");
     if (GSisGplus(gs)) {
	  if (readline(sockfd, headerline, sizeof(headerline))<=0)
		  return(0);
	  ZapCRLF(headerline);
	  if (*headerline == '+') {
	       if (*(headerline+1) == '-')
		    return(- (atoi(headerline+2)));
	       else
		    return(atoi(headerline+1));
	  } 
	  else if (*headerline == '-') {
	       /*** Oh no! an error! ***/
	       errno = atoi(headerline+1);
	       return(0);
	  }
     }
     /*** Guess if we're running old style gopher ***/
     else {
	  switch (GSgetType(gs)) {
	  case A_SOUND:
	  case A_IMAGE:
	  case A_GIF:
	  case A_UNIXBIN:
	  case A_PCBIN:
	       return(-2);
	       break;
	  default:
	       return(-1);
	  }
     }
     return(-1); /** Should never get here **/
}


/*
 * This routine will load up the item information from a gopher item
 * if the item hasn't transferred it already...
 *
 * The savename param, if TRUE will keep the current name and type
 */

void
GSgetginfo(GopherObj *gs, boolean   savename)
{
     int    sockfd, bytes;
     char   inputline[256];
     String *tempname = NULL;
     char   temptype='\0';

     if (!GSisGplus(gs))
	  return;

     /** Try not to overwrite stuff unnecessarily.. **/

     if (GSgplusInited(gs)) {
	  if (GSgetAdmin(gs)!=NULL)
	       return;
	  if (GSgetAskdata(gs) != NULL)
	       return;
     }

     if (savename) {
	  tempname = STRnew();
	  STRset(tempname, GSgetTitle(gs));
	  temptype = GSgetType(gs);
     }

     GSplusnew(gs);

     /** Send out the request **/
     if ((sockfd = GSconnect(gs)) <0) {
	  /*check_sock(sockfd, GSgetHost(gs), GSgetPort(gs));*/
	  return;
     }
     
     GStransmit(gs, sockfd, NULL, "!", NULL);
     bytes = GSrecvHeader(gs,sockfd);

     if (bytes == 0)
	  return;

     /***  Read off the first info block ***/
     readtoken(sockfd, inputline, sizeof(inputline), ' ');

     GSplusfromNet(gs, sockfd);
     
     if (savename) {
	  GSsetTitle(gs, STRget(tempname));
	  GSsetType(gs, temptype);
	  STRdestroy(tempname);
     }

}


/*
 * GSfromLink takes an FIO structure and starts reading from it.
 *
 * It reads until it finds a line it recognizes, then
 *
 * It keeps going until it finds
 *   eof, a non-recognized line, as long as there is a valid Path= line
 *
 * returns -1 on an error, 0 for EOF, 1 for success
 */

int
GSfromLink(
  GopherObj *gs,
  FileIO    *fio,
  char      *host,
  int       port,
  char      *directory,
  char      *peer)
{
     int doneflags = 0;
     char buf[1024];
     int bytesread;

     boolean DomainDefault = TRUE; /** Default for using domain stuff yes/no */
     boolean BadDomain = FALSE;	   /** For use with the Domain= line **/
     boolean DidDomain = FALSE;    /** Needed to make Domain= lines
				      into logical or's **/
     buf[0] = '\0';


     Debugmsg("GSfromLink...\n");
     while ((bytesread = FIOreadlinezap(fio, buf, sizeof(buf)))>0) {
	  if (buf[0] == '#') {
	       if (doneflags & G_PATH)
		    break;   /* comment */
	       else
		    continue;
	  }

	  ZapCRLF(buf);

	  if (strncmp(buf, "Type=", 5)==0) {
	       GSsetType(gs, buf[5]);

	       if (buf[6] == '+')
		    GSsetGplus(gs, TRUE);
	       if (buf[6] == '?')
		    GSsetAsk(gs, TRUE);
	       doneflags |= G_TYPE;
	  }

	  else if (strncmp(buf, "Name=", 5)==0) {
	       GSsetTitle(gs, buf+5);
	       doneflags |= G_NAME;
	  }

	  else if (strncmp(buf, "Path=", 5)==0) {
	       if (strncmp(buf+5, "~/",2) == 0 ||
		   strncmp(buf+5, "./",2) == 0) {
		    char tmpstr[256];
		    
		    *tmpstr = '.';
		    strcpy(tmpstr+1, directory);
		    if (directory[strlen(directory)-1] == '/')
			 strcat(tmpstr, buf+7);
		    else
			 strcat(tmpstr, buf+6);
		    GSsetPath(gs, tmpstr);
		    GSsetHost(gs, host);
	       } else
		    GSsetPath(gs, buf+5);
	       doneflags |= G_PATH;
	  }
 	  
	  else if (strncmp(buf, "Host=", 5)==0) {
	       if (buf[5] == '+' && buf[6] == '\0')
		    GSsetHost(gs, host);
	       else
		    GSsetHost(gs, buf+5);

	       doneflags |= G_HOST;
	  }

	  else if (strncmp(buf, "Port=", 5)==0) {
	       if (buf[5] == '+' && buf[6] == '\0')
		    GSsetPort(gs, port);
	       else
		    GSsetPort(gs, atoi(buf+5));

	       doneflags |= G_PORT;
	  }

	  else if (strncmp(buf, "Numb=", 5)==0)
	       GSsetNum(gs, atoi(buf+5));

	  else if (strncmp(buf, "Abstract=", 9)==0) {
	       char *acp;

	       acp = buf+9;

	       while (*(buf + strlen(buf)-1) == '\\' && bytesread >0) {
		    /* A continuation line */
		    *(buf + strlen(buf)-1) = '\0';
		    GSsetAbstract(gs, acp);
		    bytesread = FIOreadlinezap(fio, buf, sizeof(buf));
		    acp = buf;
	       }
	       GSsetAbstract(gs, acp);
	  }
	  else if (strncmp(buf, "Admin=", 6) == 0)
	       GSsetAdmin(gs, buf +6);
	  else if (strncmp(buf, "URL=", 4) == 0)
	       doneflags |= GSfromURL(gs, buf + 4, host, port, doneflags);
	  else if (strncmp(buf, "Domaindef=", 10)==0 && peer != NULL) {
	       DomainDefault = (strcasecmp(buf+10, "no")!=0);
	       Debug("Default Domain is %s\n", buf+10);
	  }
	  else if (strncmp(buf, "Domain=", 7) ==0 && peer != NULL) {
	       /** Check to see if the peer matches the domain **/
	       int     peerlen,domainlen;
	       boolean TestResult = !DomainDefault;
	       char    *host = buf+7;

	       if (*host == '!') {
		    TestResult = TRUE;
		    host++;
	       }

	       peerlen = strlen(peer);
	       domainlen = strlen(host);

	       if (DidDomain == TRUE && BadDomain == FALSE)
		    break;

	       if (domainlen > peerlen) {
		    BadDomain = !TestResult;
	       } else if (strncasecmp(buf+7, peer + peerlen - domainlen, domainlen)== 0) {
		    /** Domains match, do it! **/
		    BadDomain = TestResult;
	       } else
		    BadDomain = !TestResult;
	       
	       DidDomain = TRUE;
	  }
#ifndef VMS
	  else if (strncmp(buf, "Domain_pat=", 11) ==0 && peer != NULL) {
	       char    *host = buf+11;
	       boolean TestResult = !DomainDefault;

	       if (DidDomain == TRUE && BadDomain == FALSE)
		    break;
	       
	       if (*host == '!') {
		    host++;
		    TestResult = TRUE;
	       }

	       /** Check for domain using regexps **/
	       if (re_comp(host))
		    break;
	       if (re_exec(peer) == 1)
		    BadDomain = TestResult;
	       else
		    BadDomain = !TestResult;

	       DidDomain = TRUE;
	  }
#endif
	  else if (strncmp(buf, "TTL=", 4) == 0) {
	       GSsetTTL(gs, atoi(buf+4));
	  }
	  else
	       break;  /*** Unknown name/item ***/
     }

     Debugmsg("Done with this link item\n");

     if (BadDomain)
	  return(SOFTERROR);

     if (bytesread == 0) {
	  if (doneflags & G_PATH)
	       return(FOUNDEOF);  /** Found the eof, plus there's a g item **/
	  else 
	       return(HARDERROR); /** Mangled item, plus eof, stop the game **/
     }

     if (doneflags & G_PATH)
	  return(MORECOMING);  /** Found item, more coming. **/
     else
	  return(SOFTERROR);   /** Mangled item, more coming.. **/
}


 
/*
 * Fill in a GopherObj, given a URL
 */

 
int
GSfromURL(GopherObj *gs, char *urltxt, char *host, int port, int doneflags)
{
     char tempbuf[256];
     Url  *url;
     UrlServiceType serviceType;

     url = URLnew();
     URLset(url, urltxt);

     Debug("GSfromURL: %s\r\n",urltxt);
     GSsetURL(gs, urltxt);
     serviceType = URLgetService(url);
     
     switch (serviceType) {
     case http:
	  if (! (doneflags & G_TYPE)) {
	       /* It may not be HTML, but we lie and say it is so the  */
	       /* client passes it off to a http-speaking program */
	       GSsetType(gs, A_HTML);
	       doneflags |= G_TYPE;
	  }

	  if (! (doneflags & G_PATH)) {
	       snprintf(tempbuf, sizeof(tempbuf), "GET /%s", URLgetPath(url));
	       GSsetPath(gs, tempbuf);
	  } 
	  doneflags |= G_PATH;

	  break;

     case ftp:
	  if (! (doneflags & G_HOST)) {
	       GSsetHost(gs, host);
	       doneflags |= G_HOST;
	  }
	  if (! (doneflags & G_PORT)) {
	       GSsetPort(gs, port);
	       doneflags |= G_PORT;
	  }
	  break;

     case telnet:
	  if (! (doneflags & G_TYPE)) {
	       GSsetType(gs, A_TELNET);
	       doneflags |= G_TYPE;
	  }

     case tn3270:
	  if (! (doneflags & G_TYPE)) {
	       GSsetType(gs, A_TN3270);
	       doneflags |= G_TYPE;
	  }
	  break;

     case gopher:

	  if (! (doneflags & G_TYPE)) {
	       GSsetType(gs, URLgetGophType(url));
	       doneflags |= G_TYPE;
	  }

	  break;

     default:
	  /* A type we can't deal with... */
	  return(doneflags);
     }

     if (! (doneflags & G_NAME)) {
	  GSsetTitle(gs, URLget(url));
	  doneflags |= G_NAME;
     }

     /* Use login & password if needed & present */
     switch (serviceType) {
     case telnet:
     case tn3270:
	  if (URLgetUser(url) != NULL)
	       GSsetPath(gs, URLgetUser(url));
	  else
	       GSsetPath(gs, "");
	  doneflags |= G_PATH;
	  break;
/* Unhandled cases... */
     case gopher:
     case http:
     case ftp:
     case news:
     case unset:
     case unknown:
          break;
     }

     if (serviceType == ftp) {
	  if (!(doneflags & G_PATH)) {
	       if (URLgetPath(url) != NULL && *URLgetPath(url) != '\0')
		    snprintf(tempbuf, sizeof(tempbuf), 
			     "ftp:%s@/%s", URLgetHost(url),
			     URLgetPath(url));
	       else
		    snprintf(tempbuf, sizeof(tempbuf), 
			     "ftp:%s@/", URLgetHost(url));

	       GSsetPath(gs, tempbuf);
	       doneflags |= G_PATH;
	  }
	  if (! (doneflags & G_TYPE)) {
	       if ((*(URLgetPath(url)) == '\0') ||
	            *(URLgetPath(url) + strlen(URLgetPath(url))-1) == '/')
		    GSsetType(gs, A_DIRECTORY);
	       else
		    GSsetType(gs, A_FILE);

	       doneflags |= G_TYPE;
	  }
     } else {
	  if (! (doneflags & G_HOST)) {
	       GSsetHost(gs, URLgetHost(url));
	       doneflags |= G_HOST;
	  }
	  if (! (doneflags & G_PORT)) {
	       GSsetPort(gs, URLgetPort(url));
	       doneflags |= G_PORT;
	  }
     }
	  

     if (! (doneflags & G_PATH) || GSgetPath(gs) == NULL) {
          char *cp;
	  GSsetPath(gs, (cp=URLgetPath(url)) ? cp : "");
	  doneflags |= G_PATH;
     }

     return doneflags;
}
  



void
GStoLink(GopherObj *gs, int fd, BOOLEAN AddInfo)
{
     char gtype[2];
     char portnum[16];
     
     gtype[0] = GSgetType(gs);
     gtype[1] = '\0';

     writestring(fd, "#");
     writestring(fd, "\nType=");
     writestring(fd, gtype);
     if (GSisGplus(gs))
	  writestring(fd, "+");
     writestring(fd, "\nName=");
     writestring(fd, GSgetTitle(gs));
     writestring(fd, "\nPath=");
     writestring(fd, GSgetPath(gs));
     writestring(fd, "\nHost=");
     writestring(fd, GSgetHost(gs));
     writestring(fd, "\nPort=");
     snprintf(portnum, sizeof(portnum), "%d", GSgetPort(gs));
     writestring(fd, portnum);
     writestring(fd, "\n");
     if (GSisGplus(gs) && GSgplusInited(gs) && AddInfo) {
	  writestring(fd, "Admin=");
	  writestring(fd, GSgetAdmin(gs));
	  writestring(fd, "\nModDate=");
	  writestring(fd, GSgetModDate(gs));
	  writestring(fd, "\n");
     }
}


boolean
GSisText(GopherObj *gs, char *view)
{
     if (view == NULL) {
	  switch (GSgetType(gs)) {
	  case A_DIRECTORY:
	  case A_FILE:
	  case A_MIME:
	  case A_CSO:
	  case A_MACHEX:
	       return(TRUE);

	  case A_HTML:		/* For goofy relative urls, ugh! */
	  default:
	       return(FALSE);
	  }
     }
     else {
	  char viewstowage[64], *cp;

	  strcpy(viewstowage, view);
	  if ((cp=strchr(viewstowage, ' '))!=NULL) {
	       *cp = '\0';
	       view = viewstowage;
	  }

	  if (strncasecmp(view, "Text",4) == 0 ||
	      strncasecmp(view, "message/rfc822", 14)==0 ||
	      strncasecmp(view, "application/postscript", 21)==0 ||
	      strncasecmp(view, "application/mac-binhex40", 24)==0 ||
	      strncasecmp(view, "application/rtf", 15) == 0 ||
	      strncasecmp(view, "application/gopher", 18) == 0)



	       return(TRUE);
	  else
	       return(FALSE);
     }
}

#ifdef DEBUGGING
void
GSplusPrint(GopherObj *gs, char *head)
{
     int i;
     int oldDebug = DEBUG;
     DEBUG=FALSE;
     fprintf(stderr,"%s: Type=%c,Title=%s,Path=%s,Host=%s,Port=%d,Num=%d,Weight=%d,Plus=%d,Ask=%d\r\n",
	     head,
	     GSgetType(gs),
	     GSgetTitle(gs),
	     GSgetPath(gs),
	     GSgetHost(gs),
	     GSgetPort(gs),
	     GSgetNum(gs),
	     GSgetWeight(gs),
	     GSisGplus(gs),
	     GSisAsk(gs)
	     );
     
     if (GSgplusInited(gs))
	  for (i=0; i< GSgetNumBlocks(gs); i++) {
	       BLtoNet(GSgetBlock(gs, i), fileno(stderr), TRUE);
	  }
     fprintf(stderr,"===============\r\n");
     DEBUG = oldDebug;
}
#endif



