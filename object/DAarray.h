/********************************************************************
 * lindner
 * 3.1.1.1
 * 1993/02/11 18:03:06
 * /home/mudhoney/GopherSrc/CVS/gopher+/object/DAarray.h,v
 * Exp
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: DAarray.h
 * Dynamic Array Header file/abstraction
 *********************************************************************
 * Revision History:
 * DAarray.h,v
 * Revision 3.1.1.1  1993/02/11  18:03:06  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.2  1992/12/21  20:04:04  lindner
 * Added DAcpy()
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#ifndef DAARRAY_H
#define DAARRAY_H

/*
 *  A dynamic array class
 */ 

struct da_struct {
     char **objects;  /** Should be void** perhaps */
     
     int Top;
     int maxsize;

     char * (*newfn)();
     void   (*initfn)();
     void   (*destroyfn)();
     char * (*copyfn)();
};

typedef struct da_struct DynArray;

#define DAgetEntry(a,b)   (((a)->objects[b]))
#define DAgetTop(a)       ((a)->Top)
#define DAsetTop(a,b)     ((a)->Top=(b))
#define DAgetNumitems(a)   ((a)->Top)

DynArray *DAnew();
void     DAdestroy(DynArray *da);
void     DAinit(DynArray *da);
void     DApush(DynArray *da, char *obj);
char *   DApop(DynArray *da);
void     DAsort(DynArray *da, int (*sortfn)());
void     DAgrow(DynArray *da, int size);
void     DAsort(DynArray *da, int (*sortfn)());
void     DAcpy(DynArray *dest, DynArray *orig);
#endif


