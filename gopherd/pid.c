/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2001/01/17 19:30:25 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/pid.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991,92,93,94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: pid.c
 * Routines to implement a Maximum gopherd sessions directory
 *********************************************************************
 * Revision History:
 * $Log: pid.c,v $
 * Revision 1.2  2001/01/17 19:30:25  jgoerzen
 * Change many sprintf -> snprintf
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.7  1995/02/11  06:19:49  lindner
 * don't use crummy pid routines..
 *
 * Revision 3.6  1994/07/31  03:46:14  lindner
 * Make PID routines optional
 *
 * Revision 3.5  1994/05/25  20:58:27  lindner
 * remove unused variable
 *
 * Revision 3.4  1994/04/08  21:03:37  lindner
 * Fix typo...
 *
 * Revision 3.3  1994/03/29  18:48:26  lindner
 * Add a pid clean routine
 *
 * Revision 3.2  1994/03/17  21:15:18  lindner
 * Fix
 *
 * Revision 3.1  1994/03/17  04:33:36  lindner
 * New pid routines
 *
 *
 *
 *********************************************************************/

#ifdef UNUSED

#ifndef NO_AUTHENTICATION

#include "Dirent.h"  /** Needed by PIDnumprocs **/
#include <signal.h>  /** Needed by PIDclean **/
#include <stdio.h>   /** for sprintf() **/
#include <fcntl.h>   /** for open() **/
#include <errno.h>   /** for errno and ESRCH **/
#include "pid.h"     /** For prototypes and forward declarations. **/

#if defined(__STDC__) || defined(__stdc__)  /** For vsprintf etc. **/
#  include <stdarg.h>
#else
#  include <varargs.h>
#endif

/*************************************/

/*
 * This function writes out an entry in the pid directory.
 */

#if defined(__STDC__) || defined(__stdc__)
 void PIDwritef(char *piddir, const char *fmt, ...)
#else
 void PIDwritef(piddir, fmt, va_alist)
  char *piddir;
  char *fmt;
va_dcl
#endif
{
     char buf[512];
     va_list args;
#if defined(__STDC__) || defined(__stdc__)
     va_start(args, fmt);
#else
     va_start(args);
#endif
     (void) vsnprintf(buf, sizeof(buf), fmt, args);
     va_end(args);

     PIDwrite(piddir, buf);
}

void
PIDwrite(piddir, msg)
  char *piddir;
  char *msg;
{
     static int fd = -1;   /** We can keep this file open **/
     char pidstr[16];

     if (piddir == NULL)
	  return;

     if (*piddir == '\0')
	  return;

     if (fd == -1) {
	  if (chdir(piddir) == -1)
	       return;
     
	  snprintf(pidstr, sizeof(pidstr), "%d", (int)getpid());

	  fd = open(pidstr, O_WRONLY|O_CREAT|O_TRUNC,0644);
     }     

     if (fd < 0)
	  return;

     writestring(fd, msg);
}


/*
 * Returns a count of the number of items in the pid directory,
 * skips files that aren't all numbers
 */

int
PIDnumprocs(piddirstr)
  char *piddirstr;
{
     int           count = 0;
     DIR           *pids;
     struct dirent *dp;
     char          *cp;
     
     if (piddirstr == NULL)
	  return(-1);

     if (*piddirstr == '\0')
	  return(-1);

     pids = opendir(piddirstr);

     if (pids == NULL)
	  return(-1);

     
     for (dp = readdir(pids); dp != NULL; dp = readdir(pids)) {
	  cp = dp->d_name;
	  while (1) {
	       if (*cp == '\0') {
		    count++;
		    break;
	       } else if (*cp < '0' || *cp > '9')
		    break;
	       cp++;
	  }
     }
	  
     closedir(pids);
     
     return(count);
}


/*
 * Unlinks a pid from the pid dir.
 */

void
PIDdone(piddir, pid)
  char *piddir;
  int   pid;
{
     char pidstr[16];

     chdir(piddir);

     snprintf(pidstr, sizeof(pidstr), "%d", (int)pid);
     
     unlink(pidstr);

}


/*
 * Clean up the pid directory.  Send a USR2 signal to all pids listed
 * in the pid directory, if the process doesn't exist remove the pid
 * file
 *
 * Should check for how old the item is too, and kill it if it's over say 
 * an hour old..
 */

void
PIDclean(piddirstr)
  char *piddirstr;
{
     DIR           *pids;
     struct dirent *dp;
     int           pidnum;

     if (piddirstr == NULL)
	  return;

     if (*piddirstr == '\0')
	  return;

     pids = opendir(piddirstr);

     if (pids == NULL)
	  return;

     
     for (dp = readdir(pids); dp != NULL; dp = readdir(pids)) {
	  pidnum = atoi(dp->d_name);

	  if (pidnum < 5)
	       continue;  /** Don't send kill signals to low #'d processes **/

	  if (kill(pidnum, SIGUSR2) == -1)
	       if (errno == ESRCH)
		    PIDdone(piddirstr, pidnum);
     }
	  
     closedir(pids);

}


#else   /** NO_AUTHENTICATION **/


#if defined(__STDC__) || defined(__stdc__)
 void PIDwritef(char *piddir, const char *fmt, ...)
#else
 void PIDwritef(piddir, fmt, va_alist)
  char *piddir;
  char *fmt;
va_dcl
#endif
{
     ;
}

#endif

#endif /* UNUSED... */
