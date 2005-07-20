/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.3 $
 * $Date: 2002/01/08 21:35:49 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/gopherrc.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * Copyright 2000-2005 by John Goerzen and the Gopehr developers
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: gopherrc.c
 * Utilities to read and write the .gopherrc file 
 *********************************************************************
 * Revision History:
 * $Log: gopherrc.c,v $
 * Revision 1.3  2002/01/08 21:35:49  jgoerzen
 * Many changes:
 *  Revved the version number
 *  updated greeting
 *  updated copyright notices
 *
 * Revision 1.2  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.38  1995/11/03  21:18:19  lindner
 * ANSIfication
 *
 * Revision 3.37  1995/03/09  05:23:52  lindner
 * F.Macrides 03-Mar-1995: Ensure that files are closed on errors in
 * RCisGlobalNew(), else the user's RC file updates will fail on exit.
 *
 * Revision 3.36  1994/10/21  04:41:07  lindner
 * Add ANSI attached printer code..
 *
 * Revision 3.35  1994/07/19  20:19:14  lindner
 * Fix for globalrc checking...
 *
 * Revision 3.34  1994/07/06  15:41:12  lindner
 * gopherrc.c
 *
 * Revision 3.33  1994/06/29  05:12:47  lindner
 * Fix problem with NO_MAPS.. define, add code to check for new config file
 *
 * Revision 3.32  1994/06/09  22:13:39  lindner
 * More language conversions
 *
 * Revision 3.31  1994/06/09  04:24:50  lindner
 * (F.Macrides) Added option for not reading maps from a
 * Bookmark file when the client is running in SecureMode or NoShellMode,
 * implemenented via the compilation symbol SECURE_MAPS_GLOBALRC_ONLY
 *
 * Revision 3.30  1994/05/17  05:47:59  lindner
 * Massive internationalization change
 *
 * Revision 3.29  1994/05/14  04:13:42  lindner
 * Internationalization...
 *
 * Revision 3.28  1994/05/06  02:28:53  lindner
 * Allow getenv code to be used on Unix systems
 *
 * Revision 3.27  1994/04/19  14:31:36  lindner
 * Fix for the problem of vanishing bookmarks
 *
 * Revision 3.26  1994/04/01  04:48:57  lindner
 * Conversion to FIO routines for gopherrc i/o
 *
 * Revision 3.25  1994/03/04  23:35:49  lindner
 * fix for SCO compiler
 *
 * Revision 3.24  1993/12/28  17:30:06  lindner
 * added options for GDfromLink
 *
 * Revision 3.23  1993/11/29  01:01:10  lindner
 * getenv addition for mips
 *
 * Revision 3.22  1993/11/15  07:34:26  lindner
 * Fix stoopid fcntl include
 *
 * Revision 3.21  1993/11/05  07:26:11  lindner
 * fix for Solaris
 *
 * Revision 3.20  1993/11/02  06:45:14  lindner
 * Use /home/mudhoney/lindner instead of password routines to read gopherrc
 *
 * Revision 3.19  1993/10/22  20:25:51  lindner
 * Fix for VMS logicals in RCfromENV() (Fote)
 *
 * Revision 3.18  1993/10/11  16:57:40  lindner
 * Fote's mods for appending %s in display command handling, application/html becomes text/html
 *
 * Revision 3.17  1993/09/30  22:42:09  lindner
 * Add option for bolding of searched words
 *
 * Revision 3.16  1993/09/21  01:47:36  lindner
 * Fix for builtin pager...
 *
 * Revision 3.15  1993/09/08  01:23:46  lindner
 * Add Environment variable processing for VMS, plus add support for HTML
 *
 * Revision 3.14  1993/08/28  04:58:50  lindner
 * Moved GLOBALRC definition to conf.h for VMS
 *
 * Revision 3.13  1993/08/16  18:04:15  lindner
 * Add a purge to gopherrc on VMS
 *
 * Revision 3.12  1993/08/16  17:58:22  lindner
 * Removed REMOTEUSER ifdefs
 *
 * Revision 3.11  1993/08/03  20:00:00  lindner
 * Change audio/mulaw to audio/basic for MIME
 *
 * Revision 3.10  1993/07/29  17:23:41  lindner
 * eliminate non-used variables
 *
 * Revision 3.9  1993/07/27  05:28:51  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.8  1993/07/26  15:37:27  lindner
 * fix warning message for hosed gopher.rc lines.
 *
 * Revision 3.7  1993/07/20  23:12:34  lindner
 * Mods to use patchlevel.h
 *
 * Revision 3.6  1993/07/07  19:44:27  lindner
 * Better MIME compatibility, now accepts text/plain
 *
 * Revision 3.5  1993/06/09  22:14:42  lindner
 * VMS updates
 *
 * Revision 3.4  1993/04/15  21:21:30  lindner
 * Code for remote users (Mitra)
 *
 * Revision 3.3  1993/03/24  16:58:47  lindner
 * Added RCprintCommand()
 *
 * Revision 3.2  1993/02/19  21:10:17  lindner
 * Generalized some routines.  Added global rc file.
 *
 * Revision 3.1.1.1  1993/02/11  18:03:00  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1993/02/09  22:35:15  lindner
 * Lots of changes.  RC processing now holds the global variables inside
 * its container.  Much more objecty
 *
 * Revision 1.8  1993/01/12  21:16:44  lindner
 * Fixed gopherrc for good!
 *
 * Revision 1.7  1993/01/09  02:53:55  lindner
 * Fixed spelling error on it's
 *
 * Revision 1.6  1993/01/09  02:31:09  lindner
 * Added #define to check for MAXPATHLEN
 *
 * Revision 1.5  1992/12/31  05:46:52  lindner
 * Merged 1.2.1.1 in main code sequence
 *
 * Revision 1.4  1992/12/15  16:54:38  lindner
 * Fixed typo in initial display message.  Also changed message
 * "Press any key to continue" to "Press RETURN to continue", since
 * we're not in raw() mode.
 *
 * Revision 1.3  1992/12/14  20:41:00  lindner
 * .gopherrc was being created with a random permissions mask.  Fixed it
 * Also made it so that gopherrc is created with mode 0644
 *
 * Revision 1.2.1.1  1992/12/31  05:42:26  lindner
 * VMS mods.
 *
 * Revision 1.2  1992/12/11  19:52:59  lindner
 * Added copyright notices when the users starts the client the first time.
 *
 * Revision 1.1  1992/12/10  23:32:16  lindner
 * gopher 1.1 release
 *
 * Revision 1.1  1992/12/10  06:16:51  lindner
 * Initial revision
 *
 *
 *********************************************************************/


