/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/ext.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: ext.h
 * Header file and access methods for filename extension routines
 *********************************************************************
 * Revision History:
 * $Log: ext.h,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.5  1995/09/26  05:16:23  lindner
 * more fixes...
 *
 * Revision 3.4  1995/09/25  05:02:33  lindner
 * Convert to ANSI C
 *
 * Revision 3.3  1993/10/20  03:11:57  lindner
 * Additions for regular expressions
 *
 * Revision 3.2  1993/07/23  03:18:50  lindner
 * Mods for using decoder:'s
 *
 * Revision 3.1.1.1  1993/02/11  18:02:51  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.3  1993/02/09  22:13:57  lindner
 * Added Language attributes
 *
 * Revision 1.2  1993/01/30  23:57:44  lindner
 * Changed fcn prefix to EX and EXA
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#ifndef EXT_H
#define EXT_H

#include "DAarray.h"
#include "STRstring.h"
#include "boolean.h"

/*** Map extensions to any of these ***/
#define EXT_REGEXP   128
#define EXT_ERROR   -1
#define EXT_VIEW     1
#define EXT_IGNORE   2
#define EXT_ABSTRACT 3
#define EXT_BLOCK    4
#define EXT_BLOCKREF 5
#define EXT_DECODER  6

#define EXT_IGNOREPAT (EXT_REGEXP|EXT_IGNORE)

/****************************************************
 * A structure for mapping filenames to gopher types, and G+types
 */

struct Ext_struct {
     char   Objtype;
     String *View;
     String *ViewLang;
     String *Prefix;
     String *ext;
     int    exttype;  /** One of the constants defined above **/
     String *GenericData;
};

typedef struct Ext_struct Extobj;

/*** Macros and access methods ***/

#define EXgetObjtype(a)     ((a)->Objtype)
#define EXsetObjtype(a,b)   ((a)->Objtype=(b))

#define EXgetView(a)        STRget((a)->View)
#define EXsetView(a,b)      STRset((a)->View,(b))

#define EXgetVLang(a)       STRget((a)->ViewLang)
#define EXsetVLang(a,b)     STRset((a)->ViewLang,(b))

#define EXgetPrefix(a)      STRget((a)->Prefix)
#define EXsetPrefix(a,b)    STRset((a)->Prefix,(b))

#define EXgetExt(a)         STRget((a)->ext)
#define EXsetExt(a,b)       STRset((a)->ext,(b))

#define EXgetExttype(a)     ((a)->exttype)
#define EXsetExttype(a,b)   ((a)->exttype=(b))

#define EXgetBlockname(a)   STRget((a)->GenericData)
#define EXsetBlockname(a,b) STRset((a)->GenericData,(b))

#define EXgetDecoder(a)     STRget((a)->GenericData)
#define EXsetDecoder(a,b)   STRset((a)->GenericData,(b))

Extobj   *EXnew();
void      EXdestroy(Extobj *);
void      EXcpy(Extobj *, Extobj *);

/*** Definitions for an Array of extension objects ***/

typedef DynArray ExtArray;
#define EXAnew()            (ExtArray *)(DAnew(20,EXnew,NULL,EXdestroy,EXcpy))
#define EXAdestroy(a)       (DAdestroy(a))
#define EXAgetEntry(a,b)    (Extobj *)(DAgetEntry((DynArray*)a,b))
#define EXAadd(a,b)         (DApush((DynArray*)a,(char*)b))
#define EXAgetNumEntries(a) (DAgetTop(a))

boolean   EXAprocessLine(ExtArray *, int exttype, char *, char *);
boolean   EXAsearch(ExtArray *, Extobj *, char *, int);
boolean   EXAviewSearch(ExtArray *, Extobj *, char *);
char *    EXAfindFile(ExtArray *, char*, char*);

boolean   EXAcasedSearch(ExtArray *extarr, Extobj *ext, 
                         char *fileext, int exttype);
#endif /** EXT_H **/

