/* daemon.h
 * Written by mdallen
 * Prototypes for the functions in daemon.c
 */

void sig_child                   (void);
void daemon_start                (int ignsigcld);
