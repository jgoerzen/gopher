/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.3 $
 * $Date: 2001/01/17 19:30:25 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/index.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: index.c
 * Routines to deal with various types of indexes.
 *********************************************************************
 * Revision History:
 * $Log: index.c,v $
 * Revision 1.3  2001/01/17 19:30:25  jgoerzen
 * Change many sprintf -> snprintf
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.32  1995/09/28  21:11:31  lindner
 * More fixes...
 *
 * Revision 3.31  1995/09/26  05:16:23  lindner
 * more fixes...
 *
 * Revision 3.30  1995/09/26  04:59:47  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.29  1995/09/25  22:06:58  lindner
 * Ansification bugs
 *
 * Revision 3.28  1995/09/25  05:02:37  lindner
 * Convert to ANSI C
 *
 * Revision 3.27  1995/02/16  22:32:40  lindner
 * HTML icon support
 *
 * Revision 3.26  1994/11/29  05:07:10  lindner
 * Allow script paths with spaces, better error messages
 *
 * Revision 3.25  1994/10/19  03:32:17  lindner
 * NO_INDEXING hack
 *
 * Revision 3.24  1994/10/13  05:17:49  lindner
 * Compiler complaint fixes
 *
 * Revision 3.23  1994/10/02  02:58:08  lindner
 * Fix for gopher+ index accesses
 *
 * Revision 3.22  1994/07/21  17:22:27  lindner
 * /dev/null code from Brian
 *
 * Revision 3.21  1994/06/29  05:25:43  lindner
 * Add Gticket
 *
 * Revision 3.20  1994/05/14  04:18:14  lindner
 * Fix weird prob...
 *
 * Revision 3.19  1994/04/27  19:22:20  lindner
 * compatible.h added
 *
 * Revision 3.18  1994/04/08  21:08:07  lindner
 * Fix a bug in the Shell Search code
 *
 * Revision 3.17  1994/03/08  15:56:04  lindner
 * gcc -Wall fixes
 *
 * Revision 3.16  1994/01/21  03:55:38  lindner
 * Remove old dead HTML code, update function declarations
 *
 * Revision 3.15  1993/11/02  06:01:43  lindner
 * HTML mods
 *
 * Revision 3.14  1993/09/30  16:57:05  lindner
 * Fix for WAIS and $ requests
 *
 * Revision 3.13  1993/09/21  02:47:36  lindner
 * Fix index problem closing null file handle
 *
 * Revision 3.12  1993/09/11  04:40:44  lindner
 * Don't fork for localhost mindex databases
 *
 * Revision 3.11  1993/08/23  18:34:28  lindner
 * Add fixfile() call
 *
 * Revision 3.10  1993/08/06  14:30:44  lindner
 * Fixes for better security logging
 *
 * Revision 3.9  1993/08/05  20:44:02  lindner
 * Use Gpopen instead of popen, remove extra filtering code
 *
 * Revision 3.8  1993/08/04  22:12:43  lindner
 * Mods to use Gpopen
 *
 * Revision 3.7  1993/07/27  05:27:50  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.6  1993/07/26  15:31:13  lindner
 * mods for application/gopher-menu
 *
 * Revision 3.5  1993/07/20  23:56:44  lindner
 * LOGGopher mods
 *
 * Revision 3.4  1993/04/15  21:41:38  lindner
 * Added $ to list of naughty characters (just in case)
 *
 * Revision 3.3  1993/04/09  15:54:29  lindner
 * Fixes for indexes with gopher+
 *
 * Revision 3.2  1993/03/26  19:47:06  lindner
 * First crack at gopherplussing Indexing
 *
 * Revision 3.1.1.1  1993/02/11  18:02:52  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.3  1993/01/30  23:57:44  lindner
 * Removed html code, moved parsing of the inputline to gopherd.c
 *
 * Revision 1.2  1992/12/14  21:36:05  lindner
 * Fixed problem in ShellIndexQuery, cp wasn't being incremented.
 * Also added special character elimination from GrepIndexQuery
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#include "compatible.h"
#include "gopherd.h"
#include "command.h"
#include "index.h"

#include "fileio.h"
#include "Debug.h"
#include <stdio.h>

#ifndef NO_INDEXING

#define WAISTYPE 1
#define NEXTTYPE 2
#define SHLLTYPE 3
#define GREPTYPE 4

/*
 * Prototypes..
 */
static int  Read_hostdata(char *, char *, char *, int *, char *);
static void GrepIndexQuery(int, char *, char *, char *, int, char *);
static void ShellIndexQuery(int, char *, char *, boolean);
       void NeXTIndexQuery(int, char *, char *, char *, char *, int, char*,
			   boolean, char*);
static int  Find_index_type(char *gopherpath);