#include "gopherrc.h"
#include "CURcurses.h"
#include "conf.h"
#include "globals.h"
#include "Malloc.h"
#include "patchlevel.h"
#include "compatible.h"
#include "fileio.h"
#include "Stat.h"		/* For testing times of globalrc file */

#include <stdio.h>
#include "Locale.h"

#ifdef VMS
#include <unixlib.h>
#define MAXPATHLEN FILENAME_MAX
#include <file.h>
#else
#include <sys/file.h>
#include <fcntl.h>
#endif

#ifdef M_XENIX
#include <sys/types.h>
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif

#ifdef mips
char *getenv();
#endif



/*
 * Definitions for the mapping object 
 */

RCMapObj *
RCMnew(void)
{
     RCMapObj *rcm;

     rcm = (RCMapObj*) malloc(sizeof(RCMapObj));
     
     rcm->view = STRnew();
     rcm->displaycmd = STRnew();
     rcm->printcmd = STRnew();

     return(rcm);
}

void
RCMdestroy(RCMapObj *rcm)
{
     STRdestroy(rcm->view);
     STRdestroy(rcm->displaycmd);
     STRdestroy(rcm->printcmd);
     free(rcm);
}

void
RCMinit(RCMapObj *rcm)
{
     STRinit(rcm->view);
     STRinit(rcm->displaycmd);
     STRinit(rcm->printcmd);
}

	  
void
RCMcpy(RCMapObj *dest, RCMapObj *orig)
{
     STRcpy(dest->view, orig->view);
     STRcpy(dest->displaycmd, orig->displaycmd);
     STRcpy(dest->printcmd, orig->printcmd);
}



