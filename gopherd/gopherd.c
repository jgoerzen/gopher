/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.33 $
 * $Date: 2002/03/25 14:12:51 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/gopherd.c,v $
 * $State: Exp $
 *
 * Originally by: Paul Lindner, University of Minnesota CIS.
 *

 * COPYRIGHT
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * Copyright (C) 2000-2002 John Goerzen and other developers
 * see the file "Copyright" in the distribution for conditions of use.
 * COPYRIGHT 
 *********************************************************************
 * MODULE: gopherd.c
 * Routines to implement the gopher server.
 *********************************************************************
 * Revision 3.1.1.1  1993/02/11  18:02:55  lindner
 * Gopher+1.2beta release
 *
 *********************************************************************/


/* Originally derived from an 
 * Example of server using TCP protocol
 * pp 284-5 Stevens UNIX Network Programming
 */

#include <stdio.h>
#include "gopherd.h"
#include "command.h"
#include "patchlevel.h"
#include "Malloc.h"
#include "Debug.h"
#include "fileio.h"
#include "util.h"

#ifdef _AUX_SOURCE
#  include <compat.h>
#endif

#ifdef _AIX
#  include <sys/select.h>
#endif

#undef stat /** Stupid openers thing..**/

/***************************Protos******************************/
static GopherDirObj *GDfromSelstr(CMDobj *cmd, int sockfd);
GopherDirObj        *GDfromUFS(char *pathname, int sockfd, boolean isGplus);
static void          item_info(CMDobj *cmd, int sockfd);
static void          send_binary(int, char *, boolean);
static void          printfile(int, char *, int, int, boolean);
static void          listdir(int, char *, boolean, char *, char *, CMDobj *);
static void          SetInitialScriptEnvironment();

static int           do_command(int sockfd);

extern int           do_mindexd(int, char *, char *, int, char *);


/******* Global variables *********/

static	char*	Gdefusername = NULL;
static  uid_t   Guid;
static  gid_t   Ggid;

/**********************************/

extern char *getdesc();

#ifdef LOADRESTRICT
extern double maxload;
#endif

#include "authenticate.h"

#include "STAarray.h"
#include "STRstring.h"
#include "Sockets.h"

/* This function is called on a read timeout from the network */

#include <setjmp.h>
jmp_buf env;

RETSIGTYPE read_timeout(int sig)
{
     longjmp(env,1);
}

static void
gopherd_usage(char *progname) 
{
     fprintf(stderr, "Usage: %s [-mCDIc] [-u username] [-s securityfile] [-l logfile] [ -L loadavg ] <datadirectory> <port>\n", progname);
     fprintf(stderr, "   -C  turns caching off\n");
#ifdef DEBUGGING
     fprintf(stderr, "   -D  enables copious debugging info\n");
#endif
     fprintf(stderr, "   -I  enable \"inetd\" mode\n");
     fprintf(stderr, "   -c  disable chroot(), use secure open routines instead\n");
     fprintf(stderr, "   -u  specifies the username for use with -c\n");
     fprintf(stderr, "   -o  override the default options file '%s'\n", CONF_FILE);
     fprintf(stderr, "   -l  specifies the name of a logfile\n");
     fprintf(stderr, "   -L  specifies maximum load to run at\n");
} /* End gopherd_usage */

void
gopherd_exit(int val)
{
     exit(val);
} /* End gopherd_exit */

/*
 * This is for when we dump core..  Make an attempt to clean up..
 */

RETSIGTYPE
sigabnormalexit(void)
{
     gopherd_exit(-1);
} /* End sigabnormalexit */

int
main(int argc, char *argv[], char *envp[])
{
     int                childpid;
     int                sockfd       = -1, 
                        gophersockfd = -1, 
                        httpsockfd   = -1, 
                        newsockfd    = -1;
     int                clilen = 0;
     struct sockaddr_in cli_addr;
     boolean            OptionsRead = FALSE;
     int                i = 0;
     int                tmpstrlen = 256;
     char               tmpstr[tmpstrlen];
     int                numready;
     char               *cp=NULL;  /* Logfilename */
     fd_set             socketfds;

     /*** for getopt processing ***/
     int c;
     extern char *optarg;
     extern int optind;
     int errflag =0;

     void (*fptr_sigabnormalexit)() = sigabnormalexit;

     Gtxtlocale(LC_ALL, "");

     Argv = argv;

#ifdef NeXT
     malloc_singlethreaded();
#endif

#if !(defined(NeXT) || defined(_AIX) || defined(__osf__))
     /* NeXTs don't like their envp to be overwritten... */

     for (i=0; envp[i] != NULL; i++)
	  ;
#endif

     if (i > 0)
	  LastArgv = envp[i - 1] + strlen(envp[i - 1]);
     else
	  LastArgv = argv[argc - 1] + strlen(argv[argc - 1]);

     pname = argv[0];

     err_init();	/* openlog() early - before we chroot() of course */

     RunServer = TRUE;  /** Run the server by default **/

     Config = GDCnew();  /** Set up the general configuration **/
 
     while ((c = getopt(argc, argv, "CDIcL:l:o:u:")) != -1)
	  switch (c) {
	  case 'D':
	       DEBUG = TRUE;
	       break;

	  case 'I':
	       RunFromInetd = TRUE;
	       break;

	  case 'C':
	       GDCsetCaching(Config, FALSE);
	       break;

	  case 'c':
	       dochroot = FALSE;
	       break;

#ifdef LOADRESTRICT
	  case 'L':  /** Load average at which to restrict usage **/
	       maxload = atof(optarg);
	       break;
#endif

	  case 'l':  /** logfile name **/
	       if (*optarg == '/' || strcasecmp(optarg, "syslog") == 0)
		    GDCsetLogfile(Config, optarg);
	       else {
#ifdef HAVE_GETCWD
		    getcwd(tmpstr, tmpstrlen);
#else
                    getwd(tmpstr);
#endif /* HAVE_GETCWD */
		    strcat(tmpstr, "/");
		    strcat(tmpstr, optarg);
                    
		    GDCsetLogfile(Config, tmpstr);
	       }
	       break;
	       
	  case 'o': /** option file **/
	       if (*optarg == '/')
		    GDCfromFile(Config, optarg);
	       else {
#ifdef HAVE_GETCWD
		    getcwd(tmpstr, tmpstrlen);
#else
                    getwd(tmpstr);
#endif /* HAVE_GETCWD */
		    strcat(tmpstr, "/");
		    strcat(tmpstr, optarg);
		    GDCfromFile(Config, tmpstr);
	       }
	       OptionsRead = TRUE;
	       break;

	  case 'u':
	       {
		    struct passwd *pw = getpwnam( optarg );
		    if ( !pw ) {
			 fprintf(stderr,
			      "Could not find user '%s' in passwd file\n",
			      optarg);
			 errflag++;
		    } else {
			 if (!(pw->pw_uid == getuid() || getuid() == 0))
			      printf("Need to be root to use -u\n"), gopherd_exit(-1);
			 
			 Gdefusername = strdup(optarg);
			 
			 Guid = pw->pw_uid;
			 Ggid = pw->pw_gid;

		    }
	       }
	       break;

	  case '?':
	  case 'h':
	       errflag++;
	       break;
	  }

     if (errflag) {
	  gopherd_usage(argv[0]);
	  gopherd_exit(-1);
     }

     if (optind < argc) {
	  Data_Dir = strdup(argv[optind]);
	  optind++;
	  Debug("main: Setting data to Data Directory is %s\n",Data_Dir);
     }
     if (Data_Dir == NULL)
	  Data_Dir = strdup(DATA_DIRECTORY);

     Debug("main: Data Directory is %s\n",Data_Dir);

     if (optind < argc) {
	  GopherPort = atoi(argv[optind]);
	  optind++;
	  Debug("main: Setting port to %d\n",GopherPort);
     }
     Debug("main: Port is %d\n",GopherPort);

     /** Read the options in, if not overridden **/
     if (OptionsRead == FALSE)
	  GDCfromFile(Config, CONF_FILE);

     /** Check to make sure the options specified are workable... ***/
     GDCintegrityCheck(Config);

     /*** Make sure we do a tzset before doing a chroot() ***/
     tzset();
     

     if (!RunFromInetd) {
	  /* char *cp; */
	  printf("Internet Gopher Server %s.%s patch %d (%s)\n", GOPHER_MAJOR_VERSION, GOPHER_MINOR_VERSION, PATCHLEVEL, GOPHER_NICKNAME);
	  printf("Copyright 1991-2000 the Regents of the University of Minnesota\n");
	  printf("Copyright (C) 2000-2002 John Goerzen and the gopher developers\n");
	  printf("See the file 'Copyright' for conditions of use\n");
	  printf("Data directory is %s\n", Data_Dir);
	  printf("Port is %d\n", GopherPort);

	  /*** Check for weird inconsistencies, security warnings etal ***/
	  if (getuid() == 0 && Gdefusername == NULL)
	       printf("Warning! You should run the server with the -u option!\n");
	  else if (Gdefusername != NULL)
	       printf("Running as user '%s'\n", Gdefusername);
	  
	  if (dochroot == FALSE)
	       printf("Not using chroot() - be careful\n");

	  cp = GDCgetLogfile(Config);

	  if (cp && *cp != '\0')
	       printf("Logging to File %s\n", GDCgetLogfile(Config));
     }


     if (uchdir(Data_Dir)) {
	  fprintf(stderr, "Cannot change to data directory!! %s \n",Data_Dir);
	  exit(-1);
     }

     if (dochroot && getuid() != 0) {
	  fprintf(stderr, "Gopherd uses the privileged call chroot().  Please become root.\n");
	  exit(-1);
     }

     fflush(stderr);
     fflush(stdout);

#ifdef _AUX_SOURCE
     (void) set42sig();
#endif

     if (DEBUG == FALSE && RunFromInetd==FALSE)
	  daemon_start(TRUE);

     /** We ignore SIGUSR2, so the PID routines can "ping" us **/
     (void) signal(SIGUSR2, SIG_IGN);
     (void) signal(SIGPIPE, SIG_IGN);
     (void) signal(SIGINT,  fptr_sigabnormalexit);
     (void) signal(SIGSEGV, fptr_sigabnormalexit);

     /*** Hmmm, does this look familiar? :-) ***/
     err_init();

     /** Ask the system what host we're running on **/
     Zehostname = SOCKgetDNSname(GDCgetHostname(Config));
     Debug("I think your hostname is %s\n", Zehostname);

     SetInitialScriptEnvironment();

     if (RunFromInetd) {
	  /** Ask the system which port we're running on **/
	  int newport=0;
	  if ((newport =SOCKgetPort(0)) !=0)
	       GopherPort=newport;

	  /*** Do the stuff for inetd ***/
	  while(do_command(fileno(stdout))!=0);	/* process the request */

	  shutdown(fileno(stdout), 2);
	  shutdown(fileno(stdin), 2);
	  shutdown(fileno(stderr), 2);

	  fclose(stdout);
	  fclose(stdin);
	  fclose(stderr);

	  gopherd_exit(0);
     } else {
	  LOGGopher(-1, "Starting gopher server (pid %d)", getpid());
     }

     ServerStarted = time(NULL);
     ServerSetArgv("waiting for connection @ %d", GopherPort);

     /** Open a TCP socket (an internet stream socket **/
     gophersockfd = SOCKbind_to_port(GopherPort);
     if (gophersockfd < 0)
	  perror("Cannot bind local address"), exit(-1);

     listen(gophersockfd, 16);

     /** Do HTTP queries **/
     if (GDCgetHTTPport(Config)) {
	  httpsockfd = SOCKbind_to_port(GDCgetHTTPport(Config));
	  if (httpsockfd < 0) {
	       /** Give up on WWW serving, probably someone else using address
		**/
	       GDCsetHTTPport(Config, -1);
	  }
	  listen(httpsockfd, 5);
     }

     /* Move (well, copy) the setuid and chroot code to here.  This way,
	the server process runs chroot and setuid'd much sooner.  Much
	more secure that way.  I like it a lot better this way than the
	other. */

     /* So here is the right place to open the log file for processing log
      * messages before we go into chroot() jail.
      */

     cp = GDCgetLogfile(Config);
     /* The LOGFileDesc variable is used by LOGGopher() to log messages. */
     if (cp != NULL && *cp != '\0') {
	  if (strcasecmp(GDCgetLogfile(Config), "syslog")==0) {
	       LOGFileDesc = -2;  /** log file is syslog **/
	  } else if (LOGFileDesc < 0) {
	       LOGFileDesc = uopen(GDCgetLogfile(Config), 
				   O_WRONLY|O_APPEND|O_CREAT, 0644);
	  }
	  if (LOGFileDesc == -1) {
	       printf("Can't open the logfile: %s\n", GDCgetLogfile(Config));
	       gopherd_exit(-1);
	  }
     }

     /** Change our root directory **/
     if ( dochroot && didchroot == FALSE) {
	  if (chroot(Data_Dir))
	       Die(sockfd, 500, "Data_Dir dissappeared!");
	  
	  uchdir("/");	/* needed after chroot */
	  didchroot = TRUE;
     }

     if (Gdefusername && (getuid() == 0)) {
         setregid(Ggid, Ggid);
         setreuid(Guid, Guid);
	 LOGGopher(0, "Server running setuid/setgid to %d:%d", Guid, Ggid);
     }

     if (didchroot) {
	LOGGopher(0, "STARTUP: Server running chroot to %s", Data_Dir);
     } else {
	LOGGopher(0, "STARTUP: Server running in non-chroot mode in %s", Data_Dir);
     }

     

     numready = 0; 

     while (1) {

	  /*
	   * Wait for a connection from a client process.
	   * This is an example of a concurrent server.
	   */
	  if (numready <= 0) {
	       int maxfd = 0;
	  
	       FD_ZERO(&socketfds);
	       FD_SET(gophersockfd, &socketfds);
	       maxfd = gophersockfd + 1;
	       if (httpsockfd > -1) {
		    FD_SET(httpsockfd, &socketfds);
		    maxfd = (httpsockfd + 1 > maxfd) ? httpsockfd + 1 : maxfd;
	       }
	       
	       while ((numready = 
		       select(maxfd, &socketfds, NULL, NULL, NULL)) < 0)
		    ; /** keep trying when we get errors... **/

	  }

	  if (FD_ISSET(gophersockfd, &socketfds)) {
	       sockfd = gophersockfd;
	       FD_CLR(gophersockfd, &socketfds);
	       numready--;
	  }
	  else if (FD_ISSET(httpsockfd, &socketfds)) {
	       sockfd = httpsockfd;
	       FD_CLR(httpsockfd, &socketfds);
	       numready--;
	  }
	  
	  clilen = sizeof(cli_addr);
	  while (1) {
	       newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
				  &clilen);

	       if (newsockfd >= 0)
		    break;
	       else if (errno != EINTR)  /** Restart accept if we hit a
					     SIGCHLD signal..**/
		    LOGGopher(newsockfd, "Client went away");
	  }
		    

	  SOCKlinger(sockfd, FALSE);
	  SOCKlinger(newsockfd, FALSE);


	  ActiveSessions ++;
	  if ((GDCgetMaxconns(Config) > 0 ) && ActiveSessions > GDCgetMaxconns(Config)) {
	       Warn(newsockfd, 503, "Too Many Users at this time, try again later");
	       ActiveSessions--;
	       close(newsockfd);

	  } else {
	  
	       if ( (childpid = fork()) < 0) {
		    /** Problems forking..  **/
		    writestring(newsockfd, "3Problems forking!\tmoo\terror.host\t0\r\n.\r\n");
	       }
	       
	       else if (childpid == 0) {	/* Child process */
		    close(sockfd);		/* close original socket */
		    
		    (void)do_command(newsockfd);/* process the request */
		    gopherd_exit(0);
	       }
	       /** Parent **/
	       /** clean up any zombie children **/
	       sig_child();
	       
	       close(newsockfd); 		/* parent process */
	       Connections++;
	  }
     }

     return(0);
} /* End main() */

