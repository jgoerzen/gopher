/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.3 $
 * $Date: 2002/01/28 03:38:58 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/util.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: util.c
 * Various useful utilities for gopher clients and servers
 *********************************************************************
 * Revision History:
 * $Log: util.c,v $
 * Revision 1.3  2002/01/28 03:38:58  jgoerzen
 * Patches for FreeBSD:
 *  * Test in configure for stdlib.h
 *  * Include stdlib.h if it's available in Malloc.h
 *  * Don't include malloc.h if it's missing in Malloc.h
 *  * Don't include strcasestr if it's present in util.c and util.h
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.25  1996/01/04  18:24:24  lindner
 * Updates for autoconf
 *
 * Revision 3.24  1995/09/25  22:07:25  lindner
 * Ansification
 *
 * Revision 3.23  1995/08/29  07:12:06  lindner
 * Turn off annoying debug stmt
 *
 * Revision 3.22  1995/05/01  05:41:46  lindner
 * compatibility fixes
 *
 * Revision 3.21  1995/04/15  07:07:39  lindner
 * Add ControlC handling feature...
 *
 * Revision 3.20  1995/02/02  17:14:47  lindner
 * Fix for memory leaks and accesses
 *
 * Revision 3.19  1994/10/19  03:34:39  lindner
 * Dynamically allocate acceptable array
 *
 * Revision 3.18  1994/08/19  16:12:11  lindner
 * Sanity check for Tohexstr
 *
 * Revision 3.17  1994/07/21  22:07:52  lindner
 * none
 *
 * Revision 3.16  1994/04/25  03:37:01  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.15  1994/03/31  21:04:36  lindner
 * Mitra's 2.011 debug patch
 *
 * Revision 3.14  1994/03/08  15:56:22  lindner
 * gcc -Wall fixes
 *
 * Revision 3.13  1994/02/20  16:25:58  lindner
 * Optimize readline and friends to use fileio routines
 *
 * Revision 3.12  1993/12/27  16:14:54  lindner
 * prettify debug output
 *
 * Revision 3.11  1993/09/21  01:52:54  lindner
 * Fixes for NETLIB
 *
 * Revision 3.10  1993/08/16  19:41:23  lindner
 * Fix for DECC/Alpha
 *
 * Revision 3.9  1993/08/09  20:17:13  lindner
 * Fixes for CMULIB and NETLIB for VMS
 *
 * Revision 3.8  1993/08/05  03:23:40  lindner
 * Changes for CMUIP and NETLIB
 *
 * Revision 3.7  1993/07/27  05:30:30  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.6  1993/07/23  04:49:24  lindner
 * none
 *
 * Revision 3.5  1993/06/22  05:49:39  lindner
 * *** empty log message ***
 *
 * Revision 3.4  1993/04/23  20:11:56  lindner
 * Fixed misdeclaration of DEBUG
 *
 * Revision 3.3  1993/04/15  17:53:12  lindner
 * Debug stuff from Mitra
 *
 * Revision 3.2  1993/03/18  22:28:27  lindner
 * changed hex routines around for admit1
 *
 * Revision 3.1.1.1  1993/02/11  18:03:05  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.5  1993/01/31  00:31:12  lindner
 * New functions, readword() and readtotoken()
 *
 * Revision 2.4  1993/01/12  21:12:23  lindner
 * Reverted to old readfield behavior()  \n is now ignored again.
 *
 * Revision 2.3  1993/01/08  23:29:21  lindner
 * More mods from jqj.
 *
 * Revision 2.2  1992/12/31  04:58:41  lindner
 * merged 1.1.1.1 and 2.1
 *
 * Revision 2.1  1992/12/21  19:41:14  lindner
 * Added check for null in writestring
 * Added function skip_whitespace
 *
 * Revision 1.1.1.1  1992/12/31  04:52:01  lindner
 * Changes for VMS
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

#include "boolean.h"
#include "util.h"
#include "String.h"
#include "Debug.h"
#include "fileio.h"

#if defined(VMS) && (defined(UCX) || defined(CMUIP) || defined(NETLIB))
#include <errno.h>
#endif
 

static FileIO *Gfio = NULL;
static int Oldsockfd = -1;

boolean ControlCpressed = 0;
jmp_buf Jmpenv;			/* Save environment for jump from ctrlc */

RETSIGTYPE
controlc(int sig)
{
     ControlCpressed = TRUE;
     reprimeControlc();
}

RETSIGTYPE
controlcJmp(int sig)
{
     	Debug("controlcJmp",NULL);
	controlc(sig);
	longjmp(Jmpenv,1);
}


