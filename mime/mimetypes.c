#include <glib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void safeinsert(GHashTable *table, gpointer key, gpointer value);
static void print_entry(gpointer key, gpointer value, gpointer user_data);

void main(void) {
        GScanner *scanner;
        GScannerConfig config;
        GHashTable *table;
        GString *curmimetype = NULL;

        int filefd;
        
        curmimetype = g_string_new(NULL);

        /* Create the table. */
        table = g_hash_table_new(g_str_hash, g_str_equal);

        filefd = open("/etc/mime.types", O_RDONLY);
        if (filefd < 0) exit(1);
        
        /* Create the scanner. */
        
        config.cset_skip_characters = " \t\n";
        config.cpair_comment_single = "#\n";
        config.cset_identifier_first = G_CSET_a_2_z "_" G_CSET_A_2_Z;
        config.skip_comment_multi = 0;
        config.scan_comment_multi = 0;
        config.cset_identifier_nth = G_CSET_a_2_z G_CSET_A_2_Z G_CSET_LATINS
                G_CSET_LATINC "_-/0123456789.+";
        scanner = g_scanner_new(&config);
        g_scanner_input_file(scanner, filefd);

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

        

        g_hash_table_foreach(table, print_entry, NULL);
        g_scanner_destroy(scanner);
        g_hash_table_destroy(table);
}

void safeinsert(GHashTable *table, gpointer key, gpointer value) {
        char *old_value, *old_key;

        /* If already there... */
        if (g_hash_table_lookup_extended(table, key, &old_key, &old_value)) {
                printf("WARN: Replacing %s = %s with %s = %s\n",
                       old_key, old_value, key, value);
                /* Insert new value. */
                g_hash_table_insert(table, g_strdup(key), g_strdup(value));
                g_free(old_key);
                g_free(old_value);
        } else {
                g_hash_table_insert(table, g_strdup(key), g_strdup(value));
        }
}

                
static void print_entry(gpointer key, gpointer value, gpointer user_data) {
        printf("%s: %s\n", key, value);
}
