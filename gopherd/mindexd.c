/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.3 $
 * $Date: 2000/12/21 05:33:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/mindexd.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: mindexd.c
 * Routines to implement a fanout search server
 *********************************************************************
 * Revision History:
 * $Log: mindexd.c,v $
 * Revision 1.3  2000/12/21 05:33:20  s2mdalle
 * Miscellaneous code cleanups
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.23  1996/01/04  18:30:13  lindner
 * Fix for Ustat on Linux, autoconf changes
 *
 * Revision 3.22  1995/09/26  04:59:47  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.21  1995/09/25  22:06:59  lindner
 * Ansification bugs
 *
 * Revision 3.20  1995/05/01  05:42:14  lindner
 * compatibility fixes
 *
 * Revision 3.19  1995/05/01  04:00:47  lindner
 * Fixes for mindexd, etc from Steve Hsieh
 *
 * Revision 3.18  1994/11/28  03:41:31  lindner
 * Fix for conflicting env declarations
 *
 * Revision 3.17  1994/11/07  19:54:48  lindner
 * Add default timeout value for mindexd
 *
 * Revision 3.16  1994/11/05  03:19:27  lindner
 * Mindex timeouts, mindex gindexd support from Steve Hsieh
 *
 * Revision 3.15  1994/05/05  03:00:37  lindner
 * Fix for mindex file processing..
 *
 * Revision 3.14  1994/03/31  22:44:33  lindner
 * don't mess with the selector string in mindexd
 *
 * Revision 3.13  1994/03/31  21:09:57  lindner
 * Sanity check for mindexd, don't write to dead sockets
 *
 * Revision 3.12  1994/03/08  16:51:59  lindner
 * Add define to not compile in mindexd stuff
 *
 * Revision 3.11  1994/03/08  16:49:41  lindner
 * Add define to not compile in mindexd stuff
 *
 * Revision 3.10  1994/03/08  15:56:08  lindner
 * gcc -Wall fixes
 *
 * Revision 3.9  1993/10/19  21:00:47  lindner
 * Fix for mindex being stashed into the actual search terms...
 *
 * Revision 3.8  1993/09/22  04:33:15  lindner
 * pl7
 *
 * Revision 3.7  1993/09/11  04:40:39  lindner
 * Don't fork for localhost mindex databases
 *
 * Revision 3.6  1993/08/06  14:36:14  lindner
 * Fix for mindexd and gplus...
 *
 * Revision 3.5  1993/07/29  21:21:27  lindner
 * Removed excess variables
 *
 * Revision 3.4  1993/07/27  05:27:53  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.3  1993/07/20  23:52:43  lindner
 * Add .mindex if we can't open the file
 *
 * Revision 3.2  1993/07/13  03:58:44  lindner
 * fixed bug closing an fclose
 *
 * Revision 3.1.1.1  1993/02/11  18:02:56  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/29  23:27:21  lindner
 * Initial revision
 *
 *
 *********************************************************************/

#ifndef NO_MINDEXD

#include "gopherd.h"
#include "Debug.h"
#include "command.h"
#include <setjmp.h>
#include "Wait.h"
#include "index.h"

#define MAXSLAVES 64    

/* each individual connection to a remote database must be established
   within CONNECT_TIMEOUT seconds */
#ifndef CONNECT_TIMEOUT
#  define CONNECT_TIMEOUT 30
#endif


/* an established connection can remain open for a maximum of
    CONNECT_TIMEOUT seconds (we need this in order to ensure that mindexd
    will not hang indefinitely) */
#ifndef MINDEX_TIMEOUT
#  define MINDEX_TIMEOUT (3*60)
#endif

struct one_slave {
   int  port;
   char host[128];
   char pathname[512];
   int  sockfd;
   int  pid;
};

struct one_slave slaves[MAXSLAVES];

int LastSlave;
static jmp_buf env;

/* this function is called if it takes over CONNECT_TIMEOUT seconds to
   establish a connection */
