/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.3 $
 * $Date: 2001/01/03 22:29:33 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/gopher.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: gopher.h
 * Header file for Gopher client.
 *********************************************************************
 * Revision History:
 * $Log: gopher.h,v $
 * Revision 1.3  2001/01/03 22:29:33  s2mdalle
 * Added a bunch of includes, prototypes, compiler warning fixes.
 *
 * Revision 1.2  2000/12/27 21:25:59  s2mdalle
 * Added CleanupandExit prototype and an include for CUR* functions
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.5  1995/02/27  17:09:51  lindner
 * Add Pacbell NOBANNER patch
 *
 * Revision 3.4  1994/05/02  07:40:17  lindner
 * Mods to use setlocale()
 *
 * Revision 3.3  1994/04/25  03:37:40  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.2  1993/09/29  20:52:08  lindner
 * remove dead code
 *
 * Revision 3.1.1.1  1993/02/11  18:02:58  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1993/02/09  22:34:58  lindner
 * Stuff for RC
 *
 * Revision 1.2  1992/12/31  04:30:03  lindner
 * mods for VMS
 *
 * Revision 1.1  1992/12/10  23:32:16  lindner
 * gopher 1.1 release
 *
 *********************************************************************/


/*** All our mongo include files ***/
#include <stdio.h>

/*** Set global configuration options early ***/
#include "conf.h"

#ifdef VMS
#include <unixlib.h>
#define MAXPATHLEN FILENAME_MAX
#endif

#include "Stdlib.h"
#include <netdb.h>
#include <signal.h>
#include "String.h"
#include "Locale.h"
#include <ctype.h>
#include <errno.h>


#ifdef VMS   /** VMS does't do slashes **/
#include <file.h>

#else  /* not VMS */

#include <sys/param.h>

#include <sys/types.h>
#include <sys/file.h>

#ifdef  M_XENIX         /* SCO Xenix/UNIX */
#define MAXPATHLEN      FILENAME_MAX
#include        <sys/stream.h>
#include        <sys/fcntl.h>
#include        <sys/ptem.h>
#else
#include <fcntl.h>
#endif

#endif /* not VMS */

#define HALFLINE 40
#define MAXSTR 200           /* Arbitrary maximum string length */

/** Include definitions for our psuedo-objects. **/

#include "CURcurses.h"
#include "boolean.h"
#include "STRstring.h"
#include "GDgopherdir.h"
#include "compatible.h"
#include "util.h"
#include "gopherrc.h"

#ifdef NOBANNER
#  define TITLEPOS 0
#else
#  define TITLEPOS 2
#endif

/* Prototypes */
RETSIGTYPE CleanupandExit(int exitval);
void showfile(GopherObj *ZeGopher);

/** Get the configuration variables **/

#include "globals.h"
#include "gopherrc.h"
#include "ourutils.h"
#include "manager.h"
#include "cso.h"
#include "html2.h"
#include "CURcurses.h"
