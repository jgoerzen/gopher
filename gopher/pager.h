/* pager.h
 * Written by David Allen <s2mdalle@titan.vcu.edu>
 * Simple prototypes for functions in pager.c
 */

#define PAGER_H 1

void PagerHelp(CursesObj *cur);
void PagerInitGlobals(void);
void PagerTitles(CursesObj *cur, GopherObj *gs, int totalbytes);
int PagePosAlloc(void);
int PagePosRealloc(void);
void PagePosFree(void);
void PagerPercent(int bytes, int totalbytes);
void PagerParseSearchstring(void);
void PagerPrintLine(char *inputline);
void PagerSeekPage(FILE *thefile, int gotopage, int *bytecount);
void PagerNextPage(CursesObj *cur, FILE *thefile, char *theline, 
                   int *bytecount, int totalbytes);
boolean PagerSearch(FILE *thefile, char *theline, int *bytecount, 
                    char *search);
void PagerBuiltin(CursesObj *cur, GopherObj *gs);

