/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
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
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
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

/*#include "compatible.h"		/* For strdup, etal */

#ifdef __convex__
#  include <stdarg.h>
#endif

#if defined(USG) || defined(hpux) || defined(__svr4__) || defined(VMS) || defined(pyr) || defined(apollo) || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE) || defined(sun)
#include <string.h>
#else
#include <strings.h>
#endif

#endif /* GSTRINGS_H */
