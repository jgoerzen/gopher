/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/ext.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: ext.c
 * These fns define mapping of file extensions to gopher objects.
 *********************************************************************
 * Revision History:
 * $Log: ext.c,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.20  1995/09/25  05:02:32  lindner
 * Convert to ANSI C
 *
 * Revision 3.19  1994/09/29  19:57:45  lindner
 * Less picky language and view comparisions
 *
 * Revision 3.18  1994/07/21  15:46:22  lindner
 * Brian Coen Patch
 *
 * Revision 3.17  1994/07/19  20:27:35  lindner
 * Use local Locale.h
 *
 * Revision 3.16  1994/05/02  07:41:11  lindner
 * Mods to use setlocale()
 *
 * Revision 3.15  1994/04/26  22:43:57  lindner
 * Fix for Debugmsg
 *
 * Revision 3.14  1994/04/25  20:49:04  lindner
 * Fix for debug code
 *
 * Revision 3.13  1994/01/20  06:39:18  lindner
 * When trying to match an extension to a file type, the server prefers
 * exact matches of type & language, but will now fall back to matching
 * the type in any language (acf).
 *
 * Revision 3.12  1993/10/27  20:35:24  lindner
 * Plug memory leak
 *
 * Revision 3.11  1993/10/20  03:17:25  lindner
 * Fixed erratic problems with extension adding.  Note that the problems
 * with directories not working with Gopher0 clients is due to a missing
 * parameter in a strncasecmp() call..  Makes one wish ANSI C was more
 * widespread...
 *
 * Also added regular expression code for ignore_patt:
 *
 * Revision 3.10  1993/10/04  06:39:31  lindner
 * Fix for forward declarations
 *
 * Revision 3.9  1993/09/21  02:35:04  lindner
 * Server now adds extensions in a case insensitive manner
 *
 * Revision 3.8  1993/09/18  03:30:11  lindner
 * Remove memory leak
 *
 * Revision 3.7  1993/09/18  02:22:35  lindner
 * More debugging...
 *
 * Revision 3.6  1993/08/19  20:25:46  lindner
 * Mitra's Debug patch
 *
 * Revision 3.5  1993/08/03  06:03:08  lindner
 * Ignored files are now ignored again
 *
 * Revision 3.4  1993/07/31  00:15:37  lindner
 * Fixed weird extension problem
 *
 * Revision 3.3  1993/07/27  20:15:29  lindner
 * Fixed bug where filename was less than extension
 *
 * Revision 3.2  1993/07/23  03:18:46  lindner
 * Mods for using decoder:'s
 *
 * Revision 3.1.1.1  1993/02/11  18:02:51  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.3  1993/02/09  22:13:19  lindner
 * many additions for multilingual gopher
 *
 * Revision 1.2  1993/01/30  23:56:55  lindner
 * Lots of new code for gopher+ stuff..  Change everything to start with EX
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#include <stdio.h>
#include "ext.h"
#include "Malloc.h"
#include "Locale.h"
#include <ctype.h>
#include "util.h"
#include "serverutil.h"
#include "openers.h" 
#undef stat
#include "Debug.h"

#include "Dirent.h"
#include "Regex.h"

/*
 * Some functions to initialize and destroy sites and extensions..
 * (Needed for use with DynArrays...)
 */

Extobj *
EXnew()
{
     Extobj *temp;

     temp = (Extobj *) malloc(sizeof(Extobj));
     
     if (temp == NULL)
	  return(NULL);

     temp->Objtype   = '\0';
     temp->View      = STRnew();
     temp->ViewLang  = STRnew();
     temp->Prefix    = STRnew();
     temp->ext       = STRnew();
     temp->exttype   = EXT_ERROR;
     temp->GenericData = STRnew();

     STRinit(temp->View);
     STRinit(temp->ViewLang);
     STRinit(temp->Prefix);
     STRinit(temp->ext);
     STRinit(temp->GenericData);
     
     return(temp);
}


/** Destroy an extension structure **/

void 
EXdestroy(Extobj *ext)
{
     STRdestroy(ext->View);
     STRdestroy(ext->ViewLang);
     STRdestroy(ext->Prefix);
     STRdestroy(ext->ext);
     STRdestroy(ext->GenericData);
     
     free(ext);
}


/** Copy an extension structure **/

void
EXcpy(Extobj *ext1, Extobj*ext2)
{
     ext1->Objtype  = ext2->Objtype;
     STRcpy(ext1->View, ext2->View);
     STRcpy(ext1->ViewLang, ext2->ViewLang);
     STRcpy(ext1->Prefix, ext2->Prefix);
     STRcpy(ext1->ext, ext2->ext);
     STRcpy(ext1->GenericData, ext2->GenericData);
     EXsetExttype(ext1, EXgetExttype(ext2));
}


