/********************************************************************
 * $Author: jgoerzen $
 * $Date: 2000/12/20 04:11:07 $
 * error.h - prototypes for error.c
 *
 * Revision 1.0  2000/12/18 23:13:12  mdallen
 * created file, added prototypes.  What else?  :)
 */

void err_init                      (void);
void err_quit                      (char *va_alist, ...);
void err_sys                       (char *va_alist, ...);
void err_ret                       (char *va_alist, ...);
void err_dump                      (char *va_alist, ...);
void my_perror                     (void);
char *sys_err_str                  (void);