int interruptable_read(int fd, char *ptr, int nbytes)
{
     int retval;

     if (ControlCpressed) { /* Already interrupted */
	  Debug("interruptable_read:already interrupted\r\n",NULL);
	  return(1);
     }
     
     if (signal(SIGINT, controlcJmp) == SIG_ERR)
	  perror("signal died:\n"), exit(-1);
     
     if (setjmp(Jmpenv)) {
	  /* Note controlcJmp will reprime controlc and set flag*/
	  Debug("interruptable_read triggered\r\n",NULL);
	  return(-1);
     }
     retval = read(fd, ptr, nbytes);
     reprimeControlc();
     return(retval);
}


/* Place this after the code to be interruptable*/
void
reprimeControlc()
{
     if (signal(SIGINT, controlc) == SIG_ERR)
	  perror("signal died:\n"), exit(-1);
}


#if defined(VMS) && (defined(WOLLONGONG) || defined(MULTINET) ||defined(CMUIP)||defined(NETLIB))
/* Multinet and Wollongong,etc. (non UCX-emulation) use channel numbers */
/* for sockets, which are small multiples of 16.  The first 5 */
/* channels can be assumed to be already used, so we assume that */
/* sockets start at 64, and that only 64 VAXC fds are simultaneously */
/* open in the program.  Actually, the first socket is likely to be */
/* more like 176! */

#define IS_SOCKET(s) ((s)>=64)
 
/* Close a socket.  
 * Note that in old Wollongong and Multinet implementations close()
 * works only on fds, not sockets.
 * For UCX and Unix, closenet() is #defined to be close()
 */
int closenet(int s)
{
#ifdef DEBUGGING
     if (s == stderr) {
	  fprintf(stderr, "YUK - closing stderr");
     }
#endif

    if (IS_SOCKET(s)) {
#ifdef MULTINET
	return (socket_close(s));
#else /* WOLLONGONG, CMUIP, NETLIB */
	return (netclose(s));
#endif
    }
    else
	close(s); /* shouldn't be calling this routine */
}
#else /* WOLLANGONG or MULTINET */
#define IS_SOCKET(a) 1
#endif

static void CheckGfio(int fd)
{
     
     if (Gfio == NULL || Oldsockfd != fd) {
	  Oldsockfd = fd;
	  if (Gfio != NULL)
	       FIOdestroy(Gfio);
	  Gfio = FIOopenfd(fd, IS_SOCKET(fd));
     }
}




/* Read "n" bytes from a descriptor.
 * Use in place of read() when fd is a stream socket
 *
 * Returns the number of total bytes read.
 */

int readn(int fd, char *ptr, int nbytes)
{
     int nleft, nread;
     
     nleft = nbytes;
     while (nleft > 0) {
#if defined(VMS) && (defined(WOLLONGONG) || defined(CMUIP) || defined(NETLIB))
	  nread = IS_SOCKET(fd) ? netread(fd, ptr, nleft) : read(fd, ptr, nleft);
#else
#if defined(VMS) && defined(MULTINET)
	  nread = IS_SOCKET(fd) ? socket_read(fd, ptr, nleft) : read(fd, ptr, nleft);
#else
	  nread = read(fd, ptr, nleft);
#endif
#endif
#if defined(VMS) && (defined(UCX) || defined(CMUIP) || defined(NETLIB))
          if (nread < 0 && errno == EPIPE)
	       break;
#endif
	  if (nread < 0)
	       return(nread);	/* error, return <0 */
	  else if (nread == 0)	/* EOF */
	       break;
	  
	  nleft 	-= nread;
	  ptr 	+= nread;
     }
     return(nbytes - nleft);	/* return >= 0) */
}



/*
 * Write "n" bytes to a descriptor.
 * Use in place of write() when fd is a stream socket
 *
 * We return the number of bytes written
 */

int 
writen(int fd, char *ptr, int nbytes)
{
     int nleft, nwritten;
     
     nleft = nbytes;
     while(nleft > 0) {
#if defined(VMS) && (defined(WOLLONGONG) || defined(CMUIP) || defined(NETLIB))
	  nwritten = IS_SOCKET(fd) ? netwrite(fd, ptr, nleft) : write(fd, ptr, nleft);
#else
#if defined(VMS) && defined(MULTINET)
	  nwritten = IS_SOCKET(fd) ? socket_write(fd, ptr, nleft) : write(fd, ptr, nleft);
#else
	  nwritten = write(fd, ptr, nleft);
#endif
#endif
	  if (nwritten <= 0)
	       return(nwritten);	/* error */
	  
	  nleft	-= nwritten;
	  ptr	+= nwritten;
     }
     return(nbytes - nleft);
}


/*
 * Writestring uses the writen and strlen calls to write a
 * string to the file descriptor fd.  If the write fails
 * a -1 is returned. Otherwise zero is returned.
 */