void 
Do_IndexTrans(int sockfd, char *IndexDirectory, CMDobj *cmd, boolean SendEOF)
{
     char *cp = NULL;
     char *dbName = NULL;
     char INDEXHost[256], INDEXPath[256];  /** Hard coded limits, ugh! **/
     int  INDEXPort=0;
     int  Index_type=0;
     char *SearchString = CMDgetSearch(cmd);
     boolean isgplus;
     char *view = CMDgetView(cmd);
     char tmp_IndexDirectory[256];

     Debug("Index Dir is %s\n", IndexDirectory);

     Index_type = Find_index_type(IndexDirectory);

     isgplus = CMDisGplus(cmd);

     Debug("Index type is %d\n", Index_type);

     if (Index_type < 0) {
	  /**** Error condition, unknown index type... ****/
	  Die(sockfd, 400, "Unknown index type");
	  return;
     }
	  
     if (Index_type == WAISTYPE) {
	  /*** The selector string has both the directory and the dbname... ***/
	  cp = strrchr(IndexDirectory, '/');
	  
	  if (cp == NULL)
	       dbName = "index";
	  else {
	       dbName= cp+1;
	       *cp='\0';
	  }
     }
     else if (Index_type == SHLLTYPE) {
	  /*** The selector string has both the directory and the dbname... ***/
	  cp = strrchr(IndexDirectory, '/');
	  if (cp == NULL) {
		  dbName = IndexDirectory;
		  IndexDirectory = tmp_IndexDirectory;
		  strcpy (tmp_IndexDirectory, "/");
	  } else {
	       dbName= cp+1;
	       *cp='\0';
	  }
     }

     if (Read_hostdata(IndexDirectory, INDEXHost, INDEXPath,&INDEXPort,dbName)<0) {
	  LOGGopher(sockfd, "Malformed hostdata file");
	  writestring(sockfd, "0Error on server, malformed hostdata\t\t\t1\r\n.\r\n");
	  return;
     }
     
     if (Index_type == SHLLTYPE) {
	  if (IndexDirectory == tmp_IndexDirectory)
	       IndexDirectory = dbName;
	  else
	       *(dbName-1) = '/';
     }

     /* Doctor up the indexdirectory path if we're not running chroot()
      * we use fixfile to keep things secure....
      */

     if (!dochroot)
	  IndexDirectory = fixfile(IndexDirectory);

     /** And call the appropriate query function **/

     switch (Index_type) {

     case NEXTTYPE:
	  
	  NeXTIndexQuery(sockfd, SearchString, IndexDirectory, NULL, 
			 INDEXHost, INDEXPort, INDEXPath, isgplus, view);
	  break;

     case WAISTYPE:
	  WaisIndexQuery(sockfd, IndexDirectory, SearchString, dbName, 
			 INDEXHost, INDEXPort, INDEXPath, isgplus, view);
	  break;

     case GREPTYPE:
	  GrepIndexQuery(sockfd, IndexDirectory, SearchString, 
			 INDEXHost, INDEXPort, INDEXPath);
	  break;
	  
     case SHLLTYPE:
	  ShellIndexQuery(sockfd, IndexDirectory, SearchString, isgplus);
	  break;
     }

     /** Finish the transmission attempt **/
     if (SendEOF)
	  writestring(sockfd, ".\r\n");
     
     /** Log it here so we get the query in the logfile **/

     if (dbName)
	  LOGGopher(sockfd,"search %s/%s for %s", IndexDirectory, 
		    dbName, SearchString);
     else
	  
	  LOGGopher(sockfd, "search %s for %s", IndexDirectory, SearchString);
}



/*
 * Try to figure out what each type of object is
 *
 * index types are 
 *   Error       == -1
 *   WAIS        == 1
 *   NeXT        == 2
 *   ShellScript == 3
 *   Grep        == 4
 */

static int
Find_index_type(char *gopherpath)
{
     char Teststr[512];
     FILE *Testfile;

     strcpy(Teststr, gopherpath);
     strcat(Teststr, "/.index/index.ixif");

     Testfile = rfopen(Teststr, "r");
     if (Testfile != NULL) {
	  /*** Next Index ***/
	  fclose(Testfile);
	  return(NEXTTYPE);
     }


     strcpy(Teststr, gopherpath);
     strcat(Teststr, ".inv");

     Testfile = rfopen(Teststr, "r");
     if (Testfile != NULL) {
	  /*** WAIS Index ***/
	  fclose(Testfile);
	  return(WAISTYPE);
     }


     strcpy(Teststr, gopherpath);
     if (isadir(Teststr) == 1) {
	  return(GREPTYPE);
     }
     
     Testfile = rfopen(Teststr, "r");
     if (Testfile != NULL) {
	  /** Shell script? **/
	  if (getc(Testfile) == '#')
	       if (getc(Testfile) == '!') {
		    fclose(Testfile);
		    return(SHLLTYPE);
	       }
     }

     return(-1);
}
     

/*
 * Read in the data from a hostdata file...
 * 
 * Try "<dbname>.hostdata" first, fall back to "hostdata" otherwise
 */

