/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/fileio.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 92, 93, 94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: fileio.c
 * Socket/file input output routines.
 *********************************************************************
 * Revision History:
 * $Log: fileio.c,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.41  1996/01/04  18:25:12  lindner
 * Updates for autoconf
 *
 * Revision 3.40  1995/11/03  18:01:28  lindner
 * Fix for waitpid stuff
 *
 * Revision 3.39  1995/09/25  22:07:23  lindner
 * Ansification
 *
 * Revision 3.38  1995/06/12  15:32:41  lindner
 * Close correct file descriptor
 *
 * Revision 3.37  1995/06/08  05:17:54  lindner
 * From Fote: Deal with UCX's funny EPIPE return value on EOF
 * immediately after each socket_read().
 *
 * Ensure that file is closed on fstat() error in FIOopenUFS()
 *
 * Revision 3.36  1995/04/15  06:53:39  lindner
 * Different way of returning errors
 *
 * Revision 3.35  1995/03/09  05:28:19  lindner
 * Insure files get closed in FIO routines
 *
 * Revision 3.34  1995/02/27  17:45:52  lindner
 * Fix for sco
 *
 * Revision 3.33  1995/01/02  21:09:46  lindner
 * Fix for UCX menu ordering
 *
 * Revision 3.32  1994/12/20  17:09:46  lindner
 * Fix for binary file retrievals
 *
 * Revision 3.31  1994/12/03  02:14:13  lindner
 * Correct fix for waitpid() code
 *
 * Revision 3.30  1994/12/02  00:37:05  lindner
 * Fix for mail and misc process execution
 *
 * Revision 3.29  1994/12/01  00:30:13  lindner
 * Remove EINTR wait for FIOwaitpid.. sigh..
 *
 * Revision 3.28  1994/11/29  06:30:04  lindner
 * Make waitpid keep going after an interrupted system call
 *
 * Revision 3.27  1994/11/29  04:59:38  lindner
 * Fix for broken shell scripts with Shell Index type
 *
 * Revision 3.26  1994/10/21  02:42:47  lindner
 * fix for 0 length files
 *
 * Revision 3.25  1994/10/19  03:33:57  lindner
 * Fix nasty bug, combine more code...
 *
 * Revision 3.24  1994/10/13  05:26:16  lindner
 * Compiler complaint fixes
 *
 * Revision 3.23  1994/09/29  19:54:57  lindner
 * Routines to use memory mapped I/O
 *
 * Revision 3.22  1994/07/21  22:19:37  lindner
 * Add filename new and change debug msg
 *
 * Revision 3.21  1994/06/29  06:43:04  lindner
 * Remove VMS message about reversed values, initialize filename
 *
 * Revision 3.20  1994/05/25  20:57:20  lindner
 * Fix for piped play commands
 *
 * Revision 3.19  1994/04/25  03:44:42  lindner
 * Don't block empty FIOsystem() command on VMS.  That's what's used for
 * the '!' or '$' commands, and they've already be checked for permission
 * in Gopher.c.
 *
 * Reversed DCLsystem() return values in CURCurses.c, and eliminated the
 * reversing kludge in FIOsystem().
 *
 * Fixed break of open() for Alpha.  (all From F.Macrides)
 *
 * Revision 3.18  1994/04/25  03:37:00  lindner
 * Modifications for Debug() and mismatched NULL arguments, added Debugmsg
 *
 * Revision 3.17  1994/04/25  02:18:46  lindner
 * Fix for gcc-Wall
 *
 * Revision 3.16  1994/04/14  18:14:49  lindner
 * Fix for closing null files
 *
 * Revision 3.15  1994/04/14  15:44:55  lindner
 * Remove duplicate variable
 *
 * Revision 3.14  1994/04/08  21:07:34  lindner
 * Put back a variable
 *
 * Revision 3.13  1994/04/08  20:05:57  lindner
 * gcc -Wall fixes
 *
 * Revision 3.12  1994/04/08  19:15:47  lindner
 * Fix for old union wait stuff
 *
 * Revision 3.11  1994/04/07  22:51:24  lindner
 * Fix for typecast
 *
 * Revision 3.10  1994/04/01  04:43:31  lindner
 * Fixes for memory leak in argv, and exit status
 *
 * Revision 3.9  1994/03/30  21:37:48  lindner
 * Fix for AIX that doesn't #define unix..
 *
 * Revision 3.8  1994/03/11  00:10:52  lindner
 * Fix for FIOopenUFS()
 *
 * Revision 3.7  1994/03/09  16:57:56  lindner
 * Fix for other vms system() call
 *
 * Revision 3.6  1994/03/09  02:11:49  lindner
 * Use DCLsystem for VMS
 *
 * Revision 3.5  1994/03/08  17:23:17  lindner
 * Fix for return vals
 *
 * Revision 3.4  1994/03/08  06:17:15  lindner
 * Nuke compiler warnings
 *
 * Revision 3.3  1994/03/08  03:22:39  lindner
 * Additions for process management
 *
 * Revision 3.2  1994/03/04  17:43:36  lindner
 * Fix for weird error
 *
 * Revision 3.1  1994/02/20  16:20:48  lindner
 * New object based versions of buffered io routines
 *
 *
 *********************************************************************/

