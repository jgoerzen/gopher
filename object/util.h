/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/util.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: util.h
 * Header file for routines in util.c
 *********************************************************************
 * Revision History:
 * $Log: util.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.5  1995/09/25  22:07:26  lindner
 * Ansification
 *
 * Revision 3.4  1995/04/15  07:07:40  lindner
 * Add ControlC handling feature...
 *
 * Revision 3.3  1994/01/21  04:20:40  lindner
 * Better forward prototypes
 *
 * Revision 3.2  1993/03/18  22:28:46  lindner
 * Updated protos
 *
 * Revision 3.1.1.1  1993/02/11  18:03:06  lindner
 * Gopher+1.2beta release
 *
 * Revision 2.1  1992/12/21  19:43:15  lindner
 * Added prototype for skip_whitespace
 *
 * Revision 1.1  1992/12/10  23:27:52  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


/*
 * Definitions of stuff in util.c
 */
int    readn(int fd, char *ptr, int nbytes);
int    writen(int fd, char *ptr, int nbytes);
int    writestring(int, char*);
int    readrecvbuf(int sockfd, char *buf, int len);
int    readline(int fd, char *ptr, int maxlen);
int    readtoken(int fd, char *ptr, int maxlen, char zechar);
int    sreadword(char *input, char *output, int maxlen);
void   ZapCRLF(char * );
void   Fromhexstr(char * , char *);
void   Tohexstr(char *input, char *output);
void   Hexall(char *input, char *output);
char   *strcasestr(char *inputline, char *match);
char   *skip_whitespace(char *);

/* 
 * Interrupt stuff
 */
#include "boolean.h"
#include <setjmp.h>

extern boolean ControlCpressed;
extern jmp_buf Jmpenv;

void	controlc(int sig);
void 	controlcJmp(int sig);
int     interruptable_read(int fd, char *ptr, int nbytes);

int	primeControlc();
void	reprimeControlc();

