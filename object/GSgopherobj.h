/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.2 $
 * $Date: 2000/12/26 22:48:54 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/GSgopherobj.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: GSgopherobj.h
 * Header file, abstraction of a gopher type
 *********************************************************************
 * Revision History:
 * $Log: GSgopherobj.h,v $
 * Revision 1.2  2000/12/26 22:48:54  s2mdalle
 * Changed prototype for GSsendHeader(int, int) => GSsendHeader(int, long)
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.25  1995/11/03  18:01:08  lindner
 * Coen: changes
 *
 * Revision 3.24  1995/09/25  22:07:18  lindner
 * Ansification
 *
 * Revision 3.23  1995/06/12  15:34:17  lindner
 * Add WORLD type to list of Gopher Objects
 *
 * Revision 3.22  1995/02/06  22:12:02  lindner
 * Make GSsetAbstract a define
 *
 * Revision 3.21  1994/12/05  22:40:11  lindner
 * add prototype for GSfromURL
 *
 * Revision 3.20  1994/11/17  06:34:00  lindner
 * Fixes for VMS internationalization
 *
 * Revision 3.19  1994/10/24  22:15:54  lindner
 * Add PDF type
 *
 * Revision 3.18  1994/08/19  16:18:14  lindner
 * Bitty fix from Alan
 *
 * Revision 3.17  1994/07/21  22:25:06  lindner
 * GINTERNATIONAL stuff
 *
 * Revision 3.16  1994/06/29  06:53:21  lindner
 * ifdef GINTERNATIONAL, use a function instead of a macro for
 * GSgetModDate(), so we can localize the date string
 *
 * Move GSfromURL() return code definitions from code so other
 * files can call it. (Coopersmith)
 *
 * Revision 3.15  1994/06/29  05:45:57  lindner
 * Mods to pump tickets to the net
 *
 * Revision 3.14  1994/04/01  04:38:08  lindner
 * Fix for conditional macros
 *
 * Revision 3.13  1994/03/31  21:02:40  lindner
 * Add some error and NULL checking to GS macros
 *
 * Revision 3.12  1993/12/27  16:22:43  lindner
 * Added more prototypes for GS functions
 *
 * Revision 3.11  1993/11/02  06:15:27  lindner
 * HTML additions
 *
 * Revision 3.10  1993/10/22  20:03:32  lindner
 * Add Movie (;) and Info (i) type support
 *
 * Revision 3.9  1993/09/18  04:44:43  lindner
 * Additions to fix caching of Multiple view items
 *
 * Revision 3.8  1993/09/11  06:33:06  lindner
 * URL support
 *
 * Revision 3.7  1993/07/27  20:18:09  lindner
 * Sorry can't take address of conditionals...
 *
 * Revision 3.6  1993/07/27  05:30:25  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.5  1993/07/27  00:30:11  lindner
 * plus patch from Mitra
 *
 * Revision 3.4  1993/07/23  04:50:59  lindner
 * Additional stored askdata
 *
 * Revision 3.3  1993/04/15  17:55:18  lindner
 * GSgetAdmin fix, plus more protos
 *
 * Revision 3.2  1993/03/24  17:09:42  lindner
 * Additions for Localfile for each GopherObj
 *
 * Revision 3.1.1.1  1993/02/11  18:03:03  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1993/02/09  22:47:54  lindner
 * Added new def GSSfindBlock
 *
 * Revision 1.2  1993/01/31  00:31:12  lindner
 * Gopher+ structures and macros.
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#ifndef GSGOPHEROBJ_H
#define GSGOPHEROBJ_H

#include "boolean.h"
#include "STRstring.h"
#include "STAarray.h"

typedef struct g_struct GopherObj;
#include "BLblock.h"

#include "VIews.h"
#include "url.h"
#include "Locale.h"
#include "compatible.h"
#include "fileio.h"