/*
 * Do the grungy work of adding an extension
 */

char *
EXAfindFile(ExtArray *extarr, char *file, char *view)
{
     boolean     status = TRUE;
     Extobj      *ext, *temp;
     DIR         *ZeDir;
     struct dirent *dp;
     struct stat statbuf;
     static char tmpfile[512];
     char        directory[512], *cp;
     int         tmpfilelen = 0, i;

     if (file == NULL || *file == '\0')
	  return(file);

     strcpy(tmpfile, file);
     ext = EXnew();

     /** Find a viewext if available..
         Should be able to cycle through all of them... **/
     status = EXAviewSearch(extarr, ext, view);
     
     if (status == TRUE)
	  strcat(tmpfile, EXgetExt(ext));

     EXdestroy(ext);

     strcpy(directory, tmpfile);
     cp = mtm_basename(directory);
     *cp = '.';
     *(cp+1) = '\0';
     
     if ((ZeDir = ropendir(directory)) == NULL) {
	  /** Error **/
	  return(file);
     }

     cp = mtm_basename(tmpfile);

     for (dp = readdir(ZeDir); dp != NULL; dp = readdir(ZeDir)) {
	  int x;

	  x = strcasecmp(cp, dp->d_name);

	  if (x == 0) {
	       /** Found an exact match **/
	       strcpy(cp, dp->d_name);
	       closedir(ZeDir);
	       return(tmpfile);
	  }
     }
     /** Now check for a not so exact match.. **/

     rewinddir(ZeDir);

     for (dp = readdir(ZeDir); dp != NULL; dp = readdir(ZeDir)) {
	  int x;

	  x = strncasecmp(cp, dp->d_name, strlen(dp->d_name));
	  if (x==0)
	       /** Found a match for a possible decoder **/
	       strcpy(cp, dp->d_name); 
     }
     
     closedir(ZeDir);

     /** Hmm can't seem to find it..  Let's try the decoders... **/

     tmpfilelen = strlen(tmpfile);

     /** Check for decoders.. **/
     for (i=0; i< EXAgetNumEntries(extarr); i++) {
	  temp = EXAgetEntry(extarr,i);
     
	  if (EXgetExttype(temp) == EXT_DECODER) {
	       char decodertmp[256];
	       
	       strcpy(decodertmp, tmpfile);
	       strcat(decodertmp, EXgetExt(temp));
	       
	       if (!rstat(decodertmp, &statbuf)) {
		    /** Found it! **/
		    strcpy(tmpfile, decodertmp);
		    return(tmpfile);
	       }
	  }

     }
     /*** Couldn't find anything... **/
     return(file);
}

/*
 * Get the parameters associated with a particular extension
 *
 * Fills in ext with the values..
 */

boolean
EXAsearch(ExtArray *extarr, Extobj *ext, char *fileext, int exttype)
{
     int i, extlen;
     Extobj *temp;

     Debugmsg("EXAsearch:\r\n");
     extlen = strlen(fileext);

     /*** Linear search.  Ick. ***/
     
     for (i=0; i< EXAgetNumEntries(extarr); i++) {
	  temp = EXAgetEntry(extarr,i);
	  
	  if (exttype == EXgetExttype(temp)) {
	       int exlen = strlen(EXgetExt(temp));

	       if (exlen <= extlen && strcasecmp(fileext+extlen-exlen,
			      EXgetExt(temp)) == 0) {

		    EXcpy(ext,temp);
		    return(TRUE);
	       }
	  }
	  else if (exttype == EXT_IGNORE &&
		   EXgetExttype(temp) == EXT_IGNOREPAT) {
	       
	       if (re_comp(EXgetExt(temp)) != NULL)
		    return(FALSE);
	       
	       if (re_exec(fileext) == 1) {
		    EXcpy(ext,temp);
		    return(TRUE);
	       }
	  }
     
     }
     return(FALSE);
}


/*
 * Do an exact search...
 */

boolean
EXAcasedSearch(ExtArray *extarr, Extobj *ext, char *fileext, int exttype)
{
     int i, extlen;
     Extobj *temp;

     Debugmsg("EXAsearch:\r\n");
     extlen = strlen(fileext);

     /*** Linear search.  Ick. ***/
     
     for (i=0; i< EXAgetNumEntries(extarr); i++) {
	  temp = EXAgetEntry(extarr,i);
	  
	  if (exttype == EXgetExttype(temp)) {
	       if (strcmp(fileext+extlen-strlen(EXgetExt(temp)),
			      EXgetExt(temp)) == 0) {
	       
		    EXcpy(ext,temp);
		    return(TRUE);
	       }
	  }
     }
     return(FALSE);
}