int writestring(int fd, char *stringptr)
{
     int length;

/*     Debug("writing: %s\n",stringptr);*/

     if (stringptr == NULL)
	  return(0);

     length = strlen(stringptr);
     if (writen(fd, stringptr, length) != length) {
	  Debugmsg("writestring: writen failed\n");
	  return(-1);
     }
     else
	  return(0);
}

/*
 * Read from the socket into a buffer.  Mucho more efficent in terms of
 * system calls..
 * 
 * returns bytes read, or <0 for an error
 */

int readrecvbuf(int sockfd, char *buf, int len)
{
     int bytesread;

     CheckGfio(sockfd);
     if (Gfio == NULL)
	  return(-1);

     bytesread = FIOreadbuf(Gfio, buf, len);
     
     return(bytesread);
}     


/*
 * Read a line from a descriptor.  Read the line one byte at a time,
 * looking for the newline.  We store the newline in the buffer,
 * then follow it with a null (the same as fgets(3)).
 * We return the number of characters up to, but not including,
 * the null (the same as strlen(3))
 */

int 
readline(int fd, char *ptr, int maxlen)
{
     int n;

     CheckGfio(fd);

     if (Gfio == NULL)
	  return(-1);
     
     n = FIOreadline(Gfio, ptr, maxlen);
     Debug("readline: %s\n", ptr);

     return(n);
}


/*
 * Read a line from the file/socket, Read the line one byte at a time,
 * looking for the token.  We nuke the token from the returned string.
 * We return the number of characters up to, but not including,
 * the null (the same as strlen(3))
 */

int 
readtoken(int fd, char *ptr, int maxlen, char zechar)
{
     int bytesread;

     CheckGfio(fd);
     if (Gfio == NULL)
	  return(-1);
     
     bytesread = FIOreadtoken(Gfio, ptr, maxlen, zechar);
     
     Debug("readtoken: %s\n", (ptr-bytesread));
     return(bytesread);
}


int
sreadword(char *input, char *output, int maxlen)
{
     int n;
     char c;
     
     for (n=0; n < maxlen; n++) {
	  c = *input++;
	  *output++ = c;
	  if (isspace(c)) {
	       *(output - 1) = '\0';
	       break;
	  }
	  
	  if (c == '\0') {
	       break;
	  }
     }
     
     *output = '\0'; 				/* Tack a NULL on the end */
     return(n);
}


/*
 * ZapCRLF removes all carriage returns and linefeeds from the end of
 * a C-string.
 */

void
ZapCRLF(char *inputline)
{
     int len;

     len = strlen(inputline);

     if (len == 0)
	  return;
     inputline += len;
     inputline--;
	  
     if (*inputline == '\n' || *inputline == '\r') {
	  *inputline = '\0';
	  if (len > 1) {
	       inputline--;
	       if (*inputline == '\n' || *inputline == '\r')
		    *inputline = '\0';
	  }
     }
}

/*
 *  Utilities for dealing with HTML junk
 */

static boolean *acceptable;
static boolean acceptable_inited = FALSE;

static void init_acceptable()
{
    unsigned int i;
    char * good = 
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./-_$";

    acceptable = (boolean*) malloc(sizeof(boolean) * 256);

    for(i=0; i<256; i++) 
	 acceptable[i] = FALSE;
    for(;*good; good++) 
	 acceptable[(unsigned int)*good] = TRUE;

    acceptable_inited = TRUE;
}

static char hex[17] = "0123456789abcdef";

static char 
from_hex(char c)
{
     return (c>='0')&&(c<='9') ? c-'0'
	  : (c>='A')&&(c<='F') ? c-'A'+10
	       : (c>='a')&&(c<='f') ? c-'a'+10
		    :                      0;
}


/*
 * return a hex encoding of the char,
 */

#if 0 /* unused */
static char *
to_hex(char c)
{
     static char out[4];

     out[0] = '\0';

     out[0]='%';
     out[1]=hex[c >> 4];
     out[2]=hex[c & 15];
     out[3]='\0';

     return(out);
}
#endif /* 0 */
     
/*
 * Replace hex escape sequences with the proper codes...
 *
 * input and output can be the same if you want
 */

void
Fromhexstr(char *input, char *output)
{
     char c;
     unsigned int b;

     while (*input) {
	  if (*input == '%') {
	       input++;
	       c = *input++;
	       b = from_hex(c);
	       c = *input++;
	       if (!c) break;
	       *output++ = (b<<4) + from_hex(c);
	  }
	  else 
	       *output++ = *input++;
     }
     *output = '\0';
}
	  
void
Tohexstr(char *input, char *output)
{
     if ( (input == NULL) || (output == NULL) )
	  return;

     if (acceptable_inited == FALSE)
	  init_acceptable();
     
     while (*input) {

	  if (acceptable[(int)*input] == TRUE) {
	       *output++ = *input++;
	  }
	  else {
	       *output++ = '%';
	       *output++ = hex[*input >> 4];
	       *output++ = hex[*input & 15];
	       input++;
	  }
     }

     *output = '\0';
}

