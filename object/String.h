/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.4 $
 * $Date: 2002/01/10 18:43:18 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/String.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: String.h
 * Header file that automagically includes string/strings.h
 *********************************************************************
 * Revision History:
 * $Log: String.h,v $
 * Revision 1.4  2002/01/10 18:43:18  jgoerzen
 *   * Changes to work with Solaris:
 *     * configure.in: Now looks for strings.h and string.h
 *     * object/Regex.h: Now include re_comp.h if available.
 *       Include regex.h only if re_comp.h is unavailable.
 *       Define the SYSVREGEX stuff only if HAVE_RE_COMP_H is not defined
 *     * object/String.h: Include string.h and strings.h based on configure
 *       test.
 *
 * Revision 1.3  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.2.1 2000/12/18 23:04:29 mdallen
 * removed nested comments from unused include
 *
 * Revision 1.2  2000/08/19 01:36:18  jgoerzen
 * include more relevant header files.
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.8  1996/01/04  18:23:43  lindner
 * Updates for autoconf
 *
 * Revision 3.7  1994/07/21  22:08:24  lindner
 * include both string and strings on suns
 *
 * Revision 3.6  1994/06/29  06:38:27  lindner
 * Remove extra include of string.h
 *
 * Revision 3.5  1994/04/07  17:26:46  lindner
 * Fix for pyramids
 *
 * Revision 3.4  1994/03/08  04:03:52  lindner
 * Fix for missing strdup forward declaration
 *
 * Revision 3.3  1993/06/15  06:09:50  lindner
 * Fixes for multiple includes and prettification
 *
 * Revision 3.2  1993/05/05  18:39:49  lindner
 * Solaris and VMS mods
 *
 * Revision 3.1.1.1  1993/02/11  18:03:04  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#ifndef GSTRINGS_H
#define GSTRINGS_H

/* string.h  == Sys V & ANSI C
   strings.h == BSD

Though your milage may vary... 

*/

#if 0
#include "compatible.h"		/* For strdup, etal */
#endif /* 0 */

#ifdef __convex__
#  include <stdarg.h>
#endif

#ifdef HAVE_STRING_H
/* Define __USE_GNU for string.h so strcasestr gets included. 
   (also _GNU_SOURCE for other systems) */
#define __USE_GNU
#define _GNU_SOURCE
#include <string.h>
#undef __USE_GNU
#undef _GNU_SOURCE
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <stdio.h>
#include <errno.h>

#endif /* GSTRINGS_H */
