/* ourutils.h
 * Written by David Allen <s2mdalle@titan.vcu.edu>
 * Simple prototypes for functions in ourutils.c
 */

#define OURUTILS_H 1

#include "CURcurses.h"
#include "upload.h"
#include "pager.h"

#ifndef DOWNLOAD_H
#  include "download.h"
#endif /* DOWNLOAD_H */

void UNIXfile(char *fname);
int outchar(int c);
int twirl(void);
void GSmail(GopherObj *gs);
char *Choose_View(GopherObj *gs);
Requestitem *REQitemnew(void);
void REQitemdestroy(Requestitem *req);
char **AskBlock(GopherObj *gs);
void Save_file(GopherObj *gs, char *saveto, char *view);
void Save_list(GopherDirObj *gd);
void GSdisplay(GopherObj *gs);
boolean GStoFile(GopherObj *gs, FILE *f, char *view, 
                 int (*twirlfn) (/* ??? */));
void GSprompter(GopherObj *gs, char *view, char *command);
void CursesMsg(char *Message, char *title);
void CursesErrorMsg(char *Message);
int logrequest(char *msg, GopherObj *gs);
void Gplus_Error(int sockfd);