#include "fileio.h"
#include "Malloc.h"

#include <errno.h>
#include "Debug.h"
#include "Wait.h"
#include "String.h"
#include "Stdlib.h"
#include "compatible.h"

#ifdef VMS
#  include <stat.h>
#else
#  include <sys/stat.h>
#endif

#ifdef MMAP_IO
#  include <sys/mman.h>
#  ifndef MAP_FILE
#    define MAP_FILE 0
#  endif
#endif

static char **  FIOgetargv(char *cmd);


/*
 * Cache of used fios 
 */

static FileIO* FIOusedfios[FIOMAXOFILES];
static int  FIOnumusedfios = -1;

/*
 * Pop a recently used item.
 */

static FileIO *FIOpopUsedfio()
{
     if (FIOnumusedfios > 0) {
	  return(FIOusedfios[--FIOnumusedfios]);
     } else
	  return(NULL);
}

/*
 * Push an item on our recently used stack.
 */
 
static boolean FIOpushUsedfio(FileIO *oldfio)
{
     if (FIOnumusedfios < FIOMAXOFILES) {
	  FIOusedfios[FIOnumusedfios++] = oldfio;
	  return(0);
     } else
	  return(-1);
}

static FileIO *
FIOnew()
{
     FileIO *temp;

     if (FIOnumusedfios == -1) {
	  int i;
	  /* Initialize the usedfios struct the first time through */
	  for (i=0; i < FIOMAXOFILES; i++) {
	       FIOusedfios[i] = NULL;
	  }
	  FIOnumusedfios=0;
     }
     
     temp = FIOpopUsedfio();
     if (temp == NULL) {
	  temp = (FileIO *) malloc(sizeof(FileIO));
	  temp->filename = STRnew();
     } else {
	  STRinit(temp->filename);
     }

     
     temp->issocket = -1;
     temp->pid = -1;
     temp->fd = (pid_t) -1;
     temp->buf = NULL;

     temp->bufindex = -1;
     temp->bufdatasize = -1;
     temp->filename = STRnew();


     return(temp);
}


/*
 * Should only be called by FIOclose* functions really..
 */

void
FIOdestroy(FileIO *fio)
{
     if (FIOpushUsedfio(fio)) {
	  /** No space in cache. **/
	  free(fio);
     }
}

/*
 * Open a file, initialize data structures.
 */

