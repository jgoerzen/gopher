/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.5 $
 * $Date: 2002/03/19 20:07:16 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/Regex.c,v $
 * $Status: $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: Regex.c
 * Portable method of doing regular expressions
 *********************************************************************
 * Revision History:
 * $Log: Regex.c,v $
 * Revision 1.5  2002/03/19 20:07:16  jgoerzen
 * GSgopherobj.c: Moved Regex.h include up.
 *
 * Regex.c, Regex.h: continuing full rewrites.
 *
 * Revision 1.4  2002/03/19 19:53:31  jgoerzen
 * *** empty log message ***
 *
 * Revision 1.3  2002/02/12 20:50:21  jgoerzen
 * Beginning of MacOS X (Darwin) support.
 * Many modifications to Regex.[ch], see debian/changelog for details.
 *
 * Revision 1.2  2002/02/12 19:54:18  jgoerzen
 * Updated with regex fixes
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.2  1994/07/25  13:56:53  lindner
 * First crack at POSIX regular expressions
 *
 * Revision 3.1  1993/10/19  20:48:23  lindner
 * Portable versions of Regular expression routines for System V and BSD..
 *
 *
 *********************************************************************/

#define __GOPHER_REGEX_C__

/*
 * If you're using gcc on Solaris you might need to copy /usr/include/regexp.h
 * to /opt/whatever/regexp.h
 */

#define  REGEX_CODEIT    /* only include sysv regex code once.. */
#include "config.h"
#include "Regex.h"
#include "Malloc.h"

/**************************************************
 POSIX
**************************************************/

#ifdef REGEX_POSIX
regex_t preg;
int pregallocated = 0;

char *posix_re_comp(char *regex) {
  if (pregallocated) {
    regfree(&preg);
  }

  pregallocated = 1;
  return regcomp(&preg, regex, REG_NOSUB) ? "regcomp error" : NULL;
}

int posix_re_exec(char *string) {
  return ! regexec(&preg, string, 0, NULL, 0);
}
  
#endif /* REGEX_POSIX */

/**************************************************
 SYSV
**************************************************/

#ifdef REGEX_SYSV

#define ESIZE 512
static char expbuf[ESIZE];

char *
re_comp(char *expr)
{
     char *result;

     result = compile(expr, expbuf, &expbuf[ESIZE], '\0');

     return(NULL);
}

int
re_exec(char *string)
{
     return(step(string, expbuf));
}

#endif /* REGEX_SYSV */
