/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.5 $
 * $Date: 2002/01/08 17:36:14 $
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

#include "config.h"
#ifdef HAVE_REGEX_H
/* The following define tells glibc in Linux to pull in headers
   for BSD compatibility regex support. */
#define _REGEX_RE_COMP
#include <regex.h>
#endif
#ifdef HAVE_RE_COMP_H
#include <re_comp.h>
#endif

/*
 * Posix Regular expressions routines
 */

#if defined(REGEX_POSIX)
#  include "Malloc.h"
#  include <libgen.h>

#  define re_comp(a) REGEX_param=regcomp(a,NULL)
#  define re_exec(a) regex(REGEX_param,a)

#  undef  REGEX_POSIX
#  define REGEX_POSIX

#endif /* REGEX_POSIX */
#define _REGEX_RE_COMP

#if defined(USG) || defined(__svr4__) || defined(_AUX_SOURCE) || defined(hpux) || defined(irix) || defined(M_XENIX) || defined(SYSVREGEX)

#  include "Malloc.h"  /** For NULL **/

#  ifdef  REGEX_CODEIT
#     define INIT   register char *sp = instring;
#     define GETC() (*sp++)
#     define PEEKC()     (*sp)
#     define UNGETC(c)   (--sp)
#     define RETURN(c)   return(NULL);
#     define ERROR(c)    return("error")

#     include <regexp.h>
#     define REGEX_SYSV

#  endif /* REGEX_CODEIT */

  /*** BSDize the puppy ***/
  char *re_comp();
  int  re_exec();

#else
  /** BSD regex.. **/
  /* Or Linux... taken care of in the headers. */

#endif /* defined(....) */