int EXECflag;

static void
SetInitialScriptEnvironment(void)
{
     /* Stuff that doesn't change over the life of the server... */
     char tmpstr[256];

     snprintf(tmpstr, sizeof(tmpstr),
	      "UofMNgopherd/%s.%spl%d", GOPHER_MAJOR_VERSION,
	      GOPHER_MINOR_VERSION, PATCHLEVEL);

     SetEnvironmentVariable("SERVER_SOFTWARE", tmpstr);

     SetEnvironmentVariable("SERVER_NAME", Zehostname);

     snprintf(tmpstr, sizeof(tmpstr), "%d", GopherPort);
     SetEnvironmentVariable("SERVER_PORT", tmpstr);
     SetEnvironmentVariable("SERVER_PROTOCOL", "gopher/1.0");
     SetEnvironmentVariable("GATEWAY_INTERFACE", "CGI/1.0");
} /* End SetInitialScriptEnvironment */

static void
SetScriptEnvironment(CMDobj *cmd, char *view)
{
     SetEnvironmentVariable("GPLUS_CMD", CMDgetCommand(cmd));
     if (getenv("CONTENT_TYPE") == NULL)
	  SetEnvironmentVariable("CONTENT_TYPE", view);
     SetEnvironmentVariable("GUSER", CMDgetUser(cmd));
     SetEnvironmentVariable("GTICKET", CMDgetTicket(cmd));

     
     switch (CMDgetHTTPcmd(cmd)) {
     case UnknownHTTPcmd:
     case GET:
	  SetEnvironmentVariable("REQUEST_METHOD", "GET");
	  break;
     case POST:
	  SetEnvironmentVariable("REQUEST_METHOD", "POST");
	  break;
     }

     SetEnvironmentVariable("PATH_INFO", CMDgetSelstr(cmd));
     SetEnvironmentVariable("PATH_TRANSLATED", CMDgetSelstr(cmd));

     SetEnvironmentVariable("SCRIPT_NAME", CMDgetFile(cmd));
     SetEnvironmentVariable("QUERY_STRING", CMDgetSearch(cmd));
     
     SetEnvironmentVariable("REMOTE_HOST", CurrentPeerName);
     SetEnvironmentVariable("REMOTE_ADDR", CurrentPeerIP);
     
     SetEnvironmentVariable("REMOTE_USER", CMDgetUser(cmd));
} /* End SetScriptEnvironment */

void
OkHTTPresponse(int sockfd)
{
     writestring(sockfd, "HTTP/1.0 200 Ok\r\n");
     writestring(sockfd, "MIME-Version: 1.0\r\n");
     writestring(sockfd, "Server: GopherWEB/2.1.4\r\n");
     writestring(sockfd, "Content-Type: text/html\r\n\r\n");
} /* End OkHTTPresponse */


#ifndef NO_AUTHENTICATION
void
OutputAuthForm(int sockfd, char *pathname, char *host, int port, CMDprotocol p)
{
     char tmpbuf[512];
     if (p == HTTP1_0) 
	  OkHTTPresponse(sockfd);

     snprintf(tmpbuf, sizeof(tmpbuf),
	 "<HTML>\r\n<HEAD><TITLE>Authentication...</TITLE></HEAD><BODY>\r\n");
     writestring(sockfd, tmpbuf);

     snprintf(tmpbuf, sizeof(tmpbuf),
	     "<FORM METHOD=\"GET\" ACTION=\"http://%s:%d/halidate%%20%s\">\r\n",
	     host, port, pathname);
     writestring(sockfd, tmpbuf);
     writestring(sockfd, "Username\r\n<INPUT type=\"text\" NAME=\"user\"><BR>\r\n");
     writestring(sockfd, "Password\r\n<INPUT type=\"password\" NAME=\"pw\"><BR>\r\n");
     writestring(sockfd, "<HR><INPUT TYPE=\"submit\" VALUE=\"Log In\"><INPUT TYPE=\"reset\" VALUE=\"Clear Form\">");

     writestring(sockfd, "</FORM></BODY></HTML>\r\n");

} /* End OutputAuthForm */
#endif

void
HandleHTTPpost(CMDobj *cmd)
{
     int doneCONTENT_TYPE;
     char *hp, *qp;
     boolean ispost= 0, usecgiparams= 1;
     int  i;
  
     if (CMDgetHTTPcmd(cmd) == POST) {
	  FILE *retrfile;
	  ispost= 1;

	  /** Write the stuff out to a file, here, as gopherd user
 	      so we can remove file later on.. **/

          ASKfile = tempnam(NULL, "gdata");
          Debug("HTTP POST data is in %s\n", ASKfile);
 
          retrfile = ufopen(ASKfile, "w");
	  if (retrfile != NULL) {
	       /* ?? only last line has data we want in POST ?? */
 	       qp= CMDgetAskline(cmd, CMDnumAsklines(cmd)-1); 

 	       if (!usecgiparams) {
		    while ((hp= strchr(qp,'&'))!=0) *hp= ' '; 
		    while ((hp= strchr(qp,'+'))!=0) *hp= ' ';
		    Fromhexstr(qp,qp);
	       }
	       fputs( qp, retrfile);
 	       putc('\n', retrfile);
 		
	       /* now add env vars from other stuff for http-cgi
                  compliant child apps */
	
	       for (i=0; i < CMDnumAsklines(cmd)-1; i++) {
 	       	    qp= CMDgetAskline(cmd, i);
 	       	    if (strncasecmp( qp, "Content-type:",13) == 0) {
			 hp= qp+13; while(*hp && *hp<=' ') hp++;
			 SetEnvironmentVariable("CONTENT_TYPE", hp);
			 
			 doneCONTENT_TYPE= 1;
		    }
 	       	    else if (strncasecmp( qp, "Content-length:",15) == 0) {
			 hp= qp+15; while(*hp && *hp<=' ') hp++;
			 SetEnvironmentVariable("CONTENT_LENGTH", hp);
		    }
 	       	    else if (strncasecmp( qp, "From:",5) == 0) {
			 SetEnvironmentVariable("FROM", qp+5);
		    }
 	       	    else if (strncasecmp( qp, "Referer:",8) == 0) {
			 SetEnvironmentVariable("REFERER", qp+8);
		    }
 	       	    else if (strncasecmp( qp, "Accept:",7) == 0) {
			 /* !! need multiple env vars or all types on on var?*/
 	       	    	hp= qp+7; while(*hp && *hp<=' ') hp++;
			SetEnvironmentVariable("ACCEPT", hp);
		   }
 	
	       }
	  }
	  fclose(retrfile);
     }
} /* End HandleHTTPpost */