FileIO *
FIOopenUFS(char *fname, int flags, int mode)
{
     int    fd;
     FileIO *fio;
     struct stat buf;

     if (fname == NULL)
	  return(NULL);

     /* Okay, try and open up the file */
     fd = open(fname, flags, mode );
     
     if (fd < 0)
	  return(NULL); /* Couldn't open file */

     if (fstat(fd, &buf)) { 
          close(fd);
	  return(NULL); /* Couldn't open anything */
     }

     fio = FIOnew();
     
     FIOsetSocket(fio, FALSE);
     FIOsetFilename(fio, fname);
     FIOsetfd(fio,fd);
     
     return(fio);
}


/*
 * Start FIO routines on an already open file descriptor
 */

FileIO*
FIOopenfd(int fd, boolean issocket)
{
     FileIO *fio;

     fio = FIOnew();

     FIOsetfd(fio, fd);
     FIOsetSocket(fio, issocket);
     return(fio);
}

FileIO*
FIOopenProcess(char *prog, char **args, char   *rw)
{
     int    pfd[2];
     int    pid;
     FileIO *fio;

     if (prog == NULL) 
	  return(NULL);

     fio = FIOnew();

     dup(0);  /*** Arghh!!  pipe doesn't work right when all fds are closed! */

     if (pipe(pfd) < 0)
	  return(NULL);


     switch (pid = vfork()) {
     case -1:			/* Error */
	  (void) close(pfd[0]);
	  (void) close(pfd[1]);
	  break;
     case 0:			/* Child */
	  if (rw == NULL || *rw == '\0') {
	       /** mimic system(), don't do anything **/
	       (void) close(pfd[0]);
	       (void) close(pfd[1]);
	  }
	  else if (*rw == 'r') {
	       if (pfd[1] != 1) {
		    dup2(pfd[1], 1);
		    (void) close(pfd[1]);
	       }
		    
	       (void) close(pfd[0]);
	  } else {
	       if (pfd[0] != 0) {
		    dup2(pfd[0], 0);
		    (void) close(pfd[0]);
	       }
	       (void) close(pfd[1]);
	  }
#ifdef VMS
	  execv(prog, args);
#else
	  /** Unix **/

	  if (*prog == '/')
	       execv(prog, args);
	  else
	       execvp(prog, args); /* search the path for the command */
#endif

	  _exit(1);
     }

     /* parent.. */
     if (rw == NULL || *rw == '\0') {
	  /** Don't do anything, mimic system() **/
	  FIOsetfd(fio, -1);
	  (void) close(pfd[0]);
	  (void) close(pfd[1]);
     } else if (*rw == 'r') {
	  FIOsetfd(fio, pfd[0]);
	  (void) close(pfd[1]);
     } else {
	  FIOsetfd(fio, pfd[1]);
	  (void) close(pfd[0]);
     }
     FIOsetPid(fio, pid);
     FIOsetSocket(fio, FALSE);
     return(fio);
}


/*
 * Close a file/socket/process
 */

int
FIOclose(FileIO *fio)
{
     int result;

     if (fio == NULL)
	  return(0);

     if (FIOgetPid(fio) >= 0) {
	  
	  close(FIOgetfd(fio));
	  result = FIOwaitpid(fio);
	  FIOdestroy(fio);
	  
	  return(result);
     }


#ifdef MMAP_IO
     /** Unmap memory mapped I/O stuff here.. **/
     if (FIOgetFilename(fio) != NULL && fio->buf != NULL)
	  munmap(fio->buf, FIOgetBufDsize(fio));

#endif

     result = FIOisSocket(fio) ? socket_close(FIOgetfd(fio)) :
	  close(FIOgetfd(fio));

     FIOdestroy(fio);

     return(result);
}

/*
 * A portable waitpid fcn that returns the exit value of the child process
 *
 * Should be better about stopped and signaled processes....
 */

int
FIOwaitpid(FileIO *fio)
{
     Portawait   status;
     pid_t       result;


     do {
	  errno = 0;
	  result = waitpid(FIOgetPid(fio), &status, 0);
     } while (result != FIOgetPid(fio) && (errno == ECHILD));

     return(Gwaitstatus(status) & 0xf);
}

/*
 * write n bytes to an fd.. 
 *
 * returns -1 on error.
 */

