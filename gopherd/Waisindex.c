/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2001/01/17 19:30:25 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/Waisindex.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: Waisindex.c
 * Routines to translate wais indexes on disk to gopher
 *********************************************************************
 * Revision History:
 * $Log: Waisindex.c,v $
 * Revision 1.2  2001/01/17 19:30:25  jgoerzen
 * Change many sprintf -> snprintf
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.37  1995/09/28  21:11:28  lindner
 * More fixes...
 *
 * Revision 3.36  1995/09/26  04:59:44  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.35  1995/09/25  22:11:10  lindner
 * blobbo..
 *
 * Revision 3.34  1995/04/15  07:10:54  lindner
 * Fix for fclose, AIX
 *
 * Revision 3.33  1995/02/23  21:53:10  lindner
 * Fix for local Wais searches returning whole document
 *
 * Revision 3.32  1995/02/11  06:18:58  lindner
 * Rid the world of MacIndex
 *
 * Revision 3.31  1995/02/06  22:15:18  lindner
 * FREEWAIS_SF ifdefs
 *
 * Revision 3.30  1995/01/04  17:36:11  lindner
 * Add special keywords search code
 *
 * Revision 3.29  1994/12/12  17:41:23  lindner
 * Hack around AIX
 *
 * Revision 3.28  1994/12/10  06:12:56  lindner
 * Add field searching support for FreeWAIS-sf
 *
 * Revision 3.27  1994/11/30  21:54:48  lindner
 * Fix for one last Gticket problem
 *
 * Revision 3.26  1994/11/14  20:59:10  lindner
 * close fds when searching sidefiles in WAIS
 *
 * Revision 3.25  1994/11/13  06:28:56  lindner
 * Fix for FreeWAIS_SF
 *
 * Revision 3.24  1994/11/07  19:55:48  lindner
 * Modifications to use FreeWAIS-sf package (see Makefile.config)
 *
 * Revision 3.23  1994/10/13  05:17:46  lindner
 * Compiler complaint fixes
 *
 * Revision 3.22  1994/07/31  03:45:18  lindner
 * Fix one last GStoNet call
 *
 * Revision 3.21  1994/07/22  15:02:52  lindner
 * Fix bugs..
 *
 * Revision 3.20  1994/06/03  06:25:01  lindner
 * Fix for NO_FREEWAIS switch
 *
 * Revision 3.19  1994/05/11  02:46:51  lindner
 * Add #define for older WAIS libraries
 *
 * Revision 3.18  1994/04/25  20:49:02  lindner
 * Fix for debug code
 *
 * Revision 3.17  1994/04/19  21:50:20  lindner
 * Put back pseudo veronica Wais item, fix for number of arguments
 *
 * Revision 3.16  1994/04/13  04:26:35  lindner
 * Fix for freewais (again)
 *
 * Revision 3.15  1994/04/13  03:40:43  lindner
 * Remove logfile definitions so the code works with freeWAIS 0.3... sigh..
 *
 * Revision 3.14  1994/03/31  22:43:35  lindner
 * Fix for max hit params
 *
 * Revision 3.13  1994/03/08  15:55:22  lindner
 * gcc -Wall fixes
 *
 * Revision 3.12  1993/11/02  06:02:39  lindner
 * HTML Mods
 *
 * Revision 3.11  1993/09/30  16:56:49  lindner
 * Fix for WAIS and $ requests
 *
 * Revision 3.10  1993/09/11  04:41:43  lindner
 * Don't fork for localhost mindex databases
 *
 * Revision 3.9  1993/08/23  18:33:35  lindner
 * Fix bug with DGs patch
 *
 * Revision 3.8  1993/08/23  02:34:58  lindner
 * Limit # of hits
 *
 * Revision 3.7  1993/07/27  05:27:37  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.6  1993/07/26  15:31:10  lindner
 * mods for application/gopher-menu
 *
 * Revision 3.5  1993/07/20  23:55:38  lindner
 * LOGGopher mods
 *
 * Revision 3.4  1993/06/11  16:49:10  lindner
 * Fix for freeWAIS and Gopher+ requests
 *
 * Revision 3.3  1993/04/09  16:24:03  lindner
 * Hacks to support WAIS GIF, TIFF, DVI types
 *
 * Revision 3.2  1993/03/26  19:46:52  lindner
 * First crack at gopherplussing Indexing
 *
 * Revision 3.1.1.1  1993/02/11  18:02:50  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.5  1993/01/30  23:55:36  lindner
 * Better error messages, changed a uchdir to a rchdir
 *
 * Revision 1.4  1993/01/05  02:41:28  lindner
 * .cap files are now ignored by the indexer
 *
 * Revision 1.3  1993/01/01  00:12:41  lindner
 * Fixed parameters to GDnew()
 *
 * Revision 1.2  1992/12/21  20:36:44  lindner
 * Added #include for cutil.h (from dgg)
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#if defined(WAISSEARCH)