#define A_FILE      '0'      /* Types of objects */
#define A_DIRECTORY '1'
#define A_CSO       '2'
#define A_ERROR     '3'
#define A_MACHEX    '4'
#define A_PCBIN     '5'
#define A_INDEX     '7'
#define A_TELNET    '8'
#define A_UNIXBIN   '9'
#define A_SOUND     's'
#define A_EVENT     'e'
#define A_CALENDAR  'c'
#define A_GIF       'g'
#define A_HTML      'h'
#define A_TN3270    'T'
#define A_MIME      'M'
#define A_IMAGE     'I'
#define A_INFO      'i'
#define A_MOVIE     ';'
#define A_PDF       'P'
#define A_WORLD     'v'
#define A_APP       '!'
#define A_EOI	    '.'


/*** Formats for GStoNet() ***/
#define GSFORM_G0    0
#define GSFORM_GPLUS 1
#define GSFORM_HTML  2

/*** Part codes for GSfromLink() & GSfromURL() ***/
#define	G_PATH	1
#define	G_TYPE  2
#define	G_NAME	4
#define	G_PORT	8
#define	G_HOST	16
#define	G_ALL (G_PATH | G_TYPE | G_NAME | G_PORT | G_HOST)

typedef int GSformat;

/*** Our gopher+ attributes structure ***/

struct gplus_struct
{
     /** Abstract Block **/
     /* String *abstract;	Superceeded by BlockArray */

     /** Admin Block **/
     String *Admin;
     String *ModDate;

     VIewArray *Views;
     /** Others Blocks**/
     BlockArray *OtherBlocks;

     /** Filled in askdata **/
     char **Askdata;
};

typedef struct gplus_struct GplusObj;	  

/** Our Gopher Object Type **/

struct g_struct
{
     char    sFileType;     /* The type of object (A_FILE, A_CSO, etc)*/
     String  *Title;        /* User displayable title */
     String  *Selstr;       /* Selector string on host... */
     String  *Host;         /* Internet name of host                  */
     int     iPort;         /* Port number on host                    */
     int     Itemnum;       /* The number of the item in the directory*/
     int     weight;        /* The weight, expressed as a # from 0-1000 */
     int     ttl;           /* The Time-To-Live, how long the item is good */

     String  *Localfile;    /* The local file of the item */
     String  *Localview;    /* The view of the local file of the item */

     boolean isgplus;       /* Item can be queried gopher+ style */
     boolean isask;         /* Item contains an ASK block */
     GplusObj *gplus;       /* Gopher + attributes */
     Url      *url;         /* The Unix^H^H^Hniversal Resource Locator */
};


typedef struct g_struct GopherStruct;

#define GSgetType(a) ((a)->sFileType)
#define GSsetType(a,b) (a)->sFileType=(b)

#define GSgetTitle(a) ((STRget((a)->Title)))
#define GSsetTitle(a,b) ((STRset((a)->Title, b)))

#define GSgetPath(a) ((STRget((a)->Selstr)))
#define GSsetPath(a,b) ((STRset((a)->Selstr, b)))

#define GSgetHost(a) ((STRget((a)->Host)))
#define GSsetHost(a,b) ((STRset((a)->Host, b)))

#define GSgetPort(a) ((a)->iPort)
#define GSsetPort(a,b) (a)->iPort=(b)

#define GSgetNum(a) ((a)->Itemnum)
#define GSsetNum(a,b) ((a)->Itemnum=(b))

#define GSgetWeight(a)   ((a)->weight)
#define GSsetWeight(a,b) ((a)->weight=(b))

#define GSgetTTL(a)   ((a)->ttl)
#define GSsetTTL(a,b) ((a)->ttl=(b))

#define GSgetLocalFile(a) ((STRget((a)->Localfile)))
#define GSsetLocalFile(a,b) ((STRset((a)->Localfile, (b))))

#define GSgetLocalView(a) ((STRget((a)->Localview)))
#define GSsetLocalView(a,b) ((STRset((a)->Localview, (b))))

/****************** Gopher Plus attributes *******************/
#define GSisGplus(a)    ((a)->isgplus)
#define GSsetGplus(a,b) ((a)->isgplus=(b))

