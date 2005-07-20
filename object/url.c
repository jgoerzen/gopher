/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.4 $
 * $Date: 2002/03/19 16:57:41 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/url.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: url.c
 * Simplified method of getting urls..
 *********************************************************************
 * Revision History:
 * $Log: url.c,v $
 * Revision 1.4  2002/03/19 16:57:41  jgoerzen
 * Fixed type handling.
 *
 * Revision 1.3  2001/01/17 21:16:35  jgoerzen
 * More psinrtf -> snprintf changes
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.19  1995/10/31  16:53:57  lindner
 * Small fix for ftp urls..
 *
 * Revision 3.18  1995/09/25  22:07:24  lindner
 * Ansification
 *
 * Revision 3.17  1995/08/29  07:13:07  lindner
 * Add tickets to urls
 *
 * Revision 3.16  1995/02/06  22:10:47  lindner
 * Fix for systems without strstr, compiler casts
 *
 * Revision 3.15  1994/12/05  22:47:19  lindner
 * Fixed settings of Gopher Type in URLparse()
 *
 * Revision 3.14  1994/11/18  22:45:55  lindner
 * Remove unused variable
 *
 * Revision 3.13  1994/11/18  21:41:48  lindner
 * Fix order of FTP arguments
 *
 * Revision 3.12  1994/11/13  06:30:06  lindner
 * Expanded URL parsing routines
 *
 * Revision 3.11  1994/10/13  05:25:56  lindner
 * Compiler complaint fixes
 *
 * Revision 3.10  1994/08/19  16:28:30  lindner
 * More hexification of URL strings
 *
 * Revision 3.9  1994/07/25  03:51:41  lindner
 * Compiler fix
 *
 * Revision 3.8  1994/07/21  22:17:20  lindner
 * Add ftp urls
 *
 * Revision 3.7  1994/03/04  17:42:57  lindner
 * Fixes from Alan Coopersmith
 *
 * Revision 3.6  1994/01/21  04:25:41  lindner
 * Add support for tn3270 and better gopher url handling
 *
 * Revision 3.5  1993/12/30  04:18:08  lindner
 * translate telnet url correctly
 *
 * Revision 3.4  1993/12/27  16:14:03  lindner
 * Enlarge buffer size, make html docs on gopher server into http: refs
 *
 * Revision 3.3  1993/11/02  06:14:09  lindner
 * Add url html hack
 *
 *
 *********************************************************************/

#include "url.h"
#include "GSgopherobj.h"
#include "Malloc.h"
#include "String.h"
#include "compatible.h"

Url *
URLnew()
{
     Url *temp;

     temp = (Url *) malloc(sizeof(Url));
     temp->url  = STRnew();
     temp->Host = STRnew();
     temp->Port = -1;
     temp->Path = STRnew();
     temp->User = STRnew();
     temp->Pass = STRnew();
     temp->Gtype = '\0';

     URLsetService(temp, unset);
     return(temp);
}


void
URLdestroy(Url *url)
{
     STRdestroy(url->url);
     STRdestroy(url->Host);
     STRdestroy(url->Path);
     STRdestroy(url->User);
     STRdestroy(url->Pass);

     free(url);
}



/*
 * Make a default url from the current Gopher Object...
 */

void
URLfromGS(Url *url, GopherObj *gs, char *ticket)
{
     char u[2048], *path, *cp;

     *u = '\0';
     path  = GSgetPath(gs);

     if (GSgetHost(gs) == NULL)
	  return;

     if ((GSgetType(gs) == A_TN3270) || (GSgetType(gs) == A_TELNET)) {
	  /* [telnet|tn3270]://[host]@[login]:[port] */
	  if (GSgetType(gs) == A_TELNET)
	       strcpy(u,"telnet://");
	  else
	       strcpy(u,"tn3270://");
	  Tohexstr(GSgetHost(gs), u + 9);
	  if ( (path != NULL) && (*path != '\0') ) {
	       cp = u + strlen(u);
	       *(cp++) = '@';
	       Tohexstr(path, cp);
	  }
	  snprintf(u + strlen(u), 
		   sizeof(u) - strlen(u) - 1,
		   ":%d", GSgetPort(gs));
     }
     else if (path != NULL)
     {
	  if (strncmp(path, "GET /", 5) == 0) {
	       /* http://[host]:[port][path without "GET "] */
	       strcpy(u, "http://");
	       Tohexstr(GSgetHost(gs), u+7);
	       snprintf(u + strlen(u), 
			sizeof(u) - strlen(u) - 1,
			":%d", GSgetPort(gs));
	       Tohexstr(path + 4, u + strlen(u));
	  } else if (strncmp(path, "ftp:", 4) == 0) {
	       /** ftp://[host]/[pathname]; **/
	       char *atsign;
	       if (path[4] == '/' && path [5] == '/') {
		    strcpy(u, path);
	       } else {
		    strcpy(u, "ftp://");
		    strcat(u, path + 4);
		    atsign = strchr(u, '@');
		    if (atsign != NULL) 
			 Tohexstr( path + (atsign-u)-1, atsign );
	       }
	  } else {
	       /* gopher://[host]:[port]/[type][path] */
	       strcpy(u, "gopher://");
	       Tohexstr(GSgetHost(gs), u + 9);
	       snprintf(u + strlen(u),
			sizeof(u) - strlen(u) - 1,
			":%d/%c", GSgetPort(gs), GSgetType(gs));
	       if (ticket != NULL && strlen(ticket) > 0)
		    Tohexstr(ticket, u+strlen(u));
	       Tohexstr(path, u + strlen(u));
	  }
     }
     URLset(url, u);
}

