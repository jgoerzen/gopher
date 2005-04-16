/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/GDgopherdir.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: GDgopherdir.h
 * Header file/abstraction of a gopher directory
 *********************************************************************
 * Revision History:
 * $Log: GDgopherdir.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.7  1995/10/31  16:47:57  lindner
 * Switch to void+ pointer
 *
 * Revision 3.6  1995/09/25  22:07:17  lindner
 * Ansification
 *
 * Revision 3.5  1994/07/21  22:26:37  lindner
 * Alan's GDlocation stuff
 *
 * Revision 3.4  1994/01/21  04:47:20  lindner
 * Add function prototypes
 *
 * Revision 3.3  1994/01/10  03:29:07  lindner
 * New Method GDsetNumitems()
 *
 * Revision 3.2  1993/04/15  21:35:59  lindner
 * Changed protos
 *
 * Revision 3.1.1.1  1993/02/11  18:03:03  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#ifndef GDGOPHERDIR_H
#define GDGOPHERDIR_H
#include "GSgopherobj.h"
#include "DAarray.h"

/****************************************************************
** A Gopher directory structure...
** Features dynamic growth among other things...
**
*****************************************************************/

struct g_dir_struct {
     String *Title;

     GopherObj *Location;

     DynArray *Gophers;
     
     int currentitem;
};

typedef struct g_dir_struct GopherDirObj;
typedef struct g_dir_struct *GopherDirObjp;

#define GDgetEntry(a,b)    ((GopherObj *)(DAgetEntry((a)->Gophers,(b))))
#define GDgetTitle(a)      (STRget((a)->Title))
#define GDsetTitle(a,b)    (STRset((a)->Title,b))
#define GDgetLocation(a)   ((a)->Location)
#define GDgetNumitems(a)   (DAgetNumitems((a)->Gophers))
#define GDsetNumitems(a,b) (DAsetTop((a)->Gophers,(b)))

#define GDsetCurrentItem(a,b) ((a)->currentitem=b)
#define GDgetCurrentItem(a) ((a)->currentitem)

/*** Real live functions declared in GDgopherdir.c ***/

GopherDirObj *GDnew(int size);
void         GDdestroy(GopherDirObj *);
void         GDinit(GopherDirObj *);
void         GDsetLocation(GopherDirObj *gd, GopherObj *gs);
void         GDaddGS(GopherDirObj *gd, GopherObj *gs);
void         GDaddGSmerge(GopherDirObj *gd, GopherObj *gs);
void         GDsort(GopherDirObj *);
void         GDtoNet(GopherDirObj *gd, int sockfd, GSformat fmt, char *ticket, 
		     void (*prefcn)());
void         GDplustoNet(GopherDirObj *gd, int sockfd, char **filter, 
			 char *ticket);
void         GDfromNet(GopherDirObj *gd, int sockfd, int (*eachitem)());
int          GDplusfromNet(GopherDirObj *gd, int fd, int (*eachitem)());
void         GDgrow();
void         GDfromLink(GopherDirObj *, FileIO *, char *, int, char*, char*);
void         GDtoLink(GopherDirObj *gd, int fd);
GopherDirObj *GDdeleteGS(GopherDirObj	*gd, int j);
int          GDSearch(GopherDirObj *gd, char *text);
#endif /* GDGOPHERDIR_H */






