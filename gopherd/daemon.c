/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/daemon.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: daemon.c
 * Routines to detach from and run as a daemon.
 *********************************************************************
 * Revision History:
 * $Log: daemon.c,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.25  1996/01/04  18:30:11  lindner
 * Fix for Ustat on Linux, autoconf changes
 *
 * Revision 3.24  1995/11/12  03:41:43  lindner
 * Hmmm, fix for sigcld...
 *
 * Revision 3.23  1995/11/06  20:53:08  lindner
 * Waiting fixes
 *
 * Revision 3.22  1995/09/25  05:02:31  lindner
 * Convert to ANSI C
 *
 * Revision 3.21  1995/06/09  04:48:38  lindner
 * none
 *
 * Revision 3.20  1995/04/15  08:08:50  lindner
 * daemon.c
 *
 * Revision 3.19  1995/04/15  07:13:16  lindner
 * Fix for waitpid stuff
 *
 * Revision 3.18  1995/02/13  19:11:01  lindner
 * Fix for AIX
 *
 * Revision 3.17  1995/02/11  06:20:23  lindner
 * Modifications to keep track of our children..
 *
 * Revision 3.16  1994/12/12  17:41:37  lindner
 * Hack around AIX
 *
 * Revision 3.15  1994/10/20  04:21:46  lindner
 * Rollback AUX changes (they didn't work!)
 *
 * Revision 3.14  1994/10/02  02:32:58  lindner
 * Fix for AUX zombie handler..
 *
 * Revision 3.13  1994/08/03  04:22:40  lindner
 * Fix for svr4 irix systems
 *
 * Revision 3.12  1994/03/17  05:53:38  lindner
 * Decouple daemon.c from gopherd.h
 *
 * Revision 3.11  1994/03/17  04:28:44  lindner
 * Fix for vms include files
 *
 * Revision 3.10  1993/10/20  03:32:45  lindner
 * daemon.c
 *
 * Revision 3.9  1993/09/22  04:50:29  lindner
 * Fix for wait() probs on NeXT
 *
 * Revision 3.8  1993/09/22  04:26:22  lindner
 * Fix for sgi daemon code..
 *
 * Revision 3.7  1993/08/16  16:27:35  lindner
 * Fix for the error 'wait error (No children)
 *
 * Revision 3.6  1993/08/03  20:43:22  lindner
 * Fix for core dumps on HPUX 8.0
 *
 * Revision 3.5  1993/07/26  20:28:58  lindner
 * Fixed mode for uopen
 *
 * Revision 3.4  1993/07/23  03:20:02  lindner
 * Zombie killer code...
 *
 * Revision 3.3  1993/07/07  19:38:34  lindner
 * fix for linux and svr4
 *
 * Revision 3.2  1993/03/24  20:18:04  lindner
 * Fixes for SCO3.2
 *
 * Revision 3.1.1.1  1993/02/11  18:02:50  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#ifdef unix
#  include <sys/param.h>
#endif

#include <stdio.h>
#include <errno.h>
extern int errno;
#include <signal.h>

#include "gopherd.h"

#ifndef NOFILE
#  define NOFILE (100)
#endif

/*
 * A BSD style SIGCLD signal handler that can be used by a server
 * that's not interested in its child's exit status, but needs to
 * wait for them, to avoid clogging up the system with zombies.
 *
 * Beware that the calling process may get an interrupted system
 * call when we return, so they had better handle that.
 *
 * (From Stevens pg 82)
 */

#include "Wait.h"

#ifdef hpux
#define TIOCNOTTY       _IO('t', 113)           /* void tty association */
#endif

void
sig_child()
{
     /*
      * Use the waitpid() system call with the WNOHANG option
      */

     int pid;
     Portawait status;

     /** waitpid only once.. **/

     if ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0)
	  ActiveSessions--;
}


#ifdef SIGCHLD
static RETSIGTYPE
sig_sigchld(int signo)
{
     pid_t pid;
     Portawait  status;

     if (signal(SIGCHLD, sig_sigchld) == SIG_ERR) 
	  err_sys("signal error");
     
     if ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	  ActiveSessions--;

}
#endif /* SIGCHLD */

