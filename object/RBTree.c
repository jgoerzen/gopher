/**********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2001/01/17 21:48:05 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/RBTree.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1995 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: RBTree.c
 * Implementation of a Red-Black balanced tree container.
 *********************************************************************
 * Revision History:
 * $Log: RBTree.c,v $
 * Revision 1.2  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.3  1995/10/31  16:48:21  lindner
 * Better readability, fix bugs..
 *
 * Revision 3.2  1995/09/25  22:07:19  lindner
 * Ansification
 *
 * Revision 3.1  1995/06/30  20:31:04  lindner
 * New red-black tree code
 *
 **********************************************************************/

#include "RBTree.h"
#include "Malloc.h"
RBTree *mytree;
void printit();

/***********************************************************************
 * Prototypes
 */

static void 
RBTleft_right(RBTree      *thetree, 
			  RBTree_node *child, 
			  RBTree_node *parent,
			  RBTree_node *grandparent,
			  RBTree_node *great_grandparent);
static void 
RBTright_left(RBTree      *thetree, 
			  RBTree_node *child, 
			  RBTree_node *parent,
			  RBTree_node *grandparent,
			  RBTree_node *great_grandparent);

static void 
RBTright_right(RBTree *thetree,
	       RBTree_node * parent, 
	       RBTree_node * grandparent,
	       RBTree_node * great_grandparent);

static void
RBTleft_left(RBTree *thetree,
	     RBTree_node *parent, 
	     RBTree_node *grandparent,
	     RBTree_node *great_grandparent);

/**********************************************************************/
RBTree_node 
*RBTNnew(void)
{
     RBTree_node *temp;

     temp = (RBTree_node*) malloc(sizeof(RBTree_node));
     
     RBTsetDirection(temp, NOWHERE);
     RBTsetRBright(temp, BLACK);
     RBTsetRBleft(temp, BLACK);
     
     RBTattachLeftNode(temp, NULL);
     RBTattachRightNode(temp, NULL);

     return(temp);
}


void
RBTdestroy_node(RBTree *thetree, RBTree_node *rbt_node)
{
     if (rbt_node != NULL) {
	  RBTdestroy_node(thetree, RBTgetLeft(rbt_node));
	  RBTdestroy_node(thetree, RBTgetRight(rbt_node));

	  RBTdestroyfcn(thetree, rbt_node);
	  free(rbt_node);
     }
}




static void 
RBTred_sibling_ck(RBTree_node* child, RBTree_node * parent)
{
     if ((RBTgetRBright(child) == RED) && (RBTgetRBleft(child) == RED))
     {
	  /* if two red siblings change to black */
	  RBTsetRBright(child, BLACK);
	  RBTsetRBleft(child, BLACK);
 
	  /* if parent exists, change to red */
 
	  if (parent != NULL) {
	       if (RBTgetDirection(parent) == LEFT)
		    RBTsetRBleft(parent, RED);
	       else
		    RBTsetRBright(parent, RED);
	  }
     }
}

/**********************************************************************/

static int 
RBTbalance(RBTree      *thetree,
	   RBTree_node *child, 
	   RBTree_node *parent,
	   RBTree_node *grandparent,
	   RBTree_node *great_grandparent)
{

     
    if (grandparent != NULL) { /* must have tree nodes to need rebalance*/
	if (RBTgetDirection(parent) == LEFT) {
	    if (RBTgetRBleft(parent) == RED) {
		if (RBTgetDirection(grandparent) == LEFT) {
		    if (RBTgetRBleft(grandparent) == RED) {
			RBTleft_left(thetree, parent, grandparent,
				     great_grandparent);
			return(TRUE);
		    }
		} else {
		    if (RBTgetRBright(grandparent) == RED) {
			RBTright_left(thetree, child, parent,
				      grandparent, great_grandparent);
			return(TRUE);
		    }
		}
	    }
	} else if (RBTgetRBright(parent) == RED) {
	    if (RBTgetDirection(grandparent) == LEFT) {
		if (RBTgetRBleft(grandparent) == RED) {
		    RBTleft_right(thetree, child, parent, grandparent,
				  great_grandparent);
		    return(TRUE);
		}
	    } else {
		if (RBTgetRBright(grandparent) == RED) {
		    RBTright_right(thetree, parent, grandparent, 
				   great_grandparent);
		    return(TRUE);
		}
	    }
	}
    }
    
    return(FALSE);
}

