/* manager.h
 * Written by David Allen <s2mdalle@titan.vcu.edu>
 * Simple prototypes for functions in manager.c
 */

#define MANAGER_H 1

void Draw_Banner(void);
void Draw_Status(char *textline);
void DisplayTitle(GopherObj *gs, int maxlength, boolean dogplus);
void Display_Dir_Page(GopherDirObj *gopherdir, int iNewLine, 
                      int nNewPage, int nMaxPages, 
                      int iPageLen, int iLastPageLen);
int scline(int iOldGopher, int iNewGopher, GopherDirObj *gophersdir);
int GetMenu(GopherDirObj *gd, int *typedchar, boolean redisplay);
