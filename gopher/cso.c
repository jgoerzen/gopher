/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.2 $
 * $Date: 2001/01/03 22:27:10 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/cso.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: cso.c
 * Functions to support CSO qi/ph servers
 *********************************************************************
 * Revision History:
 * $Log: cso.c,v $
 * Revision 1.2  2001/01/03 22:27:10  s2mdalle
 * Inclusion of the new header cso.h that holds prototypes for this module.
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.14  1995/11/03  21:18:16  lindner
 * ANSIfication
 *
 * Revision 3.13  1994/05/19  14:07:22  lindner
 * use fast malloc on VMS VAXC
 *
 * Revision 3.12  1994/05/17  05:47:53  lindner
 * Massive internationalization change
 *
 * Revision 3.11  1994/05/14  04:13:38  lindner
 * Internationalization...
 *
 * Revision 3.10  1994/03/04  23:35:17  lindner
 * fix cso requester to be nice and big
 *
 * Revision 3.9  1994/02/20  16:30:36  lindner
 * Use FileIO routines for fetching fields
 *
 * Revision 3.8  1994/01/20  06:47:01  lindner
 * Fix params for showfile
 *
 * Revision 3.7  1993/10/22  20:21:02  lindner
 * misc fixes
 *
 * Revision 3.6  1993/10/11  04:44:09  lindner
 * Add early searching.. tag for CSO, better ui
 *
 * Revision 3.5  1993/10/07  05:11:22  lindner
 * Better memory management
 *
 * Revision 3.4  1993/07/07  19:42:15  lindner
 * fix for cancel from cso screen
 *
 * Revision 3.3  1993/06/08  06:31:01  lindner
 * Fixed infamous cached cso search, added fishing mode
 *
 * Revision 3.2  1993/05/20  06:00:38  lindner
 * Better cso support, fixed bug with unchangable search
 *
 * Revision 3.1.1.1  1993/02/11  18:02:57  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.4  1993/01/08  19:43:01  lindner
 * dialog box cancels automatically if the user doesn't enter anything.
 *
 * Revision 1.3  1992/12/31  05:53:01  lindner
 * Mods for VMS
 *
 * Revision 1.2  1992/12/28  19:02:58  lindner
 * Changed field selection criteria to be based on "Lookup"
 * not "Indexed".  Removed old dead static variables.
 * Changed the name of the popup box from "Ph Query" to the
 * name of the gopher item.
 *
 * Revision 1.1  1992/12/10  23:32:16  lindner
 * gopher 1.1 release
 *
 *********************************************************************/
 
#include "gopher.h"
#include "fileio.h"
#include "Malloc.h"
#include "cso.h"

void 
do_cso(GopherStruct *ZeGopher)
{
     char inputline[1024], *cp;
     int sockfd, len, numfields=0;
     char *Fields[50];
     char *Responses[50];
     char query[512];
     int i;
     FileIO *sock;

     Draw_Status(Gtxt("Fetching Fields...",87));
     refresh();

     /*** Fetch the indexed fields from the server ***/
     if ((sockfd = GSconnect(ZeGopher)) <0) {
	  check_sock(sockfd, GSgetHost(ZeGopher), GSgetPort(ZeGopher));
	  return;
     }

     sock = FIOopenfd(sockfd, TRUE);
     
     FIOwritestring(sock, "fields\r\n");
     
     while (1) {
	  len = FIOreadline(sock, inputline, 1024);
	  twirl();
	  if ((len <= 0) || (strncmp(inputline, "200", 3)==0))
	       break;

	  cp = inputline;
	  if (strstr(inputline, "Lookup") == NULL)
	       continue;
	  
	  cp = strrchr(inputline,':');
	  *cp = '\0';
	  cp--;
	  cp = strrchr(inputline, ':') + 1;
	  
	  /*** Put name at the top ***/
	  if (strcmp(cp, "name") == 0 && numfields != 0) {
	       Fields[numfields] = Fields[0];
	       Fields[0] = strdup(cp);
	  }
	  else
	       Fields[numfields] = strdup(cp);

	  Responses[numfields] = (char *) malloc(sizeof(char) * COLS);
	  *Responses[numfields] = '\0';
	  *(Responses[numfields]+1) = '\0';
	  numfields++;
     }
     Fields[numfields] = NULL;
     Responses[numfields] = NULL;

     FIOwritestring(sock, "quit\r\n");
     /** Read the stupid bye message **/
     FIOreadline(sock, inputline, 1024);
     FIOclose(sock);

     /*** Do cso stuff until user presses CTRL-G ***/
     while (1) {
	  clear();
	  Draw_Status("...");
	  
	  refresh();

	  if (CURRequest(CursesScreen, GSgetTitle(ZeGopher), Fields, Responses) < 0) {

	       /*** Free the memory that we just allocated ***/
	       
	       for (i=0; i<numfields; i++) {
		    free(Fields[i]);
		    free(Responses[i]);
	       }
	       return;
	  }
     
	  Draw_Status(Gtxt("Searching...",143));
	  refresh();
	  strcpy(query, "query ");
	  
	  for (i=0; i<numfields; i++) {
	       if (*Responses[i] != '\0') {
		    cp = Responses[i];
		    do {
			 len = strcspn(cp, " ");
			 if (len != 0) {
			      strcat(query, Fields[i]);
			      strcat(query, "=");
			      strncat(query, cp, len);
			      strcat(query, " ");
			 }
			 cp = strchr(cp, ' ');
			 if (cp != NULL)
			      cp++;
		    } while (cp != NULL);
	       }
	  }
	  if (strlen(query) > 6)
	       GSsetPath(ZeGopher, query);
	  else {
	       for (i=0; i<numfields; i++) {
		    free(Fields[i]);
		    free(Responses[i]);
	       }
	       return;
	  }
	  
	  refresh();
	  showfile(ZeGopher);      /* Receive response as a file */
	  unlink(GSgetLocalFile(ZeGopher));
	  GSsetLocalFile(ZeGopher, NULL);
	  
     }	  
}
