/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/BLblock.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992,1993 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: BLblock.h
 * Header file and abstraction of a gopher+ block
 *********************************************************************
 * Revision History:
 * $Log: BLblock.h,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.5  1995/09/26  05:16:30  lindner
 * more fixes...
 *
 * Revision 3.4  1995/09/25  22:07:16  lindner
 * Ansification
 *
 * Revision 3.3  1995/02/27  17:45:32  lindner
 * Use enums for block structures
 *
 * Revision 3.2  1993/03/26  19:50:41  lindner
 * Mitra fixes for better/clearer fromNet code
 *
 * Revision 3.1.1.1  1993/02/11  18:03:06  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1993/01/31  00:31:12  lindner
 * Initial revision
 *
 *
 *********************************************************************/


#ifndef BLBLOCK_H
#define BLBLOCK_H

#include "STRstring.h"
#include "STAarray.h"
#include "boolean.h"
#include "util.h"

/** Return Values for *fromNet() functions **/
#define SOFTERROR -1
#define HARDERROR -2
#define MORECOMING 1
#define FOUNDEOF 0


typedef struct block_struct Blockobj;
typedef DynArray BlockArray;

#include "GSgopherobj.h"


/** The different types of blocks **/
enum blocktype {
	 BLOCK_UNKNOWN  =0,
	 BLOCK_VIEW     =1,
	 BLOCK_ASK      =2,
	 BLOCK_ABSTRACT =3,
	 BLOCK_ADMIN    =4
};
typedef enum blocktype BlockType;


/** The block data is a union, it can either be a filename or the
 ** actual data in a STRarray, or a gopher reference.
 **/

union BlockData_union {
     String      *filename;
     StrArray    *text;
     GopherObj   *gs;
};

typedef union BlockData_union BlockData;


enum blockdatatype {
	BDATA_NONE =0,
	BDATA_FILE =1,
	BDATA_TEXT =2,
	BDATA_GREF =3
};
typedef enum blockdatatype BlockDataType;


struct block_struct
{
     BlockType     btype;
     String        *Blockname;
     BlockDataType datatype;
     BlockData     data;
};


/****** Macros/data access ********/
#define BLgetName(a)        (STRget((a)->Blockname))
#define BLsetName(a,b)      (STRset((a)->Blockname,(b)))

#define BLgetBlocktype(a)   (STRget((a)->btype))
#define BLsetBlocktype(a,b) (STRset((a)->btype,(b)))

#define BLgetDatatype(a)    ((a)->datatype)

/**** Prototype declarations. ****/
Blockobj *BLnew();
void      BLdestroy(Blockobj *bl);
void      BLinit(Blockobj *bl);
void      BLcpy(Blockobj *dest, Blockobj *orig);
int       BLgetNumLines(Blockobj *bl);

void      BLsetFile(Blockobj *bl, char *filename);
void      BLsetGref(Blockobj *bl, GopherObj *);
void      BLsetText(Blockobj *bl, StrArray *sta);
char *    BLgetLine(Blockobj *bl, int lineno);
void      BLtoNet(Blockobj *bl, int fd, boolean showheader);
int       BLfromNet(Blockobj *bl, int fd, char *blockname);
int       BLAsearch(BlockArray *bla, char *bname);
void      BLaddText(Blockobj *bl, char *text);

/*************************************************************
 ** Define a dynamic block array
 **/

#include "DAarray.h"

#define BLAnew(a)       (DAnew((a),BLnew,BLinit,BLdestroy,BLcpy))
#define BLAinit(a)       (DAinit((a)))
#define BLAgetTop(a)     (DAgetTop(a))
#define BLAgetEntry(a,b) (Blockobj*)(DAgetEntry(a,b))
#define BLApush(a,b)     (DApush((DynArray*)(a),(char*)(b)))
#define BLAdestroy(a)    (DAdestroy(a))
#define BLAcpy(a,b)      (DAcpy(a,b))

#endif
