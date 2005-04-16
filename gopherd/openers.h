/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/openers.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: openers.c
 * Secure file access routines.
 *********************************************************************
 * Revision History:
 * $Log: openers.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.7  1996/01/04  18:30:14  lindner
 * Fix for Ustat on Linux, autoconf changes
 *
 * Revision 3.6  1995/09/25  05:02:38  lindner
 * Convert to ANSI C
 *
 * Revision 3.5  1994/03/17  04:12:50  lindner
 * Fix for VMS
 *
 * Revision 3.4  1993/07/30  19:10:09  lindner
 * Fix for ultrix
 *
 * Revision 3.3  1993/07/23  03:17:10  lindner
 * added stat.h
 *
 * Revision 3.2  1993/03/24  20:29:45  lindner
 * Added Dirent.h, needed for those darn directory routines.
 *
 * Revision 3.1.1.1  1993/02/11  18:02:52  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

/*
 * Declarations for openers.c routines.
 */

#include <stdio.h>
#include "Dirent.h"

#ifndef VMS
#  include <sys/types.h>
#  include <sys/stat.h>
#else
#  include <stat.h>
#endif

/* restricted versions */
extern	int	ropen(char *path, int flags, int mode);
extern	FILE   *rfopen(char *filename, char *type);
extern	int	rstat(char *path, struct stat *buf);
extern	DIR    *ropendir(char *dirname);
extern	int	rchdir(char *path);

/* unrestricted versions */
extern	int	uopen(char *path, int flags, int mode);
extern	FILE   *ufopen(char *filename, char *type);
extern	int	Ustat(char *path, struct stat *buf);
extern	DIR    *uopendir(char *dirname);
extern	int	uchdir(char *path);

extern  char   *fixfile(char*);