/* 
 * Substitue filename for %s in zeline and return the command in "line",
 * 
 */

static void
RCMsubst_percent_s(RCMapObj *rcm, char *filename, char *line, char *zeline)
{
     
     while (1) {
	  if (*zeline=='\0') {
	       *line = '\0';
	       return;
	  }
	  
	  else if (*zeline == '%') {
	       if (*(zeline+1) == 's') {
		    strcpy(line, filename);
		    line += strlen(filename);
		    zeline+=2;
	       } else {
		    *line = *zeline;
		    line++;
		    zeline++;
	       }
	  }
	  else {
	       *line = *zeline;
	       line++;
	       zeline++;
	  }
     }

}



/* 
 * Construct a command line for displaying.
 */


void
RCMdisplayCommand(RCMapObj *rcm, char *filename, char *line)
{
     char *zeline;

     zeline  = RCMgetDisplaycmd(rcm);
     RCMsubst_percent_s(rcm, filename, line, zeline);
}




/* 
 * Construct a command line for printing.
 */


void
RCMprintCommand(RCMapObj *rcm, char *filename, char *line)
{
     char *zeline;

     zeline  = RCMgetPrintcmd(rcm);
     RCMsubst_percent_s(rcm, filename, line, zeline);
}

     

/*
 * Find the item that contains "view"
 */

int
RCMAviewSearch(RCMAarray *rcma, char *view)
{
     int i;
     RCMapObj *temp;
     char viewstowage[64];
     char *tempview, *cp;

     tempview = view;

     if ((cp = strchr(view, ' ')) != NULL) {
	  strcpy(viewstowage, view);
	  viewstowage[cp - view] = '\0';
	  tempview = viewstowage;
     }


     /*** Linear search.  Ick. ***/
     
     for (i=0; i< RCMAgetNumEntries(rcma); i++) {
	  temp = RCMAgetEntry(rcma,i);
	  
	  if (strcasecmp(tempview, RCMgetView(temp))==0)
	       return(i);
     }
     return(-1);
}

/*
 * Map: "view","display command","print command"
 */

boolean
RCMAfromLine(RCMAarray *rcma, char *line)
{
     char *cp;
     RCMapObj *rcm;
     char *view, *dcmd, *pcmd;
     int num;

     view = line;

     cp = strchr(line, ',');
     if (cp == NULL)
	  return(FALSE);

     *cp = '\0';
     dcmd = cp+1;

     cp = strchr(dcmd, ',');
     if (cp == NULL)
	  return(FALSE);

     *cp = '\0';
     pcmd = cp+1;
     
     if ((num = RCMAviewSearch(rcma, view)) >=0)
	  rcm = RCMAgetEntry(rcma, num);
     else
	  rcm = RCMnew();

     RCMsetView(rcm, view);
     RCMsetDisplaycmd(rcm, dcmd);
     RCMsetPrintcmd(rcm, pcmd);

     if (num <0) {
	  RCMAadd(rcma, rcm);
	  RCMdestroy(rcm);
     }

     return(TRUE);
}


/*
 * Writes out the .gopherrc file.
 */

void
RCMAtoFile(RCMAarray *rcma, FileIO *fio)
{
     int i;
     RCMapObj *rcm;
     char tmpstr[512];

     for (i = 0 ; i<RCMAgetNumEntries(rcma); i++) {
	  rcm = RCMAgetEntry(rcma, i);
	  sprintf(tmpstr, "map: %s,%s,%s\n", RCMgetView(rcm),
		  RCMgetDisplaycmd(rcm), RCMgetPrintcmd(rcm));
	  FIOwritestring(fio, tmpstr);
     }

}

/**********************************************************
*  Generic RC stuff follows
*
*/