static int
do_command(int sockfd)
{
     char    *view     = NULL;
     char    *Selstr   = NULL;
     CMDobj  *cmd;
     char    *filter   = NULL;
     /* char    *cp; */

     cmd = CMDnew();

     if(LoadTooHigh())
	  Die(sockfd, 
              503, "System is too busy right now. Please try again later.");

     (void) signal(SIGALRM,read_timeout);
     (void) alarm(READTIMEOUT);

     if (setjmp(env)) {
	  LOGGopher(sockfd,"readline: Timed out!");
	  gopherd_exit(-1);
     }
     
     view = NULL;
     filter = NULL;

     ServerSetArgv("Looking up address");

     /*** Find out who's knockin' ***/
     SOCKnetnames(sockfd, CurrentPeerName, CurrentPeerIP);
     
     ServerSetArgv("input from %s", CurrentPeerName);

     CMDfromNet(cmd, sockfd);

     if (cmd == NULL)
	  return(0);

     if (CMDgetUser(cmd) != NULL)
	  CurrentUser = strdup(CMDgetUser(cmd));

     GDCevalDir(Config, CMDgetFile(cmd));
     IsGplus = CMDisGplus(cmd);  /* need for error output.. */

#ifndef NO_AUTHENTICATION

     if (CMDgetTicket(cmd) != NULL) {
	  /** The user has already authenticated, extract from ticket **/
	  char   *tix = CMDgetTicket(cmd);
	  char   *user = CMDgetUser(cmd);
	  char   *cleartext;

	  GDCsetCaching(Config, FALSE);

	  /** decode ticket here... ***/
	  cleartext = (char*) GDESdecrypt(user, CurrentPeerIP,
				  GDCgetPW(Config), tix);
	  
	  if (strncmp(cleartext, user, strlen(user)) == 0) {
	       char *auth, *extra_args;

	       /** Test to make sure that the user is cool (groups, etc..) **/
	       auth = GDCauthType(Config, CMDgetFile(cmd));
	       extra_args = GDCauthExtraArgs(Config, CMDgetFile(cmd));
	       
	       switch (GDCvalidate(Config, auth, user, NULL,
				   CurrentPeerName, CurrentPeerIP, 
				   extra_args)) {
	       case AUTHRES_NOGROUP:
		    GplusError(sockfd, 1, "Your group does not have access to this item", NULL);
		    return(0);
                    /* Unhandled possible cases from the returned enum */
               case AUTHRES_OK:
               case AUTHRES_BADPW:
               case AUTHRES_EXPIRED:
               case AUTHRES_NOUSER:
               case AUTHRES_SYSERR:
	       }

	       Setuid_username(user);

	       Gticket = (char*) malloc(sizeof(char*) *
					(strlen(CMDgetUser(cmd)) +
					 strlen(tix) + 4));
	       sprintf(Gticket, "*%s %s ", CMDgetUser(cmd), tix); /* SAFE */

	  } else {
	       GplusError(sockfd, 1, "Your ticket is invalid", NULL);
	       return(0);
	  }

     } else if ((strncmp(CMDgetSelstr(cmd), "validate ", 9) == 0)||
		(strncmp(CMDgetSelstr(cmd), "halidate ", 9) == 0))  {
	  /** Take the information from the ASK block and validate the user,
	      then, generate tickets **/
	  char *selstr, *auth;
	  int ishtml = 0;

	  GDCsetCaching(Config, FALSE);

	  /** Only look at the ask block if it's there... **/
	  if ((CMDgetCommand(cmd) != NULL && *CMDgetCommand(cmd) != '!') ||
	      ((ishtml = (strncmp(CMDgetSelstr(cmd), "halidate ", 9)) == 0))) {
	       char *extra_args;
	       char *authuser=NULL, *authpw=NULL;

	       selstr = CMDgetSelstr(cmd) + 9;

	       /** Doctor up the item so it looks real.. **/
	       CMDsetSelstr(cmd, selstr);
	       
	       auth = GDCauthType(Config, CMDgetFile(cmd));
	       extra_args = GDCauthExtraArgs(Config, CMDgetFile(cmd));

	       /*** Check if the username was submitted with HTTP ***/
	       if (ishtml) {
		    if (CMDgetSearch(cmd) == NULL) {
			 /** No username/pw submitted, return a blank form.. */
			 OutputAuthForm(sockfd, selstr, 
                                        Zehostname, GopherPort, 
                                        CMDgetProtocol(cmd));
			 return(0);
		    } else {
			 /** Username and password are encoded in
                          *  search string **/
			 char *cp;
			 char *urlenc = strdup(CMDgetSearch(cmd));
			 
			 do {
			      cp = strchr(urlenc, '&');
			      if (cp != NULL)
				   *cp = '\0';

			      Fromhexstr(urlenc, urlenc);
			      if (strncmp(urlenc, "user=",5)==0)
				   authuser = urlenc+5;
			      if (strncmp(urlenc, "pw=",3) == 0)
				   authpw = urlenc+3;
			      urlenc = cp+1;
			 } while (cp != NULL);

			 if (authpw == NULL || authuser == NULL)
			      Die(sockfd, 411, "Missing Username or password");
		    } /* End else */
	       } else {
		    authuser = CMDgetAskline(cmd, 0);
		    authpw   = CMDgetAskline(cmd, 1);
	       }
	       
	       switch (GDCvalidate(Config, auth, authuser, authpw,
				   CurrentPeerName, CurrentPeerIP, 
				   extra_args)) {
		    char *crypted;
		    char cleartext[64];

	       case AUTHRES_OK:

		    Gticket = (char*) malloc(sizeof(char*) *
					     (strlen(authuser) +
					      strlen(authpw)+5));
		    strcpy(cleartext, authuser);
		    strcat(cleartext, " ");
		    strcat(cleartext, authpw);
		    crypted = GDESencrypt(authuser, CurrentPeerIP, 
					  GDCgetPW(Config), cleartext);

		    snprintf(Gticket, (sizeof(char*) *
				      strlen(authuser) +
				      strlen(authpw)+5),
				      "*%s %s ", authuser, crypted);


		    CMDremoveAsk(cmd);
		    ; /** Do the trans... **/
		    break;

	       case AUTHRES_BADPW:
		    if (strlen(authpw) == 0)  {
			 GplusError(sockfd, 1, 
                                    "Please enter a valid password", NULL);
			 return(0);
		    } else {
			 GplusError(sockfd, 1, 
                                    "Your password is incorrect, try again", 
                                    NULL);
			 return(0);
		    } /* End else */

		    break;

	       case AUTHRES_SYSERR:
		    GplusError(sockfd, 1, 
                               "Authentication system failure, please contact administrator", 
                               NULL);
		    return(0);
		    break;
		    
	       case AUTHRES_NOGROUP:
		    GplusError(sockfd, 1, 
                               "Your group does not have access to this item",
                               NULL);
		    return(0);
		    break;
	       default:
		    GplusError(sockfd, 1, 
                               "Your username is incorrect, try again", NULL);
		    return(0);
		    break;
	       }
	  }
	  
     } else {
	  /** Check to see if this item needs authentication, and send
	      a redirect error message **/
	  char *auth = GDCauthType(Config, CMDgetFile(cmd));
	  if (auth != NULL) {
	       /** Need to redirect **/
	       if (CMDgetProtocol(cmd) == HTTP1_0) {
		    writestring(sockfd, 
                                "HTTP/1.0 401 Please Authenticate\r\n");
		    writestring(sockfd, 
                                "WWW-Authenticate: Basic realm=\"this server\"\r\n");
		    writestring(sockfd, "Content-Type: text/html\r\n\r\n");
		    writestring(sockfd, 
                                "You need a client that can use authentication\r\n");
	       } /* End if */
	       Die(sockfd, 411, "Sorry, no access to this item");
	  } /* End if */
     } /* End else */
#endif /* not defined NO_AUTHENTICATION */

     /** At this point there won't be any more data coming in, so shutdown
         the incoming data for the socket
      **/
     shutdown(sockfd, 0);

     /** Change our root directory if it wasn't done before. **/
     if ( dochroot && didchroot == FALSE) {
	  if (chroot(Data_Dir))
	       Die(sockfd, 500, "Data_Dir dissappeared!");
	  
	  uchdir("/");	/* needed after chroot */
	  didchroot = TRUE;
     }

     if (getuid() == 0) {
          /* Drop permissions so we don't end up doing anything nasty... */
	  setgid(Ggid);
	  setuid(Guid);
     }

     EXECflag = 0;
     EXECargs = NULL;
     
     HandleHTTPpost(cmd);

     if (CMDisAskitem(cmd)) {
	  FILE *retrfile;

	  /** Write the stuff out to a file, here, as gopherd user
	      so we can remove file later on.. **/
          /* FIXME: tempnam() is dangerous and silly */
          ASKfile = tempnam(NULL, "gdata");
          Debug("Ask data is in %s\n", ASKfile);
          retrfile = ufopen(ASKfile, "w");
	  if (retrfile != NULL) {
	       int i;

	       for (i=0; i < CMDnumAsklines(cmd); i++) {
		    fputs(CMDgetAskline(cmd, i), retrfile);
                    putc('\n', retrfile);
               }
	       fclose(retrfile);
          } else
	       Die(sockfd, 500, "Cannot write ask data to '%s'", ASKfile);
     }

     /** Extract the view if it exists **/
     if (CMDgetCommand(cmd) != NULL) {
	  char *command = CMDgetCommand(cmd);
	  int  cmdlen = strlen(command);

	  if ((cmdlen > 1) && (*command == '+'))
	       view = command+1;
	  else if (*command == '!') {
	       if (strncmp(CMDgetSelstr(cmd), "exec+", 5) == 0) {
 		/* dgg patch for exec+: -- need to translate this 
		     exec+:params:/go/path/prog<tab>! 
                     into calling program as this
		     /root/go/path/prog -v application/gopher+-menu" param
 		*/
		    filter = command + 1;
		    view = "application/gopher+-menu";
		    CMDsetView(cmd, view);
		    *command = '+';
	       }
	       else {

		    item_info(cmd, sockfd);
		    if (*(command+1) != '\0')
			 filter = command + 1;
		    return(0);
	       }
	  } /* End else if */
	  else if (*command == '$') {
	       if (*(command+1) != '\0')
		    filter = command + 1;
	       view = "application/gopher+-menu";
	       CMDsetView(cmd, view);
	       *command = '+';
	  }
	  else if (*command == '+') {
	       ;
	  }
	  else
	       Die(sockfd, 500, "Malformed command"); /*** Error ***/
     } /* End if */
     
     if (strncmp(CMDgetSelstr(cmd), "waisdocid:",10)==0)
	  view = "Text/plain";

     /*** Root level null selector string.. ***/
     if (!view && strlen(CMDgetSelstr(cmd)) == 0) 
	  view = "application/gopher-menu";


     if (!view && *CMDgetSelstr(cmd) == 'h') {
 	  /** It might be a directory..., or an HTML file on disk **/
 	  struct stat    statbuf;
	  char *selp;

	  selp = CMDgetSelstr(cmd);
	  if (strncmp(selp, "hexec", 5) == 0)
	       CMDsetSelstr(cmd, selp+1);

 	  if (!rstat(CMDgetFile(cmd), &statbuf)) {
 	       if (S_ISDIR(statbuf.st_mode)) {
 		    /*** It's a directory capability ***/
  		    *CMDgetSelstr(cmd) = '1';
 	       }
 	  }
  	  view = "text/html";
     }

     /*** Try to speed things up for gopher0 requests, avoid reading
          big directories.. ***/

     if (!view && CMDisGplus(cmd) == FALSE) {
	  struct stat  statbuf;
	  char         *cp = CMDgetSelstr(cmd);

	  if (*cp == '0' ||
	      *cp == 'R') {
	       if (!rstat(CMDgetFile(cmd), &statbuf))
		    view = "text/plain";
	  }
	  else if (*cp == '1') {
	       if (!rstat(CMDgetFile(cmd), &statbuf))
		    view = "application/gopher-menu";
	  }
     }


     /*** Try to find a view if not supplied ***/
     if (view == NULL) {
	  GopherDirObj *gd;
	  int num,i;

	  /** Get gopher directory containing item in question **/
	  gd = GDfromSelstr(cmd, sockfd);

	  if (gd != NULL) {
	       num = GDSearch(gd, CMDgetSelstr(cmd));
	       if (num >=0) {
		    GopherObj *gs;
	       
		    gs= GDgetEntry(gd, num);

		    if (GSgplusInited(gs) == FALSE) {
			 view = "";
		    }

		    /**  If only one view, take it **/
		    else if (GSgetNumViews(gs) == 1)
			 view = VIgetViewnLang(GSgetView(gs, 0),(char*)malloc(128));
		    else {
			 /*** Hmmm, let's choose one.. ***/
			 for (i=0; i<GSgetNumViews(gs); i++) {
			      char *tmpview;

			      tmpview = VIgetType(GSgetView(gs,i));
			      if (GSgetType(gs) == '0') {
				   if (strcasecmp(tmpview, "Text/plain")==0) {
					view = VIgetViewnLang(GSgetView(gs, i),(char*)malloc(128));
					break;
				   }
			      }
			      if (GSgetType(gs) == 'I') {
				   if (strcmp(tmpview, "image/gif")==0) {
					view = VIgetViewnLang(GSgetView(gs, i),(char*)malloc(128));
					break;
				   }
			      }

			 }
			 if (view == NULL)
			      /** Give up, take the first view... **/
			      view = VIgetViewnLang(GSgetView(gs,0), (char*)malloc(128));
		    }
		    /** We should have a view by now **/
	       }
	       GDdestroy(gd);
	  } else {
	       ;/* Can't get a Gopher directory listing... */;
	  }
     }

     CMDsetView(cmd, view);

     /*
      * Set the environment variables for the scripts
      * shell script writers everywhere..
      */

     SetScriptEnvironment(cmd, view);


     Selstr = CMDgetSelstr(cmd);



     ServerSetArgv("%s to %s", Selstr, CurrentPeerName);

     EXECflag = 0;

     /*  Ask items are shell scripts, not directories.
	 Selector strings with '1/' should never be shell scripts */
	
     if (CMDisAskitem(cmd) && *Selstr == '1')
	  *Selstr = '0';

     /*** With the funky new capability system we can just check the
          first letter(s), end decide what the object refers to. ***/


     Debug("Attempting to get Selstr %s\n", Selstr);



     switch (*Selstr) {


     case '\0':
     case '\t':

	  /*** The null capability, so it's not a file, probably wants
	       to talk to a directory server ***/

	  /*** we'll just do a "list" of the root directory, with no user
	       capability.  ***/


	  listdir(sockfd, "/", CMDisGplus(cmd), view, filter, cmd);
	  LOGGopher(sockfd, "Root Connection");
	  break;

     case 'h':
     case '0':
     case '4':
     case '5':
     case '9':
     case 's':
     case 'I':
     case 'g':
	  /*** It's some kind of file ***/

	  /*** Is it binary??  ***/
	  
	  if (view == NULL)  {
	       Debugmsg("View is null\n");
	       if (*Selstr != '0')
		    send_binary(sockfd, Selstr+1, CMDisGplus(cmd));
	       else
		    printfile(sockfd, Selstr+1, 0, -1, CMDisGplus(cmd));
	  } else {
	       Debug("Testing view %s\n",view);
	       if (GSisText(NULL, view))
		    printfile(sockfd, Selstr+1, 0, -1, CMDisGplus(cmd));
	       else
		    send_binary(sockfd, Selstr+1, CMDisGplus(cmd));
	  }
	  /*** Log it ***/
	  LOGGopher(sockfd, "retrieved file %s", Selstr+1 );
	  break;


     case '1':
	  /*** It's a directory capability ***/
	  listdir(sockfd, Selstr+1, CMDisGplus(cmd), view, filter, cmd);

	  /** Log it **/
	  LOGGopher(sockfd, "retrieved directory %s", Selstr+1);
	  break;

#ifndef NO_INDEXING
     case '7':
	  /*** It's an index capability ***/
	  CheckAccess(sockfd, ACC_SEARCH);
	  if (CMDgetSearch(cmd) == NULL || strlen(CMDgetSearch(cmd)) == 0) {
	       char tmpbuf[256];
	       /** output HTML gunk here... **/
	       snprintf(tmpbuf, sizeof(tmpbuf),
			"<HTML><TITLE>Gopher Index</TITLE>\r\n<ISINDEX>\r\n");
	  }
	       
	  Do_IndexTrans(sockfd, Selstr+1, cmd, TRUE);

	  break;
#endif

     case 'm':
#ifndef NO_INDEXING
	  if (strncmp(Selstr, "mindex:", 7)==0) {
	       CheckAccess(sockfd, ACC_SEARCH);

	       do_mindexd(sockfd, Selstr+7, CMDgetSearch(cmd), CMDisGplus(cmd),
			  view);
	       break;
	  }
#endif

	  /*** This is an internal identifier ***/
	  /*** The m paired with an Objtype of 1 makes a mail spool file
	    into a directory.
	    ***/

	  CheckAccess(sockfd,ACC_BROWSE);
	  process_mailfile(sockfd, Selstr + 1);
	  writestring(sockfd, ".\r\n");

	  /** Log it **/
	  LOGGopher(sockfd, "retrieved maildir %s", Selstr+1 );

	  break;

     case 'R':
	  /*** This is an internal identifier ***/
	  /*** The R defines a range  ****/
	  /*** The format is R<startbyte>-<endbyte>-<filename> **/
     {
	  int startbyte, endbyte;
	  char *cp, *oldcp;

	  CheckAccess(sockfd, ACC_READ);

	  cp = strchr(Selstr+1, '-');
	  
	  if (cp == NULL)
	       Die(sockfd, 400, "Range specifier error for %s", Selstr);
	  
	  *cp = '\0';
	  startbyte = atoi(Selstr+1);
	  oldcp = cp+1;

	  cp = strchr(oldcp, '-');
	  
	  if (cp == NULL)
	       Die(sockfd, 400, "Range specifier error");

	  *cp = '\0';
	  endbyte = atoi(oldcp);
	  oldcp = cp + 1;

	  Debug("Start: %d, ", startbyte);
	  Debug("End: %d, ", endbyte);
	  Debug("File: %s\n", oldcp);

	  printfile(sockfd, oldcp, startbyte, endbyte, CMDisGplus(cmd));

	  /*** Log it ***/
	  LOGGopher(sockfd, "retrieved range %d - %d of file %s", startbyte, endbyte, oldcp);
	  break;
     }

#ifndef NO_FTP
     case 'f':
	  CheckAccess(sockfd, ACC_FTP);

	  if (strncmp(Selstr, "ftp:",4)==0){

	       LOGGopher(sockfd, "retrieved %s", Selstr);

	       GopherFTPgw(sockfd, Selstr+4, cmd);
	       break;
	  }
	  break;
#endif

     case 'e':
	  CheckAccess(sockfd, ACC_BROWSE);

	  if (strncmp(Selstr, "exec:", 5)==0) {
	       /* args are between colons */
	       char *args, *command;
	       
	       command = strrchr(Selstr + 5, ':');
	       if (command == NULL)
		    break;

	       if (*(Selstr+4) == ':' && *(Selstr+5) == ':')
		    args = NULL;
	       else
		    args = Selstr+5;

	       *command = '\0';
	       command++;
	       
	       EXECargs = args;
	       EXECflag = 1;

	       printfile(sockfd, command, 0, -1, CMDisGplus(cmd));
	       LOGGopher(sockfd, "Executed %s %s", command,  (args == NULL)
			 ? " " : args);
	  }
	  break;

#ifdef WAISSEARCH
     case 'w':
     {
	  CheckAccess(sockfd, ACC_SEARCH);

	  if (strncmp(Selstr, "waissrc:", 8) == 0) {
	       char waisfname[512];  /*** Ick this is gross ***/

	       strcpy(waisfname, Selstr+8);
	       if ((int)strlen(waisfname) <= 4 ||
		   strncmp(&waisfname[strlen(waisfname)-4],".src",4) )
		    strcat(waisfname, ".src");
	       SearchRemoteWAIS(sockfd, waisfname, cmd, view);
	       break;
	  }
	  else if (strncmp(Selstr, "waisdocid:", 10) == 0) {
	       Fetchdocid(sockfd, cmd);
	       break;
	  }
     }
#endif

     default:
	  /*** Hmmm, must be an old link... Let's see if it exists ***/

	  switch (isadir(Selstr)) {

	  case -1:
	       /* no such file */
	       Die(sockfd, 404, "'%s' does not exist", Selstr);
	       break;

	  case 0:
	       /* it's a file */
	       send_binary(sockfd, Selstr, CMDisGplus(cmd));
	       
	       /* Log it... */
	       LOGGopher(sockfd, "retrieved file %s", Selstr);

	       break;

	  case 1:
	       /* it's a directory */
	       listdir(sockfd, Selstr, CMDisGplus(cmd), view, filter, cmd);
	       LOGGopher(sockfd, "retrieved directory %s", Selstr);

	       break;
	  }
     }

     /** Free data ***/
     CMDdestroy(cmd);

     return(0);
} /* End do_command */

