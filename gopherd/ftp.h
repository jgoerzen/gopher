/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.3 $
 * $Date: 2000/12/21 05:35:16 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/ftp.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: ftp.h
 * Declartions for Routines to translate gopher protocol to ftp protocol.
 *********************************************************************
 * Revision History:
 * $Log: ftp.h,v $
 * Revision 1.3  2000/12/21 05:35:16  s2mdalle
 * Function prototype argument type change to ParseUnixList from char
 * **ftppass to char *ftppass
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.9  1995/09/25  05:02:34  lindner
 * Convert to ANSI C
 *
 * Revision 3.8  1994/07/31  04:56:51  lindner
 * Mondo new stuff for ftp gateway
 *
 * Revision 3.7  1994/07/21  15:45:07  lindner
 * Gopher+ FTP gateway (modified from patch from Brian Coan)
 *
 * Revision 3.6  1994/06/29  05:23:14  lindner
 * Use an enum
 *
 * Revision 3.5  1994/03/08  15:55:34  lindner
 * gcc -Wall fixes
 *
 * Revision 3.4  1994/01/20  06:41:18  lindner
 * FTPtype macros changed to give FTPcommand the (char *) it was expecting
 * instead of the (char) is was getting.
 *
 * Revision 3.3  1993/12/16  11:34:42  lindner
 * Fixes to work with VM ftp servers
 *
 * Revision 3.2  1993/09/21  04:16:15  lindner
 * Fix for macro declarations
 *
 *
 *********************************************************************/

#ifndef GOPHER_FTP_H
#define GOPHER_FTP_H

#include "boolean.h"
#include "command.h"

typedef enum {FTP_UNKNOWN, FTP_VMS, FTP_NOVELL, FTP_UNIX, FTP_MTS, FTP_WINNT,
              FTP_MACOS, FTP_VM, FTP_UNIX_L8, FTP_MICRO_VAX, FTP_OS2
	 } FTPsystype;
	   

struct FTP_struct {
     int        control_sock;
     int        data_sock;
     char       mode;
     FTPsystype Ftptype;
};

typedef struct FTP_struct FTP;

/** Control socket access routines **/
#define FTPsetControl(ftp,num) ((ftp)->control_sock=(num))
#define FTPgetControl(ftp)     ((ftp)->control_sock)

/** Data socket access routines **/
#define FTPsetData(ftp,num)    ((ftp)->data_sock=(num))
#define FTPgetData(ftp)        ((ftp)->data_sock)


/** Ftp type access routines **/
#define FTPsetType(ftp,num)    ((ftp)->Ftptype=(num))
#define FTPgetType(ftp)        ((ftp)->Ftptype)

/** Type command **/
#define FTPtype(ftp,type)      FTPcommand(ftp,"TYPE %s",type,201)
#define FTPbinary(ftp)         FTPtype(ftp,"I")
#define FTPascii(ftp)          FTPtype(ftp,"A")


/** Chdir **/
/*#define FTPchdir(ftp,dir)         FTPcommand(ftp,"CWD %s",dir,299)*/

void FTPcleanup();
void FTPcloseData(FTP *);
void FTPdestroy(FTP *);
void FTPfindType(FTP *);
void FTPbyebye(FTP *);
void FTPopenData(FTP *ftp, char *command, char *file, int errorlevel);
boolean IsBinaryType(char *);
void FTPabort(FTP *);

/* mdallen: more prototypes.... */
int FTPerrorMessage(FTP *ftp, char *message);
boolean FTPsend(FTP  *ftp, char *command);
int FTPgetReply(FTP  *ftp, int  errorlevel, char *message, int  maxlen);
int FTPgetReplyline(FTP  *ftp, char *message, int  maxlen);
int FTPinfoMessage(FTP *ftp, char *message);

#ifndef NO_FTP
int GopherFTPgw(int sockfd, char *ftpstr, CMDobj *cmd);
int GopherList(FTP *ftp, char *bufptr, char *theName, GopherObj *gs,  
               char *ftpuser, char *ftppass);
int ParseUnixList(FTP *ftp, char *bufptr, char *IntName, char *theName,
                  int cols, GopherObj *gs, char *ftpuser, char *ftppass);
int ParseVMSList(FTP  *ftp, char *bufptr, char *IntName, char *theName,
                 GopherObj *gs, char *ftpuser, char *ftppass);         
int ParseOS2List(FTP *ftp, char *bufptr, char *IntName, char *theName,
                 GopherObj *gs, char *ftpuser, char *ftppass);        
int GopherType(FTP  *ftp, char *bufptr, char *theName);
int GopherFile(FTP  *ftp, char *buf, char *theName);
#endif /* NO_FTP */

#endif /* GOPHER_FTP_H */


