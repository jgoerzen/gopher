#define __GOPHER_MIME_MIMETYPES_C__

#include <glib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "mimetypes.h"


static GHashTable *types = NULL, *encodings = NULL;

char *getmimetype(char *filename, char *defaulttype) {
        char *mimetype, *encoding;
        getmimetypes(filename, &mimetype, &encoding, defaulttype);
        return mimetype;
}

char *getencoding(char *filename) {
        char *mimetype, *encoding;
        getmimetypes(filename, &mimetype, &encoding, "DEFAULTTYPE");
        return encoding;
}

void getmimetypes(char *filename, char **mimetype,
                  char **encoding, char *defaulttype) {
        gchar *ext;
        gchar *myfilename;

        /* Initialize to defaults. */
        *mimetype = defaulttype;
        *encoding = NULL;

        myfilename = g_strdup(filename);
        ext = strrchr(myfilename, '.');
        if (ext) {              /* We have an extension. */
                *ext = 0;       /* Set it to null for future lookups. */
                ext++;          /* Advance to actual string */
                if ((*encoding = g_hash_table_lookup(encodings, ext))) {
                        /* OK, got an encoding.  Now reset for base type
                           lookup. */
                        ext = strrchr(myfilename, '.');
                }
                if (ext) {      /* Check again -- maybe had enc but no type */
                        gchar *type;
                        ext++;
                        type = g_hash_table_lookup(types, ext);
                        if (type) {
                                *mimetype = type;
                        }
                }
        }
        g_free(myfilename);
}

void scanmimetypes(char *filename) {
        scanfile(filename, &types);
}

void scanencodings(char *filename) {
        scanfile(filename, &encodings);
}

void scanfile(char *filename, GHashTable **tableptr) {
        int fd;
        GHashTable *table;
        GScanner *scanner;
        GString *curmimetype = NULL;
        GScannerConfig config;

        fd = open(filename, O_RDONLY);
        if (fd < 0) return;

        /* Initialize the table if needed. */

        table = *tableptr;
        if (! table) {
                *tableptr = g_hash_table_new(g_str_hash, g_str_equal);
                table = *tableptr;
        }

        /* Configure the scanner. */
        config.cset_skip_characters = " \t\n";
        config.cpair_comment_single = "#\n";
        config.cset_identifier_first = G_CSET_a_2_z "_" G_CSET_A_2_Z;
        config.skip_comment_multi = 0;
        config.scan_comment_multi = 0;
        config.cset_identifier_nth = G_CSET_a_2_z G_CSET_A_2_Z G_CSET_LATINS
                G_CSET_LATINC "_-/0123456789.+";

        /* Create the scanner. */
        
        scanner = g_scanner_new(&config);
        g_scanner_input_file(scanner, fd);

        /* Scan.  */
        while (g_scanner_peek_next_token(scanner) != G_TOKEN_EOF) {
                GTokenType token = g_scanner_get_next_token(scanner);
                GTokenValue value;
                char tmpstr[2];

                value = g_scanner_cur_value(scanner);
                switch (token) {
                case G_TOKEN_IDENTIFIER:
                        if (strchr(value.v_identifier, '/')) {
                                g_string_assign(curmimetype, 
                                                value.v_identifier);
                        } else {
                                 safeinsert(table, value.v_identifier,
                                           curmimetype->str);
                        }
                        break;
                case G_TOKEN_CHAR:
                        /* Single-char extensions come in this way. */
                        tmpstr[0] = value.v_char;
                        tmpstr[1] = 0;
                        safeinsert(table, tmpstr, curmimetype->str);
                        break;
                default:
                        break;
                }
        }

        g_scanner_destroy(scanner);
}

void safeinsert(GHashTable *table, gpointer key, gpointer value) {
        gpointer old_value, old_key;

        /* If already there... */
        if (g_hash_table_lookup_extended(table, key, &old_key, &old_value)) {
                /* Insert new value. */
                g_hash_table_insert(table, g_strdup(key), g_strdup(value));
                g_free(old_key);
                g_free(old_value);
        } else {
                g_hash_table_insert(table, g_strdup(key), g_strdup(value));
        }
}