/*
 * Look for <TITLE> tags...
 */

static void
GSfindHTMLtitle(GopherObj *gs, char *filename)
{
     int fd;
     char buf[512];
     char *titlep;
     
     if ((fd = ropen(filename, O_RDONLY,0)) < 0)
	  return;

     if (read(fd, buf, sizeof(buf)) <0) {
	  return;
     }

     close(fd);

     if ((titlep = strcasestr(buf, "<TITLE>")) != NULL) {
	  char *endtitle;
	  char titletemp[200];

	  titlep += 7;
	  if ((endtitle = strcasestr(titlep, "</TITLE>")) != NULL) {
	       int maxcpysize;
	       maxcpysize = ((endtitle-titlep) > sizeof(titletemp)) ? 
		 sizeof(titletemp) : (endtitle-titlep);
	       strncpy(titletemp, titlep, maxcpysize);
	       titletemp[maxcpysize] = '\0';

	       titlep = titletemp;
	       ZapCRLF(titlep);
	       while (isspace(*titlep))
		    titlep++;
	       
	       GSsetTitle(gs, titlep);
	  }
     }
} /* End GSfindHTMLtitle */


/*
 * This function tries to find out what type of file a pathname is.
 */

static void
Getfiletypes(char *newpath, char *filename, GopherObj *gs)
{
     int Zefilefd;
     char *cp;
     char Zebuf[100];
     char Selstr[512];

     
     switch (isadir(filename)) {
     case -1:
	  GSsetType(gs, '3');
	  return;

     case 1:
	  GSsetType(gs,A_DIRECTORY);
	  *Selstr = '1';
	  strcpy(Selstr +1, newpath);
	  GSsetPath(gs, Selstr);
	  GSsetTTL(gs, GDCgetCachetime(Config));
	  return;

     default:
	  /*** The default is a generic text file ***/
	  GSsetType(gs, A_FILE);

	  *Selstr = '0';
	  strcpy(Selstr + 1, newpath);

	  /*** Test and see if the thing exists... and is readable ***/
	  
	  if ((Zefilefd = ropen(filename, O_RDONLY,0)) < 0) {
	       GSsetType(gs, '3');
	       return;
	  }
	  
	  if (read(Zefilefd, Zebuf, sizeof(Zebuf)) <0) {
	       GSsetType(gs, '3');
	       return;
	  }
	  close(Zefilefd);
	  
	  /*** Check the first few bytes for sound data ***/
	  
	  cp = Zebuf;

	  if (strncmp(cp, ".snd", 4)==0) {
	       GSsetType(gs, A_SOUND);
	       *Selstr = 's';
	       strcpy(Selstr+1, newpath);
	  }

	  /*** Check and see if it's mailbox data ***/
	  
	  else if (is_multipartfile(Zebuf) != SPLIT_UNKNOWN) {
	       GSsetType(gs, A_DIRECTORY);
	       *Selstr = 'm';
	       strcpy(Selstr+1, newpath);
	       GSsetGplus(gs, FALSE);  /** Not yet.. **/
	  }
	  
	  /*** Check for uuencoding data ***/

	  else if (strncmp(cp,"begin",6) == 0)  {
	       GSsetType(gs, '6');
	       *Selstr = '6';
	       strcpy(Selstr+1, newpath);
	  }
	  
	  /*** Check for GIF magic code ***/
	  
	  else if (strncmp(cp, "GIF", 3) == 0) {
	       GSsetType(gs, 'I');
 	       *Selstr = '9';
 	       strcpy(Selstr + 1, newpath);
 	  }

     }
     GSsetPath(gs, Selstr);
} /* End Getfiletypes */

/*
 * Add a default view if none exists..
 */