int ShowDate = 0;

#define _search_c

#include "gopherd.h"
#include "Debug.h"

#if defined(_AIX) || defined(NeXT)
#define ANSI_LIKE
#endif

#include "irext.h"
#include "irsearch.h"
#include "docid.h"
#include "irtfiles.h"
#include "cutil.h"    /** fix for -DBIO wais needs.. **/
#include <math.h>
GopherDirObj *GDfromUFS();

#ifdef NO_FREEWAIS
FILE *logfile = NULL;
char *log_file_name = NULL;
#endif



static char *DefaultDB = "index";


#if defined(void)
#undef void
#endif

char *log_file_name;
void EveryWAISdocument();

static void
Process_Veronica(besthit, gs)
  hit *besthit;
  GopherObj *gs;
{
     FILE *ZeFile;
     char veronicabuf[1024];
     char *data, *cp;

     /*** Open up the file and seek to the right position ***/

     ZeFile = ufopen(besthit->filename, "r");

     if (ZeFile == NULL)
	  return;

     fseek(ZeFile, besthit->start_character, 0);

     bzero(veronicabuf, sizeof(veronicabuf));
     fread(veronicabuf, 1, besthit->end_character - besthit->start_character,
	   ZeFile);
     veronicabuf[besthit->end_character - besthit->start_character+1] = '\0';
     
     
     data = veronicabuf;
     GSsetType(gs, *data);
     
     ZapCRLF(data);
     
     cp = strchr(data, '\t');
     *cp = '\0';
     GSsetTitle(gs, data+1);
     
     data = cp+1;
     cp = strchr(data, '\t');
     *cp = '\0';
     GSsetPath(gs, data);
     
     data = cp + 1;
     cp = strchr(data, '\t');
     *cp = '\0';
     GSsetHost(gs, data);
     
     GSsetPort(gs, atoi(cp+1));

     fclose(ZeFile);
}

static void
ProcessKeywordsQuery(sockfd, besthit, gs, INDEXPath)
  int sockfd;
  hit *besthit;
  GopherObj *gs;
  char *INDEXPath;
{
     GopherDirObj *gd;
     char *cp, *dirname;
     char tmppath[256];
     int num;
     
     Debug("Got keyword index item %s...", besthit->headline);
     dirname = besthit->filename + strlen(INDEXPath);
     *tmppath = '0'; 
     strcpy(tmppath+1, dirname);
     *(tmppath + strlen(tmppath) - 9) = '\0';
     cp = strrchr(dirname, '/');
     if (cp == NULL)
	  return;

     *cp = '\0';
     
     gd = GDfromUFS(dirname, sockfd, TRUE);
     num = GDSearch(gd, tmppath);
     if (num != -1) {
	  GScpy(gs, GDgetEntry(gd, num));
     }
     ;
}