#ifdef SECURE_MAPS_GLOBALRC_ONLY
static BOOLEAN SettingRCdefs = FALSE;
#endif


RCobj *
RCnew(void)
{
     RCobj *rc;

     rc = (RCobj *) (malloc(sizeof(RCobj)));
     
     rc->commands  = RCMAnew();
     rc->Bookmarks = NULL;
     rc->Home	   = NULL;
     rc->ChangedDefs = FALSE;

     rc->SearchBolding = TRUE;
     rc->ANSIprinter   = FALSE;

#ifdef SECURE_MAPS_GLOBALRC_ONLY
     SettingRCdefs = TRUE;
#endif
       RCsetdefs(rc);
#ifdef SECURE_MAPS_GLOBALRC_ONLY
     SettingRCdefs = FALSE;
#endif

     return(rc);
}


static FileIO *
rcopen(int flags)
{
     char    rcfilename[MAXPATHLEN];
     FileIO *fio;
#ifndef VMS
     char *home;
     
     home = getenv("HOME");
     if (home == NULL)
	  return(NULL);

     /** First, let's find out if there is a bookmark file **/

     strcpy(rcfilename,home);   /* Home directory */
     strcat(rcfilename,"/.gopherrc");

#else
     strcpy(rcfilename,"sys$login:gopherrc");
#endif  /* VMS */

     fio = FIOopenUFS(rcfilename, flags, 0644);

     return(fio);
}     


void
RCfromFile(RCobj *rc, FileIO *rcfio)
{
     char inputline[512];

     while (FIOreadline(rcfio, inputline, sizeof(inputline))) {

	  if (*inputline == '#')
	       continue;

	  ZapCRLF(inputline);
	  if (strncasecmp(inputline, "bookmarks:", 9)==0) {
	      
	       if (RCgetBookmarkDir(rc) == NULL)
		    RCinitBookmarkDir(rc, 32);
	       
	       GDfromLink(RCgetBookmarkDir(rc), rcfio, "localhost", 70, "","localhost");
	       
	       GDsetTitle(RCgetBookmarkDir(rc), Gtxt("Bookmarks",67));
	  } else if (strncasecmp(inputline, "home:",5)==0) {
	       if (RCgetHome(rc) == NULL) {
		    RCsetHome(rc, GSnew());
		    GSfromLink(RCgetHome(rc), rcfio, "localhost", 70, "","localhost");
	       }
	  } else if (strncasecmp(inputline, "searchbolding: ",15)==0) {
	       if (strcasecmp(inputline+15, "yes")==0)
		    RCsetSearchBolding(rc, TRUE);
	       else
		    RCsetSearchBolding(rc, FALSE);
	  } else if (strncasecmp(inputline, "useansiprinter: ", 16) == 0) {
	       if (strcasecmp(inputline+16, "yes") == 0)
		    RCsetANSIprinter(rc, TRUE);
	       else
		    RCsetANSIprinter(rc, FALSE);
#ifdef SECURE_MAPS_GLOBALRC_ONLY
	  } else if (strncasecmp(inputline, "map: ", 5)==0 &&
		     (SettingRCdefs || !(SecureMode || NoShellMode))) {
#else
	  } else if (strncasecmp(inputline, "map: ", 5)==0) {
#endif
	       if (RCMAfromLine(rc->commands, inputline+5) == FALSE) {
		    /** Error parsing **/
		    printf(Gtxt("Warning, bad line in gopherrc: %s\n",215), inputline);
		    sleep(2);
	       }
	  } else
	       ;

     }
     ;
}


/*
 *  Permit key environement logicals on VMS:
 *	Printer and pager for text files.
 *	Telnet and tn3270 commands.
 *	External HTML browser.
 *  Set up other views appropriately for VMS via
 *   gopher.rc or gopherremote.rc (defined in conf.h),
 *   and via the User's gopherrc.
 */