/**********************************************************************/

static void
RBTleft_right(RBTree      *thetree, 
	      RBTree_node *child, 
	      RBTree_node *parent,
	      RBTree_node *grandparent,
	      RBTree_node *great_grandparent)
{
     RBTree_node *temp[2];
 
#ifdef RBT_TEST
     printf("left_right called\n");
#endif
     if (great_grandparent != NULL )
	  if (RBTgetDirection(great_grandparent) == RIGHT)
	       RBTattachRightNode(great_grandparent, child);
	  else
	       RBTattachLeftNode(great_grandparent, child);
     else
	  RBTsetEntry(thetree, child);
     
     temp[0] = RBTgetLeft(child);
     temp[1] = RBTgetRight(child);

     RBTattachLeftNode(grandparent, temp[1]);
     RBTattachRightNode(parent, temp[0]);
     RBTattachRightNode(child, grandparent);
     RBTattachLeftNode(child, parent);

     /* reset red_black fields */
     RBTsetRBleft(grandparent, BLACK);
     RBTsetRBright(parent, BLACK);
     RBTsetRBleft(child, RED);
     RBTsetRBright(child, RED);
}

/**********************************************************************/
static void 
RBTright_left(RBTree      *thetree, 
	      RBTree_node *child, 
	      RBTree_node *parent,
	      RBTree_node *grandparent,
	      RBTree_node *great_grandparent)
{
     RBTree_node *temp[2];

#ifdef RBT_TEST
     printf("right_left called\n");
#endif 

     if (great_grandparent != NULL )
	  if (RBTgetDirection(great_grandparent) == RIGHT)
	       RBTattachRightNode(great_grandparent, child);
	  else
	       RBTattachLeftNode(great_grandparent, child);

     else
	  RBTsetEntry(thetree, child);
     
     temp[0] = RBTgetLeft(child);
     temp[1] = RBTgetRight(child);


     RBTattachRightNode(grandparent, temp[0]);
     RBTattachLeftNode(parent, temp[1]);
     RBTattachLeftNode(child, grandparent);
     RBTattachRightNode(child, parent);

     /* reset red_black fields */
 
     RBTsetRBright(grandparent, BLACK);
     RBTsetRBleft(parent, BLACK);
     RBTsetRBright(child, RED);
     RBTsetRBleft(child, RED);
}

/**********************************************************************/
static void 
RBTright_right(RBTree *thetree,
	       RBTree_node * parent, 
	       RBTree_node * grandparent,
	       RBTree_node * great_grandparent)
{
     RBTree_node *temp;

#ifdef RBT_TEST
     printf("right_right called\n");
#endif

     if (great_grandparent != NULL) {
	  if (RBTgetDirection(great_grandparent) == RIGHT)
	       RBTattachRightNode(great_grandparent, parent);
	  else
	       RBTattachLeftNode(great_grandparent, parent);
     }
     else 
	  RBTsetEntry(thetree, parent);
 
     temp = RBTgetLeft(parent);
     RBTattachRightNode(grandparent, temp);
     RBTattachLeftNode(parent, grandparent);
 
     /* reset red_black fields appropriately */
 
     RBTsetRBright(grandparent, BLACK);
     RBTsetRBleft(parent, RED);
}


/**********************************************************************/


static void
RBTleft_left(RBTree *thetree,
	     RBTree_node *parent, 
	     RBTree_node *grandparent,
	     RBTree_node *great_grandparent)
{
     RBTree_node *temp;

#ifdef RBT_TEST
     printf("left_left called\n");
#endif
     if (great_grandparent != NULL)
	  RBTattachLeftNode(great_grandparent, parent);
     else
	  RBTsetEntry(thetree, parent);
 
     temp = RBTgetRight(parent);
     RBTattachLeftNode(grandparent, temp);
     RBTattachRightNode(parent, grandparent);
     
     RBTsetRBleft(grandparent, BLACK);/* reset rb fields */
     RBTsetRBright(parent, RED);
     
}
 

