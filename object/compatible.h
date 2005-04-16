/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/compatible.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: compatible.h
 * Header file to make things compatible and define stuff in compatible.c
 *********************************************************************
 * Revision History:
 * $Log: compatible.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.39  1996/01/04  18:25:48  lindner
 * Updates for autoconf
 *
 * Revision 3.38  1995/10/04  22:31:54  lindner
 * Add stdio for FILE* defs.
 *
 * Revision 3.37  1995/09/25  22:18:21  lindner
 * Next update
 *
 * Revision 3.36  1995/09/25  22:07:23  lindner
 * Ansification
 *
 * Revision 3.35  1995/05/01  05:41:45  lindner
 * compatibility fixes
 *
 * Revision 3.34  1995/02/01  21:46:25  lindner
 * Remove f from fgetpwent prototype
 *
 * Revision 3.33  1995/02/01  21:43:55  lindner
 * Add ultrix to machines that don't have fgetpwent
 *
 * Revision 3.32  1994/12/15  17:30:16  lindner
 * A replacement fgetpwent
 *
 * Revision 3.31  1994/11/24  08:09:53  lindner
 * Back to regular
 *
 * Revision 3.30  1994/11/18  22:04:33  lindner
 * Fix for FreeWAIS on NeXT machines
 *
 * Revision 3.29  1994/10/13  05:26:58  lindner
 * Add case for sequents
 *
 * Revision 3.28  1994/08/19  16:27:07  lindner
 * mktime() addition, sequent handling
 *
 * Revision 3.27  1994/07/22  17:19:08  lindner
 * AUX doesn't have strcoll
 *
 * Revision 3.26  1994/07/21  22:18:07  lindner
 * Add NO_PUTENV, NO_STRCOLL
 *
 * Revision 3.25  1994/07/06  02:59:12  lindner
 * DECC has strftime()
 *
 * Revision 3.24  1994/06/29  06:40:57  lindner
 * ifndef NO_UNISTD_H, include it so we can use POSIX stuff
 * use POSIX alternatives for getdtablesize()
 * ifdef _POSIX_SOURCE use POSIX functions like getcwd(), etc. (Coopersmith)
 *
 * Revision 3.23  1994/05/14  04:14:40  lindner
 * Fix for svr4.2 on unisys
 *
 * Revision 3.22  1994/05/11  02:41:57  lindner
 * Automagically define NO_VFORK for SCO
 *
 * Revision 3.21  1994/04/25  03:36:17  lindner
 * eliminate redefinition of open to open_vmsopt() for ALPHA (Fote)
 *
 * Revision 3.20  1994/04/08  20:05:56  lindner
 * gcc -Wall fixes
 *
 * Revision 3.19  1994/04/01  04:44:49  lindner
 * Fix for VMS
 *
 * Revision 3.18  1994/03/17  04:38:21  lindner
 * VMS weird directory routines
 *
 * Revision 3.17  1994/03/08  07:28:42  lindner
 * Fix for vfork on Aix
 *
 * Revision 3.16  1994/03/08  03:23:50  lindner
 * Waitpid for vms
 *
 * Revision 3.15  1994/03/07  04:43:54  lindner
 * Add pid_t, vfork
 *
 * Revision 3.14  1993/11/05  07:24:15  lindner
 * Fixes for NeXTs
 *
 * Revision 3.13  1993/10/27  18:51:12  lindner
 * Updates for VMS files/records
 *
 * Revision 3.12  1993/09/28  19:23:56  lindner
 * update for VMS getwd()
 *
 * Revision 3.11  1993/08/16  19:40:58  lindner
 * Fixes for Sequent Dynix
 *
 * Revision 3.10  1993/08/03  20:57:42  lindner
 * Really fix it for hocky pucks
 *
 * Revision 3.9  1993/08/03  06:43:48  lindner
 * Compatibility fix for hpux and seteguid
 *
 * Revision 3.8  1993/07/27  05:30:29  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.7  1993/07/20  23:33:27  lindner
 * Fix for NeXTs and pid_t
 *
 * Revision 3.6  1993/07/14  20:37:40  lindner
 * Fixes for VMS
 *
 * Revision 3.5  1993/07/07  19:29:19  lindner
 * Added LINGER compatibility option
 *
 * Revision 3.4  1993/06/22  05:53:19  lindner
 * Added getdtablesize() option
 *
 * Revision 3.3  1993/06/15  06:12:14  lindner
 * Updates for Solaris
 *
 * Revision 3.2  1993/04/15  21:36:33  lindner
 * Emulation of geteuid calls for HPs
 *
 * Revision 3.1.1.1  1993/02/11  18:03:05  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.7  1993/01/14  21:58:02  lindner
 * Added #define that makes compatible compile fine under UCX
 *
 * Revision 1.6  1993/01/11  19:56:20  lindner
 * Fixed prototype for strcasecmp
 *
 * Revision 1.5  1993/01/09  02:25:57  lindner
 * Added definition for SIG_ERR for systems that don't have it.
 *
 * Revision 1.4  1993/01/08  23:17:20  lindner
 * Added more mods for VMS.
 *
 * Revision 1.3  1993/01/06  23:09:44  lindner
 * Added definition for AIX370 for strdup()
 *
 * Revision 1.2  1992/12/31  04:42:02  lindner
 * Changes for VMS, new switch NO_STRCASECMP
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/
/*
 * Protoize a little 
 */

#ifndef compatible_h_
#define compatible_h_
#include <config.h>		/* Import autoconf variables */


/******************** get Unix standard definitions **/
#if defined(HAVE_UNISTD_H)
#  include <unistd.h> 
#endif