void
WaisIndexQuery(sockfd, index_directory, SearchWords, new_db_name, INDEXHost, INDEXPort, INDEXPath, isgplus, view)
  int sockfd;
  char *index_directory;
  char *SearchWords;
  char *new_db_name;
  char *INDEXHost;
  int  INDEXPort;
  char *INDEXPath;
  boolean isgplus;
  char *view;
{ 
     database*            db;
     long                 maxRawScore, normalScore, i;
     char                 *cp, *Selstrout;
     query_parameter_type parameters;
     boolean              search_result;
     char                 score[6];
     char                 ReturnLine[512];
     boolean              plusdirs=FALSE;
     char		  words_used[1024];
                                        
     char                 * sidename;    /* mtm 11-23-92 */
     FILE                 * SideFile = NULL;  /* mtm 11-23-92 */

     GopherDirObj         *gd;
     GopherObj            *gs;

     /*** For FreeWAIS-sf ***/
#ifdef FREEWAIS_SF
     long number_of_fields = 0;
     int  global_dictionary_exists = 0;
     extern char *field_name_array[];
     char field_name[1024 + 1];
#endif

     gs = GSnew();
     gd = GDnew(32);

     if (view != NULL) {
	  if (strcasecmp(view, "application/gopher+-menu")==0)
	       plusdirs = TRUE;
     }

     Debug("WaisIndexQuery: IndexPath: %s\n", INDEXPath);

     if (new_db_name == NULL) {
	  new_db_name = DefaultDB;
     }
     if (view != NULL) {
	  if (strcasecmp(view, "application/gopher+-menu")==0)
	       plusdirs = TRUE;
     }


     if (uchdir(index_directory))
	  Die(sockfd, 500, "Couldn't change to index directory '%s'",index_directory); 

     if (SearchWords != NULL && strlen(SearchWords) == 0) {
	  EveryWAISdocument(sockfd,new_db_name,INDEXHost, INDEXPort, INDEXPath);
	  return;
     }
/*********************/

#ifdef FREEWAIS_SF
{
     char *SeedWords = NULL;
     
     if((SeedWords = analyse_string(SearchWords,
				    &number_of_fields, 
				    &global_dictionary_exists)) != NULL)
	  SearchWords = SeedWords;
}
#endif
/*********************/
     db = openDatabase(new_db_name, false, true
#ifdef FREEWAIS_SF
		       , (number_of_fields > 0)?true:false
#endif
		       );


     if (db == NULL) {
	  Die(sockfd, 404, "Failed to open database %s in index dir %s", 
	      new_db_name, index_directory);
	  return;
     }


#ifdef FREEWAIS_SF
    if (number_of_fields > 0) {
	 insert_fields(field_name_array, number_of_fields, db);
	 if(!open_field_streams_for_search(field_name, global_dictionary_exists, db))
	      Die(sockfd, 400, "Field does not exist: %s\n", field_name);
    }


#endif

     
#ifdef BIO            /* dgg */
{
     char *cp= read_delimiters( db);  /* use data-specific delim, available */

     if (cp != NULL) {
	  strcpy( gDelimiters, cp);
	  wordDelimiter= wordbreak_user;
     }
     else
	  wordDelimiter= wordbreak_notalnum;
}
#endif
 
parameters.max_hit_retrieved = 256;


/* check query string for ">max_retrieve" number */
{
     char *hitcp;
     int  ihit;

     hitcp = strrchr( SearchWords, '>');

     if ((hitcp != NULL) && (hitcp > SearchWords) && isdigit(*(++hitcp)) ) {
	  ihit= atoi( hitcp);
	  if (ihit>0) {
	       parameters.max_hit_retrieved = ihit;
	       *(--hitcp) = '\0';  /* drop rest of string */
	  }
     }
}

     set_query_parameter(SET_MAX_RETRIEVED_MASK, &parameters);
     
     search_result = false;
     words_used[0] = '\0';      /* what IS this, anyway? */



     search_result |= search_for_words(SearchWords, db, 0, words_used);
     
     if (isgplus)
         GSsendHeader(sockfd, -1);

     if (search_result == true) {
	  /* the search went ok */
	  hit best_hit;
	  
	  finished_search_word(db);
	  Debugmsg("WaisIndexQuery:After finished_search\n");

	  uchdir(Data_Dir); /* necessary to find side files */


	  for (i = 0; i < parameters.max_hit_retrieved; i++){ 
	       if (0 != next_best_hit(&best_hit, db))
		    break;		/* out of hits */
	       if (i == 0)
		    maxRawScore = best_hit.weight;
	       if (best_hit.weight > 0 && 
		   strstr(best_hit.filename, ".cache")==NULL &&
		   strstr(best_hit.filename, ".cap/")==NULL){

		    normalScore = (long)floor((((double)best_hit.weight) /
					       ((double)maxRawScore)) *	
					      (MAX_NORMAL_SCORE + 1));

		    if (normalScore > MAX_NORMAL_SCORE)
			 normalScore = MAX_NORMAL_SCORE;
		    

		    /*** Strip off the first part of the path in the filename*/
		    /*** Plus it gets rid of weird automount things... ***/
		    Selstrout =strstr(best_hit.filename, INDEXPath);
		    if (Selstrout == NULL)
			 Selstrout = "Error in Hostdata!";
		    else
			 Selstrout += strlen(INDEXPath);
		    

                    snprintf(score, sizeof(score),
			     "%3d ",best_hit.weight);

		    /*  Causes freewais to core dump... 
                    waislog(0,99,"%s: Score %3d:%s",SearchWords,best_hit.weight,Selstrout);*/
		    
		    /** Make the outgoing string **/

		    ZapCRLF(best_hit.headline);
		    
		    /*** Remove the gopher data directory pathname if
		         it's there from the headline
		    ***/

		    if ((cp = strstr(best_hit.headline, INDEXPath)) != NULL) {
			 /*** Dangerous.... ***/
			 strcpy(cp, cp+strlen(INDEXPath));
		    } 
			 
		    if ((strstr(best_hit.type, "PS") != NULL)
			|| (strstr(best_hit.type, "DVI") != NULL)
			|| (strstr(best_hit.type, "GIF") != NULL)
			|| (strstr(best_hit.type, "TIFF") != NULL))
			 GSsetType(gs, A_IMAGE);
		    else
			 GSsetType(gs, A_FILE);

		    GSsetTitle(gs, best_hit.headline);
		    GSsetHost(gs, INDEXHost);
		    GSsetPort(gs, INDEXPort);

		         /* removed "/" from following line (before %s) . 
			    Was getting double slash at least with w8b5bio; 
			    mtm 11-23-92 */

		    snprintf(ReturnLine, sizeof(ReturnLine),
			     "R%d-%d-%s",
			     best_hit.start_character, best_hit.end_character,
			     Selstrout);
		    
		    GSsetPath(gs, ReturnLine);
		    GSsetWeight(gs, best_hit.weight);
		    
                    /* 
		     * Find and process sidefile. 
		     * Allow worst case name length. 
		     */

		    if((sidename = (char *) malloc((unsigned) 
		        strlen(Selstrout) + 
	                strlen("/.cap/") + 1)) != NULL) {
		      if((cp = mtm_basename(Selstrout)) != Selstrout) {
			/*  turn "/foo/bar/baz" into "/foo/bar/.cap/baz" */
			strncpy(sidename,Selstrout,(cp - Selstrout));
			*(sidename + (cp - Selstrout)) = '\0';
			strcat(sidename,".cap/");
			strcat(sidename,cp);
		      }
		      else {
		      /* root of the gopher tree, this is easier... */
			strcpy(sidename,"/.cap/");
			strcat(sidename,Selstrout);
		      }
		      if ((SideFile = rfopen(sidename, "r")) != NULL) {
			   Debug("Side file name: %s\n", sidename);
			   Process_Side(SideFile, gs);
			   fclose(SideFile);
		      }
		      free(sidename);
		    }
		    
		    Debug("Doc type is %s\n", best_hit.type);
                    if (strcmp(best_hit.type, "GOPHER")==0) {
			 Debug("Got a veronica style thing %s\n",best_hit.headline);
			 Process_Veronica(&best_hit, gs);
		    }
		    if (strcmp(best_hit.type, "GOPHER_KEYWORDS")==0) {
			 ProcessKeywordsQuery(sockfd, &best_hit, gs, INDEXPath);
		    }
		    if (plusdirs)
			 GSplustoNet(gs, sockfd, NULL, Gticket);
		    else
			 GStoNet(gs,sockfd, GSFORM_G0, Gticket);
	       }
	       
	       Debug("WaisIndexQuery ReturnLine=%s\r\n", ReturnLine);
	       Debug("WaisIndexQuery: End=%d;", best_hit.end_character);
	       Debug("Length=%d;", best_hit.document_length);
	       Debug("lines=%d\r\n", best_hit.number_of_lines);
	  }
     }
     else {
	  /* something went awry in the search */
	  LOGGopher(sockfd, "Something went wrong in the search!");
	  return;
     }
     finished_best_hit(db);

     /* free everything */
     closeDatabase(db);
     return;
}

