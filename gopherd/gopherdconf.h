/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/gopherdconf.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: gopherdconf.h
 * Header file for routines in gopherdconf.c
 *********************************************************************
 * Revision History:
 * $Log: gopherdconf.h,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.25  1995/09/26  04:59:46  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.24  1995/09/25  05:02:37  lindner
 * Convert to ANSI C
 *
 * Revision 3.23  1995/08/30  20:13:14  lindner
 * Remove tixfile
 *
 * Revision 3.22  1995/07/29  04:39:38  lindner
 * Smaller struct for sites with no authentication
 *
 * Revision 3.21  1995/06/30  20:33:30  lindner
 * Add group authentication
 *
 * Revision 3.20  1995/04/15  07:14:00  lindner
 * Add http port to gopherd.conf
 *
 * Revision 3.19  1995/02/13  19:07:06  lindner
 * Add MaxConnection limit for total server
 *
 * Revision 3.18  1995/02/11  06:21:27  lindner
 * remove pid junk
 *
 * Revision 3.17  1994/12/15  17:33:20  lindner
 * Add BlockScript: keyword support
 *
 * Revision 3.16  1994/08/18  22:28:47  lindner
 * Abstract for top level and malloc casts
 *
 * Revision 3.15  1994/07/22  22:56:19  lindner
 * More NO_AUTH stuff..
 *
 * Revision 3.14  1994/07/22  22:32:28  lindner
 * fix num args
 *
 * Revision 3.13  1994/07/22  22:29:01  lindner
 * Remove tix.h
 *
 * Revision 3.12  1994/07/21  17:23:06  lindner
 * Add File Separator code
 *
 * Revision 3.11  1994/06/29  05:30:10  lindner
 * Add code to handle lists of authscripts and authitems
 *
 * Revision 3.10  1994/03/17  04:31:01  lindner
 * Many, many parameter additions
 *
 * Revision 3.9  1994/03/08  15:56:03  lindner
 * gcc -Wall fixes
 *
 * Revision 3.8  1994/01/21  03:59:54  lindner
 * Add support for Timezone
 *
 * Revision 3.7  1993/09/30  17:05:15  lindner
 * start of subconf
 *
 * Revision 3.6  1993/09/21  04:16:52  lindner
 * Move cache settings into gopherd.conf
 *
 * Revision 3.5  1993/08/23  19:37:32  lindner
 * Add a semicolon
 *
 * Revision 3.4  1993/08/23  18:46:20  lindner
 * Crude addition of a veronica top-level block
 *
 * Revision 3.3  1993/08/20  18:03:11  lindner
 * Mods to allow gopherd.conf files control ftp gateway access
 *
 * Revision 3.2  1993/03/24  20:25:46  lindner
 * Addition for secureusers file
 *
 * Revision 3.1.1.1  1993/02/11  18:02:52  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.2  1993/01/30  23:57:44  lindner
 * Added macros for Geog and Lang
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/



#ifndef GOPHERDCONF_H
#define GOPHERDCONF_H

#include "boolean.h"
#include "STRstring.h"
#include "STAarray.h"
#include "ext.h"		/** Filename extensions **/
#include "site.h"		/** Hostname/IP security **/
#include "AUTH.h"		/** Username/pw security  */

/**************************************************************************
 * A structure that defines all the changable parameters of a gopher server
 * Read from the file gopherd.conf
 */

struct gdconf_struct {
     ExtArray     *Extensions;	   /* Filename extensions  */

#ifndef NO_AUTHENTICATION
     SiteArray    *Sites;	   /* IPnum/hostname based security */
     AUTHarray    *Authroutines;   /* Authentication routines/scripts */
     AUTHITEMS    *Authitems;	   /* list of items to authenticate */
     String       *groupdir;       /* directory where group files exist */
     String       *Serverpw;       /* The internal server password */

     Accesslevel  Defaccess;       /* Default access level for unknowns **/
#endif

