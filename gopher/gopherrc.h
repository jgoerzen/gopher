/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.2 $
 * $Date: 2001/01/03 22:30:11 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/gopherrc.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: gopherrc.h
 * Abstraction and macros for stuff in gopherrc.c
 *********************************************************************
 * Revision History:
 * $Log: gopherrc.h,v $
 * Revision 1.2  2001/01/03 22:30:11  s2mdalle
 * Just a pile of function prototypes
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.8  1995/11/03  21:18:19  lindner
 * ANSIfication
 *
 * Revision 3.7  1994/10/21  04:41:08  lindner
 * Add ANSI attached printer code..
 *
 * Revision 3.6  1994/07/06  15:43:42  lindner
 * Allow 'home' gopher server in gopherrc files
 *
 * Revision 3.5  1994/06/29  05:13:14  lindner
 * Define, add code to check for new config file
 *
 * Revision 3.4  1993/12/28  17:30:34  lindner
 * more typecasting for picky compilers
 *
 * Revision 3.3  1993/09/30  22:42:14  lindner
 * Add option for bolding of searched words
 *
 * Revision 3.2  1993/02/19  21:10:54  lindner
 * Updated declarations
 *
 * Revision 3.1.1.1  1993/02/11  18:03:00  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1993/02/09  22:35:54  lindner
 * initial rev.
 *
 *
 *
 *********************************************************************/

#ifndef GOPHERRC_H
#define GOPHERRC_H

#include "String.h"
#include "DAarray.h"
#include "GSgopherobj.h"
#include "GDgopherdir.h"

/*
 * stuff to map from a gopher+ view attribute to a command
 */

struct gopherrcmap_struct {
     String *view;
     
     String *displaycmd;
     String *printcmd;
};

typedef struct gopherrcmap_struct RCMapObj;

/** Macros and external functions ***/
#define   RCMgetView(a)         (STRget((a)->view))
#define   RCMgetDisplaycmd(a)   (STRget((a)->displaycmd))
#define   RCMgetPrintcmd(a)     (STRget((a)->printcmd))

#define   RCMsetView(a,b)       (STRset((a)->view,b))
#define   RCMsetDisplaycmd(a,b) (STRset((a)->displaycmd,b))
#define   RCMsetPrintcmd(a,b)   (STRset((a)->printcmd,b))

RCMapObj *RCMnew(void);
void     RCMdestroy(RCMapObj *rcm);
void     RCMinit(RCMapObj *rcm);
void     RCMdestroy(RCMapObj *rcm);

/** Dynamic Array of the above... **/
/*** Definitions for an Array of extension objects ***/

typedef DynArray RCMAarray;
#define RCMAnew()            (RCMAarray *)(DAnew(20,RCMnew,NULL,RCMdestroy,RCMcpy))
#define RCMAdestroy(a)       (DAdestroy(a))
#define RCMAgetEntry(a,b)    (RCMapObj *)(DAgetEntry((DynArray*)a,b))
#define RCMAadd(a,b)         (DApush(((DynArray*)a),((char*)b)))
#define RCMAgetNumEntries(a) (DAgetTop((DynArray*)a))

int     RCMAviewSearch(RCMAarray *rcma, char *view);
boolean RCMAfromLine(RCMAarray *rcma, char *line);
void    RCMAtoFile(RCMAarray *rcma, FileIO *fio);


/*
 * General gopherrc stuff.
 */

struct RC_struct {
     RCMAarray    *commands;
     GopherDirObj *Bookmarks;
     GopherObj    *Home;

     boolean      SearchBolding;
     boolean      ANSIprinter;

     boolean      ChangedDefs;
};


typedef struct RC_struct RCobj;

#define RCgetBookmarkDir(a)    ((a)->Bookmarks)
#define RCsetBookmarkDir(a,b)  ((a)->Bookmarks=(b))
#define RCinitBookmarkDir(a,b) ((a)->Bookmarks=GDnew(b))

#define RCgetHome(a)		((a)->Home)
#define RCsetHome(a,b)		((a)->Home = (b))

#define RCsearchBolding(a)      ((a)->SearchBolding)
#define RCsetSearchBolding(a,b) ((a)->SearchBolding=(b))

#define RCuseANSIprinter(a)     ((a)->ANSIprinter)
#define RCsetANSIprinter(a,b)   ((a)->ANSIprinter=(b))


RCobj  *RCnew(void);
void    RCsetdefs(RCobj *rc);
void    RCfromFile(RCobj *rc, FileIO *rcfio);
void    RCtoFile(RCobj *rc);
boolean RCisGlobalNew(void);

#endif

/* Prototypes */
RCMapObj *RCMnew(void);
void RCMdestroy(RCMapObj *rcm);
void RCMinit(RCMapObj *rcm);
void RCMcpy(RCMapObj *dest, RCMapObj *orig);
void RCMdisplayCommand(RCMapObj *rcm, char *filename, char *line);
void RCMprintCommand(RCMapObj *rcm, char *filename, char *line);
int RCMAviewSearch(RCMAarray *rcma, char *view);
boolean RCMAfromLine(RCMAarray *rcma, char *line);
void RCMAtoFile(RCMAarray *rcma, FileIO *fio);
RCobj *RCnew(void);
void RCfromFile(RCobj *rc, FileIO *rcfio);
void RCfromENV(RCobj *rc);
void RCfromUser(RCobj *rc);
void RCreadGlobalRC(RCobj *rc);
void RCsetdefs(RCobj *rc);
void RCtoFile(RCobj *rc);
boolean RCdisplayCommand(RCobj *rc, char *view, char *filename, char *line);
boolean RCprintCommand(RCobj *rc, char *view, char *filename, char *line);
boolean RCisGlobalNew(void);