void
AddDefaultView(GopherObj *gs, int size, char *dirname)
{
     char *lang = GDCgetLang(Config);
     STATSTR statbuf;

     switch (GSgetType(gs)) {
     case A_FILE:
	  GSaddView(gs, "Text/plain", lang, size);
	  break;
     case A_DIRECTORY:
	  if (GDCgetCaching(Config) && dirname != NULL) {
	       strcat(dirname, "/.cache");
	       if (Ustat(dirname, &statbuf) == 0)
		    size = statbuf.st_size;
	       
	       GSaddView(gs, "application/gopher-menu", lang, size);

	       strcat(dirname, "+");
	       if (Ustat(dirname, &statbuf) == 0)
		    size = statbuf.st_size;
	       
	       GSaddView(gs, "application/gopher+-menu", lang, size);
	       GSaddView(gs, "text/html", lang, size);
	  } else {
	       GSaddView(gs, "application/gopher-menu", lang, size);
	       GSaddView(gs, "application/gopher+-menu", lang, size);
	       GSaddView(gs, "text/html", lang, size);
	  }
	       
	  break;
     case A_MACHEX:
	  GSaddView(gs, "application/mac-binhex40", lang, size);
	  break;
     case A_PCBIN:
	  GSaddView(gs, "application/octet-stream", lang, size);
	  break;
     case A_CSO:
	  GSaddView(gs, "application/qi", lang, 0);
	  break;
     case A_INDEX:
	  GSaddView(gs, "application/gopher-menu", lang, size);
	  GSaddView(gs, "application/gopher+-menu", lang, size);
	  break;
     case A_TELNET:
	  break;
     case A_SOUND:
	  GSaddView(gs, "audio/basic", lang, size);
	  break;
     case A_UNIXBIN:
	  GSaddView(gs, "application/octet-stream", lang, size);
	  break;
     case A_GIF:
	  GSaddView(gs, "image/gif", lang, size);
	  break;	
     case A_HTML:
	  GSaddView(gs, "text/html", lang, size);
	  break;
     case A_TN3270:
	  GSaddView(gs, "application/tn3270", lang, 0);
	  break;
     case A_MIME:
	  GSaddView(gs, "multipart/mixed", lang, size);
	  break;
     case A_IMAGE:
	  GSaddView(gs, "image", lang, size);
	  break;
     case A_PDF:
	  GSaddView(gs, "application/pdf", lang, size);
	  break;
     }
} /* End AddDefaultView */

/*
 * Fix a default view
 */

static void
ModifyDefaultView(GopherObj *gs, int viewnum)
{
     VIewobj *viewobj = GSgetView(gs, viewnum);

     switch (GSgetType(gs)) {
     case A_FILE:
 	  VIsetType(viewobj, "Text/plain");
 	  break;
     case A_DIRECTORY:
 	  VIsetType(viewobj, "application/gopher-menu");
 	  break;
     case A_MACHEX:
 	  VIsetType(viewobj, "application/mac-binhex40");
 	  break;
     case A_PCBIN:
 	  VIsetType(viewobj, "application/octet-stream");
 	  break;
     case A_CSO:
 	  VIsetType(viewobj, "application/qi");
 	  break;
     case A_INDEX:
	  VIsetType(viewobj, "application/gopher-menu");
 	  break;
     case A_TELNET:
 	  VIsetType(viewobj, "application/telnet");
 	  break;
     case A_SOUND:
 	  VIsetType(viewobj, "audio/basic");
 	  break;
     case A_UNIXBIN:
 	  VIsetType(viewobj, "application/octet-stream");
 	  break;
     case A_GIF:
 	  VIsetType(viewobj, "image/gif");
 	  break;	
     case A_HTML:
 	  VIsetType(viewobj, "text/html");
 	  break;
     case A_TN3270:
 	  VIsetType(viewobj, "application/tn3270");
 	  break;
     case A_MIME:
 	  VIsetType(viewobj, "multipart/mixed");
 	  break;
     case A_IMAGE:
	  VIsetType(viewobj, "image");
 	  break;
     case A_PDF:
	  VIsetType(viewobj, "application/pdf");
	  break;
     }
} /* End ModifyDefaultView */

static void
GSaddDateNsize(GopherObj *gs, struct stat statbuf)
{
#ifdef ADD_DATE_AND_TIME
     int           fd, i;
     char         longname[256];
     char         *cdate, *ti, *fp, *stitle;
     
     switch (GSgetType(gs)) {
     case '1': /*** It's a directory ***/
     case '7': /*** It's an index ***/
     case 'f': /*** ftp link ***/
     case 'e': /*** exec link ***/
     case 'h': /*** www link ***/
     case 'w': /*** wais link ***/
     case 'm':
	  break;
     default:
     {
	  stitle = GSgetTitle(gs);
	  if (strstr( stitle, "kb]") == 0) {
	       /* Correct for multiple view items */

	       cdate= ctime( &statbuf.st_mtime); /* last mod time */
	       cdate[ 7]= 0; cdate[10]= 0; cdate[24]= 0;
	       snprintf( longname, sizeof(longname),
			 "%s  [%s%s%s, %ukb]", stitle,
		      cdate+8,cdate+4,cdate+22, (statbuf.st_size+1023) / 1024);
	       GSsetTitle(gs,longname);
	  }
     }
	  break;
     }
#endif /* ADD_DATE_AND_TIME */
     ;
} /* End GSaddDateNsize */



/*
 * Add a DL description if it's there ...
 */

static void
GStitlefromDL(GopherObj *gs, char *filename)
{
#ifdef DL
     char               dlpath[2];    /*** for DL**/
     char               *dlout;

     /* Process a "dl" description if there is one! */
     
     dlpath[0] = '.';
     dlpath[1] = '\0';
     dlout = getdesc(NULL,dlpath,filename,0);
     
     Debug("dl: %s", dlpath);
     Debug(" %s", filename);
     Debug(" %s\n", dlout);

     if (dlout != NULL) {
	  GSsetTitle(gs, dlout);
     }
#endif
     ;
} /* End GStitlefromDL */


static void
GSfromCapfile(GopherObj *gs, char *filename)
{
#ifdef CAPFILES
     
     char capfile[MAXPATHLEN];
     FILE *SideFile;
     
     strcpy(capfile,".cap/");
     strcat(capfile, filename);
     
     if ((SideFile = rfopen(capfile, "r"))!=0) {
	  Debug("cap file name: %s\n", capfile);
	  (void) Process_Side(SideFile, gs);
	  fclose (SideFile);
     }
#endif
     ;
} /* End GSfromCapfile */

/*
 * Run scripts that generate blocks...
 */

void
GSrunScripts(GopherObj *gs, char *filename)
{
     FileIO *fio;
     char tmpstr[4096];
     char *bname, *sname;
     int i, blocks;
     
     blocks = GDCnumBlkScripts(Config);

     for (i=0; i < blocks; i++) {
	  bname = GDCgetBlkName(Config, i);
	  sname = GDCgetBlkScript(Config, i);

	  if (GSfindBlock(gs, bname) == NULL) {
	       *tmpstr = '\0';
	       if (!dochroot)
		 snprintf(tmpstr, 256, "%s", Data_Dir);
		    
	       strcat(tmpstr, sname);
	       strcat(tmpstr, " ");
	       strcat(tmpstr, filename);
	       
	       fio = FIOopenCmdline(tmpstr, "r");
	       if (fio == NULL)
		    return;

	       while (FIOreadlinezap(fio, tmpstr,256)) {
		    GSsetBlock(gs, bname, tmpstr, TRUE);
	       }
	       
	       FIOclose(fio);
	  }
     }
} /* End GSrunScripts */

static void
GSsetDefaults(GopherObj *gs)
{
     GSinit(gs);
     GSsetHost(gs, Zehostname);
     GSsetPort(gs, GopherPort);
     GSsetGplus(gs, TRUE);
     GSsetType(gs, '\0');
} /* End GSsetDefaults */

/*
 * Load up a gopher directory from the file system given a directory
 */

