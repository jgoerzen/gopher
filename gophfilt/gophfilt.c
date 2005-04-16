/********************************************************************
 * 
 * @(#)gophfilt.c	1.2
 *
 *********************************************************************
 * MODULE: gophfilt.c
 * Semi-traditional unix filter for gophering.
 *********************************************************************
 */

#include <stdio.h>
#include "Stdlib.h"
#include <signal.h>
#include "conf.h"
#include "string.h"
#include "GSgopherobj.h"

GopherObj *Oneshot;

int DEBUG = 0;

SIGRETTYPE timeout();

/*** for getopt processing ***/
extern char *optarg;
extern int optind;

#ifdef VMS
int
DCLsystem(command)
char *command;
{
    if(command && strcmp(command, ""))
        fprintf(stdout, "Cannot execute: %s\n", command);
    else
        fprintf(stdout, "Cannot spawn.\n");
    return(0);
}
#endif /* VMS */

int
main(argc, argv)
  int argc;
  char *argv[];
{

  int manual = 0;
  int path = 0;
  int type = 0;
  unsigned timer = 10;
  char *host = CLIENT1_HOST;
  int port = CLIENT1_PORT;
  char *item = "";
  int c, x = 0;
  int numread, sockfd;
  char buf[1024];

  /* Allocate the Gopher object structure */

  Oneshot = GSnew();

  while ((c = getopt(argc, argv, "h:s:p:i:t:T:")) != -1)
    switch (c) {
      case 's':
	port = atoi(optarg);
	manual++;
        break;
      case 'p':
        GSsetPath(Oneshot, optarg);
	manual++;
	path++;
        break;
      case 'h':
	host = optarg;
	manual++;
        break;
      case 'T':
	timer = (unsigned)atoi(optarg);
	break;
      case 't':
        GSsetType(Oneshot, *optarg);
	manual++;
	type++;
        break;
      case 'i':
	item = optarg;
	manual++;
	break;

      default:
#ifdef VMS
	fprintf(stdout, "Error in parsing command line arguments.\n");
	exit(0);
#else
	return(-1);
#endif
    }

     /**** Get a URL from the command line, if present ****/
     
     if (optind < argc && strchr(argv[optind], ':') != NULL) {

        x=GSfromURL(Oneshot, argv[optind], AFTP_HOST, AFTP_PORT, x);
	manual+=x;
	if (x & G_PATH)
	     path++;
	if (x & G_TYPE)
	     type++;
        if (x & G_HOST)
	     host = GSgetHost(Oneshot);
	if (x & G_PORT)
	     port = GSgetPort(Oneshot);
     }

    /* Set the port to either the default or one provided */

    GSsetPort(Oneshot, port);
    GSsetHost(Oneshot, host);

    /* See if the minimum stuff was provided */

    if (manual && (!path || !type)) {
#ifdef VMS
      fprintf(stdout,
              "Manual operation and insufficient parameters were provided.\n");
      exit(0);
#else
      return(-2);		/* error if not */
#endif
    }

  /* Get the selector item from stdin if not already filled in by hand */

  if (!manual) {
    if (GSfromNet(Oneshot, 0) < 0) {
      /* Error if cannot get it */
#ifdef VMS
      fprintf(stdout,
      	      "Piped operation and unable to read selector from stdin.\n");
      exit(0);
#else
      return(-3);
#endif
    }
  }

   /** Get the kind of file from the first character **/
   /** Filter out files that we can't deal with **/

   switch (GSgetType(Oneshot)) {
     case A_FILE:
     case A_DIRECTORY:
     case A_MACHEX:
     case A_PCBIN:
     case A_CSO:
     case A_INDEX:
     case A_SOUND:
     case A_UNIXBIN:
     case A_GIF:
     case A_HTML:
     case A_MIME:
     case A_IMAGE:
       break;
     default:
#ifdef VMS
       fprintf(stdout, "Unsupported item type requested.\n");
       exit(0);
#else
       return(-4);  
#endif
     }
     

     /* Connect to the specified server */
     if ((sockfd = GSconnect(Oneshot)) <0) {
#ifdef VMS
	  fprintf(stdout,
	        "Unable to connect to specified host and port combination.\n");
	  exit(0);
#else
	  return(-5);
#endif
     }

     /** Send out the request **/

     writestring(sockfd, GSgetPath(Oneshot));
     if (strlen(item)) {
       writestring(sockfd, "\t");
       writestring(sockfd, item);
     }
     writestring(sockfd, "\r\n");

     /* Set an alarm in case we get nothing */

     (void)signal(SIGALRM, timeout);
     (void)alarm(timer);
	  
     switch (GSgetType(Oneshot)) {

     case A_FILE:		/* contents of a plain file */
     case A_DIRECTORY:		/* listing of a directory  */
     case A_MACHEX:		/* results of a search */
     case A_INDEX:
	  /* Text ends with a lonesome '.' on a line */
	  while (readline(sockfd, buf, sizeof(buf)) > 0) {
	       (void)alarm((unsigned)0);
	       (void)alarm(timer);
	       ZapCRLF(buf);
	       if (*buf == '.' && *(buf+1) == '\0')
		    break;
	       fputs(buf, stdout);
	       putc('\n', stdout);
	  }

	  break;
	  
     case A_SOUND:		/* Binary stuff */
     case A_IMAGE:
     case A_GIF:
     case A_UNIXBIN:
     case A_PCBIN:
	  /* Binary stuff we just read til it's gone */
	  while ((numread = readn(sockfd, buf, sizeof buf)) > 0) {
	       (void)alarm((unsigned)0);
	       (void)alarm(timer);
	       if (fwrite(buf, numread, 1, stdout) == 0) {
		    close(sockfd);
#ifdef VMS
		    fprintf(stderr, "Error encountered writing to stdout.\n");
#else
		    return(-6);
#endif
	       }
	  }
	  break;
     }

     close(sockfd);
     (void)alarm((unsigned)0);
#ifdef VMS
     exit(1);
#else
     return(0);
#endif
}
/*SUBTTL timeout() - Alarm expired while awaiting data */
/*
 */
SIGRETTYPE timeout()
{
#ifdef VMS
  fprintf(stdout, "Timeout occurred while waiting for more data.\n");
  exit(0);
#else
  exit(-7);
#endif
}
