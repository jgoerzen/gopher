/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2002/03/19 20:42:48 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/Sockets.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: Sockets.c
 * Socket functions
 *********************************************************************
 * Revision History:
 * $Log: Sockets.c,v $
 * Revision 1.2  2002/03/19 20:42:48  jgoerzen
 *   * Removed the DOMAIN configure support, DOMAIN_NAME in gopherd,
 *     backupdomain in Socket.c, etc.  This should prevent the no.domain.name
 *     problem that people have been seeing.  Regenerated configure stuff.
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.22  1995/09/25  22:07:20  lindner
 * Ansification
 *
 * Revision 3.21  1995/06/30  20:31:39  lindner
 * Fix for weirdness
 *
 * Revision 3.20  1995/04/15  07:07:39  lindner
 * Add ControlC handling feature...
 *
 * Revision 3.19  1994/07/31  05:09:47  lindner
 * Add option to log only IP
 *
 * Revision 3.18  1994/04/25  03:46:03  lindner
 * Fix for NETLIB and CMUIP
 *
 * Revision 3.17  1994/04/25  03:36:59  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.16  1994/04/08  20:05:53  lindner
 * gcc -Wall fixes
 *
 * Revision 3.15  1994/03/31  21:03:35  lindner
 * Use better, more descriptive return codes for socket routines
 *
 * Revision 3.14  1994/03/17  04:45:39  lindner
 * Add needed stdio.h
 *
 * Revision 3.13  1994/03/17  04:44:39  lindner
 * Add needed stdio.h
 *
 * Revision 3.12  1994/03/17  04:36:01  lindner
 * Move socket specific server code here, rearrange include files
 *
 * Revision 3.11  1994/03/08  15:56:20  lindner
 * gcc -Wall fixes
 *
 * Revision 3.10  1994/01/12  22:23:50  lindner
 * Fixes for Data General
 *
 * Revision 3.9  1993/12/27  16:16:03  lindner
 * Fix for sign on integer..
 *
 * Revision 3.8  1993/10/19  20:49:02  lindner
 * Fix for NETLIB
 *
 * Revision 3.7  1993/10/11  17:26:01  lindner
 * Fix for cmuip/netlib
 *
 * Revision 3.6  1993/09/21  01:51:34  lindner
 * Moved netnames fcn..
 *
 * Revision 3.5  1993/08/09  20:17:10  lindner
 * Fixes for CMULIB and NETLIB for VMS
 *
 * Revision 3.4  1993/08/05  03:23:37  lindner
 * Changes for CMUIP and NETLIB
 *
 * Revision 3.3  1993/07/29  20:01:02  lindner
 * Removed dead variables
 *
 * Revision 3.2  1993/07/27  05:30:27  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.1  1993/07/07  19:27:25  lindner
 * Socket functions
 *
 *
 *
 *********************************************************************/

/* Generic stuff */
#include <stdio.h>
#include "boolean.h"
#include "compatible.h"
#include "Debug.h"
#include "util.h"

#ifdef unix
#  include <sys/param.h>
#endif

#include "Sockets.h"
#include "Malloc.h"


/*
 * This turns the linger output off
 */

void
SOCKlinger(int sockfd, boolean onoff)
{
#if defined(SO_LINGER) && !defined(NO_LINGER)
    struct linger linger;
     
    linger.l_onoff = onoff;
    linger.l_linger = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (char *)&linger,
		   sizeof (linger)) < 0)
	 perror("server: can't turn off linger sockopt"),exit(-1);
#endif
}



/*
 * This function returns a socket file descriptor bound to the given port
 */

#if !defined(CMUIP) && !defined(NETLIB)
int
SOCKbind_to_port(int port)
{
    struct sockaddr_in serv_addr;
    int reuseaddr = 1;
    int sockfd;
    
    
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	 perror("server: can't open stream socket"), exit(-1);
    
    /** Bind our local address so that the client can send to us **/
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family 		= AF_INET;
    serv_addr.sin_addr.s_addr 	= htonl(INADDR_ANY);
    serv_addr.sin_port		= htons(port);
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseaddr,
		   sizeof(reuseaddr)) < 0)
	  perror("server: can't set REUSEADDR!"),exit(-1);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
	 return(-1);
/*	 perror("server: can't bind local address"),exit(-1);*/
    
    SOCKlinger(sockfd, FALSE);
    return(sockfd);
}



/*
 * This routine finds out the hostname of the server machine.
 * It uses a couple of methods to find the fully qualified 
 * Domain name
 *
 * If gdchost is non NULL then use that paramater instead of 
 * the code in here.
 */

