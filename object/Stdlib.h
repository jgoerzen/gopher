/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.3 $
 * $Date: 2002/02/12 19:54:18 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/Stdlib.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: Stdlib.h
 * Include stdlib.h on systems that have it.
 *********************************************************************
 * Revision History:
 * $Log: Stdlib.h,v $
 * Revision 1.3  2002/02/12 19:54:18  jgoerzen
 * Updated with regex fixes
 *
 * Revision 1.2  2000/08/23 00:03:59  jgoerzen
 * Updates
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.6  1996/01/04  18:26:50  lindner
 * Updates for autoconf
 *
 * Revision 3.5  1994/04/07  17:26:26  lindner
 * Fix for pyramids
 *
 * Revision 3.4  1994/03/07  04:42:05  lindner
 * Add sys/types.h and compatible.h
 *
 * Revision 3.3  1993/07/07  19:28:04  lindner
 * Mods for SGIs
 *
 * Revision 3.2  1993/06/15  06:07:16  lindner
 * Mods for VMS
 *
 * Revision 3.1.1.1  1993/02/11  18:03:04  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#ifndef  GSTDLIB_H
#define  GSTDLIB_H

#include "compatible.h"

/**  These definitely need stdlib.h **/
#if defined(STDC_HEADERS)
#  ifndef sony_news   /** Ack! sonynews is STDC but no stdlib! **/
#    include <stdlib.h>
#  endif
#else

/** These definitely *don't* want stdlib.h **/
#  if !defined(mips) && !defined(sequent) && !defined(n16) && !defined(NeXT) &&      !defined(ultrix) && !defined(UMAX43) && !defined(sony_news) && !defined(pyr)

#     include <stdlib.h>
#  endif
#endif

#if !defined(VMS)
#  include <sys/types.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <string.h>
#endif



#endif /* GSTDLIB_H */
