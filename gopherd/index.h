/* index.h
 * Written by mdallen 2000/12/18 23:25:39
 *
 * Prototypes for index.c
 */

#include "command.h"

void Do_IndexTrans                (int sockfd,
                                   char *IndexDirectory,
                                   CMDobj *cmd, boolean SendEOF);
void WaisIndexQuery               (int sockfd, char *index_directory,
                                   char *SearchWords, char *new_db_name,
                                   char *INDEXHost, int INDEXPort,      
                                   char *INDEXPath, boolean isgplus,
                                   char *view);