char *
SOCKgetDNSname(char *gdchost)
{
     static char DNSname[MAXHOSTNAMELEN];
     struct hostent *hp;

     if (gdchost != NULL && *gdchost != '\0')
	  return(gdchost);

     DNSname[0] = '\0';
     /* Work out our fully-qualified name, for later use */
     
     if (gethostname(DNSname, MAXHOSTNAMELEN) != 0) {
	  fprintf(stderr, "Cannot determine the name of this host\n");
	  exit(-1);
     }

     /* Now, use gethostbyname to (hopefully) do a nameserver lookup */
     hp = gethostbyname( DNSname);

     /*
      ** If we got something, and the name is longer than hostname, then
      ** assume that it must the the fully-qualified hostname
      */
     if ( hp!=NULL && strlen(hp->h_name) > strlen(DNSname) ) 
	  strncpy( DNSname, hp->h_name, MAXHOSTNAMELEN );

     return(DNSname);
}


/*
 * Tries to figure out what the currently connected port is.
 * 
 * If it's a socket then it will return the port of the socket, 
 * if it isn't a socket then it returns -1.
 */

int 
SOCKgetPort(int fd)
{
     struct sockaddr_in serv_addr;

     socklen_t length = sizeof(serv_addr);
     
     /** Try to figure out the port we're running on. **/
     
     if (getsockname(fd, (struct sockaddr *) &serv_addr,&length) == 0)
	  return(ntohs(serv_addr.sin_port));
     else
	  return(-1);

}

#endif /* not CMUIP nor NETLIB */



/* SOCKconnect performs a connection to socket 'service' on host
 * 'host'.  Host can be a hostname or ip-address.  If 'host' is null, the
 * local host is assumed.   The parameter full_hostname will, on return,
 * contain the expanded hostname (if possible).  Note that full_hostname is a
 * pointer to a char *, and is allocated by connect_to_gopher()
 *
 * Errors: ErrSocket* defined in Sockets.h
 *
 */

int
SOCKconnect(char *hostname, int port)
{
#if !defined(CMUIP) && !defined(NETLIB)
     struct sockaddr_in Server;
     struct hostent *HostPtr;
     int sockfd = 0;
     unsigned int ERRinet = -1;

#ifdef _CRAY
     ERRinet = 0xFFFFFFFF;  /* -1 doesn't sign extend on 64 bit machines */
#endif

     /*** Find the hostname address ***/
     
     if (hostname == NULL || *hostname == '\0')
	  return(ErrSocketNullHost);

#ifdef DGUX
     Server.sin_addr = inet_addr(hostname);
     if (Server.sin_addr.s_addr == ERRinet)
#else
     if ((Server.sin_addr.s_addr = inet_addr(hostname)) == ERRinet)
#endif
     {
	  if ((HostPtr = gethostbyname(hostname)) != NULL) {
	       bzero((char *) &Server, sizeof(Server));
	       bcopy(HostPtr->h_addr, (char *) &Server.sin_addr, HostPtr->h_length);
	       Server.sin_family = HostPtr->h_addrtype;
	  } else
	       return (ErrSocketGetHost);
     } else
	  Server.sin_family = AF_INET;

     Server.sin_port = (unsigned short) htons(port);

     /*** Open the socket ***/

     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	  return (ErrSocketCall);

#ifndef UCX
     setsockopt(sockfd, SOL_SOCKET, ~SO_LINGER, 0, 0);
#endif

     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 0, 0);
     setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, 0, 0);

     /*** Connect ***/
     if (ControlCpressed) { /* Already interrupted */
	  return(ErrSocketInterrupted);
     }
     if (signal(SIGINT, controlcJmp) == SIG_ERR)
	  perror("signal died:\n"), exit(-1);

     if (setjmp(Jmpenv)) {
	  /* Note controlcJmp will reprime controlc and set flag*/
	  Debug("interruptable_connect triggered\r\n",NULL)
	       return(ErrSocketInterrupted);
     }

     if (connect(sockfd, (struct sockaddr *) &Server, sizeof(Server)) < 0) {
	  closenet(sockfd);
	  reprimeControlc();
	  return (ErrSocketConnect);
     }
     reprimeControlc();

     return(sockfd);

#else /* !NETLIB && !CMUIP */
#ifdef NETLIB
     int status;
     static int iSock = 0;
     struct {
          long len;
          char *adr;
     } host_desc;