/*
 * This fcn hexifies everything
 */

void
Hexall(char *input, char *output)
{
     while (*input) {

	  *output++ = hex[*input >> 4];
	  *output++ = hex[*input & 15];
	  input++;
     }

     *output = '\0';
}


/*
 * String insensitive strstr
 */

#ifndef HAVE_STRCASESTR

char *
strcasestr(char *inputline, char *match)
{
     int matchlen=0;
     int i, inlen;

     matchlen = strlen(match);
     inlen = strlen(inputline);

     for(i=0; i<inlen; i++) {
	  if (strncasecmp(inputline+i, match, matchlen)==0)
	       return(inputline+i);
     }

     return(NULL);
}

#endif

/*
 * Iterate over a string, return a pointer to the next character
 * that isn't whitespace.
 */

char *
skip_whitespace(char *str)
{
     while (isspace(*str) && *str!='\0')
	  str++;

     return(str);
}



#if defined(VMS) && defined(NETLIB)

/*
 * netclose, netread, and netwrite for NETLIB
 */

#include iodef
#include ssdef

static struct {
     short status;
     short size;
     long xxx;
} netlib_iosb;

void netlib_ast (param)
int param;
{
     SYS$SETEF (0);
}

int netclose (channel)
int channel;
{
     NET_DEASSIGN (&channel);

     return (0);
}

int netread (channel, buffer, length)
int channel;
char *buffer;
int length;
{
     struct {
          long len;
          char *adr;
     } buffer_desc;
     int status;

     if (length > 1500) length = 1500;
     buffer_desc.len = length;
     buffer_desc.adr = buffer;
     SYS$CLREF (0);
     status = TCP_RECEIVE (&channel, &buffer_desc, &netlib_iosb, netlib_ast, 0);
     if ((status & 1) == 0)
          return (-1);
     SYS$WAITFR (0);
     if ((netlib_iosb.status & 1) == 0) {
          if ((netlib_iosb.status == SS$_ABORT &&
	       netlib_iosb.xxx == 0x086380da) /* CMUIP Connection Closing */ ||
              netlib_iosb.status == SS$_LINKDISCON) /* UCX link disconnecting */
               errno = EPIPE;
          return (-1);
     }
     return (netlib_iosb.size);
}

int netwrite (channel, buffer, length)
int channel;
char *buffer;
int length;
{
     struct {
          long len;
          char *adr;
     } buffer_desc;
     int status;

     buffer_desc.len = length;
     buffer_desc.adr = buffer;
     SYS$CLREF (0);
     status = TCP_SEND (&channel, &buffer_desc, 6, &netlib_iosb, netlib_ast, 0);
     if ((status & 1) == 0)
          return (-1);
     SYS$WAITFR (0);
     if ((netlib_iosb.status & 1) == 0) {
          return (-1);
     }
     return (netlib_iosb.size);
}

#endif	/* NETLIB */

#if defined(VMS) && defined(CMUIP)

/*
 * netclose, netread, and netwrite for CMUIP
 */

#include iodef
#include ssdef

static struct {
     short status;
     short size;
     long xxx;
} cmu_iosb;
globalvalue NET$_CC; /* Connection Closing */

int netclose (channel)
  int channel;
{
     int status;

     status = SYS$QIOW (0, channel, IO$_DELETE, &cmu_iosb, 0, 0,
			0, 0, 0, 0, 0, 0);
     status = SYS$DASSGN (channel);

     return (0);
}

int netread (channel, buffer, length)
  int channel;
  char *buffer;
  int length;
{
     int status;

     if (length > 1500) length = 1500;
     status = SYS$QIOW (0, channel, IO$_READVBLK, &cmu_iosb, 0, 0,
			buffer, length, 0, 0, 0, 0);
     if ((status & 1) == 0)
          return (-1);
     if ((cmu_iosb.status & 1) == 0) {
          if (cmu_iosb.status == SS$_ABORT && cmu_iosb.xxx == NET$_CC)
               errno = EPIPE;
          return (-1);
     }
     return (cmu_iosb.size);
}

int netwrite (channel, buffer, length)
  int channel;
  char *buffer;
  int length;
{
     int status;

     status = SYS$QIOW (0, channel, IO$_WRITEVBLK, &cmu_iosb, 0, 0,
			buffer, length, 0, 1, 0, 0);
     if ((status & 1) == 0)
          return (-1);
     if ((cmu_iosb.status & 1) == 0)
          return (-1);
     return (cmu_iosb.size);
}

#endif	/* CMUIP */



