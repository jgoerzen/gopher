/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/dedot.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: dedot.c
 * See below.
 *********************************************************************
 * Revision History:
 * $Log: dedot.c,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.10  1995/09/25  05:02:32  lindner
 * Convert to ANSI C
 *
 * Revision 3.9  1994/04/22  03:35:16  lindner
 * Fix for spinning gopherd processes
 *
 * Revision 3.8  1994/04/14  15:48:52  lindner
 * Fix for files with single quotes
 *
 * Revision 3.7  1994/03/04  23:25:14  lindner
 * Fix for dedot weirdness
 *
 * Revision 3.6  1993/09/18  03:26:38  lindner
 * Important Security fix
 *
 * Revision 3.5  1993/08/23  18:33:57  lindner
 * Fix off by one error
 *
 * Revision 3.4  1993/08/06  14:28:14  lindner
 * Further strip out single quotes
 *
 * Revision 3.3  1993/08/05  22:19:43  lindner
 * Fix for single quotes
 *
 * Revision 3.2  1993/08/05  20:43:07  lindner
 * Added fix for dedot to remove quotes for when using system or popen
 *
 * Revision 3.1.1.1  1993/02/11  18:02:50  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


#include <stdio.h>

/*
** These routines can be used to remove ../ and ./ entries from
** pathnames.  These routines do it radically, without any attempt
** at all at interpretation.  This is okay for a gopher server,
** because clients aren't supposed to ask for .. or . in a request.
**
** We want to do this so that we can avoid the chroot(), so that we
** can have symbolic links under the gopher directory to other things
** (like man pages for example) outside our structure, and still be
** safe.  Unless of course someone makes a link to / or somewhere silly.
**
** dedot1 will remove dots from one string in place, dedot2 will copy
** one to the other, removing in the process - the src and dst can be
** the same.  dedot1 can use this to advantage - it checks first if
** things have to be removed, and calls dedot2 if necessary.  This
** is a slight advantage because in most cases we won't do anything,
** and we save the expense of copying data back and forth.  This
** seems to save almost half the time (very loose testing though).
**
** John Sellens jmsellens@watdragon.waterloo.edu
*/

void dequote1(char *);
void dedot1(char *);
void dedot2(char *, char *);


void
dequote1(char *src)
{
     char *cp2; 

     /** Strip out the quotes.. **/
     while (*src != '\0') {
	  if (*src == '"' || *src == '\'') {
	       for (cp2=src; *cp2 != '\0'; cp2++) {
		    *cp2 = *(cp2+1);
	       }
	  }
	  src++;
     }
}

void
dedot1(char *src )
{
     dequote1(src);
     if ( *src == '.' ) {
	  if (src[1] == '\0' || src[1] == '/' ||
	      ( src[1] == '.' && ( src[2] == '\0' || src[2] == '/' ) ) ) {
	       dedot2( src, src );
	       return;
	  }
     }
     while ( *src ) {
	  if ( *src++ == '/' ) {
	       if ( *src == '.' ) {
		    if (src[1] == '\0' || src[1] == '/' ||
			( src[1] == '.' && ( src[2] == '\0' || src[2] == '/' ) ) ) {
			 dedot2( src, src );
			 return;
		    }
	       }
	  }
     }
     return;
}

/* copy src to dst, blindly removing (not interpreting) ./ and ../ */

void
dedot2(char *src, char *dst)
{
     /*
      ** We either have /, a filename, ./ or ../
      */
     int i;

     while ( *src ) {
          switch ( *src ) {
          case '/':
               /* copy it, and skip any extras e.g. /a///b */
               *dst++ = *src++;
               while ( *src == '/' )
                    src++;
               break;
          case '.':
               /* don't forget about trailing . and .. */

               /* Nuke any quotes */
               for (i=1; src[i] == '"' || src[i] == '\''; i++)
                    ;
               
               if ( src[i] == '/'  ) {          /* ./ */
                    src += i+1;
                    break;
               }
               if ( src[i] == '\0'  ) { /* .\0 */
                    src += i;   /* only 1 so we don't fall off the end */
                    break;
               }
               if ( src[i] == '.' &&
                    (src[i+1] == '"' || src[i+1] == '\'')) { /* ..", ..' */
                    src += i+2;
                    break;
               }
               if ( src[i] == '.' && src[i+1] == '/' ) { /* ../ */
                    src += i+2;
                    break;
               }
               if ( src[i] == '.' && src[i+1] == '\0' ) {       /* .. */
                    src += i+1; /* don't fall off the end */
                    break;
               }
               /* must be a filename - fall through */
          default:
               /* must be filename - copy it over */
               while ( *src != '\0' && *src != '/')
		    /** Note quotes are taken care of by Gpopen **/
                    *dst++ = *src++;
               break;
          }
     }
     /* and terminate it */
     *dst = '\0';
}