#define NET_K_TCP 1
     status = NET_ASSIGN (&iSock);
     if ((status & 1) == 0)
          return (ErrSocketCall);
     status = NET_BIND (&iSock, NET_K_TCP);
     if ((status & 1) == 0)
          return (ErrSocketCall);
     host_desc.adr = hostname;
     host_desc.len = strlen (host_desc.adr);
     status = TCP_CONNECT (&iSock, &host_desc, port);
     if ((status & 1) == 0) {
          NET_DEASSIGN (&iSock);
          if (status == SS$_REJECT || status == SS$_TIMEOUT) {
	       if (status == SS$_REJECT)
		    errno = ECONNREFUSED;
	       else
		    errno = ETIMEDOUT;
	       
	       return (ErrSocketConnect);
	  }
	  if (status == SS$_ENDOFFILE)
	       return (ErrSocketGetHost);
          return (ErrSocketGetService);
     }
     return (iSock);
#else /* ifdef NETLIB:  assume CMUIP */
     short channel;
     int status;
     struct {
          short status;
          short size;
          long xxx;
     } cmu_iosb;
     static struct {long l; char *a;} ip_dev = {12, "INET$DEVICE:"};
     globalvalue NET$_CREF;           /* Connection refused */
     globalvalue NET$_FTO;            /* Function timedout */
     globalvalue NET$_DSNAMERR;       /* Domain server name error */
     status = SYS$ASSIGN (&ip_dev, &channel, 0, 0);
     if ((status & 1) == 0)
          return (ErrSocketCall);
     status = SYS$QIOW (0, channel, IO$_CREATE, &cmu_iosb, 0, 0,
          hostname, port, 0, 1, 0, 0);
     if ((status & 1) == 0 || (cmu_iosb.status & 1) == 0) {
          SYS$DASSGN (channel);
          if (cmu_iosb.status == SS$_ABORT || cmu_iosb.xxx == NET$_FTO) {
               if (cmu_iosb.xxx == NET$_CREF)
		    errno = ECONNREFUSED;
	       else
		    errno = ETIMEDOUT;

	       return(ErrSocketConnect);
	  }
	  if (cmu_iosb.xxx == NET$_DSNAMERR)
	       return (ErrSocketGetHost);
     }
     return (channel);
#endif
#endif

}




/*
 *
 */

#if !defined(CMUIP) && !defined(NETLIB)         /* temp - MLH */ 
int
SOCKlisten(struct sockaddr_in * We)
{
     int             sockfd = 0;
     struct hostent *HostPtr;
     socklen_t       len = sizeof(struct sockaddr);
     char            name[100];


     sockfd = SOCKbind_to_port(sockfd);
     if (listen(sockfd, 5) || getsockname(sockfd, (struct sockaddr *) We, &len)) {
          closenet(sockfd);
          return(ErrSocketGetHost);
     }
     
     gethostname(name, 100);
     if ((HostPtr = gethostbyname(name)))
          bcopy(HostPtr->h_addr, (char *) &We->sin_addr, HostPtr->h_length);
     return(sockfd);
}


/* SOCKaccept accepts a connection form some socket. *
 * Errors: ErrSocket* defined in Sockets.h
 */

int
SOCKaccept(int s, struct sockaddr_in we)
{
     int            sockfd    = 0;
     socklen_t      len       = sizeof(struct sockaddr);
     unsigned short tem;

     tem = ntohs(we.sin_port);

     Debugmsg("Here we go...\n");

     if ((sockfd = accept(s, (struct sockaddr *) &we, &len)) < 0) {
	  return ErrSocketConnect;
     }
     close(s); /* Der Mohr hat seine Schuldigkeit getan */

     return(sockfd);
}



/*
 * SOCKnetnames -- return the network, subnet, and host names of
 * our peer process for the Internet domain.
 *
 *      Parameters:     "sock" is our socket
 *                      "host_name"
 *                      is filled in by this routine with the
 *                      corresponding ASCII names of our peer.
 *       
 *                      if there doesn't exist a hostname in DNS etal,
 *                      the IP# will be inserted for the host_name
 *
 *                      "ipnum" is filled in with the ascii IP#
 */

void
SOCKnetnames(int  sockfd, char *host_name, char *ipnum)
{
     struct sockaddr_in      sa;
     socklen_t               length;
     struct hostent          *hp;

     length = sizeof(sa);
     if (getpeername(sockfd, (struct sockaddr *)&sa, &length))
	  /** May fail if sockfd has been closed **/
	  return;

     if (ipnum != NULL)
	  strcpy(ipnum, inet_ntoa(sa.sin_addr));

     if (host_name != NULL)
	  strcpy(host_name, inet_ntoa(sa.sin_addr));

#ifdef LOG_IP_ONLY
     hp = NULL;
#else
     hp = gethostbyaddr((char *) &sa.sin_addr,sizeof (sa.sin_addr), AF_INET);
#endif

     if (hp != NULL && host_name != NULL)
	  (void) strcpy(host_name, hp->h_name);

}

#endif  /** CMUIP etal **/
