/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.4 $
 * $Date: 2001/01/17 19:30:25 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/error.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: error.c
 * Error handling/logging routines.
 *********************************************************************
 * Revision History:
 * $Log: error.c,v $
 * Revision 1.4  2001/01/17 19:30:25  jgoerzen
 * Change many sprintf -> snprintf
 *
 * Revision 1.3  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.2  2000/08/19 01:33:42  jgoerzen
 * errlist fix
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.15  1996/01/04  18:30:12  lindner
 * Fix for Ustat on Linux, autoconf changes
 *
 * Revision 3.14  1995/09/28  21:11:28  lindner
 * More fixes...
 *
 * Revision 3.13  1995/09/25  22:10:14  lindner
 * Remove syslog junk
 *
 * Revision 3.12  1995/05/01  05:42:12  lindner
 * compatibility fixes
 *
 * Revision 3.11  1995/05/01  03:44:36  lindner
 * NetBSD fix
 *
 * Revision 3.10  1995/02/07  07:12:32  lindner
 * oops!
 *
 * Revision 3.9  1995/02/07  07:04:29  lindner
 * performance fixes
 *
 * Revision 3.8  1994/07/19  20:28:08  lindner
 * Fix for syslog problem with WAIS
 *
 * Revision 3.7  1994/01/20  06:37:43  lindner
 * Fix for stdc declaration for err_quit()
 *
 * Revision 3.6  1993/10/11  04:40:49  lindner
 * Changes to allow logging via daemon.info syslogd facility
 *
 * Revision 3.5  1993/07/30  17:19:53  lindner
 * Fix for NeXTs
 *
 * Revision 3.4  1993/07/20  23:58:01  lindner
 * LOGGopher mods
 *
 * Revision 3.3  1993/07/10  04:21:55  lindner
 * mods for solaris2.2
 *
 * Revision 3.2  1993/07/07  19:39:09  lindner
 * fix for linux
 *
 * Revision 3.1.1.1  1993/02/11  18:02:50  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#include "serverutil.h"		/* For LOGGopher */
#include "compatible.h"

/*
 * Error handling routines from Stevens: UNIX network programming
 * pp 722-731
 */

/*
 * Error handling routines.
 *
 * The functions in this file are independent of any application
 * variables, and may be used in any C program.  Either of the names
 * CLIENT of SERVER may be defined when compiling this function.
 * If neither are defined, we assume SERVER.
 */

void my_perror(void);

#ifndef NULL
#define NULL ((void *) 0)
#endif

extern char *pname;

#if defined(HAVE_SYSLOG_H)
 /*
  * Under useful OS's, these server routines use the syslog(3) facility.
  * They don't append a newline for example.
  */

/*
 * Get around the existance of syslog.h in the wais source code...
 * Arghhh!!!
 */
#include <syslog.h>

#else /* no syslog() */
/*
 * There really ought to be a better way to handle server logging
 * under System V
 */
#define syslog(a,b)	fprintf(stderr, "%s\n", (b))
#define openlog(a,b,c)	fprintf(stderr, "%s\n", (a))

#endif  /* NOSYSLOG */

#include <stdarg.h>

char emesgstr[255] = {0};  /* used by all server routines */

/*
 * Identify ourselves, for syslog() messages.
 *
 * LOG_PID is an option that says prepend each message with our pid.
 * LOG_CONS is an option that says write to the console if unable to
 * send the message to syslogd.
 * LOG_DAEMON is our facility.
 */

void
err_init(void)
{
	/* have to be able to reopen, since daemon_start keeps closing us */
	closelog();	/* in case we've been closed by daemon_start */
#ifdef LOG_DAEMON
	openlog("gopherd", (LOG_PID|LOG_CONS), LOG_DAEMON);
	setlogmask(LOG_UPTO(LOG_INFO));
#else
	/* old old syslog - probably Ultrix */
	openlog("gopherd", LOG_PID);
#endif
}