static 
RETSIGTYPE server_timeout(int sig)
{
  longjmp(env,1);
}


/*  read the config file and fill the slaves struct, returns
    the number of slave servers */
static int
getconfig(char *the_filename)
{
    char *cp;
    char *local;
    int i;
    FILE *fp;
    char    line[1024];      /* line buffer for reading config */
    char    *linep;         /* pointer to 'line' */

    strcpy(line, the_filename);
    local = line;

    if ((fp = rfopen(local, "r")) == NULL) {
	 /** Try adding .mindex to the filename **/
	 local = (char *) malloc(sizeof(char*) *(strlen(local) + 10));
	 strcpy(local, the_filename);
	 strcat(local, ".mindex");
	 if ((fp = rfopen(local, "r")) == NULL) {
	      exit(1);
	 }
    }

    LastSlave = -1;
    for (;;) {
        if (fgets(line, sizeof line, fp) == NULL)
               break;  /* done */

        if ((i = strlen(line)))
               line[i-1] = '\0';  /* remove trailing newline */
        if (line[0] == '#' || line[0] == '\0')
               continue;       /* skip comment lines */
        else {
	     char *pathpart;

               LastSlave++;
               linep = line;
               /* the port number is seperated from the host name by a space */
	       /* The path is everything after the port */
	     
 	       pathpart = strchr(strchr(linep, ' ')+1, ' ');
	       if (pathpart != NULL)
		 {
		   *pathpart = '\0';
		   pathpart++;
		   cp = strchr(pathpart, ' ');
		   if (cp != NULL)
		     {
		       /* strip off trailing spaces in pathname*/
		       *cp = '\0';
		     }
		 }

               cp = strchr(linep, ' ');
               slaves[LastSlave].port = atoi(cp+1);
               *cp = '\0'; /* trim off stuff after the hostname */
               cp = slaves[LastSlave].host;
               strcpy(cp, linep);
	       cp = slaves[LastSlave].pathname;
	       if (pathpart != NULL) 
		    strcpy(cp, pathpart);
	       else
		    cp = "";
        }
    }
    fclose(fp);

    return( LastSlave );
}


/* take the query the client passed us, and send it on to the slave index
   servers, gather up their responses and return them to our client */
