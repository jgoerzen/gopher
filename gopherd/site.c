/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/site.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: site.c
 * Routines to build up a table of hostnames and access levels
 *********************************************************************
 * Revision History:
 * $Log: site.c,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.12  1995/09/25  05:02:39  lindner
 * Convert to ANSI C
 *
 * Revision 3.11  1995/02/17  23:15:30  lindner
 * Fix for access: lines
 *
 * Revision 3.10  1995/02/02  17:13:55  lindner
 * Fix memory leaks
 *
 * Revision 3.9  1994/07/22  22:56:20  lindner
 * More NO_AUTH stuff..
 *
 * Revision 3.8  1994/07/22  22:25:51  lindner
 * NO_AUTHENTICATION mods
 *
 * Revision 3.7  1994/07/19  20:24:12  lindner
 * Use local Locale.h
 *
 * Revision 3.6  1994/05/02  07:41:15  lindner
 * Mods to use setlocale()
 *
 * Revision 3.5  1994/03/17  21:18:24  lindner
 * Massive reworking of access limits
 *
 * Revision 3.4  1994/03/17  04:27:49  lindner
 * Additions for maxsessions
 *
 * Revision 3.3  1993/08/20  18:03:14  lindner
 * Mods to allow gopherd.conf files control ftp gateway access
 *
 * Revision 3.2  1993/07/27  05:27:57  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.1.1.1  1993/02/11  18:02:53  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.2  1993/02/09  22:29:23  lindner
 * added util.h to includes
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#ifndef NO_AUTHENTICATION

#include "site.h"
#include "Malloc.h"
#include "Locale.h"
#include <ctype.h>
#include "util.h"
#include "Debug.h"

static Site *
SiteNew()
{
     Site *temp;

     temp = (Site *) malloc(sizeof(Site));

     if (temp == NULL)
	  return(NULL);

     temp->domain = STRnew();
     STRinit(temp->domain);

     SITEsetLevel(temp, ACC_FULL);
     SITEsetisnum(temp, FALSE);
     SITEsetmaxsessions(temp, -1);

     return(temp);
}

static void 
SiteDestroy(Site *site)
{
     STRdestroy(site->domain);
     free(site);
}

static void
Sitecpy(Site *site1, Site *site2)
{
     STRcpy(site1->domain, site2->domain);
     
     site1->Level       = site2->Level;
     site1->isnum       = site2->isnum;
     site1->maxsessions = site2->maxsessions;
}

static void
SiteSet(Site *site, char *dom, Accesslevel access, int maxsess)
{
     STRset(site->domain, dom);
     site->Level = access;
     
     if (isdigit(*dom))
	  site->isnum = TRUE;
     else
	  site->isnum = FALSE;

     site->maxsessions = maxsess;

}

/******************************************************/

SiteArray *
SiteArrayNew()
{
     SiteArray *temp;
     
     temp = DAnew(20, SiteNew, NULL, SiteDestroy, Sitecpy);
     
     return(temp);
}

static void
SiteArrayAdd(SiteArray *sitearr, char *name, Accesslevel Level, int sessions)
{
     Site *temp;

     temp = SiteNew();

     SiteSet(temp, name, Level, sessions);

     SiteArrPush(sitearr, temp);

     SiteDestroy(temp);
     return;
}

/*
 * Find the default access level
 */

Accesslevel
SiteDefAccess(SiteArray *sitearr)
{
     int i;
     Site *temp;

     for (i=0; i< DAgetTop(sitearr); i++) {
	  temp = SiteArrgetEntry(sitearr, i);
	  
	  if (strcmp(SITEgetDomain(temp), "default") == 0)
	       return(SITEgetLevel(temp));
     }
     return(ACC_UNKNOWN);
}


/*
 * Tries to find a match in sitearr for "hostname" or ipnum, then tests the
 * accesslevel against "mask".
 *
 * If it finds it, it returns the result of the search
 *
 * If it doesn't it tests agains the "default" item
 * 
 */

