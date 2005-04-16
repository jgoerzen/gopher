/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/VIews.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: VIews.h
 * Header file, abstraction of a gopher+ view.
 *********************************************************************
 * Revision History:
 * $Log: VIews.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.5  1995/09/26  05:16:33  lindner
 * more fixes...
 *
 * Revision 3.4  1995/09/25  22:07:22  lindner
 * Ansification
 *
 * Revision 3.3  1993/07/23  04:49:07  lindner
 * Added PrettyView fcn
 *
 * Revision 3.2  1993/02/19  21:33:24  lindner
 * Gopher1.2b2 release
 *
 * Revision 3.1.1.1  1993/02/11  18:03:07  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1993/02/09  22:48:34  lindner
 * Changes for multilingual views
 *
 * Revision 1.1  1993/01/31  00:34:40  lindner
 * Initial revision
 *
 *
 *********************************************************************/

#ifndef VIEWS_H
#define VIEWS_H


#include "STRstring.h"
#include "boolean.h"

struct view_struct 
{
     String *Type;
     String *Lang;
     String *Size;
     String *comments;
};

typedef struct view_struct VIewobj;

#define VIgetType(a)     ((STRget((a)->Type)))
#define VIgetLang(a)     ((STRget((a)->Lang)))
#define VIgetSize(a)     ((STRget((a)->Size)))
#define VIgetComments(a) ((STRget((a)->comments)))
     
#define VIsetType(a,b)     (STRset((a)->Type,(b)))
#define VIsetLang(a,b)     (STRset((a)->Lang,(b)))
#define VIsetSize(a,b)     (STRset((a)->Size,(b)))
#define VIsetComments(a,b) (STRset((a)->comments,(b)))

/** Real view functions in VIews.c **/
VIewobj *VInew();
void     VIinit(VIewobj *vi);
void     VIcpy(VIewobj *videst, VIewobj *viorig);
void     VIdestroy(VIewobj *vi);
void     VItoLine(VIewobj *vi, char *tmpstr);
boolean  VIfromLine(VIewobj *vi, char *line);
char    *VIgetViewnLang(VIewobj *vi, char *line);
char    *VIgetPrettyView(VIewobj *vi, char *line);
char    *VIprettyLang(VIewobj *vi, char *currentlang);

/*******************************************************/
/** This stuff is for a dynamic array of VIewobjs     **/

#include "DAarray.h"

typedef DynArray VIewArray;
#define VIAnew(a)        (DAnew((a),VInew,VIinit,VIdestroy,VIcpy))
#define VIAinit(a)       (DAinit((a)))
#define VIAgetTop(a)     (DAgetTop(a))
#define VIAgetEntry(a,b) (VIewobj*)(DAgetEntry(a,b))
#define VIApush(a,b)     (DApush((DynArray*)(a),(char*)(b)))
#define VIAdestroy(a)    (DAdestroy(a))
#define VIAcpy(a,b)      (DAcpy(a,b))

#include "BLblock.h"

void     VIAfromBL(VIewArray *via, Blockobj *bl);

#endif
