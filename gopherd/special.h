/* special.h
 * Prototypes for functions in special.c
 */

#include "dedot.h"

FILE    *Specialfile(int sockfd, FILE *fp, char *pathname);
int      Specialclose(FILE *fp);
