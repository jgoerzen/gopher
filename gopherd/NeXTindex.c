/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2001/01/17 19:30:25 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/NeXTindex.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: NeXTindex.c
 * index interface to the NeXT text indexing routines. 
 *********************************************************************
 * Revision History:
 * $Log: NeXTindex.c,v $
 * Revision 1.2  2001/01/17 19:30:25  jgoerzen
 * Change many sprintf -> snprintf
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.14  1995/09/26  04:59:43  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.13  1995/09/25  05:02:30  lindner
 * Convert to ANSI C
 *
 * Revision 3.12  1995/02/20  23:36:49  lindner
 * Fix for searches
 *
 * Revision 3.11  1995/02/11  06:18:57  lindner
 * Rid the world of MacIndex
 *
 * Revision 3.10  1994/10/24  22:16:53  lindner
 * Fix for printf error message
 *
 * Revision 3.9  1994/06/29  05:25:27  lindner
 * Add Gticket
 *
 * Revision 3.8  1993/11/02  06:02:38  lindner
 * HTML Mods
 *
 * Revision 3.7  1993/09/22  05:40:40  lindner
 * Remove extra fixfile() call
 *
 * Revision 3.6  1993/09/11  04:41:14  lindner
 * Don't fork for localhost mindex databases
 *
 * Revision 3.5  1993/07/27  05:27:35  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.4  1993/07/26  15:31:09  lindner
 * mods for application/gopher-menu
 *
 * Revision 3.3  1993/04/09  16:24:49  lindner
 * Fixes for Gopher+ operation
 *
 * Revision 3.2  1993/03/26  19:46:49  lindner
 * First crack at gopherplussing Indexing
 *
 * Revision 3.1.1.1  1993/02/11  18:02:50  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#include <sys/stat.h>
#include "text/wftable.h"
#include "text/ix.h"

#include "gopherd.h"
#include "Debug.h"

static int
myInterruptRoutine()
{
     /* for now, always return 0 so the search is not interrupted */
     return(0);
}

void
NeXTIndexQuery(int sockfd,
  char *SearchWords,
  char *ZIndexDirectory,
  char *DatabaseNm,  /*** Not used by the next indexer... ***/
  char *INDEXHost,
  int INDEXPort,
  char *INDEXPath,
  boolean isgplus,
  char *view)
{
     unsigned long i;
     char          *cp;
     int           j;
     Index         *workingIndex;
     RefList       theRefList;
     RefList       *ptrtheRefList;
     Reference     *MyReference;
     FileCell      *f;
     char          tempstr[40];
     char          outputline[1024];
     GopherObj     *gs;
     GopherDirObj  *gd;
     boolean       plusdirs = FALSE;


     if (view != NULL) {
	  if (strcasecmp(view, "application/gopher+-menu")==0)
	       plusdirs = TRUE;
     }


     gs = GSnew();
     gd = GDnew(32);

     Debug("Nextindexer called: Search %s,", SearchWords);
     Debug(" Indexdir %s\r\n", ZIndexDirectory);


     for (j=0; j< 4; j++) {
	  
	  workingIndex = ixOpen( ZIndexDirectory, "r" );
	  if (workingIndex != NULL)
	       break;
	  else
	       usleep (50);
     }

     if ( workingIndex != 0 ) {
	  theRefList = ixIndexQuery(workingIndex, SearchWords, ixSearchByFullWord,
				    ixMatchContent, ixLiteralString,
				    (myInterruptRoutine));

	  for( i=0; i < theRefList.n; i++ ){
	       MyReference = &(theRefList.r[i]);
	       f = (*MyReference).f;
	       
	       /*** The Selector String ***/
	       /*** So far we only index text files, so put a 0 in front ***/

	       if (strstr((*f).file, ".cache") != NULL) {
		    continue;
	       }
	       
	       GSsetType(gs, '0');
	       /*** Process the description field, remove any crud, replace
                    with spaces. ***/
               {
			char *moo = f->desc;
			while (*moo != '\0') {
				if (!isprint(*moo))
					*moo = ' ';
				moo++;
			}
		}
		
	       GSsetTitle(gs, (f->desc)+1);
	       GSsetHost(gs, INDEXHost);
	       GSsetPort(gs, INDEXPort);

	       cp = strstr(f->file, INDEXPath);
	       if (cp == NULL)
		    snprintf(outputline, sizeof(outputline), "0/%s", f->file);
	       else
		    snprintf(outputline, sizeof(outputline), "0/%s", cp);

	       GSsetPath(gs, outputline);
	       GSsetWeight(gs, (int)(MyReference->weight * 1000.0));
	       GDaddGS(gd, gs);
	  }

	  if (isgplus) {
	       GSsendHeader(sockfd, -1);
	  }

	  if (plusdirs)
	       GDplustoNet(gd, sockfd, NULL, Gticket);
	  else
	       GDtoNet(gd, sockfd, GSFORM_G0, Gticket, NULL);

     }
     else {

	  Die(sockfd, 500, "can't open working index" );
     }

     GSdestroy(gs);
     GDdestroy(gd);
     
     /* all done.... close the index file */
}

