/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/command.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992, 1993 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: command.h
 * Definitions and prototypes for CMD object.
 *********************************************************************
 * Revision History:
 * $Log: command.h,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.11  1995/09/25  05:02:31  lindner
 * Convert to ANSI C
 *
 * Revision 3.10  1995/08/29  07:10:21  lindner
 * Many changes to support HTTP
 *
 * Revision 3.9  1995/07/06  20:12:29  lindner
 * More mods for HTTP serving
 *
 * Revision 3.8  1994/07/22  22:28:22  lindner
 * Remove tix.h
 *
 * Revision 3.7  1994/06/29  05:27:44  lindner
 * Add define to remove ask items
 *
 * Revision 3.6  1993/10/04  06:49:21  lindner
 * ASK data stored in core, for a while..
 *
 * Revision 3.5  1993/09/30  16:56:58  lindner
 * Fix for WAIS and $ requests
 *
 * Revision 3.4  1993/07/23  03:11:22  lindner
 * Added CMDgetFile() fcn
 *
 * Revision 3.3  1993/04/09  16:50:26  lindner
 * nothing
 *
 * Revision 3.2  1993/03/24  20:18:50  lindner
 * Fixed bug in macro declaration
 *
 * Revision 3.1  1993/03/19  19:56:56  lindner
 * New CMD object
 *
 *
 *********************************************************************/


#ifndef COMMAND_H
#define COMMAND_H

#ifdef HAVE_CTYPE_H
#include "ctype.h"
#endif /* HAVE_CTYPE_H */

#include "STRstring.h"
#include "STAarray.h"
#include "boolean.h"
#include "serverutil.h"

typedef enum {UnknownProtocol, Gopher0, GopherPlus, HTTP0_9, HTTP1_0} CMDprotocol;
typedef enum {UnknownHTTPcmd, POST, GET} HTTPcmd;

struct command_struct {
     String      *datafromnet;

     boolean     isGplus;
     CMDprotocol proto;

     char    *selstr;
     char    *command;
     char    *search;
     
     String  *view;
     String  *secureuser;
     String  *ticket;
     HTTPcmd httpcmd;

     StrArray *asklines;
};

typedef struct command_struct CMDobj;


#define CMDgetSelstr(a)    ((a)->selstr)
#define CMDsetSelstr(a,b)  ((a)->selstr=(b))
#define CMDgetCommand(a)   ((a)->command)
#define CMDsetCommand(a,b) ((a)->command=(b))

#define CMDgetSearch(a)    ((a)->search)
#define CMDsetSearch(a,b)  ((a)->search=(b))

#define CMDgetProtocol(a)    ((a)->proto)
#define CMDsetProtocol(a,b)  ((a)->proto=(b))

#define CMDgetData(a)      (STRget((a)->datafromnet))
#define CMDsetData(a,b)    (STRset((a)->datafromnet, b))

#define CMDgetHTTPcmd(a)      ((a)->httpcmd)
#define CMDsetHTTPcmd(a,b)    ((a)->httpcmd=(b))

#define CMDremoveAsk(a)     (STAinit((a)->asklines))
#define CMDgetAskline(a,b)  (STAgetText((a)->asklines,b))
#define CMDisAskitem(a)     (STAgetTop((a)->asklines)!= 0 && (CMDgetHTTPcmd(cmd)==UnknownProtocol))
#define CMDnumAsklines(a)   (STAgetTop((a)->asklines))

#define CMDisGplus(a)      ((a)->isGplus)
#define CMDsetGplus(a,b)   ((a)->isGplus=b)

#define CMDgetView(a)      (STRget((a)->view))
#define CMDsetView(a,b)    (STRset((a)->view,b))

#define CMDgetUser(a)      (STRget((a)->secureuser))
#define CMDsetUser(a,b)    (STRset((a)->secureuser,(b)))

#define CMDgetTicket(a)    (STRget((a)->ticket))
#define CMDsetTicket(a,b)  (STRset((a)->ticket,(b)))

CMDobj *CMDnew();
void    CMDdestroy(CMDobj*);
void    CMDfromNet(CMDobj *cmd, int sockfd);
char   *CMDgetFile(CMDobj *);
#endif