void
RCfromENV(RCobj *rc)
{
     char tmpstr[512], Printer[128], PrinterP[128], Pager[128], PagerP[128];
     boolean PrinterFlag = FALSE;

     if (getenv("GOPHER_PRINTER") != NULL) {
     	   PrinterFlag = TRUE;
           strcpy(Printer, getenv("GOPHER_PRINTER"));
	   strcpy(PrinterP, Printer);
     }
     else {
	  if(RCprintCommand(GlobalRC,"Text","",Printer)) {
	       if (Printer[strlen(Printer)-1] == ' ')
	            Printer[strlen(Printer)-1] = '\0';
	  }
	  else
	       strcpy(Printer, PRINTER_COMMAND);

	  if(RCprintCommand(GlobalRC,"Text/plain","",PrinterP)) {
	       if (PrinterP[strlen(PrinterP)-1] == ' ')
	            PrinterP[strlen(PrinterP)-1] = '\0';
	  }
	  else
	       strcpy(PrinterP, PRINTER_COMMAND);
     }

     if (getenv("PAGER") != NULL) {
          sprintf(tmpstr, "Text,%s %%s,%s %%s", getenv("PAGER"),Printer);
          RCMAfromLine(rc->commands, tmpstr);

          sprintf(tmpstr, "Text/plain,%s %%s,%s %%s", getenv("PAGER"),PrinterP);
          RCMAfromLine(rc->commands, tmpstr);
     }
     else if (PrinterFlag) {
	  if (RCdisplayCommand(GlobalRC,"Text","",Pager)) {
	       if (Pager[strlen(Pager)-1] == ' ')
	             Pager[strlen(Pager)-1] = '\0';
	  }
	  else
	       strcpy(Pager, PAGER_COMMAND);

	  if (strncasecmp(Pager, "builtin", 7) == 0)
               sprintf(tmpstr, "Text,%s,%s %%s", Pager, Printer);
	  else
               sprintf(tmpstr, "Text,%s %%s,%s %%s", Pager, Printer);
          RCMAfromLine(rc->commands, tmpstr);

	  if(RCdisplayCommand(GlobalRC,"Text/plain","",PagerP)) {
	       if (PagerP[strlen(PagerP)-1] == ' ')
	             PagerP[strlen(PagerP)-1] = '\0';
	  }
	  else
	       strcpy(PagerP, PAGER_COMMAND);

	  if (strncasecmp(PagerP, "builtin", 7) == 0)
               sprintf(tmpstr, "Text/plain,%s,%s %%s", PagerP, PrinterP);
	  else
               sprintf(tmpstr, "Text/plain,%s %%s,%s %%s", PagerP, PrinterP);
          RCMAfromLine(rc->commands, tmpstr);
     }

     if (getenv("GOPHER_TELNET") != NULL) {
          sprintf(tmpstr, "Terminal/telnet,%s %%s,", getenv("GOPHER_TELNET"));
          RCMAfromLine(rc->commands, tmpstr);
     }

     if (getenv("GOPHER_TN3270") != NULL) {
          sprintf(tmpstr, "Terminal/tn3270,%s %%s,", getenv("GOPHER_TN3270"));
          RCMAfromLine(rc->commands, tmpstr);
     }

     if (getenv("GOPHER_HTML") != NULL) {
          sprintf(tmpstr, "text/html,%s,", getenv("GOPHER_HTML"));
          RCMAfromLine(rc->commands, tmpstr);
     }
}


/*
 * Process the User's .gopherrc file and read it into rc
 * Then call RCfromENV() for VMS
 */

