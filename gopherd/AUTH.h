/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.6 $
 * $Date: 2002/01/08 17:50:35 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/AUTH.h,v $
 *
 * Paul Lindner, University of Minnesota DCS.
 *
 * Copyright 1991, 92, 93, 94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: AUTH.c
 * Routines to verify authentication
 *********************************************************************
 * Revision History:
 * $Log: AUTH.h,v $
 * Revision 1.6  2002/01/08 17:50:35  jgoerzen
 *   * AUTH.h: Add a check to prevent including twice.
 *   * gopherdconf.c: Add a missing prototype.
 *
 * Revision 1.5  2002/01/08 17:48:31  jgoerzen
 *   * AUTH.h: Add missing AUTHITEMSprocessLine prototype
 *
 * Revision 1.4  2002/01/08 16:38:38  jgoerzen
 * Modified to:
 *   Have configure.in look for crypt.h
 *   Reran autoconf and autoheader
 *   Make Makefile.in do a more thorough cleaning job
 *   Make AUTH.h include crypt.h and unistd.h only if present
 *
 * Revision 1.3  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.2  2000/08/19 01:25:26  jgoerzen
 * Now includes crypt.h
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.9  1995/09/26  05:16:22  lindner
 * more fixes...
 *
 * Revision 3.8  1995/09/25  05:02:29  lindner
 * Convert to ANSI C
 *
 * Revision 3.7  1995/06/30  20:33:10  lindner
 * Add group authentication
 *
 * Revision 3.6  1995/06/02  17:24:37  lindner
 * Add the ability to specify alternate password files for unixfile auth.
 *
 * Revision 3.5  1995/02/13  19:10:34  lindner
 * next compile
 *
 * Revision 3.4  1995/02/06  21:24:58  lindner
 * Add stuff for unix pw file
 *
 * Revision 3.3  1994/12/12  16:58:13  lindner
 * Add new AUTHresult code, AUTHRES_SYSERR
 *
 * Revision 3.2  1994/07/22  22:27:44  lindner
 * NO_AUTHENTICATION mods
 *
 * Revision 3.1  1994/06/29  05:34:20  lindner
 * New authentication routines
 *
 *
 *********************************************************************/

#ifndef __SEEN_GOPHERD_AUTH_H__
#define __SEEN_GOPHERD_AUTH_H__

/*
 * Platform independent authentication routines go here...
 *
 * AUTH object     --> contains auth type name and script/fcn to call
 * AUTHA object    --> dynamic array of AUTH objects.
 *
 * AUTHITEM object --> contains directory, authtype, link to dns sites
 */

#include "config.h"
#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif
#ifdef HAVE_UNISTD_H
#endif
#include "boolean.h"
#include "STAarray.h"

typedef enum {AUTH_UNINITED, AUTH_INTERNAL, AUTH_SCRIPT} AUTHtype;
typedef enum {AUTHRES_OK, AUTHRES_BADPW, AUTHRES_EXPIRED,
	      AUTHRES_NOUSER, AUTHRES_SYSERR, AUTHRES_NOGROUP} AUTHresult;

struct AUTHstruct {
     AUTHtype      type;
     String        *name;

     AUTHresult    (*authfcn)();        /* The authentication test */
     char        **(*authfcnask)();     /* The authentication askblock fcn */
     String        *scriptname;
};
     
typedef struct AUTHstruct AUTH;

/*
 * Member functions
 */

#define AUTHgetType(a)         (a->type)
#define AUTHgetName(a)         (STRget(a->name))
#define AUTHgetScriptName(a)   (STRget(a->scriptname))
#define AUTHgetFcn(a)          (a->authfcn)
#define AUTHgetAskFcn(a)       (a->authfcnask)

#define AUTHsetType(a,b)       (a->type=(b))
#define AUTHsetName(a,b)       (STRset(a->name,(b)))
#define AUTHsetScriptName(a,b) (STRset(a->scriptname,b))
#define AUTHsetFcn(a,b)        (a->authfcn=(b))
#define AUTHsetAskFcn(a,b)     (a->authfcnask=(b))

/*
 * Prototypes
 */
AUTH *AUTHnew();
void  AUTHdestroy();
void  AUTHinit();
AUTH *AUTHcpy();

AUTHresult AUTHunix();
AUTHresult AUTHident();

char **AUTHgenericAsk();

/******************************************************************
 * Array of auth stuff...
 */

#include "DAarray.h"

typedef DynArray AUTHarray;

#define AUTHAgetEntry(a,b)  (AUTH*)(DAgetEntry((DynArray*)a,b))
#define AUTHAdestroy(a)     (DAdestroy(a))
#define AUTHApush(a,b)      (DApush((a),(char*)(b)))
#define AUTHAgetTop(a)      (DAgetTop(a))

AUTHarray   *AUTHAnew();
AUTHresult   AUTHAvalidate();
AUTH*        AUTHAlocate();

/******************************************************************
 *  List of items and authentication types.
 *
 */ 

struct AUTHITEMS_struct {
     StrArray *regexps;
     StrArray *authtypes;
     StrArray *extra_args;	/* Used for Unix PW files for now.. */
};

typedef struct AUTHITEMS_struct AUTHITEMS;

#define AUTHITEMSgetRegexp(a,i)   (STRget(STAgetEntry((a)->regexps,i)))
#define AUTHITEMSgetAuthtype(a,i) (STRget(STAgetEntry((a)->authtypes,i)))
#define AUTHITEMSgetPwFile(a,i)   (STRget(STAgetEntry((a)->extra_args,i)))

#ifndef NO_AUTHENTICATION
   AUTHITEMS *AUTHITEMSnew();
   void       AUTHITEMSdestroy(AUTHITEMS *);
   void       AUTHITEMSpush(AUTHITEMS *, char *, char *, char *);
   AUTH      *AUTHITEMSfindAUTH(AUTHITEMS *ai, AUTHarray *aa, char *fname);
   char      *AUTHITEMSfindType(AUTHITEMS *ai, char *fname);
   char      *AUTHITEMSfindExtraArgs(AUTHITEMS *ai, char *fname);
   boolean   AUTHAprocessLine(AUTHarray *autharr, char *inputline);
   boolean   AUTHITEMSprocessLine(AUTHITEMS *ai, char *inputline);
#else
#  define AUTHAvalidate(a,b,c,d,e,f) (AUTHRES_OK)
#  define AUTHITEMSdestroy(a) 
#  define AUTHAnew(a) (NULL)
#  define AUTHAprocessLine(a,b) (1)
#  define AUTHITEMSfindType(a,b) (NULL)
#  define AUTHITEMSprocessLine(a,b) (1)
#  define AUTHITEMSnew(a) (NULL)
#  define AUTHITEMSfindAUTH(a,b,c) (NULL)
#endif

#endif      /* __SEEN_GOPHERD_AUTH_H__ */
