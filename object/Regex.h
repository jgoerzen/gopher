/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.17 $
 * $Date: 2002/03/19 20:57:55 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/Regex.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: Regex.h
 * Portable method of doing regular expressions
 *********************************************************************
 * Revision History:
 * $Log: Regex.h,v $
 * Revision 1.17  2002/03/19 20:57:55  jgoerzen
 * Modified #warn -> #warning to use correct syntax.
 *
 * Revision 1.16  2002/03/19 20:37:28  jgoerzen
 * Updated configure stuff with regcomp and regfree checks.
 * Made Regex.h more robust with regcomp stuff.
 *
 * Revision 1.15  2002/03/19 20:07:16  jgoerzen
 * GSgopherobj.c: Moved Regex.h include up.
 *
 * Regex.c, Regex.h: continuing full rewrites.
 *
 * Revision 1.14  2002/03/19 19:58:12  jgoerzen
 * More updates.  This is part of a major rewrite.
 *
 * Revision 1.13  2002/03/19 19:53:31  jgoerzen
 * *** empty log message ***
 *
 * Revision 1.12  2002/03/19 18:07:42  jgoerzen
 * Fixed regex stuff for Solaris -- I hope!
 *
 * Updated changelog.
 *
 * More notes in Platforms.
 *
 * Revision 1.11  2002/02/12 20:57:18  jgoerzen
 * Fixed an incorrect comment and added parens
 *
 * Revision 1.10  2002/02/12 20:50:21  jgoerzen
 * Beginning of MacOS X (Darwin) support.
 * Many modifications to Regex.[ch], see debian/changelog for details.
 *
 * Revision 1.9  2002/02/12 19:54:18  jgoerzen
 * Updated with regex fixes
 *
 * Revision 1.8  2002/01/10 18:48:31  jgoerzen
 * Fix for Linux
 *
 * Revision 1.7  2002/01/10 18:43:18  jgoerzen
 *   * Changes to work with Solaris:
 *     * configure.in: Now looks for strings.h and string.h
 *     * object/Regex.h: Now include re_comp.h if available.
 *       Include regex.h only if re_comp.h is unavailable.
 *       Define the SYSVREGEX stuff only if HAVE_RE_COMP_H is not defined
 *     * object/String.h: Include string.h and strings.h based on configure
 *       test.
 *
 * Revision 1.6  2002/01/08 18:28:02  jgoerzen
 *   * object/Regex.h: now conditionally includes sys/types.h for regex.h
 *
 * Revision 1.5  2002/01/08 17:36:14  jgoerzen
 * Finally builds!
 *
 * Changes:
 *
 *   * config.h.in: rebuilt by autoheader
 *
 *   * configure: rebuilt by autoconf
 *
 *   * configure.in:
 *     * Added REGEXLIBS test for NetBSD -- look for re_comp in -lcompat
 *     * Added checkes for term.h, re_comp.h, regex.h
 *
 *   * gopher/ourutils.c, gopher/CURcurses.c: Use term.h check
 *
 *   * gopher/Makefile.in, gopherd/Makefile.in, gophfilt/Makefile.in:
 *     * Use REGEXLIBS
 *
 *   * gopher/globals.h, gopher/gopher.c: Remove sys_errlist
 *
 *   * object/GSgopherobj.c: Removed <regex.h> #include, now use
 *     "Regex.h" that has proper regex determining logic
 *
 *   * object/Regex.h: Moved regex.h include to here.  Make it conditional
 *     based on configure test.  Add conditional re_comp.h include.
 *
 * Revision 1.4  2001/02/26 21:22:08  jgoerzen
 * Updated
 *
 * Revision 1.3  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.7  1994/07/25  13:56:54  lindner
 * First crack at POSIX regular expressions
 *
 * Revision 3.6  1994/07/21  17:29:13  lindner
 * Fix typo
 *
 * Revision 3.5  1994/03/08  04:04:16  lindner
 * add regex.h for NeXT systems
 *
 * Revision 3.4  1994/03/04  17:42:00  lindner
 * Fix for SCO machines
 *
 * Revision 3.3  1994/02/20  16:24:02  lindner
 * no message
 *
 * Revision 3.2  1993/10/27  18:50:38  lindner
 * Support for more sysv platforms
 *
 * Revision 3.1  1993/10/19  20:48:26  lindner
 * Portable versions of Regular expression routines for System V and BSD..
 *
 *
 *********************************************************************/

#ifndef __GOPHER_REGEX_H__
#define __GOPHER_REGEX_H__

#include "config.h"

/*****************
 Basic POSIX support detection */

#ifdef HAVE_SYS_TYPES_H
/* POSIX and Linux regex.h implementations want sys/types.h */
#include <sys/types.h>
#endif

#ifdef HAVE_REGEX_H
#include <regex.h>
#endif

#if defined(HAVE_REGCOMP) && defined(HAVE_REGFREE)
#define REGEX_POSIX
#endif

/*************
 OK, handle the POSIX case. */

#ifdef REGEX_POSIX

/* Try including files until we get REG_EXTENDED. */

#ifndef REG_EXTENDED
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif

#ifndef REG_EXTENDED
#ifdef HAVE_REGEXP_H
#include <regexp.h>
#endif
#endif

#ifndef REG_EXTENDED
#warning REGEX_POSIX defined but REG_EXTENDED not found.
#endif

/* These are named posix_re_comp and then #defined to the original names
because MacOS X is nice enough to typedef re_comp and re_exec differently
in unistd.h.  Eww. */

char *posix_re_comp(char *regex);
int posix_re_exec(char *string);
#define re_comp(a) (posix_re_comp(a))
#define re_exec(a) (posix_re_exec(a))
#else

/* ********** Try to figure out what else they have. */

#ifdef HAVE_RE_COMP
/* This generally means that they have re_comp themselves.  Just
   include it for them and leave it. */

#ifdef HAVE_RE_COMP_H
#include <re_comp.h>
#else
#include <unistd.h>
#endif /* HAVE_RE_COMP_H */

#else
#ifdef HAVE_REGEXP_H
/* This means SYSV... */
#define SYSVREGEX
#define REGEX_SYSV
#  include "Malloc.h"  /** For NULL **/

#  ifdef  REGEX_CODEIT
#     define INIT   register char *sp = instring;
#     define GETC() (*sp++)
#     define PEEKC()     (*sp)
#     define UNGETC(c)   (--sp)
#     define RETURN(c)   return(NULL);
#     define ERROR(c)    return("error")

#     include <regexp.h>

#  endif /* REGEX_CODEIT */

/* Prototypes for Regex.c */
  char *re_comp(char *expr);
  int  re_exec(char *string);

#else

#error "Unsupported or no regexp support."

#endif /* REGEX_SYSV */
#endif /* HAVE_RE_COMP */
#endif /* REGEX_POSIX */

#endif /* __GOPHER_REGEX_H__ */