void
RCfromUser(RCobj *rc)
{
     FileIO *rcfile;
#ifndef VMS
     char *home;
     home = getenv("HOME");

     if (home == NULL)
	  return;
#endif
     rcfile = rcopen(O_RDONLY);

     if (rcfile == NULL) {
	  /*** Make the .gopherrc file ***/
	  if (!NoShellMode) { /*  No point in telling a user with no shell 
				about a file they cant get at! */
	       rcfile = rcopen(O_CREAT);
	       FIOclose(rcfile);
	       printf(Gtxt("Welcome to the wonderful world of Gopher!\n\n",1));
	       printf(Gtxt("Gopher has limitations on its use and comes without\n",2));
	       printf(Gtxt("a warranty.  Please refer to the file 'Copyright' included\n",3));
	       printf(Gtxt("in the distribution.\n\n",4));
	       printf(Gtxt("Internet Gopher Information Client %s.%s patch%d (%s)\n",6),
		      GOPHER_MAJOR_VERSION, GOPHER_MINOR_VERSION, PATCHLEVEL,
		      GOPHER_NICKNAME);
	       printf(Gtxt("Copyright (C) 1991-2000 by the Regents of the University of Minnesota\nCopyright (C) 2000-2005 John Goerzen and the gopher developers\n\n",5));
	       printf(Gtxt("Press RETURN to continue\n",7));
	       while ('\n' != getchar())
		    ;
	  } /*NoShellMode*/
	  return;    /*** No such file ***/
     }
     else {
	  RCfromFile(GlobalRC, rcfile);
     }
     FIOclose(rcfile);

#ifdef VMS
     RCfromENV(rc);
#endif
}


void
RCreadGlobalRC(RCobj *rc)
{
     FileIO *rcfile;

     if (RemoteUser)
	  rcfile = FIOopenUFS(REMOTERC, O_RDONLY, 0);
     else
	  rcfile = FIOopenUFS(GLOBALRC, O_RDONLY, 0);

     if (rcfile != NULL) {
	  RCfromFile(rc, rcfile);
	  
	  FIOclose(rcfile);
     }
}




/*
 * Set up some useful definitions for RCfiles 
 */

void
RCsetdefs(RCobj *rc)
{
     char    tmpstr[512];

     sprintf(tmpstr, "Text,%s,%s %%s", PAGER_COMMAND, PRINTER_COMMAND);
     RCMAfromLine(rc->commands, tmpstr);

     sprintf(tmpstr, "Text/plain,%s,%s %%s", PAGER_COMMAND, PRINTER_COMMAND);
     RCMAfromLine(rc->commands, tmpstr);

     sprintf(tmpstr, "Audio/basic,%s,", PLAY_COMMAND);
     RCMAfromLine(rc->commands, tmpstr);

     if (strncasecmp(IMAGE_COMMAND, "- none -", 8) == 0)
          sprintf(tmpstr, "Image,%s,", IMAGE_COMMAND);
     else
          sprintf(tmpstr, "Image,%s,%s %%s", IMAGE_COMMAND, PRINTER_COMMAND);
     RCMAfromLine(rc->commands, tmpstr);

     sprintf(tmpstr, "Terminal/telnet,%s %%s,", TELNET_COMMAND);
     RCMAfromLine(rc->commands, tmpstr);
 
     sprintf(tmpstr, "Terminal/tn3270,%s %%s,", TN3270_COMMAND);
     RCMAfromLine(rc->commands, tmpstr);

     sprintf(tmpstr, "text/html,%s,", HTML_COMMAND);
     RCMAfromLine(rc->commands, tmpstr);

     RCreadGlobalRC(rc);
}     

