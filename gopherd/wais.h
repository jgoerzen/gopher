/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/wais.h,v $
 * $Status: $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: wais.h
 * Header file for Wais stuff.
 *********************************************************************
 * Revision History:
 * $Log: wais.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.4  1995/01/04  17:36:53  lindner
 * Fixes for FreeWAIS-0.4
 *
 * Revision 3.3  1994/05/14  04:18:43  lindner
 * Use new locale.h
 *
 * Revision 3.2  1994/05/02  07:41:16  lindner
 * Mods to use setlocale()
 *
 * Revision 3.1.1.1  1993/02/11  18:02:53  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/




/* WIDE AREA INFORMATION SERVER SOFTWARE:
   No guarantees or restrictions.  See the readme file for the full standard
   disclaimer.

   This is part of the shell user-interface for the WAIS software.  Do with it
   as you please.

   jonathan@Think.COM

 * $Log: wais.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.4  1995/01/04  17:36:53  lindner
 * Fixes for FreeWAIS-0.4
 *
 * Revision 3.3  1994/05/14  04:18:43  lindner
 * Use new locale.h
 *
 * Revision 3.2  1994/05/02  07:41:16  lindner
 * Mods to use setlocale()
 *
 * Revision 3.1.1.1  1993/02/11  18:02:53  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 * Revision 1.6  92/02/15  19:50:24  jonathan
 * Removed old cruft.  Added $Log for RCS
 * 
*/

#ifndef _H_WAIS
#define _H_WAIS

/* usefull definitions */

#define STRINGSIZE	256

#include <Locale.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

/* from the IR directory */

#include <cutil.h>
/*#include <irdirent.h>*/
#include <ui.h>
#include <irfileio.h>
#include <sockets.h>
FILE *connect_to_server _AP((char* host_name,long port));

/* for this application */

#include "list.h"
#include "source.h"
#include "document.h"
/*#include "../ui/util.h"*/
#include "question.h"
#define CHARS_PER_PAGE 2000

#ifdef MAIN
#define ext
#else
#define ext extern
#endif

ext char* command_name;

ext int numtosave;

ext int NumQuestions;

ext Question the_Question;

ext SList Sources;
ext int NumSources;

ext int maxDocs;

#endif