static void
HandleQuery(int sockfd, char *queryline )
{
     char      answerline[512];
     int       length_answer=0;
     int       i=0;
     int       childpid=0;
     Portawait status;

     GopherObj *gs;
     CMDobj    *cmd;
     
     gs = GSnew();
     cmd = CMDnew();
     
     Debug("Number slaves is %d",LastSlave);
     Debug(", query is %s\n", queryline);
     
     /* send queries to all the slaves and gather up responses */
     for (i = 0; i < ( LastSlave + 1); i++ ) {
	  
	  GSsetType(gs, '7');
	  GSsetTitle(gs,"");
	  GSsetHost(gs, slaves[i].host);
	  GSsetPort(gs, slaves[i].port);
	  GSsetPath(gs, slaves[i].pathname);
	  
	  if (strcmp(slaves[i].host, "localhost") == 0 ||
	      strcasecmp(slaves[i].host, Zehostname)  == 0) {
	       CMDsetSelstr(cmd, GSgetPath(gs));
	       CMDsetSearch(cmd, queryline);
	       CMDsetGplus(cmd, FALSE);
	       
	       Do_IndexTrans(sockfd, slaves[i].pathname+1, cmd, FALSE);
	  } else {

	       if ( (childpid = fork()) < 0)
		    err_dump("server: fork error");
	       
	       else if (childpid == 0) {     /* Child process */
		    /* fork child processes to handle each of the slaves */
		    
                    /* time how long a slave has been running,
                       and abort if hasn't finished within 
                       MINDEXTIMEOUT secs */
                    (void) signal(SIGALRM, server_timeout);
                    (void) alarm(CONNECT_TIMEOUT);

                    if (setjmp(env))
                      {
                        LOGGopher(sockfd, "mindexd: unable to connect with %s", slaves[i].host);
                        sprintf(answerline, "0\t\t\t\n0>>> NOTE: unable to connect with %s\t\t\t\n0\t\t\t\n", slaves[i].host);
                        writestring(sockfd, answerline);
                        close(slaves[i].sockfd);
                        exit(-1);
                      }

		    slaves[i].sockfd = GSconnect(gs);
		    
		    (void) alarm(0);

		    if (slaves[i].sockfd < 0) {
			 LOGGopher(sockfd, "mindexd: connection refused by %s",slaves[i].host);
			 sprintf(answerline, "0\t\t\t\r\n0>>> Error: connection refused by %s\t\t\t\r\n0\t\t\t\r\n", slaves[i].host);
			 writestring(sockfd, answerline);
			 close(slaves[i].sockfd);
			 exit(-1);

		    }

		    if (slaves[i].pathname != "")
		      {
			/* a pathname was specified */
			writestring(slaves[i].sockfd, slaves[i].pathname);
			writestring(slaves[i].sockfd, "\t");
			writestring(slaves[i].sockfd, queryline );
			writestring(slaves[i].sockfd, "\r\n");
		      }
		    else
		      {
			/* no pathname; must be trying to connect
			   to an old-style gopher server that still uses
			   gindexd */
			writestring(slaves[i].sockfd, queryline);
			writestring(slaves[i].sockfd, "\r\n");
		      }

                   /* the total allowable connection time to read data
                      from remote machine is determined by MINDEX_TIMEOUT */
                    (void) signal(SIGALRM, server_timeout);
                    (void) alarm(MINDEX_TIMEOUT);
                    if (setjmp(env))
                      {
                      LOGGopher(sockfd, "mindexd: connection to %s timed out", slaves[i].host);
                      sprintf(answerline, "0\t\t\t\r\n0>>> Error: connection to %s timed out\t\t\t\r\n0\t\t\t\r\n", slaves[i].host);
                      writestring(sockfd, answerline);
                      close(slaves[i].sockfd);
                      exit(-1);
		 }

		    for(;;) {
			 length_answer = readline(slaves[i].sockfd, answerline, 512);
                         if (length_answer > 0)
                           {
                             if (answerline[0] == '.')
                               {
                                 close(slaves[i].sockfd);
                                 exit(0);
                               }
                             else {
                               writestring(sockfd, answerline);
                             }
                           }
                         else
                           {
			     LOGGopher(sockfd, "mindexd: lost connection with %s", slaves[i].host);
			     sprintf(answerline, "0\t\t\t\n0Error: lost connection with %s\t\t\t\n0\t\t\t\n", slaves[i].host);
			     writestring(sockfd, answerline);
                             close(slaves[i].sockfd);
                             exit(-1);
                           }
		    }
	       } else {  /** Parent process **/
		    slaves[i].pid = childpid;
	       }
	       
	       
	  }
     }   
	  
     /* make sure all the children are done */
     for (i = 0; i < ( LastSlave + 1); i++ )
	  waitpid(-1, &status, 0);

     while (wait((int * ) 0) != -1) 
	  ;
     
     /* all done now, tell the client we are finished */
     writestring(sockfd, ".\r\n");
	  
}



int
do_mindexd(int sockfd, char *config_filename, char *search, int isgplus,
	   char *view)
{

     if (isgplus) 
	  GSsendHeader(sockfd, -1);

     if ((LastSlave = getconfig(config_filename)) == -1) 
         err_dump("can't read config file" );
     
     HandleQuery(sockfd, search);

     LOGGopher(sockfd, "mindex search using %s.mindex for %s", config_filename, search);

     close(sockfd); 
     return(0);
}

#else
int
do_mindexd(int sockfd, char *config_filename, char *search, int isgplus,
	   char *view)
{
     Die(sockfd, 501, "Sorry, no mindexd support compiled in this server");
}

#endif
