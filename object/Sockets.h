/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2002/03/19 20:42:48 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/Sockets.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: Sockets.h
 * Socket protos etc.
 *********************************************************************
 * Revision History:
 * $Log: Sockets.h,v $
 * Revision 1.2  2002/03/19 20:42:48  jgoerzen
 *   * Removed the DOMAIN configure support, DOMAIN_NAME in gopherd,
 *     backupdomain in Socket.c, etc.  This should prevent the no.domain.name
 *     problem that people have been seeing.  Regenerated configure stuff.
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.7  1995/09/25  22:07:21  lindner
 * Ansification
 *
 * Revision 3.6  1995/02/13  19:09:45  lindner
 * Add sys/param.h
 *
 * Revision 3.5  1994/04/08  20:05:54  lindner
 * gcc -Wall fixes
 *
 * Revision 3.4  1994/03/31  21:03:36  lindner
 * Use better, more descriptive return codes for socket routines
 *
 * Revision 3.3  1994/03/17  04:35:04  lindner
 * Move definitions from gopherd.h here
 *
 * Revision 3.2  1994/01/21  04:14:31  lindner
 * Add forward declaration for SOCKnetnames()
 *
 * Revision 3.1  1993/07/07  19:27:27  lindner
 * Socket functions
 *
 *
 *********************************************************************/


#ifndef SOCKETS_H
#define SOCKETS_H

/* Socket specific stuff, ugh! */
#ifdef VMS
#  if !defined(CMUIP) && !defined(NETLIB)
#    include <socket.h>
#    include <in.h>
#    include <file.h>
#    include <inet.h>
#    include <netdb.h>
#    include iodef
#    include ssdef
#  else
#    include iodef
#    include ssdef
#    include errno
#    include perror
#  endif        /* if !CMUIP && !NETLIB */

#else  /* VMS */
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <sys/file.h>
#  include <sys/param.h>
#  ifndef hpux
#    include <arpa/inet.h>
#  endif
#  include <netdb.h>
#endif  /* not VMS */


#ifndef MAXHOSTNAMELEN
#  define MAXHOSTNAMELEN                (64)
#endif




void SOCKlinger(int sockfd, boolean onoff);
int  SOCKbind_to_port(int port);
int  SOCKconnect(char *hostname, int port);
int  SOCKlisten(struct sockaddr_in * We);
int  SOCKaccept(int s, struct sockaddr_in we);
void SOCKnetnames(int  sockfd, char *host_name, char *ipnum);
char *SOCKgetDNSname(char *gdchost);
int  SOCKgetPort(int fd);

#define ErrSocketGetService 	-1
#define ErrSocketGetHost	-2
#define ErrSocketCall		-3
#define ErrSocketConnect	-4
#define ErrSocketNullHost	-5
#define ErrSocketInterrupted	-6

#endif  /* SOCKETS_H */
