/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/html2.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: html2.c
 * More lame code for html
 *********************************************************************
 * Revision History:
 * $Log: html2.c,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.18  1995/11/03  21:18:20  lindner
 * ANSIfication
 *
 * Revision 3.17  1995/11/03  20:50:49  lindner
 * Coen: fixes..
 *
 * Revision 3.16  1994/10/13  05:29:16  lindner
 * Compiler complaint fixes
 *
 * Revision 3.15  1994/05/17  05:48:00  lindner
 * Massive internationalization change
 *
 * Revision 3.14  1994/05/14  04:13:43  lindner
 * Internationalization...
 *
 * Revision 3.13  1994/04/25  20:47:34  lindner
 * Remove html stuff from menu
 *
 * Revision 3.12  1994/04/14  02:56:55  lindner
 * Replace yet another system call
 *
 * Revision 3.11  1994/03/04  23:31:56  lindner
 * mem fix
 *
 * Revision 3.10  1994/02/20  16:29:56  lindner
 * Use urls embedded in gopher obj instead of GSfromHREF
 *
 * Revision 3.9  1993/12/30  04:17:24  lindner
 * use URL from GS instead of hack method
 *
 * Revision 3.8  1993/10/22  20:27:10  lindner
 * Fix for trailing blanks on HTML viewer (Fote)
 *
 * Revision 3.7  1993/10/11  17:03:46  lindner
 * application/html-->text/html, and simplified test for absence of a display command (Fote)
 *
 * Revision 3.6  1993/09/08  05:23:54  lindner
 * Use and external browser for html
 *
 * Revision 3.5  1993/07/29  17:21:27  lindner
 * eliminate non-used variables
 *
 * Revision 3.4  1993/07/27  05:28:53  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.3  1993/07/27  00:32:46  lindner
 * HTML patch from Mitra
 *
 * Revision 3.2  1993/04/15  21:19:43  lindner
 * Debug line added (Mitra)
 *
 * Revision 3.1.1.1  1993/02/11  18:02:58  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.2  1992/12/31  03:59:42  lindner
 * Renamed to html2.c for case insensitive VMS. Fixed State: line.
 *
 * Revision 1.1  1992/12/10  23:32:16  lindner
 * gopher 1.1 release
 *
 * Revision 1.1  1992/12/10  06:16:51  lindner
 * Initial revision
 *
 *
 *********************************************************************/


#include "gopher.h"
#include "Malloc.h"
#include "Debug.h"

char *process_tag();

/*
 * Enable use of an external HTML browser until the
 * builtin HTML pager is functional (if ever 8-).
 */

#include "fileio.h"

void
do_html(GopherObj *ZeGopher)
{
     char command[256], HTMLcmd[128];
     char *Dialogmess[7];

     if (!RCdisplayCommand(GlobalRC, "text/html", "", HTMLcmd) ||
         !strncasecmp(HTMLcmd, "- none -", 8) ||
	 HTMLcmd == NULL || HTMLcmd[0] == '\0') {
	  CursesErrorMsg(Gtxt("Sorry, this client does not support HTML",47));
          return;
     }
     else
          if (HTMLcmd[strlen(HTMLcmd)-1] == ' ')
               HTMLcmd[strlen(HTMLcmd)-1] = '\0';

     sprintf(command, "%s \"%s\"",HTMLcmd, GSgetURLhtml(ZeGopher,""));

     clear();
#ifdef VMS
     refresh();
#endif
     Dialogmess[0] = Gtxt("Warning!!!!!, you are about to leave the Internet Gopher",42);
     Dialogmess[1] = Gtxt("Client and connect to an HTML Browser.  Use the Browser",43);
     Dialogmess[2] = Gtxt("Help if you are not familiar with its commands.",44);
     Dialogmess[3] = "";
     Dialogmess[4] = Gtxt("Upon quitting from the Browser, you will be returned to",45);
     Dialogmess[5] = Gtxt("the Internet Gopher Client.",46);
     Dialogmess[6] = NULL;
     
     if (CURDialog(CursesScreen, GSgetTitle(ZeGopher), Dialogmess) <0)
	  return;
      
     CURexit(CursesScreen);
     FIOsystem(command);
     CURenter(CursesScreen);

}