static int
Read_hostdata(char *IndexDirectory, char *INDEXHost, char *INDEXPath,
	      int *INDEXPort, char *dbName)
{
     FILE *Hostfile;
     char hostdataName[256];

     /** Read in the proper hostdata file.... **/

     rchdir(IndexDirectory);  /** Change into the index directory **/

     snprintf(hostdataName, sizeof(hostdataName),
	      "%s.hostdata", dbName);  /* try idx.hostdata */
     if ((Hostfile = ufopen(hostdataName, "r")) == NULL)
	  Hostfile = ufopen("hostdata", "r");

     if (Hostfile == NULL) {
	  /*** Use the current host/port as the default ***/
	  strcpy(INDEXHost, Zehostname);
	  *INDEXPort = GopherPort;
	  strcpy(INDEXPath, Data_Dir);
     } 
     else {
	  char tempbuf[255];

	  if (fgets(INDEXHost, 64, Hostfile) == NULL)
	       return(-1);
	  
	  ZapCRLF(INDEXHost);
	  
	  if (fgets(tempbuf, 255, Hostfile) == NULL)
	       return(-1);
	  
	  if ((*INDEXPort=atoi(tempbuf))==0)
	       return(-1);
	  
	  if (fgets(INDEXPath, 256, Hostfile) == NULL)
	       return(-1);
	  
	  ZapCRLF(INDEXPath);
	  fclose(Hostfile);
     }
     
     return(0);
}


/*
 * This is a searching function that runs grep across files
 * in a single directory...
 */

static void
GrepIndexQuery(int sockfd, char *Indexdir, char *Searchstr, char *INDEXHost,
	       int INDEXPort, char *INDEXPath)
{
     FILE *moocow;
     char command[512];
     char inputline[512];
     char *cp;
     GopherObj *gs;
     GopherDirObj *gd;

     gs = GSnew();
     gd = GDnew(32);

     cp = Searchstr;
     while (*cp != '\0') {
	  if (*cp == ';' ||*cp == '"' || *cp == '`' || *cp == '$')
	       *cp = '.';
	  cp++;
     }

     snprintf(command, sizeof(command),
	      "egrep \"%s\" \"%s\"/*", Searchstr, Indexdir);
     Debug("Grep command is %s\n", command);

     moocow = Gpopen(sockfd, command, "r");
     
     if (moocow == NULL) {
	  LOGGopher(sockfd, "Couldn't open grep command");
	  return;
     }

     while (fgets(inputline, 512, moocow)) {
	  ZapCRLF(inputline);
	  GSsetType(gs, '0');

	  cp = strstr(inputline, INDEXPath) + strlen(INDEXPath);
	  GSsetTitle(gs, cp);

	  cp = strchr(inputline, ':');
	  *cp='\0';
	  cp =strstr(inputline, INDEXPath) + strlen(INDEXPath);
	  GSsetPath(gs, cp);
	  GSsetHost(gs, INDEXHost);
	  GSsetPort(gs, INDEXPort);

	  GDaddGS(gd, gs);
     }
     if (UsingHTML)
	  GDtoNet(gd, sockfd, GSFORM_HTML, Gticket, NULL);
     else {
	  GDtoNet(gd, sockfd, GSFORM_G0, Gticket, NULL);
     }

     pclose(moocow);
}



/*
 * This starts up a shell script that's defined to be an index gateway
 * 
 * The shell script should write out standard gopher directory protocol.
 */

static void
ShellIndexQuery(int sockfd, char *Script, char *Searchstring, boolean isgplus)
{
     GopherDirObj *gd;
     char          Command[512];
     FileIO       *Searchf;
     int           result;

     gd = GDnew(32);

     snprintf(Command, sizeof(Command), "\"%s\" %s", Script, Searchstring);

     Searchf = FIOopenCmdline(Command, "r");

     if (Searchf == NULL) {
	  Die(sockfd, 500, "Cannot execute shell script");
     }

     GDfromNet(gd, FIOgetfd(Searchf), NULL);

     if (UsingHTML)
	  GDtoNet(gd, sockfd, GSFORM_HTML, Gticket, NULL);
     else {
	  if (isgplus)
	       GDplustoNet(gd, sockfd, NULL, Gticket);
	  else
	       GDtoNet(gd, sockfd, GSFORM_G0, Gticket, NULL);
     }

     result = FIOclose(Searchf);

     GDdestroy(gd);
}

	  
#ifndef WAISSEARCH

void
WaisIndexQuery(
  int sockfd,
  char *index_directory,
  char *SearchWords,
  char *new_db_name,
  char *INDEXHost,
  int  INDEXPort,
  char *INDEXPath,
  boolean isgplus,
  char *view)
{
     Die(sockfd, 501, "This Server doesn't have WAIS compiled in it...");
     return;
}
#endif 

#ifndef NEXTSEARCH
void
NeXTIndexQuery(
  int sockfd,
  char *SearchWords,
  char *ZIndexDirectory,
  char *DatabaseNm,  /*** Not used by the next indexer... ***/
  char *INDEXHost,
  int INDEXPort,
  char *INDEXPath,
  boolean isgplus,
  char *view)
{
     Die(sockfd, 501, "This isn't a NeXT... is it?");
     return;
}
#endif

#else /* NO_INDEXING */
void 
Do_IndexTrans(int sockfd, char *IndexDirectory, CMDobj *cmd, boolean SendEOF)
{
     Die(sockfd, 501, "Sorry, no indexing in this server");
}
#endif /* NO_INDEXING */