/*
 * Fatal error.  Print a message and terminate.
 * Don't print the system's errno value.
 *
 *	err_quit(str, arg1, arg2, ...)
 */

void err_quit(char *va_alist, ...)
{
	va_list args;
	char *fmt;


	fmt = va_alist;
	va_start(args, va_alist);

	(void) vsnprintf(emesgstr, sizeof(emesgstr), fmt, args);
	va_end(args);

	syslog(LOG_ERR, emesgstr);
	LOGGopher(-1, "%s", emesgstr);

	exit(1);
}


/*
 * Fatal error related to a system call.  Print the message and terminate.
 * Don't dump core, but do print the systems errno value and its associated
 * message.
 */

void err_sys(char *va_alist, ...)
{
	va_list args;
	char *fmt;


	fmt = va_alist;
	va_start(args,va_alist);

	(void) vsnprintf(emesgstr, sizeof(emesgstr), fmt, args);
	va_end(args);

	my_perror();
	syslog(LOG_ERR, emesgstr);
	LOGGopher(-1, "%s", emesgstr);

	exit(1);
}


/*
 * Recoverable error.  Print a message, and return to caller.
 */

void err_ret(char *va_alist, ...)
{
	va_list args;
	char *fmt;


	fmt = va_alist;
	va_start(args, va_alist);

	(void) vsnprintf(emesgstr, sizeof(emesgstr), fmt, args);
	va_end(args);

	my_perror();
	syslog(LOG_ERR, emesgstr);
	LOGGopher(-1, "%s", emesgstr);

	return;
}


/*
 * Fatal error.  Print a message, dump core and terminate.
 */

void err_dump(char *va_alist, ...)
{
	va_list args;
	char *fmt;

	fmt = va_alist;
	va_start(args, va_alist);

	(void) vsnprintf(emesgstr, sizeof(emesgstr), fmt, args);
	va_end(args);

	my_perror();
	syslog(LOG_ERR, emesgstr);
	LOGGopher(-1, "%s", emesgstr);

	abort();
}

/*
 * Print the UNIX errno value.
 * We just append it the the end of the emesgstr[] array.
 */

void my_perror(void)
{
	int len;
	char *sys_err_str();

	len = strlen(emesgstr);
	snprintf(emesgstr + len, 
		 sizeof(emesgstr) - len - 1,
		 " %s", sys_err_str());
}


extern int errno;		/* UNIX error number */
extern int sys_nerr;		/* # of error message strings in sys table */
#ifdef __NetBSD__
extern const char *const sys_errlist[];	/* the system error message table */
#else
#ifndef linux
extern char *sys_errlist[];	/* the system error message table */
#endif
#endif

#ifdef SYS5
int t_errno;
int t_nerr;
char *t_errlist[1];
#endif

/*
 * Return a string containing some additional operating system
 * dependent information.
 * Note that different versions of UNIX assign different meanings
 * to the same value of "errno".  This means that if an error
 * condition is being sent ot another UNIX system, we must interpret
 * the errno value on the system that generated the error, and not
 * just send the decimal value of errno to the other system.
 */

char *
sys_err_str(void)
{
     static char msgstr[200];

     if (errno != 0) {
	  if (errno >0 && errno < sys_nerr)
	       snprintf(msgstr, sizeof(msgstr), "(%s)", sys_errlist[errno]);
	  else
	       snprintf(msgstr, sizeof(msgstr), "(errno = %d)", errno);
     } else {
	  msgstr[0] = '\0';
     }

#ifdef SYS5
     
     if (t_errno != 0) {
	  char tmsgstr[100];
	  
	  if (t_errno > 0 && t_errno < sys_nerr)
	       snprintf(tmsgstr, sizeof(tmsgstr), " (%s)", t_errlist[t_errno]);
	  else
	       snprintf(tmsgstr, sizeof(tmsgstr), ", (t_errno = %d)", t_errno);
	  
	  strcat(msgstr, tmsgstr);
     }
#endif
     
     return(msgstr);
}