int
FIOwriten(FileIO *fio, char *ptr, int nbytes)
{
     int nleft, nwritten;
     int fd = FIOgetfd(fio);

     nleft = nbytes;
     while(nleft > 0) {
	  nwritten = FIOisSocket(fio) ? socket_write(fd, ptr, nleft) :
	       write(fd, ptr, nleft);

	  if (nwritten <= 0)
	       return(nwritten);	/* error */
	  
	  nleft	-= nwritten;
	  ptr	+= nwritten;
     }
     return(nbytes - nleft);
}

/*
 * write a string to a FileDescriptor, eventually buffer outgoing input
 *
 * If write fails a -1 is returned. Otherwise zero is returned.
 */
   
int
FIOwritestring(FileIO *fio, char *str)
{
     int length;

     Debug("writing: %s\n",str);

     if (str == NULL)
	  return(0);

     length = strlen(str);
     if (FIOwriten(fio, str, length) != length) {
	  Debugmsg("writestring: writen failed\n");
	  return(-1);
     }
     else
	  return(0);
}

#ifdef MMAP_IO
static int
FIOreadbuf_mmap(FileIO *fio, char *newbuf, int newbuflen)
{
     struct stat buf;
     char        *bytes;

     if (FIOgetFilename(fio) != NULL) {
	  /** Do MMAP IO if we can... **/

	  if (fio->buf == NULL) {
	       if (fstat(FIOgetfd(fio), &buf)) {
		    close(FIOgetfd(fio));
		    return(-1);
	       }
	  
	       if (buf.st_size == 0)
		    return(0);

	       FIOsetBufDsize(fio, buf.st_size);
	       FIOsetBufIndex(fio, 0);

	       bytes = mmap(0, buf.st_size, PROT_READ,
			    MAP_SHARED | MAP_FILE,
			    FIOgetfd(fio), 0);
#ifdef HAVE_MADVISE
	       madvise(bytes, buf.st_size, MADV_SEQUENTIAL);
#endif

	       if (bytes == (caddr_t) -1)
		    return(-1);
	       fio->buf = bytes;
	  }

	  if (FIOgetBufIndex(fio) == -1)
	       return(0);  

	  /** Okay, lets return some data... **/
	  if (FIOgetBufIndex(fio) >= FIOgetBufDsize(fio))
	       return(0);

	  if ((FIOgetBufIndex(fio) + newbuflen) > FIOgetBufDsize(fio)) {
	       newbuflen = FIOgetBufDsize(fio) - FIOgetBufIndex(fio);
	  }
	  memcpy(newbuf, fio->buf+FIOgetBufIndex(fio), newbuflen);
	  FIOsetBufIndex(fio, FIOgetBufIndex(fio) + newbuflen);

	  return(newbuflen);
     }
     /** This is an error condition **/
     return(-1);
     
}
#endif

/*
 * Read through a buffer, more efficient for character at a time
 * processing.  Not so good for block binary transfers
 */