/******************** for SIG_ERR **/
#include <signal.h>

#if !defined(SIG_ERR) && defined(sequent) && defined(BADSIG)
#  define SIG_ERR  BADSIG
#endif

#ifndef SIG_ERR
#  if defined(sequent) && defined(BADSIG)
#    define SIG_ERR	    BADSIG
#  else
#    ifdef __STDC__
#      define SIG_ERR         ((void (*) (int))-1)
#    else
#      define SIG_ERR         ((void (*) ())-1)
#    endif        /* __STDC__ */
#  endif
#endif        /* SIG_ERR */



/******************** for strstr() **/

#if !defined(HAVE_STRSTR)
   char *Gopher_strstr(char *, char *);
#  define strstr Gopher_strstr
#endif

/******************** for tempnam() **/

#if !defined(HAVE_TEMPNAM)
   char *tempnam(char *dir, char *pfx);
#endif

/******************************** for strdup() **/

#if !defined(HAVE_STRDUP)
   char *goph_strdup(char *);
#  define strdup goph_strdup
#endif

/********************** For bzero()/bcopy() etc */

#if !defined(HAVE_BZERO)

#define bzero(a,b)   memset(a,'\0',b)
#define bcopy(a,b,c) memcpy(b,a,c)
     
#endif

/********************* For getwd() */

#if !defined(HAVE_GETWD)
# define getwd(a) getcwd(a,sizeof(a))
#endif

/********************* For tzset() */

#if !defined(HAVE_TZSET)
  void tzset();
#endif

/******************** For strcasecmp() */

#if defined(NO_STRCASECMP) || defined(VMS) || defined(__USLC__)
#  undef NO_STRCASECMP
#  define NO_STRCASECMP
   int strcasecmp(const char *, const char *);
   int strcasecmp(const char *, const char *, size_t);
#endif

/******************** For geteuid() */

#if !defined(HAVE_SETEUID)
#  if defined (__hpux)
#    define seteuid(x) setresuid(-1,x,-1)
#    define setegid(x) setresgid(-1,x,-1)  
#  endif
#  if defined(_SEQUENT_)
#    define seteuid(x) setreuid(-1,x)
#    define setegid(x) setregid(-1,x)
#  endif
#endif


/********************* For getdtablesize() **/
#if !defined(HAVE_GETDTABLESIZE)
# if defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE) || defined(HAVE_SYSCONF)

#   ifdef _SC_OPEN_MAX	/* prefered POSIX method */
#    define    getdtablesize()	sysconf(_SC_OPEN_MAX) 
#   else /* if !defined(systems_without_limits.h) */
#    include <limits.h>
#    ifdef _POSIX_OPEN_MAX    /* alternate POSIX method */
#     define   getdtablesize()	(_POSIX_OPEN_MAX)
#    else
#     define NEED_GETDTABLESIZE 1
      int getdtablesize();
#    endif
#   endif
#  endif
#else
# define NEED_GETDTABLESIZE 1
  int getdtablesize();
#endif
/********************* For VMS */

#if defined(VMS)
#  include <stdio.h>

#  define popen(a,b) fopen(a,b)
#  define pclose(a)  fclose(a)
#  define unlink delete

#  define fopen fopen_VMSopt
   FILE *fopen_VMSopt();
#ifndef __ALPHA
#  define open open_VMSopt
   int open_VMSopt();
#endif

#  if defined(UCX)
#    define closenet close
#  endif

   FILE *fopen_VAR();
   FILE *fopen_FIX();

#else
   /* non-VMS systems don't need a special netclose either */
#  define closenet close

#endif  /** VMS **/


/**********  For linger setsockopt() ***/

#if defined(linux) || defined(_SEQUENT_)
#  define NO_LINGER
#endif

/********** For NeXTs **************/
#ifdef NeXT
#  define ANSI_LIKE  /** freeWAIS bastards! **/
#endif

/********* Systems that can't set proctitles *******/
#if defined(_AUX_SOURCE) || defined(__svr4__) || defined(apollo)
#   undef SETPROCTITLE
#endif

/********* Fix for old stat.h stuff on bsd (I think) ******/
#if defined(NeXT) || defined(sequent) || defined(NO_NEWSTATDOTH)
#define S_IXUSR S_IEXEC
#endif

/*

Now fixed by autoconf... 

#if defined(VMS) || defined(NeXT) || defined(sequent) || defined(NO_PID_T)
  typedef int pid_t;
#endif
*/

/********* strftime() - should be present in ANSI C libraries ******/
#ifndef __DECC
#if defined(sequent) || defined(NO_STRFTIME) || defined(VMS)
#  undef HAVE_STRFTIME
   int strftime(char *buf, int bufsize, char *fmt, struct tm *tm);
#endif
#endif /* __DECC */

/********* mktime() - should be present in ANSI C libraries ******/
#if defined(NO_MKTIME) || defined(sequent)
#  undef NO_MKTIME
#  define NO_MKTIME
#endif


/********* strcoll - should be present in ANSI C libraries ******/
#include "String.h"  /* include this here for broken strcolls */
#if !defined(HAVE_STRCOLL)
#  define strcoll strcmp
#endif


/******** putenv, in most systems except old BSD... *********/
#if !defined(HAVE_PUTENV)
   int putenv(char *);
#endif

/******* fgetpwent(), sortof non-standard BSD derived fcn *******/
#if !defined(HAVE_FGETPWENT)
#  include <pwd.h>
#  include <stdio.h>

   struct passwd *fgetpwent(FILE *);
#endif

#endif  /* compatible_h_ */