#ifdef SIGCLD
static RETSIGTYPE
sig_sigcld(int signo)
{
     pid_t pid;
     Portawait  status;

/*     pid = wait(&status);*/


     if ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	  ActiveSessions--;

     signal(SIGCLD, sig_sigcld);
     return;
}
#endif /* SIGCLD */


#if defined(SIGTSTP) && !defined(M_XENIX) && !defined(USG)/* True on a BSD system */
#include <sys/file.h>
#include <sys/ioctl.h>
#endif

/*
 * Detach a daemon process from login session context
 * Nonzero ignsigcld-> nuke zombie children
 */

void
daemon_start(int ignsigcld)
{
     register int childpid, fd;

     /*
      * If we were started by init (process 1) from the /etc/inittab
      * file there's no need to detach.
      */
      
     if (getppid() != 1) {

	  

#ifdef SIGTTOU
	  signal(SIGTTOU, SIG_IGN);
#endif
#ifdef SIGTTIN
	  signal(SIGTTIN, SIG_IGN);
#endif
#ifdef SIGTSTP
	  signal(SIGTSTP, SIG_IGN);
#endif

	  /*
	   * If we were not started in the background, fork and let
	   * the parent exit.  This also guarantees the first child
	   * is not a process group leader.
	   */

	  if ( (childpid = fork()) < 0)
	       err_sys("can't fork first child");
	  else if (childpid >0)
	       exit(0);     /* parent goes bye-bye */
	  
	  /*
	   * First Child process
	   *
	   * Disassociate from controlling terminal and process group.
	   * Ensure the process can't reacquire a new controlling terminal
	   */
	  
/* Need to add defined(TIOCNOTTY) because SCO2.3.2 has SIGTSTP defined
   but is NOT BSD and doesnt have TIOCNOTTY */

#if defined(SIGTSTP) && !defined(USG) && defined(TIOCNOTTY)  /* BSD */
	  

#if defined(HAVE_SETSID)
	  setsid();
#else
	  if (setpgrp(0, getpid()) == -1)
	       err_sys("can't change process group");
	  
	  if ( (fd = uopen("/dev/tty", O_RDWR,000)) >= 0) {
	       ioctl(fd, TIOCNOTTY, (char *)NULL); /* lose controlling TTY*/
	       close(fd);
	  }
#endif /* hpux  et al.*/
	  
#else /* System V */
	  
	  if (setpgrp() == -1)
	       err_sys("Can't change process group");
	  
	  signal(SIGHUP, SIG_IGN);  /* immune from pgrp leader death */
	  
	  if ( (childpid = fork()) < 0)
	       err_sys("Can't fork second child");
	  else if (childpid > 0)
	       exit(0); /* First child */
	  
	  /* second child */
#endif

     }  /* End of if test for ppid == 1 */

     /** Close any file descriptors **/

     for (fd = 0; fd < NOFILE; fd++)
	  close(fd);

     errno = 0;  /* probably set to EBADF from a close */


     /** Change the current directory to / **/

     /* No need for this, since we're already where we want to be */
     /* rchdir("/"); */

     /** Clear inherited file mode creation mask. **/

     umask(0);

     /*
      * See if the caller isn't ineterested in the exit status of its
      * children, and doesn't want to have them become zombies
      */

     if (ignsigcld) {
#if defined(SIGCLD) /*&& !defined(_AIX)*/
	  if (signal(SIGCLD, sig_sigcld))
	       perror("Signal Error..");    /* System V */
#endif

	  /**  Lots of brain dead systems equate SIGCLD with SIGCHLD

	       This is wrong since the semantics are different between the two

	       But to insure no zombies do SIGCLD first, then SIGCHLD... **/

#if defined(SIGCHLD) && !defined(_AUX_SOURCE) && !defined(__svr4__) &&           !defined(USG) && !defined(SYSV) && !defined(hpux) && !defined(sgi) && !defined(_AIX)
	  if (signal(SIGCHLD, sig_sigchld))
	       perror("signal error");
#endif

     }
}