#define GSisAsk(a)      ((a)->isask)
#define GSsetAsk(a,b)   ((a)->isask=(b))

#define GSgplusInited(a) ((a)->gplus != NULL)

#define GSgetAdmin(a)    (((a)->gplus == NULL) ? NULL : (STRget((a)->gplus->Admin)))

#if defined(VMS) || (defined(GINTERNATIONAL) && !defined(NO_STRFTIME))
   char	 *GSgetModDate(GopherObj *);
#else
#  define GSgetModDate(a)  (((a)->gplus == NULL) ? NULL : (((a)->gplus->ModDate == NULL) ? NULL : (STRget((a)->gplus->ModDate))))
#endif

#define GSgetOtherBlocks(a) (BlockArray*)(((a)->gplus == NULL) ? NULL : ((a)->gplus->OtherBlocks))

#define GSgetAskdata(a)    (GSgplusInited(a) ? (a)->gplus->Askdata : NULL)
#define GSsetAbstract(g,b) (GSsetBlock(g,"ABSTRACT",b,TRUE))

/*** Real live functions defined in GSgopherobj.c ***/

GopherObj *GSnew();
void      GSdestroy(GopherObj *gs);
void      GSinit(GopherObj *gs);
void      GSplusnew(GopherObj *);
void      GStoNet(GopherObj *gs, int sockfd, GSformat fmt, char *ticket);
void      GStoNetHTML();
int       GSfromNet(GopherObj *gs, int sockfd);
int       GSplusfromNet(GopherObj *gs, int fd);
int       GSfromLink(GopherObj *, FileIO *,  char*, int, char *, char*);
void      GStoLink(GopherObj *gs, int fd, BOOLEAN AddInfo);
void      GScpy(GopherObj *dest, GopherObj *orig);
void      GScpy();
void      GSsetBlock(GopherObj *gs, char *blockname, char *text, boolean);
void      GSsetURL(GopherObj *, char *url);
void      GSsetAdmin(GopherObj *gs, char *admin);
void      GSsetModDate(GopherObj *gs, char *str);
void      GSaddView(GopherObj *gs, char *view, char *language, int estsize);
void      GSaddBlock(GopherObj *gs, char *Blockname, char *file);
void      GSmerge(GopherObj *gs, GopherObj *overlay);
void      GSgetginfo(GopherObj *gs, boolean   savename);
boolean   GSisText(GopherObj *gs, char *view);


char     *GSgetURL(GopherObj *gs, char *ticket);
char     *GSgetURLhtml(GopherObj *gs, char *ticket);


struct tm *GSgetModDateTM(GopherObj *);

int       GSgetNumViews(GopherObj *);
VIewobj  *GSgetView(GopherObj *, int);

int       GSgetNumBlocks(GopherObj *);
Blockobj *GSgetBlock(GopherObj *, int);
Blockobj *GSfindBlock(GopherObj *gs, char *blockname);

int       GSfromURL(GopherObj *gs, char *urltxt, char *host, int port, int doneflags);

/****** Protocol transmission functions *******/
int  GSconnect(GopherObj*);
void GStransmit(GopherObj *gs, int sockfd, char *search, char *, char *);
void GSsendHeader(int, long);
void GSsendErrorHeader(GopherObj *gs, int sockfd, int errortype, char *);
int  GSrecvHeader(GopherObj *gs, int sockfd);
void GStransmit();

/******Gopher+ functions *******/
void GSplusnew();
void GSplusdestroy(GopherObj*);
void GSplusInit(GopherObj*);
void GSplustoNet(GopherObj *gs, int sockfd, char **filter, char *ticket);
void GSpluscpy(GopherObj *dest, GopherObj *orig);


char **GSsetAskdata(GopherObj *gs, char **askdata);

/***** Debugging functions *******/
void GSplusPrint(GopherObj *gs, char *head);

#endif /*GSGOPHEROBJ_H*/