     boolean      RunFromInetd;    /* -I option **/
     boolean      Caching;         /* -C option **/
     int          Cachetime;       /* Cachetime: ***/
     String       *Logfile;        /* The filename for the logfile **/
     String       *Errorfile;      /* The filename for the Errorfile **/
     String       *BummerMsg;      /* Message given to non-secure sites **/
     String       *Data_Dir;       /* Where our data directory is **/
     String       *Hostname;       /* A FQDN for the host **/
     int          Port;            /* The Port we're running at **/
     boolean      chroot;          /* Are we chroot()ing? **/

     boolean      Securityon;      /* Are we restricting access or not? **/
 
     String       *Admin;          /* Administrator info **/
     String       *AdminEmail;     /* E-mail address of administrator **/
     
     String       *Site;           /* Description of site **/
     String       *Org;            /* Name of Organization **/
     String       *Loc;            /* Location of site **/
     String       *Geog;           /* Latitude and Longitude **/
     String       *Lang;           /* Default language ***/

     double       MaxLoad;         /* Maximum Load Average ***/

     int          TZ;              /* Timezone **/
     String       *TimeZoneText;   /* Timezone in Text Format ***/

     boolean      VeronicaIndex;   /* Index it or not? **/

     String       *Abstract;       /* The abstract for the server */
     
     StrArray     *other_dirs;     /* Subdirectories ***/
     StrArray     *other_gdcs;     /* The associated gdc file ***/

     StrArray     *FileSeparators; /* Separators for Multipart files */
     
     StrArray     *BlkScriptBlocks;/* Block Names for block generation */
     StrArray     *BlkScripts;     /* Associated Script to run for Block */

     int          MaxConnections;  /* # of connections we can handle.. */
     int          HTTPport;        /* Port to use for HTTP connections */
};

typedef struct gdconf_struct GDCobj;

#define GDCgetDefAccess(a)      ((a)->Defaccess)

#define GDCgetInetdActive(a)    ((a)->RunFromInetd)
#define GDCsetInetdActive(a,b)  ((a)->RunFromInetd=(b))

#define GDCgetCaching(a)        ((a)->Caching)
#define GDCsetCaching(a,b)      ((a)->Caching=(b))

#define GDCgetCachetime(a)      ((a)->Cachetime)
#define GDCsetCachetime(a,b)    ((a)->Cachetime=(b))

#define GDCgetLogfile(a)        STRget((a)->Logfile)
#define GDCsetLogfile(a,b)      STRset((a)->Logfile,(b))

#define GDCgetErrorfile(a)      STRget((a)->Errorfile)
#define GDCsetErrorfile(a,b)    STRset((a)->Errorfile,(b))

#define GDCgetDatadir(a)        STRget((a)->Data_Dir)

#define GDCgetHostname(a)       STRget((a)->Hostname)
#define GDCsetHostname(a,b)     STRset((a)->Hostname,(b))

#define GDCgetPort(a)           ((a)->Port)

#define GDCgetchroot(a)         ((a)->chroot)

#define GDCgetBummerMsg(a)      STRget((a)->BummerMsg)
#define GDCsetBummerMsg(a,b)    STRset((a)->BummerMsg,(b))

#define GDCgetAdmin(a)       STRget((a)->Admin)
#define GDCsetAdmin(a,b)     STRset((a)->Admin,(b))

#define GDCgetAdminEmail(a)  STRget((a)->AdminEmail)
#define GDCsetAdminEmail(a,b) STRset((a)->AdminEmail,(b))

#define GDCgetSite(a)        STRget((a)->Site)
#define GDCsetSite(a,b)      STRset((a)->Site,(b))

#define GDCgetOrg(a)         STRget((a)->Org)
#define GDCsetOrg(a,b)       STRset((a)->Org,(b))

#define GDCgetLoc(a)         STRget((a)->Loc)
#define GDCsetLoc(a,b)       STRset((a)->Loc,(b))

#define GDCsetGeog(a,b)      STRset((a)->Geog,(b))
#define GDCgetGeog(a)        STRget((a)->Geog)

#define GDCgetLang(a)        STRget((a)->Lang)
#define GDCsetLang(a,b)      STRset((a)->Lang,(b))

#define GDCgetTixfile(a)        STRget((a)->Tixfile)
#define GDCsetTixfile(a,b)      STRset((a)->Tixfile,(b))