/**********************************************************************/
void
RBTinsert(RBTree *thetree, char *data)
{
     RBTree_node *data_node ;
     RBTree_node *local_root ;
     RBTree_node *current    ;
     RBTree_node *parent = NULL; 
     RBTree_node *grandparent = NULL;
     RBTree_node *great_grandparent = NULL ;
     boolean      balanced=FALSE;
 
     data_node = RBTNnew(); /* create and initialize new node*/
     RBTaddData(data_node, data);
 
     local_root = RBTgetEntry(thetree);

     if (local_root == NULL) { /* start a new tree */
	  RBTsetEntry(thetree, data_node);
	  return;
     }


     current = local_root; /* get ready to insert new node*/
     
     while (current != NULL) {
	  /* check for two red siblings*/
	  RBTred_sibling_ck(current,parent);
	  
	  if (!balanced) { /* only one rebalance per insertion*/
	       balanced = RBTbalance(thetree, current, parent,
				     grandparent, great_grandparent);
	       
	       if (balanced == 0) {
		    /* if we haven't rebalanced update grandparent
		     * and great grand parent. ie if rebalancing have been
		     * done, all of these local ptrs execpt current are
		     * invalid.
		     */
		    
		    great_grandparent=grandparent;
		    grandparent=parent;
	       }
	  }
	  parent=current;
	  
	  if (RBTcmp(thetree, RBTgetData(current), data) > 0)  {
	       /* mark where we're moving  and go left */
	       RBTsetDirection(current, LEFT); 
	       current = RBTgetLeft(current);
	  } else {
	       /* mark where we're moving  and go right */
	       RBTsetDirection(current, RIGHT); 
	       current = RBTgetRight(current);
	  }
     } 
     
     if (RBTgetDirection(parent) == LEFT) {
	  RBTattachLeftNode(parent, data_node);
	  RBTsetRBleft(parent, RED); 
     } else {
	  RBTattachRightNode(parent, data_node);
	  RBTsetRBright(parent, RED);
     }
}

/**********************************************************************
 * Functions for the top level tree structure...
 */

RBTree *
RBTnew(int (*cmpfunc)(), void (*destroyfn)())
{
     RBTree *temp;

     temp = (RBTree*) malloc(sizeof(RBTree));
     
     if (temp == NULL)
	  return(NULL);

     temp->cmpfn = cmpfunc;
     temp->destroyfn = destroyfn;
     temp->entry = NULL;

     return(temp);
}

void
RBTdestroy(RBTree *rbt)
{
     RBTdestroy_node(rbt, RBTgetEntry(rbt));
     free(rbt);
}


static void
RBTinorder(RBTree_node *node, void (*func)())
{

     if (node != NULL) {

 	  RBTinorder(RBTgetLeft(node), func);

	  func(RBTgetData(node));

	  RBTinorder(RBTgetRight(node), func);
     }
}

void
RBTtraverse(RBTree *rbt, void (*func)())
{
     RBTinorder(RBTgetEntry(rbt), func);
}


char *
RBTfind(RBTree *rbt, char *data)
{
     RBTree_node *rbt_node = RBTgetEntry(rbt);
     int whichway;

     while ((whichway = RBTcmp(rbt, data, RBTgetData(rbt_node))) != 0) {
	  if (whichway < 0)
	       rbt_node = RBTgetLeft(rbt_node);
	  else
	       rbt_node = RBTgetRight(rbt_node);

	  if (rbt_node == NULL)
	       return(NULL);
     }

     return(RBTgetData(rbt_node));
}

/**********************************************************************
 * Test code below here....
 */

#ifdef RBT_TEST
#include <stdio.h>
#include <string.h>
static int count = 0;

void
printit(char *data)
{

     printf("%s", data);
}

void
main()
{
     FILE *foo;
     char myline[256];
     char *moo;

     mytree = RBTnew(strcmp, free);
     
     foo = fopen("testfile", "r");
     while(fgets(myline, sizeof(myline), foo)) {
	  RBTinsert(mytree, strdup(myline));

/*	  RBTtraverse(mytree, printit);*/

     }
     /** Find an item **/
     moo = RBTfind(mytree, "butterfly\n");
     if (moo) 
	  printf("Found %s\n", moo);
     else 
	  printf("Didn't find butterfly...\n");

     RBTtraverse(mytree, printit);

     RBTdestroy(mytree);

}
#endif