int
FIOreadbuf(FileIO *fio, char *newbuf, int newbuflen)
{
     int len;
     int fd = FIOgetfd(fio);
     char *recvbuf;
     int  bytesread = 0;

#ifdef MMAP_IO
     if (FIOisMMAPable(fio)) {
	  return(FIOreadbuf_mmap(fio,newbuf, newbuflen));
     }
#endif

     if (FIOgetBufIndex(fio) == -1) {

	  if (fio->buf == NULL)
	       fio->buf = (char *) malloc(sizeof(char) * FIOBUFSIZE);
	  
	  len = FIOisSocket(fio) ? socket_read(fd, fio->buf, FIOBUFSIZE) :
	       read(fd, fio->buf, FIOBUFSIZE);
	  
#if defined(FIO_NOMULTIEOF)
	  if (len < 0 && errno == EPIPE) { /** EOF **/
	       FIOsetBufIndex(fio,-1);
	       return(0);
	  }
#endif

	  FIOsetBufDsize(fio, len);
	  FIOsetBufIndex(fio, 0);

	  if (len == 0) {
	       FIOsetBufIndex(fio,-1);
	       return(bytesread); /** EOF **/
	  }

     }
     recvbuf = fio->buf;

     while (newbuflen--) {
	  *newbuf++ = recvbuf[FIOgetBufIndex(fio)++];
	  bytesread++;
	  
	  if (FIOgetBufIndex(fio) == FIOgetBufDsize(fio) && newbuflen != 0) {
	       /** Read another buffer **/
	       len = FIOisSocket(fio) ? socket_read(fd, fio->buf, FIOBUFSIZE) :
		    read(fd, fio->buf, FIOBUFSIZE);
#if defined(FIO_NOMULTIEOF)
	       if (len < 0 && errno == EPIPE) { /** EOF **/
		    len = 0;
		    errno = 0;
	       }
#endif

	       if (len == 0) {
		    FIOsetBufIndex(fio,-1);
		    return(bytesread); /** EOF **/
	       }
	       if (len < 0)
		    return(len);       /** Error **/

	       FIOsetBufDsize(fio, len);
	       FIOsetBufIndex(fio, 0);
	  } else if (FIOgetBufIndex(fio) >= FIOgetBufDsize(fio))
	       /* Read a new buffer next time through */
	       FIOsetBufIndex(fio, -1);
     }
     return(bytesread);
     
}


/* Read 'n' bytes from a descriptor, non buffered direct into the storage. */
int
FIOreadn(FileIO *fio, char   *ptr, int    nbytes)
{
     int nleft, nread;
     int fd = FIOgetfd(fio);
     
     nleft = nbytes;
     while (nleft > 0) {
	  nread = FIOisSocket(fio) ? socket_read(fd, ptr, nleft) :
	       read(fd, ptr, nleft);
#if defined(FIO_NOMULTIEOF)
          if (nread < 0 && errno == EPIPE) { /** EOF **/
	       nread = 0;
	       errno = 0;
	  }
#endif

	  if (nread < 0)
	       return(nread);  /* error, return < 0 */
	  else if (nread == 0) /* EOF */
	       break;

	  nleft -= nread;
	  ptr   += nread;
     }
     return(nbytes - nleft);

}


/*
 * Read a line from the file/socket, Read the line one byte at a time,
 * looking for the newline.  We store the newline in the buffer,
 * then follow it with a null (the same as fgets(3)).
 * We return the number of characters up to, but not including,
 * the null (the same as strlen(3))
 */

int
FIOreadline(FileIO *fio, char *ptr, int maxlen)
{
     int bytesread;
     int rc;
     char c;

     for (bytesread=1; bytesread < maxlen; bytesread++) {
	  if ( (rc = FIOreadbuf(fio, &c, 1)) == 1) {
	       *ptr++ = c;
	       if (c == '\n')
		    break;
	  }
	  else if (rc == 0) {
	       if (bytesread == 1)
		    return(0);	/* EOF, no data read */
	       else
		    break;		/* EOF, some data was read */
	  }
	  else
	       return(rc);		/* error */
     }
     
     *ptr = 0; 				/* Tack a NULL on the end */
     Debug("FIOreadline: %s\n", (ptr-bytesread));

     return(bytesread);
}


/*
 * This does the same as readline, except that all newlines and 
 * carriage returns are automatically zapped.
 *
 * More efficient than doing a readline and a ZapCRLF
 */

int 
FIOreadlinezap(FileIO *fio, char *ptr, int maxlen)
{
     int len;

     len = FIOreadtoken(fio, ptr, maxlen, '\n');
     ptr += len;
     ptr --;
     
     if (len && *ptr == '\r') {
	  ptr[len] = '\0';
	  len--;
     }
     return(len);
}  


/*
 * Read a line from the file/socket, Read the line one byte at a time,
 * looking for the token.  We nuke the token from the returned string.
 * We return the number of characters up to, but not including,
 * the null (the same as strlen(3))
 */