#define GDCgetShouldIndex(a)    ((a)->VeronicaIndex)
#define GDCsetShouldIndex(a,b)  ((a)->VeronicaIndex=(b))

#define GDCgetOtherGDCpath(a,b)  (STAgetText((a)->other_dirs,(b)))
#define GDCgetOtherGDCconf(a,b)  (STAgetText((a)->other_gdcs,(b)))

#define GDCgetTZ(a)             (a)->TZ
#define GDCsetTZ(a,b)           ((a)->TZ=(b))

#define GDCgetTimeZone(a)	STRget((a)->TimeZoneText)
#define GDCsetTimeZone(a,b)	STRset((a)->TimeZoneText,(b))

#define GDCgetMaxLoad(a)	(a)->MaxLoad
#define GDCsetMaxLoad(a,b)	((a)->MaxLoad=(b))

#define GDCgetPW(a)             (STRget((a)->Serverpw))
#define GDCsetPW(a,b)           (STRset((a)->Serverpw,(b)))

#define GDCgetAbstract(a)       (STRget((a)->Abstract))
#define GDCsetAbstract(a,b)     (STRset((a)->Abstract,(b)))

#define GDCgetFileSep(a,b)      (STAgetText((a)->FileSeparators,(b)))
#define GDCgetNumFileSep(a)     (STAgetTop((a)->FileSeparators))

#define GDCnumBlkScripts(a)     (STAgetTop((a)->BlkScripts))
#define GDCgetBlkName(a,b)      (STAgetText((a)->BlkScriptBlocks,(b)))
#define GDCgetBlkScript(a,b)    (STAgetText((a)->BlkScripts,(b)))

#define GDCgetMaxconns(a)       ((a)->MaxConnections)
#define GDCsetMaxconns(a,b)     ((a)->MaxConnections=(b))

#define GDCgetHTTPport(a)       ((a)->HTTPport)
#define GDCsetHTTPport(a,b)     ((a)->HTTPport = (b))

#define GDCgetGroupdir(a)       (STRget((a)->groupdir))
#define GDCsetGroupdir(a,b)     (STRset((a)->groupdir,(b)))


/*
 * Real live functions in gopherdconf.c
 */

GDCobj       *GDCnew();
void         GDCdestroy(GDCobj *gdc);

boolean      GDCignore(GDCobj *, char *);

/* mdallen: added extra prototypes */

boolean      GDCignore(GDCobj *gdc, char *fname);
boolean      GDCViewExtension(GDCobj *gdc, char *fext, Extobj **extin);
boolean      GDCBlockExtension(GDCobj *gdc, char *fext, Extobj *ext);
void         GDCaddFileSep(GDCobj *gdc, char *expr);
void         GDCintegrityCheck(GDCobj *gdc);
void         GDCfromFile(GDCobj *gdc, char *filename);

#ifndef NO_AUTHENTICATION

  AccessResult     GDCaccess(GDCobj *, char *, char *, int, Accesslevel);

  char            *GDCauthType(GDCobj*, char*);
  char            *GDCauthExtraArgs(GDCobj*, char*);
  AUTHresult       GDCvalidate(GDCobj*, char*, char*, char*, char*, char*,
				char*);
  char           **GDCauthAsk(GDCobj*, char*);

#else
#  define GDCCanRead(a,b,c,d)   (SITE_OK)
#  define GDCCanBrowse(a,b,c,d) (SITE_OK)
#  define GDCCanSearch(a,b,c,d) (SITE_OK)
#  define GDCCanFTP(a,b,c,d)    (SITE_OK)
#  define GDCauthType(a,b)      (NULL)
#  define GDCvalidate(a,b,c,d,e,f) (AUTHRES_OK)
#  define GDCauthAsk(a,b)       (NULL)
#endif


void         GDCpushOtherGDC(GDCobj *, char *, char*);
void         GDCaddFileSep(GDCobj *, char*);
void         GDCpushBlockScript(GDCobj *, char*, char*);
boolean      GDCevalDir(GDCobj *gdc,  char *dir);
#endif /* GOPHERDCONF_H */