void
EveryWAISdocument(sockfd, db, INDEXHost, INDEXPort, INDEXPath)
  int sockfd;
  char *db;
  char *INDEXHost;
  int  INDEXPort;
  char *INDEXPath;
{
     FILE         *dbcatalog;
     char         db_name[MAXPATHLEN];
     char         inputline[512];
     String       *Headline;
     String       *Filename;
     int          StartByte, EndByte;
     GopherObj    *gs;
     GopherDirObj *gd;
     boolean      Headlineset = FALSE;
     boolean      DocIDset    = FALSE;

     gs = GSnew();
     gd = GDnew(32);
     Headline = STRnew();
     Filename = STRnew();

     strcpy(db_name, db);
     strcat(db_name, ".cat");

     dbcatalog = rfopen(db_name, "r");
     
     while (fgets(inputline, sizeof(inputline), dbcatalog) != NULL) {
	  if (strncmp(inputline, "Headline: ", 10)==0) {
	       STRset(Headline, inputline +10);
	       Headlineset = TRUE;
	  }
	  else if (strncmp(inputline, "DocID: ", 7)==0) {
	       char *cp;

	       StartByte = atoi(inputline);
	       cp = strchr(inputline+7, ' ');
	       if (cp == NULL) break;

	       cp++;
	       EndByte = atoi(cp);

	       cp = strchr(inputline+7, ' ');
	       cp++;
	       if (cp == NULL) break;

	       cp =strstr(cp, INDEXPath);
	       if (cp == NULL) break;
	       
	       STRset(Filename, cp);

	       DocIDset = TRUE;
	  }
	  
	  if (DocIDset == TRUE && Headlineset == TRUE) {
	       char tmppath[512];
	       Extobj *ext = EXnew();

	       if (GDCBlockExtension(Config, STRget(Filename), ext)) {
		    GSsetType(gs, EXgetObjtype(ext));
	       } else
		    GSsetType(gs, '0');
		    
	       EXdestroy(ext);

	       snprintf(tmppath, sizeof(tmppath),
			"R%d-%d-%s", StartByte, EndByte, STRget(Filename));

	       GSsetTitle(gs, STRget(Headline));
	       GSsetHost(gs, INDEXHost);
	       GSsetPort(gs, INDEXPort);
	       GSsetPath(gs, tmppath);

	       GDaddGS(gd, gs);

	       DocIDset = FALSE;
	       Headlineset = FALSE;
	  }
     }	  
}

#endif /** WAISSEARCH **/