/*
 * Hack gopher directories into an HTML type...
 */

void
URLmakeHTML(Url *url)
{
     char *cp = URLget(url);

     if (cp == NULL) 
	  return;

     if (strncmp(cp, "gopher://", 9) != 0)
	  return;

     /** find the type character **/
     cp = strchr(cp+10, '/');
     
     if (cp ==NULL)
	  return;

     /** Test link for current host **/
/*   host = cp+10; 
     if (strcasecmp(host, hostname) != 0) 
	  return;*/
     
     cp ++;
     /** cp is now pointed at the type character **/
     
     if (*cp == '1' && *(cp+1) == '1') {
	  /** It's a directory **/
	  *cp = 'h';
	  *(cp+1) = 'h';
     }
}


/*
 * Get the transport of the specified URL
 */

UrlServiceType
URLgetService(Url *url)
{
     UrlServiceType st;

     if (url->service == unset) {
	  char *urlcp;

	  urlcp = URLget(url);

	  if (urlcp == NULL)
	       return(unknown);
	  
	  st = unknown;
	  
	  if (strncasecmp("ftp:", urlcp, 4)==0)
	       st = ftp;
	  else if (strncasecmp(urlcp, "gopher:", 7)==0)
	       st = gopher;
	  else if (strncasecmp(urlcp, "telnet:", 7)==0)
	       st = telnet;
	  else if (strncasecmp(urlcp, "tn3270:", 7)==0)
	       st = tn3270;
	  else if (strncasecmp(urlcp, "http:", 5)==0)
	       st = http;
	  
	  URLsetService(url, st);
     }
     return(url->service);
}
     
static boolean
URLparse(Url *url)
{
     char *cp, *cp2, *cp3, *urlcp;
     UrlServiceType serviceType;

     serviceType = URLgetService(url);

     if (serviceType == unknown)
	  return(FALSE);

     urlcp = URLget(url);

     if (urlcp == NULL)
	  return(FALSE);

     urlcp = strdup(urlcp);       /* This lets us scribble on the string */

     cp = strstr(urlcp, "://");   /* skip over servicetype + "://" */

     if (cp == NULL) 
	  return(FALSE);
     
     cp += 3;

     cp3 = strchr(cp, '/');  /* end of login:passwd@host:port/ section */

     if (cp3 == NULL) {
	  cp3 = cp + strlen(cp) + 1; /* No slash at the end... */
	  URLsetPath(url, "/");
     } else {
	  Fromhexstr(cp3, cp3);
	  URLsetPath(url,cp3);
	  *(cp3++) = '\0';      /* trunc the host part, remove sep slash, now
                                   cp3 points at the type. */
     }

     if (serviceType == gopher) {
	  if (cp3 != NULL && (int)strlen(cp3) > 1) {
	       URLsetGophType(url, *(cp3++));
	       URLsetPath(url,cp3++);
	  } else {
	       URLsetGophType(url, '1');
          }
     }


     /* 
      * Find Username/pass for ftp/telnet 
      */

     cp2 = strchr(cp, '@');
     if ((cp2 != NULL) && (cp2 < cp3)) {
	  char *pass;
	  

	  *cp2 = '\0';
	  cp2++;
	  
	  pass = strchr(cp, ':');
	  if ((pass != NULL) && (pass < cp3)) {
	       *pass = '\0';
	       pass++;
	       Fromhexstr(pass, pass);

	       URLsetPass(url, pass);

	  }
	  URLsetUser(url, cp);
	  
	  cp = cp2;               /** cp now points to hostname **/
     }
	  
     /*
      * Find port
      */

     cp2 = strchr(cp, ':');

     if ((cp2 != NULL) && (cp2 < cp3)) {
	  *cp2 = '\0';
	  cp2++;
	  URLsetPort(url, atoi(cp2));
	  URLsetHost(url, cp);
     } else {
	  switch (serviceType) {
	  case http:
	       URLsetPort(url, 80);
	       break;
	  case telnet:
	  case tn3270:
	       URLsetPort(url, 23);
	       break;
	  case gopher:
	       URLsetPort(url, 70);
	       break;
	  case news:
	       URLsetPort(url, 119);
               break;
          case ftp:
          case unset:
          case unknown:
               break;
	  }
	  URLsetHost(url, cp);
     }

	  
     return(TRUE);
}



char *
URLgetHost(Url *url)
{
     if (STRget(url->Host) == NULL)
	  URLparse(url);

     return(STRget(url->Host));
}

int
URLgetPort(Url *url)
{
     if (url->Port == -1) 
	  URLparse(url);
     
     return(url->Port);
}
     

char *
URLgetPath(Url *url)
{
     if (STRget(url->Path) == NULL)
	  URLparse(url);
     return(STRget(url->Path));
}

char *
URLgetUser(Url *url)
{
     if (STRget(url->User) == NULL)
	  URLparse(url);
     return(STRget(url->User));
}     

char *
URLgetPass(Url *url)
{
     if (STRget(url->Pass) == NULL)
	  URLparse(url);
     return(STRget(url->Pass));
}     


char
URLgetGophType(Url *url)
{
     if (URLgetService(url) != gopher)
	  return('\0');

     if (url->Gtype == '\0')
	  URLparse(url);
     return(url->Gtype);
}     