int 
FIOreadtoken(FileIO *fio, char *ptr, int maxlen, char zechar)
{
     int bytesread;
     int rc;
     char c;

     for (bytesread=1; bytesread < maxlen; bytesread++) {
	  rc = FIOreadbuf(fio, &c, 1);
	  if (rc == 1) {
	       *ptr++ = c;
	       if (c == zechar) {
		    *(ptr - 1) = '\0';
		    break;
	       }
	  }
	  else if (rc == 0) {
	       if (bytesread == 1)
		    return(0);	/* EOF, no data read */
	       else
		    break;		/* EOF, some data was read */
	  }
	  else
	       return(rc);		/* error */
     }
     
     *ptr = 0; 				/* Tack a NULL on the end */
     Debug("readtoken: %s\n", (ptr-bytesread));
     return(bytesread);
}

static int
FIOexecv(char *prog, char **args)
{
     FileIO *fio;
     int    result;

#ifdef VMS
     int     i = 0;
     char    buffer[1024];

     /* DCL hog heaven */
     strcpy(buffer, prog);
     strcat(buffer, " ");

     while (i++) {
	  if (args[i] == NULL)
	       break;

	  strcat(buffer, args[i]);
	  strcat(buffer, " ");
     }
     result = DCLsystem(buffer);

#else
     fio = FIOopenProcess(prog, args, NULL);
     result = FIOclose(fio);
#endif /* VMS */

     return(result);
}


/*
 * Do the minimal shell/dcl processing
 */

static char **
FIOgetargv(char *cmd)
{
     int           inquote = 0;
     int           insquote = 0;
     int           i;
     static char  *argv[256];		/* Sufficient for now.. */
     int           argc = 0;
     char          buf[256];
     char         *cp = buf;

     if (cmd == NULL || strlen(cmd) >= 245)
	  return(NULL);

     for (i=0; cmd[i] != '\0'; i++) {

	  switch (cmd[i]) {

	  case ' ': case '\t':
	       /* Separators */
	       if (insquote || inquote) {
		    *cp = cmd[i]; cp++;
		    break;
	       } else {
		    *cp = '\0';
		    argv[argc++] = strdup(buf);
		    cp = buf;

		    /** Get rid of any other whitespace **/
		    while (cmd[i+1] == ' ' || cmd[i+1] == '\t')
			 i++;
	       }
	       break;

	  case '"':
	       if (!insquote)
		    inquote = 1-inquote;
	       break;
	       
	  case '\'':
	       if (!inquote)
		    insquote = 1-insquote;
	       break;

	  case '\\':
	       /* Quote next character if not in quotes */
	       if (insquote || inquote) {
		    *cp = cmd[i]; cp++;
	       } else {
		    *cp = cmd[i+1]; cp++; i++;
	       }

	       break;

	  default:
	       *cp = cmd[i]; cp++;
	       break;
	  }
     }
     if (buf != cp) {
	  *cp = '\0';
	  argv[argc++] = strdup(buf);
     }
     argv[argc] = NULL;
     
     return(argv);
}


/*  
 *  An emulation of the system() call without the shell
 *  returns the exit status of the child
 */

int
FIOsystem(char *cmd)
{
     char **argv;
     int    result, i;

#ifdef VMS
     return(DCLsystem(cmd));
#else

     if (cmd == NULL || *cmd == '\0')
	  return(-1);

     argv = FIOgetargv(cmd);

     result = FIOexecv(argv[0], argv);
     
     for (i=0; argv[i] != NULL; i++)
	  free(argv[i]);

     return(result);
#endif
}



/*
 * Similar to popen...
 */

FileIO
*FIOopenCmdline(char *cmd, char *rw)
{
     char   **argv;
     FileIO *fio;

     if (cmd == NULL)
	  return(NULL);

     if (*cmd == '|')
	  cmd++;

     argv = FIOgetargv(cmd);

     fio = FIOopenProcess(argv[0], argv, rw);
     
     return(fio);
}