GopherDirObj *
GDfromUFS(char *pathname, int sockfd, boolean isGplus)
{
     DIR                *ZeDir;
     char               filename[256];
     char               newpath[512];
     static GopherObj   *Gopherstow = NULL;
     static Extobj      *extstow = NULL;
     Extobj             *ext;
     GopherObj          *gs;
     struct dirent      *dp;
     GopherDirObj       *gd;
     struct stat        statbuf;
     boolean            AddItem = TRUE;
     StrArray           *Linkfiles;
     int                i;


     Debug("GDfromUFS:%s\r\n",pathname);
     Debug("GDfromUFS:Config=%d\r\n",Config);

     /*** Initialize static memory... ****/
     if (Gopherstow == NULL) {
	  Gopherstow = GSnew();
     }

     if (isGplus && GSgplusInited(Gopherstow) == FALSE)
          GSplusnew(Gopherstow);

     if (extstow == NULL)
	  ext = extstow = EXnew();
     else
	  ext = extstow;


     gs = Gopherstow;
     gd = GDnew(32);

     if (rchdir(pathname)<0) {
	  return(NULL);
     }

     if (GDCgetCaching(Config) && 
	 Cachetimedout(".cache+", GDCgetCachetime(Config), ".")==FALSE) {
	  int cachefd; 

	  if ((cachefd = ropen(".cache+", O_RDONLY,0)) >=0) {
	       GDplusfromNet(gd, cachefd, NULL);
	       close(cachefd);
	       return(gd);
	  }
     }

     /* open "." since we just moved there - makes it work when not
	chroot()ing and using relative paths */
     if ((ZeDir = uopendir(".")) == NULL) {

	  Die(sockfd, 404, "Cannot access directory '%s'", pathname); 
	  return(NULL);
     }

     Linkfiles = STAnew(10); if (Linkfiles==NULL) return(NULL);


     for (dp = readdir(ZeDir); dp != NULL; dp = readdir(ZeDir)) {
	  
          strcpy(newpath, pathname);
	  strcpy(filename, dp->d_name);

          if (newpath[strlen(newpath)-1] != '/')
               strcat(newpath, "/");
          strcat(newpath, dp->d_name);

	  gs = Gopherstow;
	  GSinit(gs);

	  /*********************************************************/
	  /*** Ignored file ***/
	  if (strcmp(filename, ".")==0 || strcmp(filename, "..")==0 ||
	      strncmp(filename, ".cache", 6) ==0 || 
	      strcmp(filename, ".about.html") == 0 ||
	      GDCignore(Config,filename))
	       continue;

	  /*********************************************************/
	  /*** This is a link file, process it after other files ***/
	  else if (filename[0] == '.' && isadir(filename)==0) {
	       String *temp;
	       
	       temp = STRnew();
	       STRset(temp, filename);
	       
	       STApush(Linkfiles, temp);
	       STRdestroy(temp);
	       continue;
	  } 

	  /*********************************************************/
	  /*** directory starting with a period ***/
	  else if (filename[0] == '.') {
	       continue;
	  } 

	  /*********************************************************/
	  /*** Gopher+ block file  ***/

	  else if (GDCBlockExtension(Config, filename, ext)) {
	       char Selstr[512];
	       char *tmpstr = Selstr;
	       int num;

	       *Selstr = '0'; strcpy(Selstr+1, newpath);

	       /** Strip off the extension from the path **/
	       tmpstr[strlen(tmpstr) - strlen(EXgetExt(ext))]='\0';

	       num = GDSearch(gd, tmpstr);
	       if (num != -1) {
		    gs = GDgetEntry(gd, num);
		    AddItem = FALSE;
	       } else {
		    GSsetDefaults(gs);
		    GSsetPath(gs, tmpstr);
	       }
	       
	       if (strcasecmp(EXgetBlockname(ext), "ASK") == 0)
		    GSsetAsk(gs, TRUE);

	       GSaddBlock(gs, EXgetBlockname(ext), fixfile(newpath));
	       
	  }

	  /*********************************************************/
	  /*** Some kind of data file                            ***/
	  else {
	       Ustat(filename, &statbuf);

	       /* Strip any Decoder extensions from newpath before processing
		  them, filename needs to remain as is for type checking*/
	  
	       if (EXAcasedSearch(Config->Extensions, ext, filename, 
				  EXT_DECODER)) {
  		    /* Disabled because it's a buggy thing.  The filename[]
		       line was commented out upstream already.
		       newpath[strlen(newpath) - strlen(EXgetExt(ext))] = '\0'; */
/*		    filename[strlen(filename) - strlen(EXgetExt(ext))] = '\0';*/
	       }

	       /*** Add views to item... ***/

	       if (GDCViewExtension(Config, filename, &ext)) {
		    char *Prefix;
		    char  Pathp[512];
		    
		    Prefix = EXgetPrefix(ext);
		    
		    strcpy(Pathp, Prefix);
		    strcpy(Pathp+strlen(Prefix), newpath);
		    

		    /*** search for existing entry to add view to **/
		    /*		    num = GDSearch(gd, Pathp);
		    if (num != -1) {
			 gs = GDgetEntry(gd, num);
			 AddItem = FALSE;
			 } else { JPG */
		    GSsetDefaults(gs);
			 /*		    } */
			 
		    GSsetPath(gs, Pathp);

		    if (GSgetType(gs) == '\0')
			 GSsetType(gs, EXgetObjtype(ext));
		    
		    if (EXgetObjtype(ext) == A_HTML) {
			 GSfindHTMLtitle(gs, filename);
		    }
		    

		    if (GSgetTitle(gs) == NULL) {
			 /*** Strip extension off of title***/
			 filename[strlen(filename)-strlen(EXgetExt(ext))]
			      = '\0';
			 GSsetTitle(gs, filename);
		    }

		    /** Oh say can we hack, by the dawns early day :-) **/
		    if (strcasecmp(EXgetExt(ext), ".mindex")==0) {
			 GSsetGplus(gs, FALSE);
		    }
		    
		    if (isGplus) {
			 char *lang;
			 
			 lang = EXgetVLang(ext);
			 if (lang == NULL || strcmp(lang, "")==0)
			      lang = GDCgetLang(Config);
			 GSaddView(gs, EXgetView(ext), lang, statbuf.st_size);
		    }
	       }
	       /** Mystery file without known extension ***/
	       else {
		    char Selstr[512];
		    int num;
		    
		    strcpy(Selstr+1, newpath);
		    *Selstr = '0';

		    num = GDSearch(gd, Selstr);
		    if (num != -1) {
			 gs = GDgetEntry(gd, num);
			 AddItem = FALSE;
		    } else {
			 GSsetDefaults(gs);
		    }

		    Getfiletypes(newpath ,filename, gs);
		    if (GSgetType(gs) == '3')
			 continue;

		    if (GSgetTitle(gs) == NULL)
			 GSsetTitle(gs, filename);
		    
#ifndef NO_AUTHENTICATION
		    if (strncmp(GSgetPath(gs), "validate ", 9) == 0) {
			 /** Ugly hack for now...  **/
			 char **askb = GDCauthAsk(Config, GSgetPath(gs)+10);
			 int i;
			 
			 GSsetAsk(gs, TRUE);
			 
			 for (i=0; askb[i] != NULL; i++) {
			      GSsetBlock(gs, "ASK", askb[i], TRUE);
			 }
		    }
#endif
		    if (GSisGplus(gs) && isGplus)
			 AddDefaultView(gs, statbuf.st_size, NULL);
	       }

	       /** Check DL database for a good name**/
	       GStitlefromDL(gs, filename);

	       if (GSgetTitle(gs) == NULL)
		    GSsetTitle(gs, filename);
	  
	       GSaddDateNsize(gs, statbuf);

	       GSfromCapfile(gs, dp->d_name);
	       if (GSgetType(gs) == '3' || GSgetType(gs) == '-')
		    continue;
	  }

	  /*** Here we go, we have either a filled in block or data item ***/

	  if (isGplus && AddItem) {
	       char tmpstr[256];
	       char timeval[16];
	       struct tm *tmthing;
	       char *cp;
	       
	       if (GSgplusInited(gs) == FALSE)
		    GSplusnew(gs);
	       
	       /*** Add admin, abstract entries, etal ***/
	       if (!GSgetAdmin(gs)) {
		    snprintf(tmpstr, sizeof(tmpstr), "%s <%s>", 
			    GDCgetAdmin(Config), GDCgetAdminEmail(Config));
		    GSsetAdmin(gs, tmpstr);
	       }

	       if (GSgetModDate(gs) == NULL) {
		    /** Set mod date entry **/
		    tmthing = localtime(&(statbuf.st_mtime));
		    strftime(timeval,sizeof(timeval), "%Y%m%d%H%M%S", tmthing);
		    snprintf(tmpstr, sizeof(tmpstr), 
			     "%s<%s>", asctime(tmthing),timeval);
		    cp = strchr(tmpstr, '\n');
		    if (cp != NULL)
			 *cp = ' ';
		    GSsetModDate(gs, tmpstr);
	       }	       

	  }

	  
	  /*** Add the entry to the directory ***/
	  if (AddItem) {
	       GDaddGS(gd, gs);
	  } else
	       AddItem = TRUE;
	  
     }

     /*** We don't have links yet, so we can process the BlockScripts ***/
     if (isGplus) {
	  char *cp;

	  for (i=0; i < GDgetNumitems(gd); i++) {
	       gs = GDgetEntry(gd, i);
	       cp = strrchr(GSgetPath(gs), '/');
	       if (cp != NULL) 
		    GSrunScripts(gs, cp+1);
	  }
     }

     /** Process .Link files **/

     for (i=0 ; i<STAgetTop(Linkfiles); i++) {
	  FileIO *fio;

	  fio = FIOopenUFS(STAgetText(Linkfiles,i), O_RDONLY, 0);
     
	  if (fio != NULL) {
	       GDfromLink(gd, fio, Zehostname, GopherPort, pathname, CurrentPeerName);
	       FIOclose(fio);
	  }
     }

     closedir(ZeDir);

     for (i=0; i< GDgetNumitems(gd); i++) {
	  char *tmpview;
	  gs = GDgetEntry(gd, i);
	  if (GSgetType(gs) == '0')
	       continue;
	  if (GSisGplus(gs) && GSgetNumViews(gs) == 1) {
	       if ((tmpview = VIgetType(GSgetView(gs,0))) &&
		   !strcmp(tmpview, ""))
		    ModifyDefaultView(gs,0);
	  }
     }
     
     GDsort(gd);
     STAdestroy(Linkfiles);

     return(gd);
     
} /* End GDfromUFS */

/* Misleading title - its actually loading a GD from the parent of cmd */

static GopherDirObj *
GDfromSelstr(CMDobj *cmd, int sockfd)
{
     char *it = NULL;
     char *cp;
     GopherDirObj *gd;
     char directory[512];

     it = CMDgetFile(cmd);

     if (it == NULL)
	  return(NULL);
     else
	  strcpy(directory, it);

     cp = strrchr(directory, '/');
     if (cp != NULL)
	  *(cp+1) = '\0';
     
     if (rchdir(directory)<0) {
	  if (isadir(directory)==0)
	       return(NULL);

	  Die(sockfd, 404,"- Cannot access directory '%s'", directory);
     }
     
     gd = GDfromUFS(directory, sockfd, TRUE); /** Returns NULL if error **/
     
     return(gd);
} /* End GDfromSelstr */

/*
 * Send item information to client
 */
static void
item_info(CMDobj *cmd, int sockfd)
{
     GopherDirObj *gd = NULL;
     GopherObj *gs = NULL;
     int num;
     struct stat statbuf;
     char tmpstr[256];
     char *cp;

     cp = CMDgetSelstr(cmd);
     num = strlen(cp);

#ifndef NO_FTP
     /* If the selstr starts with ftp: hand it off to the gateway code */
     if (strncmp(cp, "ftp:", 4) == 0) {
	  GopherFTPgw(sockfd, cp+4, cmd);
	  return;
     }
#endif

     /* If the selstr ends in '/', strip it off. */
     if ( num != 0 && *(cp + num - 1) == '/' )
 	  *(cp + num - 1) = '\0';

     /** For now, strip off first character and find the directory above **/

     Debug("Item info for %s\n", cp);
     LOGGopher(sockfd, "Item info requested for %s\n", cp);

     if ((*cp == '/') || (*cp == '\0') || (*(cp+1) == '\0')) {
	  gs = GSnew();
	  GSsetDefaults(gs);
	  GSsetPath(gs, "");
	  GSsetTitle(gs, GDCgetSite(Config));
	  GSsetType(gs, '1');
	  GSplusnew(gs);

	  GSsendHeader(sockfd, -1);
	  writestring(sockfd, "+INFO ");
	  GStoNet(gs,sockfd, GSFORM_G0, Gticket);

	  snprintf(tmpstr, sizeof(tmpstr),
		   "+ADMIN:\r\n Admin: %s <%s>\r\n", 
		   GDCgetAdmin(Config),
		   GDCgetAdminEmail(Config));
	  writestring(sockfd, tmpstr);

	  if (GSgetModDate(gs) == NULL) {
	       if (rstat("/", &statbuf) == 0) {
		    char timeval[16];
		    struct tm *tmthing;

		    tmthing = localtime(&(statbuf.st_mtime));
		    strftime(timeval,sizeof(timeval), "%Y%m%d%H%M%S", tmthing);
		    snprintf(tmpstr, sizeof(tmpstr),
			     " Mod-Date: %s<%s>\r\n",
			    asctime(tmthing),timeval);
		    cp = strchr(tmpstr, '\n');
		    if (cp != NULL)
			 *cp = ' ';
		    writestring(sockfd, tmpstr);
	       }
	  } else {
	       snprintf(tmpstr, sizeof(tmpstr),
			" Mod-Date: %s\r\n", GSgetModDate(gs));
	       writestring(sockfd, tmpstr);
	  }

	  if (GSgetTTL(gs) > -1) {
	       snprintf(tmpstr, sizeof(tmpstr), " TTL: %d\r\n", GSgetTTL(gs));
	  } else {
	       snprintf(tmpstr, sizeof(tmpstr), 
			" TTL: %d\r\n", GDCgetCachetime(Config));
	  }

	  writestring(sockfd, tmpstr);

	  snprintf(tmpstr, sizeof(tmpstr), 
		   " Site: %s\r\n", GDCgetSite(Config));
	  writestring(sockfd, tmpstr);
	  snprintf(tmpstr, sizeof(tmpstr),
		   " Org: %s\r\n", GDCgetOrg(Config));
	  writestring(sockfd, tmpstr);
	  snprintf(tmpstr, sizeof(tmpstr),
		   " Loc: %s\r\n", GDCgetLoc(Config));
	  writestring(sockfd, tmpstr);
	  snprintf(tmpstr, sizeof(tmpstr),
		   " Geog: %s\r\n", GDCgetGeog(Config));
	  writestring(sockfd, tmpstr);
	  snprintf(tmpstr, sizeof(tmpstr),
		  " Version: U of Minnesota Unix %s.%s pl%d\r\n",
		  GOPHER_MAJOR_VERSION, GOPHER_MINOR_VERSION, PATCHLEVEL);
	  writestring(sockfd, tmpstr);

	  if (Connections != 0) {
	       time_t howlong  = time(NULL) - ServerStarted + 1;
	       int connperhour = (Connections * 3600) / howlong;
	       char *started = ctime(&ServerStarted);
	       
	       *(started+24) = '\0';
	       snprintf(tmpstr, sizeof(tmpstr),
			"+STATISTICS:\r\n Total Connections: %ld\r\n Server Started: %s\r\n Connections per Hour: %d\r\n Concurrent Sessions: %d\r\n", 
		       Connections, started, connperhour, ActiveSessions);
	       writestring(sockfd, tmpstr);
	       
	  }


	  if (GDCgetAbstract(Config) != NULL) {
	       char abline[256];
	       char *cp, *nl;

	       writestring(sockfd, "+ABSTRACT:\r\n");
	       nl = cp = GDCgetAbstract(Config);
	       while (nl) {
		    nl = strchr(cp, '\n');
		    writestring (sockfd, " ");
		    if (nl == NULL)
			 writestring(sockfd, cp);
		    else {
			 strncpy(abline, cp, (nl-cp));
			 abline[nl-cp] = '\0';
			 writestring(sockfd, abline);
			 cp = nl+1;
		    }
		    writestring(sockfd, "\r\n");
	       }

	  }

	  writestring(sockfd, "+VERONICA:\r\n treewalk:");
	  if (GDCgetShouldIndex(Config) == TRUE)
	       writestring(sockfd, " yes");
	  else
	       writestring(sockfd, " no");

	  writestring(sockfd, "\r\n+VIEWS:\r\n");
	  if (GDCgetCaching(Config)) {
	       if (rstat("/.cache", &statbuf) == 0) {
		    snprintf(tmpstr, sizeof(tmpstr),
			     " application/gopher-menu %s: <%ldk>\r\n",
			    GDCgetLang(Config), 
			    (long) ((statbuf.st_size + 512)/1024));
		    writestring(sockfd, tmpstr);
	       } else {
		    writestring(sockfd, " application/gopher-menu: <0k>\r\n");
	       }

	       if (rstat("/.cache+", &statbuf) == 0) {
		    snprintf(tmpstr, sizeof(tmpstr),
			     " application/gopher+-menu %s: <%ldk>\r\n",
			    GDCgetLang(Config), 
			    (long) ((statbuf.st_size + 512)/1024));
		    writestring(sockfd, tmpstr);
	       } else {
		    writestring(sockfd, " application/gopher+-menu: <0k>\r\n");
	       }

	  } else {
	       writestring(sockfd, " application/gopher-menu: <0k>\r\n");
	       writestring(sockfd, " application/gopher+-menu: <0k>\r\n");
	  }

	  writestring(sockfd, " text/html: <0k>\r\n");
	  writestring(sockfd, " Directory/recursive: <0k>\r\n");
	  writestring(sockfd, " Directory+/recursive: <0k>\r\n");

	  GSdestroy(gs);
     } 
#ifndef NO_AUTHENTICATION
     else if (strncmp(CMDgetSelstr(cmd), "validate ",9) == 0) {

	  gd = GDfromSelstr(cmd,sockfd);
	  
	  num = GDSearch(gd, CMDgetSelstr(cmd)+9);

	  uchdir("/");

	  if (num < 0 || gd == NULL) {
	       GplusError(sockfd, 1, "Cannot find item information for that item", NULL);
	       
	       return;
	  }
	  else {
	       /* Find the authenticator associated with the directory/item **/
	       char *selstr = CMDgetSelstr(cmd);
	       char **askb = GDCauthAsk(Config, CMDgetFile(cmd));
	       int i;

	       gs = GDgetEntry(gd, num);
	       GSsetPath(gs, selstr);
	       GSsetAsk(gs, TRUE);

	       if (askb) {
		    Debug("Askblock is %s\n", askb[0]);
		    for (i=0; askb[i] != NULL; i++) {
			 GSsetBlock(gs, "ASK", askb[i], TRUE);
		    }
	       }

	       DebugGSplusPrint(gs,"got here...");


	       GSsendHeader(sockfd, -1);
	       GSplustoNet(gs, sockfd, NULL, "");
	  }
	  
     } 
#endif /* NO_AUTHENTICATION */

     else {

	  gd = GDfromSelstr(cmd,sockfd);

	  num = GDSearch(gd, CMDgetSelstr(cmd));
	  uchdir("/");

	  if (num < 0 || gd == NULL) {
	       GplusError(sockfd, 1, "Cannot find item information for that item", NULL);
	       return;
	  }
	  else {
	       gs = GDgetEntry(gd, num);
	       GSsendHeader(sockfd, -1);
	       GSplustoNet(gs, sockfd, NULL, Gticket);
	  }

     }
     if (gd)
	  GDdestroy(gd);

     writestring(sockfd, ".\r\n");
} /* End item_info */

