#ifndef __GOPHER_MIME_MIMETYPES_H__
#define __GOPHER_MIME_MIMETYPES_H__

#include <glib.h>

#ifdef __GOPHER_MIME_MIMETYPES_C__
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN void scanfile(char *filename, GHashTable **tableptr);
EXTERN void getmimetypes(char *filename, char **mimetype,
              char **encoding, char *defaulttype);
EXTERN char *getmimetype(char *filename, char *defaulttype);
EXTERN char *getencoding(char *filename);
EXTERN void scanmimetypes(char *filename);
EXTERN void scanencodings(char *filename);
EXTERN void safeinsert(GHashTable *table, gpointer key, gpointer value);


#endif
