/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/site.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: site.h
 * Main functions for the gopher client
 *********************************************************************
 * Revision History:
 * $Log: site.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.10  1995/09/26  05:16:23  lindner
 * more fixes...
 *
 * Revision 3.9  1995/09/25  05:02:40  lindner
 * Convert to ANSI C
 *
 * Revision 3.8  1994/07/22  22:59:40  lindner
 * More NO_AUTH stuff..
 *
 * Revision 3.7  1994/07/22  22:56:21  lindner
 * More NO_AUTH stuff..
 *
 * Revision 3.6  1994/06/29  05:31:15  lindner
 * Use an enum
 *
 * Revision 3.5  1994/03/17  21:18:26  lindner
 * Massive reworking of access limits
 *
 * Revision 3.4  1994/03/17  04:27:17  lindner
 * Add funky site defines
 *
 * Revision 3.3  1993/12/27  16:32:38  lindner
 * Fix cvs header
 *
 * Revision 3.2  1993/08/20  18:03:17  lindner
 * Mods to allow gopherd.conf files control ftp gateway access
 *
 * Revision 3.1.1.1  1993/02/11  18:02:53  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

/*
 * Definitions for security and hostname access
 */

#ifndef SITE_H
#define SITE_H

#include "STRstring.h"
#include "boolean.h"
#include "DAarray.h"
#include <errno.h>

typedef int Accesslevel;
#define ACC_BROWSE (1<<0)
#define ACC_READ   (1<<1)
#define ACC_SEARCH (1<<2)
#define ACC_FTP    (1<<3)
#define ACC_UNKNOWN (-1)

#define ACC_FULL (ACC_BROWSE | ACC_READ | ACC_SEARCH | ACC_FTP)

struct Site_struct {
     String       *domain;
     Accesslevel  Level;
     boolean      isnum;
     int          maxsessions;
};

typedef struct Site_struct Site;

#define SITEgetDomain(a)   (STRget((a)->domain))
#define SITEgetLevel(a)    ((a)->Level)
#define SITEisnum(a)       ((a)->isnum)
#define SITEmaxsessions(a) ((a)->maxsessions)

#define SITEsetDomain(a,b)      (STRset((a)->domain),b)
#define SITEsetLevel(a,b)       ((a)->Level=(b))
#define SITEsetisnum(a,b)       ((a)->isnum=(b))
#define SITEsetmaxsessions(a,b) ((a)->maxsessions=(b))


typedef DynArray SiteArray;

#define SiteArrgetEntry(a,b) (Site *)(DAgetEntry((DynArray*)a,b))
#define SiteArrDestroy(a) (DAdestroy(a))
#define SiteArrPush(a,b) (DApush((DynArray*)(a),(char*)(b)))

typedef enum {SITE_UNDEF, SITE_OK, SITE_NOACCESS, SITE_TOOBUSY} AccessResult;

/**  Functions declared in site.c **/
#ifndef NO_AUTHENTICATION
  SiteArray       *SiteArrayNew();
  AccessResult    SiteAccess(SiteArray*, char*, char*, Accesslevel, int);
  Accesslevel     SiteDefAccess(SiteArray *sitearr);

  boolean         SiteProcessLine(SiteArray *, char *, Accesslevel);
#else
# define SiteArrayNew() (NULL)
# define SiteAccess(a,b,c,d,e) (SITE_OK)
# define SiteProcessLine(a,b,c) (1)
# define SiteDefAccess(a) (ACC_BROWSE|ACC_READ|ACC_SEARCH|ACC_FTP)
#endif

#endif  /* SITE_H */