static void
GDhtmlfooter(int sockfd)
{
     STATSTR statbuf;
     char *localfooter = ".footer.html";
     char *globalfooter = "/lib/globalfooter.html";
     
     if (Ustat(localfooter, &statbuf) == 0)
	  send_binary(sockfd, localfooter, 0);
     else if (rstat(globalfooter, &statbuf)==0)
	  send_binary(sockfd, globalfooter, 0);
} /* End GDhtmlfooter */

/** Output a header.. **/

static boolean
GDhtmlheader(int sockfd)
{
     STATSTR statbuf;
     char *localheader = ".header.html";
     char *aboutheader = ".about.html"; /* For compatibility urg.. */
     char *globalheader = "/lib/globalheader.html";
     
     if (Ustat(localheader, &statbuf) == 0) {
	  send_binary(sockfd, localheader, 0);
	  return(TRUE);
     } else if (Ustat(aboutheader, &statbuf)==0) {
	  send_binary(sockfd, aboutheader, 0);
	  return(TRUE);
     } else if (rstat(globalheader, &statbuf)==0) {
	  send_binary(sockfd, globalheader, 0);
	  return(TRUE);
     }
     return(FALSE);
}

/*
 * Output HTML for cutesy icons
 */

void
GSicontoNet(GopherObj *gs, int  sockfd)
{
     char iconfile[64];
     int fd;
     char *img = NULL;

     if (gs==NULL)
	  return;

     snprintf(iconfile, sizeof(iconfile), "/lib/htmlicon.%c", GSgetType(gs));

     switch (GSgetType(gs)) {
     case A_FILE: 
     case A_MIME:
     case A_HTML:  img= "text"; break;

     case A_DIRECTORY: img= "menu"; break;
     case A_GIF:
     case A_IMAGE: img= "image"; break;
     case A_MOVIE: img= "movie"; break;
     case A_INDEX: img= "index"; break;
     case A_SOUND: img= "sound"; break;
	  
     case A_TN3270: img= "3270"; break;
     case A_TELNET: img= "telnet"; break;
	  
     case A_MACHEX:
     case A_PCBIN:
     case A_UNIXBIN: img= "binary"; break;
	  
     case A_INFO: img = NULL; break;
     case A_CSO: img = "phone"; break;
     default:
	  img= NULL;
	  break;
     }
     
     if ((fd = ropen(iconfile, O_RDONLY,0)) >= 0) {
	  char imgline[256];
	  strcpy(imgline, "<IMG ALIGN=bottom BORDER=0 ");

	  if (img) {
	       snprintf(imgline + strlen(imgline), 
			sizeof(imgline) - strlen(imgline) - 1,
			"ALT=\"[%-6s]\" ", img);
	  }
	  snprintf(imgline + strlen(imgline),
		   sizeof(imgline) - strlen(imgline) - 1,
		   "SRC=\"gopher://%s:%d/I9%s\">",
		   Zehostname, GopherPort, iconfile);
	  writestring(sockfd, imgline);
	  close(fd);
     }
}



/*
** This function lists out what is in a particular directory.
** it also outputs the contents of link files.
**
** It also checks for the existance of a .cache file if caching is
** turned on...
**
** Ack is this ugly.
*/

