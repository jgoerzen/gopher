/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/fileio.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 92, 93, 94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: fileio.h
 * Abstration of socket/file input output routines.
 *********************************************************************
 * Revision History:
 * $Log: fileio.h,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.16  1996/01/04  18:25:00  lindner
 * Updates for autoconf
 *
 * Revision 3.15  1995/09/25  22:07:24  lindner
 * Ansification
 *
 * Revision 3.14  1995/04/15  06:51:18  lindner
 * Use interruptable_read
 *
 * Revision 3.13  1995/02/06  22:12:52  lindner
 * Smaller max for fileio structs
 *
 * Revision 3.12  1994/10/18  21:39:06  lindner
 * NeXT doesn't have mmap
 *
 * Revision 3.11  1994/09/29  19:54:24  lindner
 * Add definition for Memory mapped I/O
 *
 * Revision 3.10  1994/08/19  16:29:37  lindner
 * Add definition for memory mapped I/O
 *
 * Revision 3.9  1994/07/21  17:30:44  lindner
 * Mods for Xenix
 *
 * Revision 3.8  1994/04/25  03:40:07  lindner
 * VMS socket stuff still wasn't working..
 *
 * Revision 3.7  1994/04/22  06:42:02  lindner
 * better compatibility checking
 *
 * Revision 3.6  1994/04/13  04:28:23  lindner
 * add vms compatible.h
 *
 * Revision 3.5  1994/04/01  04:43:55  lindner
 * fixes for VMS includes
 *
 * Revision 3.4  1994/03/17  04:36:32  lindner
 * Fix for UCX
 *
 * Revision 3.3  1994/03/11  00:09:14  lindner
 * Fix for UCX
 *
 * Revision 3.2  1994/03/08  03:21:18  lindner
 * Mods for opening processes without sh
 *
 * Revision 3.1  1994/02/20  16:20:50  lindner
 * New object based versions of buffered io routines
 *
 *
 *********************************************************************/

#ifndef FILEIO_H
#define FILEIO_H

#include "STRstring.h"
#include "boolean.h"
#include "Stdlib.h"
#include "util.h"

#ifdef VMS
#  include <processes.h>
#  include <unixio.h>
#  include <file.h>
#else
#  ifdef M_XENIX
#    include <sys/stream.h>
#    include <sys/fcntl.h>
#    include <sys/ptem.h>
#  else
#    include <fcntl.h>
#  endif
#endif /* VMS */
#include "compatible.h"

struct fileio_struct 
{
     int     bufindex;
     int     bufdatasize;
     int     bytepos;
     char    *buf;

     boolean issocket;
     
     String  *filename;
     int     fd;
     pid_t   pid;
};

typedef struct fileio_struct FileIO;


#define FIOBUFSIZE   4096
#define FIOMAXOFILES 64

#define FIOisSocket(a)      ((a)->issocket)
#define FIOgetPid(a)        ((a)->pid)
#define FIOgetfd(a)         ((a)->fd)
#define FIOgetFilename(a)   (STRget((a)->filename))
#define FIOgetBufIndex(a)   ((a)->bufindex)
#define FIOgetBufDsize(a)   ((a)->bufdatasize)


#define FIOsetfd(a,b)       ((a)->fd=(b))
#define FIOsetPid(a,b)      ((a)->pid=(b))
#define FIOsetSocket(a,b)   ((a)->issocket=(b))
#define FIOsetFilename(a,b) (STRset((a)->filename,(b)))
#define FIOsetBufIndex(a,b) ((a)->bufindex = (b))
#define FIOsetBufDsize(a,b) ((a)->bufdatasize = (b))

#define FIOisMMAPable(a)    (FIOgetFilename(a) != NULL)

/*
 * Optionally use BSD memory mapped I/O
 */

#if defined(HAVE_MMAP)
#  define MMAP_IO
#endif

/*
 * More elegant way of doing socket writes than ifdefs everywhere
 */

#if defined(VMS)
    int DCLsystem(/* char */);
#   define system(a) DCLsystem(a)

    /**  Note that Multinet already has socket_*  **/
#   if defined(WOLLONGONG) || defined(CMUIP) || defined(NETLIB)
#       define socket_write netwrite
#       define socket_read  netread
#       define socket_close netclose
#   endif
#   if defined(UCX)
#       define socket_write write
#       define socket_read  read
#       define socket_close close
#   endif
    /**  Note that Wollongong is like Multinet for EPIPE handling  **/
#   if defined(UCX) || defined(CMUIP) || defined(NETLIB)
#       define FIO_NOMULTIEOF
#   endif

#else
    /* Unix systems */
#   define socket_write write
#   define socket_read  interruptable_read
#   define socket_close close
#endif


/** Forward Declarations **/

FileIO *FIOopenUFS(char *fname, int flags, int mode);
FileIO *FIOopenfd(int fd, int issocket);
FileIO *FIOopenProcess(char *prog, char **args, char   *rw);
FileIO *FIOopenCmdline(char *cmd, char *rw);
int     FIOclose(FileIO *fio);
int     FIOwriten(FileIO *fio, char *ptr, int nbytes);
int     FIOwritestring(FileIO *, char *);
int     FIOreadbuf(FileIO *fio, char *newbuf, int newbuflen);
int     FIOreadn(FileIO *fio, char   *ptr, int nbytes);
int     FIOreadline(FileIO *fio, char *ptr, int maxlen);
int     FIOreadlinezap(FileIO *fio, char *ptr, int maxlen);
int     FIOreadtoken(FileIO *fio, char *ptr, int maxlen, char zechar);
int     FIOwaitpid(FileIO *fio);
int     FIOsystem(char *cmd);
void    FIOdestroy(FileIO *fio);
#endif
