/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/Dirent.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: Dirent.h
 * Portably include dir.h dirent.h, etc.
 *********************************************************************
 * Revision History:
 * $Log: Dirent.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.10  1996/01/04  18:25:27  lindner
 * Updates for autoconf
 *
 * Revision 3.9  1995/02/01  21:43:13  lindner
 * Put back an AIX dirent fix..
 *
 * Revision 3.8  1994/12/12  17:43:27  lindner
 * Hack around AIX
 *
 * Revision 3.7  1994/11/18  21:43:35  lindner
 * Fix error msgs on NeXTstep
 *
 * Revision 3.6  1994/03/17  04:37:12  lindner
 * Lame VMS port
 *
 * Revision 3.5  1993/12/27  16:12:07  lindner
 * Mods for Apollo Domain systems
 *
 * Revision 3.4  1993/09/30  23:18:25  lindner
 * pl8
 *
 * Revision 3.3  1993/07/13  04:03:59  lindner
 * added 386BSD
 *
 * Revision 3.2  1993/05/25  22:02:10  lindner
 * Fix problems with CDC epix systems
 *
 * Revision 3.1.1.1  1993/02/11  18:03:02  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#if !defined(GDIRENT_H) && !defined(VMS)
#define GDIRENT_H

#include <sys/types.h>

#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#include "Stat.h"

#ifndef S_ISDIR
#  define S_ISDIR(m)      (((m)&S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#  define S_ISREG(m)      (((m)&S_IFMT) == S_IFREG)
#endif


#endif /* GDIRENT_H */