/*
 * Search for a specific view extension...
 *   Prefers exact match of type & lang, but if it can't find one it will
 *   fall back to matching type in any language.
 */

boolean
EXAviewSearch(ExtArray *extarr, Extobj *ext, char *view)
{
     int     i;
     Extobj  *temp;
     char    viewnlang[256];
     boolean found = FALSE;

     /*** Linear search.  Ick. ***/
     
     for (i=0; i< EXAgetNumEntries(extarr); i++) {

	  temp = EXAgetEntry(extarr,i);

	  if (EXgetExttype(temp) == EXT_VIEW) {

	       strcpy(viewnlang, EXgetView(temp));

 
 	       if (!found && strcasecmp(view, viewnlang)==0) {
 		    EXcpy(ext,temp);
 		    found = TRUE;
 	       }
 

	       if (EXgetVLang(temp) != NULL && *EXgetVLang(temp) != '\0') {
		    strcat(viewnlang, " ");
		    strcat(viewnlang, EXgetVLang(temp));
	       }
	       
	       if (strncasecmp(view, viewnlang, strlen(viewnlang))==0) {
		    EXcpy(ext,temp);
		    return(TRUE);
	       }
	  }
     }
     return(found);
     ;
}


/*
 * Process gopherd.conf lines "viewext", "ignore", "blockext" and others.
 */


boolean 
EXAprocessLine(ExtArray *extarr, int exttype, char *inputline, char *deflang)
{
     int    i;
     char   ext[64];
     char   secondparm[64];
     char   prefix[64];
     char   Gplustype[64];
     char   Gpluslang[16];
     Extobj *temp;

     temp = EXnew();

     /*** Set the type of extension we've been given ***/
     EXsetExttype(temp, exttype);

     if (deflang != NULL)
	  EXsetVLang(temp, deflang);


     inputline = skip_whitespace(inputline);
     if (*inputline == '\0') return(FALSE);

     /*** The first parameter, the extension ***/
     i=0;
     while (!isspace(*inputline) && (*inputline != '\0'))
	  ext[i++] = *inputline++;

     ext[i] = '\0';
     EXsetExt(temp,ext);

     
     inputline = skip_whitespace(inputline);

     if (exttype == EXT_IGNORE || exttype == EXT_IGNOREPAT) {
	  EXAadd(extarr, temp);
	  EXdestroy(temp);
	  return(TRUE);
     }

     if (*inputline == '\0') return(FALSE);

     /*** Second parameter, depends on which token we're using.. ***/
     i=0;
     while (!isspace(*inputline) && (*inputline != '\0'))
	  secondparm[i++] = *inputline++;

     secondparm[i] = '\0';
     
     if (exttype == EXT_BLOCK || exttype == EXT_BLOCKREF) {
	  EXsetBlockname(temp, secondparm);
	  EXAadd(extarr, temp);
	  EXdestroy(temp);
	  return(TRUE);
     }
     else if (exttype == EXT_DECODER) {
	  EXsetDecoder(temp, secondparm);
	  EXAadd(extarr, temp);
	  EXdestroy(temp);
	  return(TRUE);
     }
     else if (exttype == EXT_VIEW)
	  EXsetObjtype(temp, *secondparm);
     
     if (*inputline == '\0') return(FALSE);

     inputline = skip_whitespace(inputline);
     if (*inputline == '\0') return(FALSE);

     /*** Prefix ***/
     i=0;
     while (!isspace(*inputline) && (*inputline != '\0'))
	  prefix[i++] = *inputline++;
     if (*inputline == '\0') return(FALSE);
     prefix[i]='\0';
     EXsetPrefix(temp, prefix);

     inputline = skip_whitespace(inputline);
     if (*inputline == '\0') return(FALSE);

     /*** Gopher + view type ***/
     i=0;
     while (!isspace(*inputline) && (*inputline != '\0'))
	  Gplustype[i++]= *inputline++;

     Gplustype[i] = '\0';
     EXsetView(temp, Gplustype);

     inputline = skip_whitespace(inputline);

     /** Gopher+ view language **/
     i=0;
     while (!isspace(*inputline) && (*inputline != '\0'))
	  Gpluslang[i++]= *inputline++;

     Gpluslang[i] = '\0';
     EXsetVLang(temp, Gpluslang);

     EXsetExttype(temp, EXT_VIEW);

     EXAadd(extarr, temp);
     EXdestroy(temp);
     return(TRUE);
}

