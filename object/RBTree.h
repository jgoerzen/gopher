/**********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/RBTree.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1995 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: RBTree.h
 * Definition of a Red-Black balanced tree container.
 *********************************************************************
 * Revision History:
 * $Log: RBTree.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.2  1995/10/31  16:48:21  lindner
 * Better readability, fix bugs..
 *
 * Revision 3.1  1995/06/30  20:31:04  lindner
 * New red-black tree code
 *
 **********************************************************************/

#include "boolean.h"

enum direction {LEFT, RIGHT, NOWHERE };
enum nodecolor {RED, BLACK};

typedef enum direction direction;
typedef enum nodecolor nodecolor;

struct RBTree_node_struct {
     char *data;

     struct RBTree_node_struct *l_child;
     struct RBTree_node_struct *r_child;

     nodecolor rb_right;
     nodecolor rb_left;
     
     direction last_pass;

};

typedef struct RBTree_node_struct RBTree_node;


#define RBTsetRBright(a,v) ((a)->rb_right=(v))   /* Sets ptr to black or Red */
#define RBTsetRBleft(a,v)  ((a)->rb_left=(v))

#define RBTgetRBright(a)  ((a)->rb_right)
#define RBTgetRBleft(a)   ((a)->rb_left)

#define RBTgetDirection(a)   ((a)->last_pass)
#define RBTsetDirection(a,v) ((a)->last_pass=(v))

#define RBTattachLeftNode(a,v)  ((a)->l_child = (v))
#define RBTattachRightNode(a,v) ((a)->r_child = (v))

#define RBTgetRight(a)       ((a)->r_child)
#define RBTgetLeft(a)        ((a)->l_child)

#define RBTaddData(a,v)      ((a)->data=(v))
#define RBTgetData(a)        ((a)->data)

/**********************************************************************/

struct RBTree_struct {
     RBTree_node *entry;
     
     int    (*cmpfn)();
     void   (*initfn)();
     void   (*destroyfn)();
     char * (*copyfn)();

};

typedef struct RBTree_struct RBTree;

/*** For the top level tree structure ***/
#define RBTsetEntry(a,v)     ((a)->entry = (v))
#define RBTgetEntry(a)       ((a)->entry)

#define RBTcmp(a,b,c) ((a)->cmpfn(b,c))
#define RBTdestroyfcn(a,b) ((a)->destroyfn(RBTgetData(b)))

RBTree *RBTnew();
void    RBTdestroy(RBTree *rbt);
void    RBTtraverse(RBTree *rbt, void (*func)());
char   *RBTfind(RBTree *rbt, char* data);
void    RBTinsert(RBTree *rbt, char *data);
