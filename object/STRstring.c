/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/STRstring.c,v $
 * $Status: $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: STRstring.c
 * Implement dynamic string library functions
 *********************************************************************
 * Revision History:
 * $Log: STRstring.c,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.11  1995/09/25  22:07:19  lindner
 * Ansification
 *
 * Revision 3.10  1995/06/30  20:32:04  lindner
 * start protoing
 *
 * Revision 3.9  1994/03/17  05:52:49  lindner
 * Fix for multiple STRinits
 *
 * Revision 3.8  1994/03/04  17:31:39  lindner
 * Don't assume a char is one byte long
 *
 * Revision 3.7  1994/02/20  16:23:13  lindner
 * Optimize STRinit so that memory isn't freed then reallocated
 *
 * Revision 3.6  1993/10/27  18:53:23  lindner
 * don't forget \0
 *
 * Revision 3.5  1993/10/22  20:15:52  lindner
 * Remove superfulous declaration of len (Fote)
 *
 * Revision 3.4  1993/10/19  20:46:00  lindner
 * Better, tighter STRstring stuff (Fote)
 *
 * Revision 3.3  1993/08/16  19:35:09  lindner
 * Return a correct value for STRcpy
 *
 * Revision 3.2  1993/03/24  17:07:52  lindner
 * STRset with a NULL value will STRinit() the string
 *
 * Revision 3.1.1.1  1993/02/11  18:03:03  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#include "STRstring.h"
#include "String.h"
#include "Malloc.h"

/*
 * Make a new string, don't set anything for default yet.
 */

String *
STRnew(void)
{
     String *temp;

     temp = (String *) malloc(sizeof(String));
     temp->data = NULL;
     temp->len = 0;

     return(temp);
}

/*
 * Destroy a string
 */

void
STRdestroy(String *st)
{
     if (st != NULL) {
	  if (st->data != NULL)
	       free(st->data);
	  free(st);
     } else
	  perror("STRdestroy: non existant string!\n");

}


/*
 * Don't free the memory.  Instead, negate the length value and keep the
 * memory for later use.  More efficient that way.
 */

void 
STRinit(String *st) 
{
     if (st != NULL) {
	  if (st->len > 0)
	       st->len  = - st->len;
/*	  if (st->data != NULL) {
	       free(st->data);
	       st->data = NULL;
	  }*/
     } else
	  perror("STRinit, non existant string!");
}

/*
 * Set a string value
 */

void
STRset(String *st, char *str)
{
     register int len;

     /* To set a null value, STRinit the item */

     if (str == NULL) {
	  STRinit(st);
	  return;
     }

     /* Negative value for len means memory is initialized, reset to 
	positive value so the rest of the code works well.
      */
 
     if (st->len < 0) 
	  st->len = -st->len;

     if (*str == '\0')
	  len = 1;
     else
	  len = strlen(str)+1; /** Don't forget the '\0' **/

     /* Uninitialized data... */

     if (st->data == NULL) {
	  st->data = (char *) malloc(sizeof(char*) * len);
	  st->len = len;
     }

     /** Something's already there... **/

     else if (STRsize(st) < len) {
	  char *temp;

	  temp = (char *) realloc(st->data, sizeof(char) * len);
	  /*** Should check for NULL ... ***/
	  if (temp == NULL)
	       perror("realloc failed...");

	  st->data = temp;
	  st->len  = len;
     }
     /* space is ok and st->len is set, so copy in the new string */
     strcpy(st->data, str);
}

/*
 * Add a string to the end of the string that's there already
 */

String*
STRcat(String *st, char *cp)
{
     int len;
     char *temp;

     if (cp == NULL)
	  return(NULL);
     
     if (STRlen(st) == 0) {
	  STRset(st, cp);
	  return(st);
     }

     len = strlen(cp) + STRlen(st);

     temp = (char *) malloc(sizeof(char) * len);
     strcpy(temp, STRget(st));
     strcat(temp, cp);

     STRset(st, temp);
     
     free(temp);

     return(st);
}


int
STRcmp(String *st1, String *st2)
{
     register char *cp1, *cp2;

     cp1 = STRget(st1);
     cp2 = STRget(st2);

     if (cp1 == NULL) 
	  return(- !0);
     else if (cp2 == NULL)
	  return( !0);
     else
	  return(strcmp(cp1, cp2));
}
     
String*
STRcpy(String *s1, String *s2)
{
     STRset(s1, STRget(s2));
     return(s1);
}