void
RCtoFile(RCobj *rc)
{
     FileIO *rcfile;
     char   rcfilename[512];
     char   oldrcfilename[512];
#ifndef VMS
     char *home;

     home = getenv("HOME");

     if (home == NULL)
	  return;
#endif /* not VMS */

     /* In NoShellMode we just stop writing of map: stuff */
     if (SecureMode)
	  return;

     /** First, let's find out if there is a rc file **/

#ifndef VMS
     strcpy(rcfilename,home);   /* Home directory */
     strcat(rcfilename,"/.gopherrc");
     strcpy(oldrcfilename,home); 
     strcat(oldrcfilename,"/.gopherrc~");

     rcfile = FIOopenUFS(rcfilename, O_RDONLY, 0);

     if (rcfile != NULL)
	  if (rename(rcfilename, oldrcfilename)<0) {
	       CursesErrorMsg(Gtxt("Aborting gopher configuration save!!",216));
	       return;
	  }

     FIOclose(rcfile);

#else
     strcpy(rcfilename,"sys$login:gopherrc");
#endif
 
     rcfile = FIOopenUFS(rcfilename, O_WRONLY|O_CREAT|O_TRUNC, 0644);

     if (rcfile ==NULL) {
#ifdef VMS
          fprintf(stderr,Gtxt("Can't write gopher configuration file!!",8));
#else
	  CursesErrorMsg(Gtxt("Can't write gopher configuration file!!",8));
#endif
	  return;    
     }

     FIOwritestring(rcfile, "RCversion: 1.1\n");

     if (!NoShellMode)  
	  RCMAtoFile(rc->commands, rcfile);
     if (!NoShellMode) {
	  FIOwritestring(rcfile, "SearchBolding: ");
	  if (RCsearchBolding(rc))
	       FIOwritestring(rcfile, "yes\n");
	  else
	       FIOwritestring(rcfile, "no\n");
     }

     if (!NoShellMode) {
	  FIOwritestring(rcfile, "UseANSIprinter: ");
	  if (RCuseANSIprinter(rc))
	       FIOwritestring(rcfile, "yes\n");
	  else
	       FIOwritestring(rcfile, "no\n");
     }

     if (RCgetHome(rc) != NULL) {
	  FIOwritestring(rcfile, "\nhome:\n");
	  GStoLink(RCgetHome(rc), FIOgetfd(rcfile), FALSE);
     }

     if (RCgetBookmarkDir(GlobalRC) != NULL) {
	  FIOwritestring(rcfile, "\nbookmarks:\n");
	  GDtoLink(RCgetBookmarkDir(GlobalRC), FIOgetfd(rcfile));
     }

     FIOwritestring(rcfile, "\n");
     FIOclose(rcfile);
#ifdef VMS
     while (unlink("sys$login:gopherrc.;-1") == 0) ;        /* purge/keep=1  */
     rename("sys$login:gopherrc.;","sys$login:gopherrc.;1");/* reset version */
#endif
}


/*
 * This makes a display command for the client.
 * It returns FALSE if it finds a method of displaying the item
 *
 * It returns TRUE and fills line with a command if one is found
 */

boolean 
RCdisplayCommand(RCobj *rc, char *view, char *filename, char *line)
{
     int num;  

     num = RCMAviewSearch(rc->commands, view);

     if (num >=0) {
	  RCMdisplayCommand(RCMAgetEntry(rc->commands, num), filename, line);
	  return(TRUE);
     }
     else
	  return(FALSE);
}

/*
 * This makes a display command for the client.
 * It returns FALSE if it finds a method of printing the item
 *
 * It returns TRUE and fills line with a command if one is found
 */

boolean 
RCprintCommand(RCobj *rc, char *view, char *filename, char *line)
{
     int num;  

     num = RCMAviewSearch(rc->commands, view);

     if (num >=0) {
	  RCMprintCommand(RCMAgetEntry(rc->commands, num), filename, line);
	  return(TRUE);
     }
     else
	  return(FALSE);
}


/*
 * This function tests to see whether the global gopherrc file has changed... 
 */

boolean
RCisGlobalNew(void)
{
     FileIO *fiolocal;
     FileIO *fioglobal;
     struct stat lbuf, gbuf;
     
     fiolocal = rcopen(O_RDONLY);

     if (fiolocal == NULL) 
	  return(FALSE);


     fioglobal = FIOopenUFS(GLOBALRC, O_RDONLY, 0644);

     if (fioglobal == NULL) {
          FIOclose(fiolocal);
	  return(FALSE);
     }

     if (fstat(FIOgetfd(fiolocal), &lbuf)) {
          FIOclose(fiolocal);
          FIOclose(fioglobal);
	  return(FALSE);
     }

     if (fstat(FIOgetfd(fioglobal), &gbuf)) {
          FIOclose(fiolocal);
          FIOclose(fioglobal);
	  return(FALSE);
     }
     
     FIOclose(fioglobal);
     FIOclose(fiolocal);

     if (gbuf.st_mtime > lbuf.st_mtime)
	  return(TRUE);
     else
	  return(FALSE);
}

