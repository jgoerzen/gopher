/* download.h
 * Written by David Allen <s2mdalle@titan.vcu.edu>
 * Simple prototypes for functions in download.c
 */

#define DOWNLOAD_H 1

#ifndef OURUTILS_H
#  include "ourutils.h"
#endif /* OURUTILS_H */

void Download_file(GopherObj *gs);
void BuiltinDownload(char *dirname);