static void
listdir(int sockfd, char *pathname, boolean isgplus, char *view, char *filter,
	CMDobj *cmd)
{
     GopherDirObj *gd = NULL;
     boolean      attrlist = FALSE;
     boolean      HTMLit   = FALSE;
     boolean      Recurse  = FALSE;
     char         *filtereddata[16], **filtered=filtereddata;
     int          i=0;
     StrArray     *RecurseDirs = NULL;
     String       *stapath;

     CheckAccess(sockfd, ACC_BROWSE);

     if (uchdir("/"))
	  perror("Problems changing directory to /");

     if (filter != NULL) {
	  while (*filter != '\0') {
	       if (*filter=='+') {
		    *filter = '\0';
		    filtered[i] = filter+1;
		    filter++; i++;
	       }
	       filter++;
	  }
	  filtered[i] = NULL;
     } else
	  filtered = NULL;


     if (view != NULL) {
	  if (strncmp(view, "application/gopher+-menu",24) == 0)
	       attrlist = TRUE;
	  else if (strncmp(view, "text/html", 9) == 0) {
	       attrlist = TRUE;
	       HTMLit   = TRUE;
	  }
	  else if (strncmp(view, "Directory+/recursive", 20)==0)
	       Recurse = TRUE;
	  else if (strncmp(view, "Directory/recursive", 19)==0)
	       Recurse = TRUE;
     }
     if (Recurse)
	  RecurseDirs = STAnew(32);

     if (rchdir(pathname)<0)
	  Die(sockfd, 404,"- Cannot access directory '%s'", pathname);

     if (isgplus)
	  GSsendHeader(sockfd, -1);


     do {
	  Debug("Sending %s\n", pathname);

	  rchdir("/");
	  rchdir(pathname);
	  
	  if (!attrlist && GDCgetCaching(Config) && !HTMLit &&
	     (Cachetimedout(".cache", GDCgetCachetime(Config), ".") ==FALSE)) {
	       /*** Old style cache  ***/
	       send_binary(sockfd, ".cache", FALSE);

	  } else if (GDCgetCaching(Config) && !HTMLit &&
		     Cachetimedout(".cache+", GDCgetCachetime(Config)
				   , ".")==FALSE) {
	       /*** Gopher+ cache. ***/
	       
	       if (strncmp(view, "application/gopher+-menu",24)==0)
		    send_binary(sockfd, ".cache+", FALSE);
	       else if (strncmp(view, "application/gopher-menu",23)==0)
		    send_binary(sockfd, ".cache", FALSE);
	       
	  } else {
	       /** If we didn't cache then we have to 
		 load up the directory **/
	       gd = GDfromUFS(pathname, sockfd, attrlist);
	       
	       if (gd == NULL) {
		    /** Should generate an error message here **/
		    break;
	       }

	       if (HTMLit) {
		    if (CMDgetProtocol(cmd) == HTTP1_0) {
			 OkHTTPresponse(sockfd);

		    }
		    writestring(sockfd, "<HTML>\r\n<HEAD>\r\n");
		    writestring(sockfd, "<LINK rev=made href=\"mailto:");
		    writestring(sockfd, GDCgetAdminEmail(Config));
		    writestring(sockfd, "\">\r\n");
		    
		    if (GDgetTitle(gd) != NULL) {
			 writestring(sockfd, "<TITLE>");
			 writestring(sockfd, GDgetTitle(gd));
			 writestring(sockfd, "</TITLE>");
		    }
		    writestring(sockfd, "</HEAD>\r\n<BODY>\r\n");
		    
		    if (!GDhtmlheader(sockfd)) {
			 /* Top level, for sites with no headers at all */
			 if (strcmp(pathname, "/") == 0) {
			      writestring(sockfd, "<H1>");
			      writestring(sockfd, GDCgetSite(Config));
			      writestring(sockfd, "</H1>\r\n<H2>");
			      writestring(sockfd, GDCgetOrg(Config));
			      writestring(sockfd, "</H2>\r\n");
			 }
		    }
		    
		    GDtoNet(gd, sockfd, GSFORM_HTML, Gticket, &GSicontoNet);
		    /** Do the footer here **/
		    GDhtmlfooter(sockfd);
		    writestring(sockfd, "</BODY>\r\n</HTML>\r\n");
	       }
	       else if (attrlist)
		    GDplustoNet(gd, sockfd,filtered, Gticket);
	       else
		    GDtoNet(gd, sockfd, GSFORM_G0, Gticket, NULL);

	  }
	  /*
	   * Write out the cache... After we send out the data to the net.
	   */
	  if (GDCgetCaching(Config) && gd != NULL) {
	       int cachefd;
	       char cachefile[MAXPATHLEN];
	       
	       strcpy(cachefile, pathname);
	       strcat(cachefile, "/.cache");
	       
	       cachefd = ropen(cachefile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
	       
	       if (cachefd >= 0) {
		    Debug("Caching directory... into %s\n",cachefile);
		    GDtoNet(gd, cachefd, GSFORM_G0, NULL, NULL);
		    close(cachefd);
	       }
	       
	       if (attrlist) {
		    strcat(cachefile, "+");
			 
		    cachefd = ropen(cachefile, O_WRONLY|O_CREAT|O_TRUNC,0644);
		    
		    if (cachefd >= 0) {
			 GDplustoNet(gd, cachefd,filtered, "");
			 close(cachefd);
		    }
	       }
	  }
	  
	  if (Recurse) {
	       GopherObj *gs;
	       String    *pushstring = STRnew();

	       /** Push entries on the stack **/
	       for (i=0; i< GDgetNumitems(gd); i++) {
		    STATSTR stbuf;
		    char    *cp;

		    gs = GDgetEntry(gd, i);

		    if ((GSgetType(gs) == A_DIRECTORY) &&
			(strcmp(GSgetHost(gs), Zehostname) == 0)) {
			 STRset(pushstring,  GSgetPath(gs));

			 rchdir("/");

			 cp = STRget(pushstring);
			 if (rstat(cp+1, &stbuf) == 0)
			      STApush(RecurseDirs, pushstring);
		    }
	       }

	       do {
		    stapath = STApop(RecurseDirs);
		    if (stapath == NULL) {
			 Recurse = FALSE;  /** Done **/
			 break;
		    }
		    pathname = STRget(stapath);
		    
		    if (*pathname == 'm')
			 process_mailfile(sockfd, pathname+1);
	       } while (*pathname == 'm');

	       pathname++;

	       if (gd != NULL) {
		    GDdestroy(gd);
		    gd = NULL;
	       }
	       STRdestroy(pushstring);
	  }
     } while (Recurse);

     if (!HTMLit)
	  writestring(sockfd, ".\r\n");

     if (gd != NULL) {
          GDdestroy(gd);
          gd = NULL;
     }
}


/*
 * This processes a file containing any subset of
 * Type, Name, Path, Port or Host, and returns pointers to the
 * overriding data that it finds.
 *
 * The caller may choose to initialise the pointers - so we don't
 * touch them unless we find an over-ride.
 */

int
Process_Side(FILE *sidefile, GopherObj *Gopherp)
{
     char inputline[MAXLINE];
     char *cp;
     int  retval = TRUE;

     inputline[0] = '\0';

     for (;;) {
	  for (;;) {
	       cp = fgets(inputline, 1024, sidefile);
	       if (inputline[0] != '#' || cp == NULL)
		    break;
	  }
	  
	  /*** Test for EOF ***/
	  if (cp==NULL)
	       break;
	  
	  ZapCRLF(inputline);  /* should zap tabs as well! */

	  /*** Test for the various field values. **/
	  
	  if (strncmp(inputline, "Type=", 5)==0) {
	       GSsetType(Gopherp, inputline[5]);
	       if (inputline[5] == '7') {
		    /*** Might as well set the path too... ***/
		    cp = GSgetPath(Gopherp);
		    *cp = '7';
	       } else if (inputline[5] == '9') {
		    /*** Might as well set the path too... ***/
		    cp = GSgetPath(Gopherp);
		    *cp = '9';
	       } else if (inputline[5] == '-')
		    retval = FALSE;
	  }

	  else if (strncmp(inputline, "Name=", 5)==0) {
	       GSsetTitle(Gopherp, inputline+5);
	  }

	  else if (strncmp(inputline, "Host=", 5)==0) {
	       GSsetHost(Gopherp, inputline+5);
	  }

	  else if (strncmp(inputline, "Port=", 5)==0) {
	       GSsetPort(Gopherp, atoi(inputline+5));
	  }

	  else if (strncmp(inputline, "Path=", 5)==0) {
	       GSsetPath(Gopherp, inputline+5);
	  }

	  else if (strncmp(inputline, "Numb=", 5)==0) {
	       GSsetNum(Gopherp, atoi(inputline+5));
	  }

	  else if (strncmp(inputline, "Name=", 5)==0) {
	       GSsetTitle(Gopherp, inputline+5);
	  }

	  else if (strncmp(inputline, "Abstract=", 9)==0) {
	       GSsetAbstract(Gopherp, inputline+9);
	  }

	  else if (strncmp(inputline, "Admin=", 9)==0) {
	       GSsetAdmin(Gopherp, inputline+6);
	  }

     }

     return(retval);
}

/* dgg++   test if file is ASK script */
boolean
IsAskfile(char *pathname)
{
     struct stat statbuf;
     int retval = 0;
     /* char tmpfile[512]; */
     char *tmpfile = (char *) malloc(strlen(pathname) + 5 * 
                                     sizeof(char *));
     
#if 0
     strcpy(tmpfile, pathname);
     strcat(tmpfile, ".ask"); /* hack -- should do fancy lookup in Config */
#endif /* 0 */
     sprintf(tmpfile, "%s.ask", pathname);
     
     retval = rstat(tmpfile, &statbuf);
     free(tmpfile);
     
     if(!retval) return TRUE;
     else        return FALSE;
} /* End IsAskfile() */

/*
** This function opens the specified file, starts a zcat if needed,
** and barfs the file across the socket.
**
** It now also checks and sees if access is allowed
**
**  This also used the global variable ASKfile
**
*/

/* This function represents from start to finish what gopherd sends out on the
 * wire when the client requests a regular (text) file
 */
static void
printfile(int sockfd, char *pathname, int startbyte, int endbyte, boolean Gplus)
{
     FILE        *ZeFile;
     char         inputline[MAXPATHLEN+4];
     FILE        *pp;
     static char *writebuf = NULL;
     static char *bufptr   = NULL;
     int          len;
     /* If dontbuffer is false, data is sent line by line, otherwise in
      * large chunks
      */
     boolean      dontbuffer = FALSE;
     struct stat  buf;

     /* Protocol note:  this will be sent to the client as the reported 
      * length of the data we are sending them.  -1 means that the server 
      * doesn't know how much data it's going to send, so it's a good default.
      * -2 is the same as -1 but that the data may contain "\r\n.\r\n" which
      * would suck if the client didn't know about it.  But since we take 
      * pains below to remove '.' chars on lines by themselves, we dont' have
      * to worry about that.
      */
     long filesize = -1;
     /*** Check and see if the peer has permissions to read files ***/

     CheckAccess(sockfd, ACC_READ);

     Debugmsg("starting printfile");

     /* dgg++  patch to block ASK calls from bad clients */
     if (IsAskfile(pathname)) {
	  if (!Gplus || !ASKfile) {
	       char errmsg[256];
	       if (!ASKfile) {
		    ;;;; /*** HTMLize form here... ****/
		    snprintf(errmsg, sizeof(errmsg),
			     " Missing input data to ASK form\t\t\t\r\n");
	       }
	       else
		    Die(sockfd, 400, " This ASK form needs a gopher+ client\t\t\t\r\n");
          }
     }

     if ( (ZeFile = rfopen(pathname, "r")) == NULL) {
	  /*
	   * The specified file does not exist
	   */

	  Die(sockfd, 404, "'%s' does not exist!!", pathname);

     } 

     if (writebuf == NULL) {
	  writebuf = (char *) malloc(4096 * sizeof(char));
	  bufptr   = writebuf;
     }

     if (startbyte != 0)
	  fseek(ZeFile, startbyte, 0);
     
     /* CHANGEME:  Isn't reopening this file a bit silly? */
     if ((pp = Specialfile(sockfd, ZeFile, pathname))!=NULL) {
	  fclose(ZeFile);
	  Debugmsg("This is a special file\n");

          /* This may be a pipe or something, we don't know how much data
           * is going to come of it, so make sure we tell the client so.
           */
          filesize = -1;
	  ZeFile = pp;
	  dontbuffer = TRUE;
     } else {
          /* It isn't a special file.  Figure out how large it is. */
	  rstat(pathname, &buf);
	  filesize = buf.st_size;
     } /* End else */

     if (Gplus) { 
          /* This reports to the client how many bytes we are going to send
           * it.  (filesize bytes if filesize > 0)
           */
          if (endbyte > 0 && (startbyte - endbyte) > 0) { 
               GSsendHeader(sockfd, (long)(startbyte - endbyte));
          } else if(endbyte > 0) {
               /* Hmm...this is odd... */
               GSsendHeader(sockfd, -1);
          } else {
               GSsendHeader(sockfd, filesize);
          } /* End else */
     } /* End if */

     while (fgets(inputline, MAXPATHLEN, ZeFile) != NULL) {
	  len = strlen(inputline);

	  if (dontbuffer) {
	       alarm(WRITETIMEOUT);
	       writestring(sockfd, inputline);
	       alarm(0);
	  } else {
	       if (((bufptr-writebuf) + len+1) > 4096) {
		    /** Write out the buffer if it's too big to fit.. **/
		    (void) alarm(WRITETIMEOUT);
		    if (writestring(sockfd, writebuf))
			 LOGGopher(sockfd, "Client went away"), gopherd_exit(-1);
		    (void) alarm(0);
		    bufptr=writebuf;
	       }
	       
	       strcpy(bufptr, inputline);
	       bufptr += len;
	       *bufptr = '\0';
	  }
	  if (endbyte > 0) {
               /* FIXME:  We may be reading in large chunks, and we may be
                * reading line by line.  Either way, we are very unlinkely
                * to send just the write number of bytes
                */
	       if (ftell(ZeFile) >= endbyte)
		    break;
	  }
     }

     len = Specialclose(ZeFile);
     if (len)
	  Debug("Specialclose returned %d\n", len);

     if (bufptr != writebuf) {
	  alarm(WRITETIMEOUT);
	  writestring(sockfd, writebuf);
	  alarm(0);
     }
}


#define BUFSIZE 8192  

/* This function represents from start to finish what gopherd sends out on the
 * wire when the client requests a binary file.  
 */
static void
send_binary(int sockfd, char *filename, boolean isGplus)
{
     FILE           *sndfile,*pp;
     unsigned char  in[BUFSIZE];
     register int   j;
     int            gotbytes, size;
     struct stat    buf;
     int            isSpecial = 0;  /* Whether or not file is special or opened
                                     * via popen() 
                                     */

     Debug("Sending %s, binary way\n", filename);
     /** Don't check decoder/extension if .cache **/
     if (strncmp(filename, ".cache",6)!=0) {
	  CheckAccess(sockfd, ACC_READ);

	  if (strcmp(filename, "-") == 0) {
	       /*** Do some live digitization!! **/
	       sndfile = popen("record -", "r");
               isSpecial = 1;
	  }
	  else {
	       sndfile = rfopen(filename, "r");
               isSpecial = 0;
          }
	  
	  if (!sndfile) {
	       /*
		* The specified file does not exist
		*/

	       Die(sockfd, 404, "'%s' does not exist!!", filename);
	  }

	  if ((pp = Specialfile(sockfd, sndfile, filename)) != NULL) {
	       fclose(sndfile);
	       sndfile = pp;
               isSpecial = 1;
	  }
     } else {
	  sndfile = rfopen(filename, "r");

	  if (!sndfile)
	       Die(sockfd, 404, "'%s' does not exist!!", filename);

          isSpecial = 0;
     } /* End else */

     /* We have to send -2 as the filesize if it's a special file.  This gets
      * around an extremely annoying bug where a file gets run through a 
      * decoder, (say, zcat for foobar.txt.gz) and even though 
      * we report foobar.txt.gz as 20134 bytes in length, we send the actual
      * uncompressed file.  (foobar.txt, which is much bigger).  Not positive
      * about all client implementations on whether or not they will stop 
      * reading, but it's better to nip this out.
      */
     if(isGplus) {
          if ((strcmp(filename, "-") == 0) || isSpecial) 
               GSsendHeader(sockfd, -2);
          else {
               rstat(filename, &buf);
               size = buf.st_size;
               GSsendHeader(sockfd, size);
          } /* End else */
     } /* End if */

     while(1) {
	  gotbytes = fread(in, 1, BUFSIZE, sndfile);
	  
	  if (gotbytes == 0) { 
               if(ftell(sndfile) == -1 && feof(sndfile)) { 
                    /* Highly likely that the server is misconfigured and we're
                     * actually trying to run a script that isn't working 
                     * because we chroot()'d to a location where utilities
                     * referenced in the script aren't present.
                     * Example: chroot("/var/gopher") and then try to execute
                     * this script: 
                     * #!/bin/sh
                     * echo "oops"
                     * yields these conditions.
                     */
                    ;
               }

               if(ferror(sndfile)) {
                    /* This won't happen if it's just regular EOF */
                    LOGGopher(sockfd, "send_binary(%s): error reading data",
                              filename);
               } /* End if */
               
	       break;       /*** end of file or error... ***/
          }

	  (void) alarm(WRITETIMEOUT);  /** In case client is hung.. **/
          j = writen(sockfd, in, gotbytes);
	  (void) alarm(0);

	  Debug("Wrote %d binary bytes\n",j);
	  if (j <= 0) {
               LOGGopher(sockfd, "send_binary(%s): short byte count on write",
                         filename);
	       break;       /*** yep another error condition ***/
          }

     } /* End while */

     Specialclose(sndfile);
} /* End send_binary */