AccessResult
SiteAccess(
  SiteArray   *sitearr,
  char        *hostname,
  char        *ipnum,
  Accesslevel  mask,
  int          sessions)
{
     int        i;
     Site       *temp=(Site *)NULL;
     Accesslevel defaccess = ACC_FULL, level;
     int         defmaxsess = -1;
     int         maxsess;

     if (hostname == NULL && ipnum == NULL)
	  /*** ??? We need to compare *something* ***/
	  return(SITE_UNDEF);  


     for (i=0; i< DAgetTop(sitearr); i++) {
	  temp = SiteArrgetEntry(sitearr, i);

	  Debug("Testing for %s\n", STRget(temp->domain));

	  if (strcmp(SITEgetDomain(temp), "default") == 0) {
	       defaccess = SITEgetLevel(temp);
	       defmaxsess = SITEmaxsessions(temp);
	  } else if (SITEisnum(temp) == TRUE) {
	       /*** Check for a match with the domain name
		 from the beginning ***/
	       int iplen, domainlen;

	       iplen = strlen(ipnum);
	       domainlen = strlen(STRget(temp->domain));

	       if (iplen >domainlen)
		    iplen = domainlen;

	       if (strncmp(ipnum, SITEgetDomain(temp), iplen) == 0) {
		    break;
	       }
		    
	  } else {
	       /*** Check for a match from the end ***/
	       /*** It's a domain name, not an ip number ***/
	       int namelen, domainlen;

	       namelen = strlen(hostname);
	       domainlen = strlen(SITEgetDomain(temp));


	       /*** don't compare if incoming name is shorter than domain ***/
	       if (domainlen <= namelen)
		    if (strcasecmp((hostname+namelen-domainlen), 
				   SITEgetDomain(temp))==0)
			 break;
	  }
     }

     if (i == DAgetTop(sitearr)) {
	  /*** Hmmm, didn't find a match, return "default" val ***/
	  level   = defaccess;
	  maxsess = defmaxsess;
     } else {
	  /** Otherwise we found a match.. **/
	  level   = SITEgetLevel(temp);
	  maxsess = SITEmaxsessions(temp);
     }
     
     if (sessions > 0) {
	  if (maxsess < sessions && maxsess > 0)
	       return(SITE_TOOBUSY);
     }
     
     if ((level & mask) == mask)
	  return(SITE_OK);
     else
	  return(SITE_NOACCESS);

}



/*
 * process a site description line.
 */

boolean
SiteProcessLine(SiteArray *sitearr, char *inputline, Accesslevel defaccess)
{
     char         name[256];
     char        *namep = name;
     Accesslevel  level = defaccess;
     int          maxsess = 0;
     
     /*** the first word is the domain/ip# ***/
     while (*inputline == ' ' || *inputline == '\t')
	  inputline++;

     while (*inputline != ' ' && *inputline != '\t') {
	  *namep = *inputline;
	  namep++;
	  inputline++;
     }
     
     *namep = '\0';  /*** Terminate it ***/

     if (namep == name)
	  return(FALSE); /*** bad line ***/
     
     while (*inputline != '\0') {

	  if (*inputline == ' ' || *inputline == '\t' || *inputline == ',') {
	       inputline++;
	       if (*inputline == '!')
		    inputline++;


	       switch (*inputline) {
	       case '0': case '1': case '2': case '3': case '4': case '5':
	       case '6': case '7': case '8': case '9':
		    /** Maxsessions **/
		    while (*inputline <= '9' && '0' <= *inputline) {
			 maxsess = maxsess * 10 + (*inputline - '0');
			 inputline++;
		    }
		    
		    break;

	       case 'b':
		    /** Browse **/
		    if (*(inputline -1) == '!')
			 level &= (ACC_SEARCH | ACC_READ | ACC_FTP);
		    else
			 level |= ACC_BROWSE;
		    break;

	       case 'r':
		    /*** Read ***/
		    if (*(inputline -1) == '!')
			 level &= (ACC_BROWSE | ACC_SEARCH | ACC_FTP);
		    else
			 level |= ACC_READ;
		    break;

	       case 's':
		    /*** Search ***/
		    if (*(inputline -1) == '!')
			 level &= (ACC_BROWSE | ACC_READ | ACC_FTP);
		    else
			 level |= ACC_SEARCH;
		    break;

	       case 'f':
		    /*** FTP ***/
		    if (*(inputline -1) == '!')
			 level &= (ACC_BROWSE | ACC_READ | ACC_SEARCH);
		    else
			 level |= ACC_FTP;
		    break;
	       }
	  } else
	       inputline++;
	  
     }

     SiteArrayAdd(sitearr, name, level, maxsess);
     return(TRUE);
}

#else

#endif
