/*
 * These routines make using the international messages facilities easier.
 * 
 * This interface could be applied to methods other than X/Open coding
 * (VMS etc..)
 *
 * Copyright (c) 1994 University of Minnesota
 */

#ifndef GLOCALE_H
#define GLOCALE_H


#if defined(_XOPEN_SOURCE) || defined(_AIX) || defined(__osf__)
#  define GINTERNATIONAL
#endif

#ifdef VMS

#  define LC_ALL ""
   void setlocale(char *, char *);
   void rsetlocale(int);
   char *Gtxt(char *, int);
   int Gtxtopen(char *,int);
   char *catgets(int, int, int, char *);
#  define A_LANGUAGE		    '\001'
#  define GINTERNATIONAL
#  define NL_SETD 0
   typedef int nl_catd;

#else /* Unix: */

#  ifndef GINTERNATIONAL	/* Only use i18n routines if we're sure */
#    define Gtxtlocale(a,b)
#    define setlocale(a,b)
#    define Gtxtopen(a,b)
#    define Gtxt(a,b)	    a
#  else
#    include "compatible.h"

#    if !defined(HAVE_LOCALE_H)
#      undef  NO_LOCALE
#      define NO_LOCALE
#      undef  NO_XPGCAT	 /** Can't use X/Open message catalogs **/
#      define NO_XPGCAT	 /** without setlocale()	       **/	 
#      define LC_ALL	0
#      define LC_MESSAGES	1
#    else
#      include <locale.h>
#    endif

/** Figure out which systems have catopen() etal **/
#    if !defined(__svr4__) && !defined(_XOPEN_SOURCE) && !defined(_XOPEN_VERSION)
#      undef  NO_XPGCAT
#      define NO_XPGCAT
#    endif

#    ifndef NO_XPGCAT
#      include <nl_types.h>
#    else
       typedef int nl_catd;
#    endif

#    ifndef NL_SETD
#      define NL_SETD 0		/* SunOS 4.1.3 with xpg2bin/cc compiler */
#    endif

     /*** Define some nice convenience routines ***/
     /**  It would sure be nice to use gettext() routines... oh well...***/

     extern nl_catd Gcatd;

     char	*Gtxtlocale();
     char	*Gtxt();
     nl_catd	 Gtxtopen();

#  endif /* GINTERNATIONAL */
#endif /* VMS */
#endif /* GLOCALE_H */

