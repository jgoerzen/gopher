/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.7 $
 * $Date: 2001/01/19 00:32:10 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/serverutil.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: serverutil.c
 * utilities for the server.
 *********************************************************************
 * Revision History:
 * $Log: serverutil.c,v $
 * Revision 1.7  2001/01/19 00:32:10  s2mdalle
 * Added mktmpdir() - returns a character array with the name of a
 * created directory (0700) and NULL on error.  This has not yet been
 * integrated but is intended to be used with tempnam() calls elsewhere.
 *
 * Revision 1.6  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.5  2001/01/17 19:30:25  jgoerzen
 * Change many sprintf -> snprintf
 *
 * Revision 1.4  2001/01/17 18:17:16  jgoerzen
 * More vsprintf fixes.
 *
 * Revision 1.3  2001/01/17 18:13:21  jgoerzen
 * Added mp3 MIME type.  Add logging of chroot and setuid/setgid status.
 *
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.52  1996/01/04  18:30:14  lindner
 * Fix for Ustat on Linux, autoconf changes
 *
 * Revision 3.51  1995/11/06  20:53:40  lindner
 * aix, no setproctitle
 *
 * Revision 3.50  1995/11/03  18:02:10  lindner
 * Small null check
 *
 * Revision 3.49  1995/10/04  21:04:55  lindner
 * NO_AUTH mod
 *
 * Revision 3.48  1995/09/28  22:58:53  lindner
 * Minor fixes
 *
 * Revision 3.47  1995/09/28  21:11:31  lindner
 * More fixes...
 *
 * Revision 3.46  1995/09/26  05:12:45  lindner
 * Add include...
 *
 * Revision 3.45  1995/09/26  04:59:48  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.44  1995/09/25  22:09:51  lindner
 * Remove static buf..
 *
 * Revision 3.43  1995/09/25  05:02:38  lindner
 * Convert to ANSI C
 *
 * Revision 3.42  1995/05/01  05:42:15  lindner
 * compatibility fixes
 *
 * Revision 3.41  1995/05/01  04:00:47  lindner
 * Fixes for mindexd, etc from Steve Hsieh
 *
 * Revision 3.40  1995/02/25  20:49:05  lindner
 * More timeouts..
 *
 * Revision 3.39  1995/02/11  06:22:49  lindner
 * Fix for setvbuf parameters
 *
 * Revision 3.38  1995/02/07  19:34:08  lindner
 * A little more readable
 *
 * Revision 3.37  1995/02/07  08:37:37  lindner
 * Rewrite of mailfile/multifile parsing
 *
 * Revision 3.36  1995/02/07  07:03:45  lindner
 * performance fixes
 *
 * Revision 3.35  1995/02/02  17:13:54  lindner
 * Fix memory leaks
 *
 * Revision 3.34  1994/12/31  07:46:45  lindner
 * Make shell script output line buffered for slow scripts
 *
 * Revision 3.33  1994/12/20  17:20:56  lindner
 * Put environment variable setter here..
 *
 * Revision 3.32  1994/12/06  20:56:41  lindner
 * Fix for multi-line bummer messages
 *
 * Revision 3.31  1994/11/29  05:00:09  lindner
 * Fix for exec:
 *
 * Revision 3.30  1994/11/16  18:53:35  lindner
 * Allow multi-line Bummer messages
 *
 * Revision 3.29  1994/10/13  05:17:50  lindner
 * Compiler complaint fixes
 *
 * Revision 3.28  1994/09/29  19:57:09  lindner
 * Make Grep Index queries work
 *
 * Revision 3.27  1994/07/21  15:46:49  lindner
 * New multipart file code
 *
 * Revision 3.26  1994/07/03  21:18:04  lindner
 * Add initgroup() call
 *
 * Revision 3.25  1994/06/29  05:36:54  lindner
 * Add username logging
 *
 * Revision 3.24  1994/04/22  03:35:50  lindner
 * for sunos 4.0.3
 *
 * Revision 3.23  1994/03/31  22:45:50  lindner
 * Generate gopher- error responses for gopher- clients
 *
 * Revision 3.22  1994/03/31  21:10:30  lindner
 * Don't need Setuid_username unless using UMNDES
 *
 * Revision 3.21  1994/03/08  15:56:09  lindner
 * gcc -Wall fixes
 *
 * Revision 3.20  1994/03/04  23:25:38  lindner
 * faster isadir()
 *
 * Revision 3.19  1994/01/21  04:01:10  lindner
 * Add support for flock(), fix LOGGopher() function declaration
 *
 * Revision 3.18  1993/11/05  07:25:44  lindner
 * futzing with stdarg lines
 *
 * Revision 3.17  1993/10/11  04:40:54  lindner
 * Changes to allow logging via daemon.info syslogd facility
 *
 * Revision 3.16  1993/09/30  23:16:50  lindner
 * Hack out SETPROCTITLE on systems that can't hack it
 *
 * Revision 3.15  1993/09/30  17:01:18  lindner
 * Remove unnessesary logging
 *
 * Revision 3.14  1993/09/20  16:54:00  lindner
 * Remove dead code, moved to Sockets.c
 *
 * Revision 3.13  1993/08/10  20:28:10  lindner
 * return true for non-existant cache file
 *
 * Revision 3.12  1993/08/06  14:30:47  lindner
 * Fixes for better security logging
 *
 * Revision 3.11  1993/08/05  20:46:36  lindner
 * Fix for Gpopen for single quotes and !
 *
 * Revision 3.10  1993/08/04  22:14:51  lindner
 * Mods to use Gpopen
 *
 * Revision 3.9  1993/08/04  22:12:48  lindner
 * Mods to use Gpopen
 *
 * Revision 3.8  1993/07/27  05:27:56  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.7  1993/07/20  23:57:29  lindner
 * Added LOGGopher here, added routine to set proctitle
 *
 * Revision 3.6  1993/07/07  19:34:53  lindner
 * fixed typo in GplusError
 *
 * Revision 3.5  1993/06/22  07:07:14  lindner
 * prettyfication
 *
 * Revision 3.4  1993/04/10  06:07:40  lindner
 * More debug msgs
 *
 * Revision 3.3  1993/04/09  15:12:09  lindner
 * Better error checking on getpeername()
 *
 * Revision 3.2  1993/03/24  20:28:55  lindner
 * Moved some code from gopherd.c and changed error message delivery
 *
 * Revision 3.1.1.1  1993/02/11  18:02:53  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.2  1993/02/09  22:16:24  lindner
 * Mods for gopher+ error results.
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/



#include "gopherd.h"
#include "serverutil.h"
#include "Debug.h"
#include "Dirent.h"		/* For S_ISADIR */
#include "gopherdconf.h"	/* For GDCaccess */
#include "Regex.h"

#if defined(HAVE_SYSLOG_H)
#include <syslog.h>
#else
#define syslog(a,b)	fprintf(stderr, "%s\n", (b))
#define openlog(a,b,c)	fprintf(stderr, "%s\n", (a))
#endif

#include <stdarg.h>		/* For ellipsis routines ... */

/* Try using flock() if fcntl() won't let us lock. */
#if !defined(USE_FLOCK) && !defined(F_SETLKW)
#define USE_FLOCK
#endif

#ifdef USE_FLOCK
#include <sys/file.h>
#endif

#include <grp.h>

#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */

/* Makes a temporary directory in /tmp and returns the name of the created 
 * directory.  (Character array, not allocated memory) 
 * On error, returns NULL.  
 */
char *mktmpdir(void) {
     char tmpdirname[128];
     int prog_pid = getpid();
     int orig_pid = prog_pid;
     struct stat statbuf;
     int CHKDIR_LOOP_LIMIT = 50;   /* Check this many possible directory names.
                                    * if we can't find one in this many tries,
                                    * return failure
                                    */
     
     sprintf(tmpdirname, "/tmp/gopherd.%d", getpid());
     
     while(!rstat(tmpdirname, statbuf)) { 
          if((orig_pid + CHKDIR_LOOP_LIMIT) < prog_pid)
               break;

          prog_pid++;

          /* Add an underscore - not really necessary, just to signify that
           * the number is not a pid, but a number chosen to avoid conflict
           * with an existing file
           */
          sprintf(tmpdirname, "/tmp/gopherd._%d", prog_pid);
     } /* End while */

     if(!mkdir(tmpdirname, 0700)) 
          return &tmpdirname[0];
     else { 
          /* What happened? */
          LOGGopher(-1, "Couldn't create directory at \"%s\": %d", 
                    tmpdirname, errno);
          return((char *)NULL);
     }
} /* End mktmpdir() */

/*
 * This finds the current peer and the time and  jams it into the
 * logfile (if any) and adds the message at the end
 */
void LOGGopher(int sockfd, const char *fmt, ...)
{
     va_list args;
     char message[512];
     time_t          Now;
     char            *cp;
                     /* cp + ' ' + host_name + ' : ' + MAXLINE + '\n' + '\0' */
     char            buf[286+MAXLINE];
     char            userandhost[256];
#ifndef USE_FLOCK
     struct flock    lock;
#endif

     va_start(args, fmt);
     
     (void) vsnprintf(message, sizeof(message), fmt, args);

     if (LOGFileDesc == -1) {
	  return;
     }

     if (LOGFileDesc == -2) {
	  /** Syslog case.. **/
	  syslog(LOG_INFO, "%s : %s", CurrentPeerName, message);
	  return;
     }
     /** File case ***/
  
#ifdef USE_FLOCK
     flock(LOGFileDesc, LOCK_EX);
#else
     lock.l_type = F_WRLCK;
     lock.l_whence = SEEK_SET;
     lock.l_start = 0L;
     lock.l_len = 0L;
     fcntl(LOGFileDesc, F_SETLKW, &lock);
#endif

     time(&Now);         /* Include this in the lock to make sure */
     cp = ctime(&Now);   /*  log entries are chronological */
     ZapCRLF(cp);
     
     /* someone else may have written to the file since we opened it */
     lseek(LOGFileDesc, 0L, SEEK_END);

     userandhost[0] = '\0';
     if (CurrentUser != NULL) {
         snprintf(userandhost, sizeof(userandhost), "%s@%s", CurrentUser,
		  CurrentPeerName);
     } else
          snprintf(userandhost, sizeof(userandhost), "%s", CurrentPeerName);

     snprintf(buf, sizeof(buf),
	      "%s %d %s : %s\n", cp, getpid(), userandhost, message);
     write(LOGFileDesc, buf, strlen(buf));
     
     /* unlock the file */
#ifdef USE_FLOCK
     flock(LOGFileDesc, LOCK_UN);
#else
     lock.l_type = F_UNLCK;
     fcntl(LOGFileDesc, F_SETLKW, &lock);
#endif

     Debug("%s", buf);
     
}


/*
 * Gopher + error mechanism
 *
 * errclass is the type of error
 * text is the first line of error text,
 * moretext is a char array of yet more text to send
 */

void
GplusError(int sockfd, int errclass, char *text, char **moretext)
{
     char outputline[256];
     int i;
     char *tempstr = NULL;
     char *temptext[256];
     
     if (text == NULL) 
	  text = "Unspecified error";

     if ((moretext == NULL) && (strchr(text, '\n') != NULL)) {
	  char *nl;
	  int i = 0;

	  tempstr = strdup(text);
	  nl = tempstr;
	  while ((nl = strchr(nl, '\n'))) {
	       *nl = '\0';
	       nl++;
	       
	       temptext[i++] = nl;
	  }
	  temptext[i] = NULL;

	  moretext = temptext;
	  text     = tempstr;
     }


     if (!IsGplus)
	  snprintf(outputline, sizeof(outputline),
		   "0%s\t\terror.host\t1\r\n", text);
     else
	  snprintf(outputline, sizeof(outputline),
		   "--1\r\n%d %s <%s>\r\n%s\r\n", 
		   errclass, GDCgetAdmin(Config),
		   GDCgetAdminEmail(Config), text);

     (void) alarm(WRITETIMEOUT);
     if (writestring(sockfd, outputline)<0) {
	  LOGGopher(sockfd, "Client went away!");
	  exit(-1);
     }
     (void) alarm(0);

     if (moretext != NULL)
	  for (i=0; moretext[i] != NULL; i++) {
	       if (!IsGplus) {
		    snprintf(outputline, sizeof(outputline),
			     "0%s\t\terror.host\t1\r\n", 
			     moretext[i]);
		    writestring(sockfd, outputline);
	       } else  {
		    (void) alarm(WRITETIMEOUT);
		    writestring(sockfd, moretext[i]);
		    writestring(sockfd, "\n");
		    (void) alarm(0);
	       }
	  }
     (void) alarm(WRITETIMEOUT);
     writestring(sockfd, ".\r\n");
     (void) alarm(0);

     close(sockfd);

     return;
}


/* 
 * This routine cleans up an open file descriptor and sends out a bogus
 * filename with the error message
 */

void
Abortoutput(int sockfd, char *errmsg)
{
     GplusError(sockfd, 1, errmsg, NULL);

}

void
Die(int sockfd, int errorlevel, char *fmt, ...)
{
     va_list args;
     char message[512];
     
     va_start(args, fmt);

     vsnprintf(message, sizeof(message), fmt, args);
     GplusError(sockfd, 1, message, NULL);
     gopherd_exit(-1);
}

void
Warn(int sockfd, int errorlevel, char *fmt, ...)
{
     va_list args;
     char message[512];
     
     va_start(args, fmt);

     vsnprintf(message, sizeof(message), fmt, args);
     GplusError(sockfd, 1, message, NULL);
}


/*
 * only works if non-chroot really...
 */

boolean
Setuid_username(char *username)
{
     struct passwd *pw;

     if (getuid() != 0)
	  return(FALSE);

     pw = getpwnam(username);
     
     if (!pw) {
	  Debug("Couldn't find user '%s'\n", username);

	  return(FALSE);
     }

     if (pw->pw_uid == 0)   /* Don't allow this to be used for root access*/
	  return(FALSE);

     if (initgroups(pw->pw_name, pw->pw_gid) != 0 )
          return(FALSE);

     if (setgid(pw->pw_gid) < 0)
	  return(FALSE);

     if (setuid(pw->pw_uid) < 0)
	  return(FALSE);

     Debug("Successfully changed user privs to '%s'\n", username);

     return(TRUE);
}

/*
 * is_mail_from_line - Is this a legal unix mail "From " line?
 *
 * Given a line of input will check to see if it matches the standard
 * unix mail "from " header format. Returns 0 if it does and <0 if not.
 *
 * 2 - Very strict, also checks that each field contains a legal value.
 *
 * Assumptions: Not having the definitive unix mailbox reference I have
 * assumed that unix mailbox headers follow this format:
 *
 * From <person> <date> <garbage>
 *
 * Where <person> is the address of the sender, being an ordinary
 * string with no white space imbedded in it, and <date> is the date of
 * posting, in ctime(3C) format.
 *
 * This would, on the face of it, seem valid. I (Bernd) have yet to find a
 * unix mailbox header which doesn't follow this format.
 *
 * From: Bernd Wechner (bernd@bhpcpd.kembla.oz.au)
 * Obfuscated by: KFS (as usual)
 */

#define MAX_FIELDS 10

static char legal_day[]         = "SunMonTueWedThuFriSat";
static char legal_month[]       = "JanFebMarAprMayJunJulAugSepOctNovDec";
static int  legal_numbers[]     = { 1, 31, 0, 23, 0, 59, 0, 60, 1969, 2199 };

int is_mail_from_line(char *line)
{
     char *fields[MAX_FIELDS];
     char *sender_tail;
     register char *lp, **fp;
     register int n, i;
     
     if (strncmp(line, "From ", 5)) return -100;
     
     lp = line + 5;
     /* sender day mon dd hh:mm:ss year */
     for (n = 0, fp = fields; n < MAX_FIELDS; n++) {
	  while (*lp && *lp != '\n' && isascii(*lp) && isspace(*lp)) lp++;
	  if (*lp == '\0' || *lp == '\n') break;
	  *fp++ = lp;
	  while (*lp && isascii(*lp) && !isspace(*lp))
	       if (*lp++ == ':' && (n == 4 || n == 5)) break;
	  if (n == 0) sender_tail = lp;
     }
     
     if (n < 7) return -200-n;
     
     fp = fields;
     
     if (n > 8 && !isdigit(fp[7][0])) fp[7] = fp[8]; /* ... TZ year */
     if (n > 9 && !isdigit(fp[7][0])) fp[7] = fp[9]; /* ... TZ DST year */
     
     fp++;
     for (i = 0; i < 21; i += 3)
	  if (strncmp(*fp, &legal_day[i], 3) == 0) break;
     if (i == 21) return -1;
     
     fp++;
     for (i = 0; i < 36; i += 3)
	  if (strncmp(*fp, &legal_month[i], 3) == 0) break;
     if (i == 36) return -2;
     
     for (i = 0; i < 10; i += 2) {
	  lp = *++fp;
	  if (!isdigit(*lp)) return -20-i;
	  n = atoi(lp);
	  if (n < legal_numbers[i] || legal_numbers[i+1] < n) return -10-i;
     }
     return 0;
}


Splittype
is_multipartfile(char *line)
{
     int success, i;
     int numitems = GDCgetNumFileSep(Config);

     success = is_mail_from_line(line);
     if (success == 0)
	  return(SPLIT_MAIL);

     for (i=0; i< numitems; i++) {
	  re_comp(GDCgetFileSep(Config, i));
	  if (re_exec(line))
	       return(i);
     }
     
     return(SPLIT_UNKNOWN);
}


#define NO_SUBJECT "<no subject>"

void
process_mailfile(int sockfd, char *Mailfname)
{
     FILE       *Mailfile;
     char       Zeline[MAXLINE];
     char       outputline[MAXLINE];
     char       Title[MAXLINE];
     long       Startbyte=0, Endbyte=0, Bytecount=0;
     boolean    foundtitle = 0;
     char       *p;
     Splittype  septype = SPLIT_UNKNOWN;

     Debug("process_mailfile %s\n",Mailfname);

     Mailfile = rfopen(Mailfname, "r");

     if ((Mailfile == NULL) || (fgets(Zeline, MAXLINE, Mailfile) == NULL)) {
	  Die(sockfd, 404, "Cannot access file '%s'", Mailfname);
	  return;
     }

     septype = is_multipartfile(Zeline);
     Bytecount += strlen(Zeline);

     /** Read through the file, finding sections **/
     while (fgets(Zeline, MAXLINE, Mailfile) != NULL) {
	  if (!foundtitle) {
	       if (septype==SPLIT_MAIL) {
		    if (strncmp(Zeline,"Subject: ",9)==0) {
			 foundtitle = TRUE;
			 /* trim out the white space.. */
			 p = Zeline + 8;
			 while ((*p == ' ')||(*p == '\t'))
			      p++;
			 if (*p == '\n')
			      strcpy(Title, NO_SUBJECT);
			 else
			      strcpy(Title,p);
			 
			 ZapCRLF(Title);
			 Debug("Found title %s\n", Title);
		    } else if (strcmp(Zeline, "\n")==0) {
			 foundtitle = TRUE;
			 strcpy(Title, NO_SUBJECT);
			 Debugmsg("No subject found - using default\n");
		    }
	       } else {
		    /** Not SPLIT_MAIL **/
		    strcpy(Title, Zeline);
		    ZapCRLF(Title);
		    foundtitle = TRUE;
	       }
	  }
	  else if (is_multipartfile(Zeline) == septype) {
	       Endbyte = Bytecount;
	       foundtitle = FALSE;

	       if (Endbyte != 0) {
		    snprintf(outputline, sizeof(outputline),
			     "0%s\tR%ld-%ld-%s\t%s\t%d\r\n", 
			     Title, Startbyte, Bytecount, Mailfname,
			     Zehostname, GopherPort);
		    if (writestring(sockfd, outputline) < 0)
			 gopherd_exit(-1);
		    Startbyte=Bytecount;
		    *Title = '\0';
	       }
	  }

	  Bytecount += strlen(Zeline);
     }

     if (*Title != '\0') {
	  snprintf(outputline, sizeof(outputline),
		   "0%s\tR%ld-%ld-%s\t%s\t%d\r\n", 
		   Title, Startbyte, Bytecount, Mailfname, 
		   Zehostname, GopherPort);
	  if (writestring(sockfd, outputline)<0)
	    gopherd_exit(-1);
     }	  

}





/* 
 * Return the basename of a filename string, i.e. everything
 * after the last "/" character. 
 */

char *mtm_basename(char *string)
{
     char *buff;
     buff = string + strlen(string); /* start at last char */
     while (*buff != '/' && buff > string)
	  buff--;
     return( (char *) (*buff == '/'? ++buff : buff));
}



/*
 * Cache timeout value.
 *   If cache is less than secs seconds old, it's ok.
 *   If cache is newest, it's ok, otherwise it must be rebuilt.
 * 
 */

boolean
Cachetimedout(char *cache, int secs, char *dir)
{
     STATSTR       buf;
     int           result;
     time_t        now;

     result = rstat(cache, &buf);

     if (result != 0)
	  return(TRUE);

     time(&now);
     
     Debug("Cache now: %d, ", now);
     Debug("cache file: %d\n", buf.st_mtime);
     
     if ( now < (buf.st_mtime + secs))
	  return(FALSE);
     else
	  return(TRUE);

}

/*
 * Returns true (1) for a directory
 *         false (0) for a file
 *         -1 for anything else
 */

boolean
isadir(char *path)
{
     static STATSTR buf;
     int result;

     result = rstat(path, &buf);

     if (result != 0)
	  return(-1);
     
     if (S_ISDIR(buf.st_mode)) {
	  char *cp = fixfile(path);
	  if (! access(cp, F_OK))
	       return(1);
	  else
	       return(-1);
     }
     else if (S_ISREG(buf.st_mode))
	  return(0);
     else
	  return(-1);
}

/*
 * This function sets the process title given by ps on a lot of BSDish
 * systems
 */

void
ServerSetArgv(const char *fmt, ...)
{
#if defined(SETPROCTITLE) && !defined(__sgi) && !defined(_AUX_SOURCE) && !defined(sgi) && !defined(USG) && !defined(_AIX)

     va_list args;

     register char *p;
     register int i;
     char buf[MAXLINE];



     Argv[1] = NULL;

     va_start(args, fmt);
     (void) vsnprintf(buf, sizeof(buf), fmt, args);
     
     va_end(args);
     
     /* make ps print "(gopherd)" */
     p = Argv[0];
     *p++ = '-';
     
     i = strlen(buf);
     if (i > LastArgv - p - 2)
     {
	  i = LastArgv - p - 2;
	  buf[i] = '\0';
     }
     (void) strcpy(p, buf);
     p += i;
     while (p < LastArgv)
	  *p++ = ' ';
#endif /* SETPROCTITLE */
     ;
}

FILE*
Gpopen(int sockfd, char *cmd, char *rw)
{
     int inquote = 0;
     int insquote = 0;
     int i;
     FILE *theproc;

     /** Strip out the naughty bits..  **/
     for (i=0; cmd[i] != '\0'; i++) {
	  switch (cmd[i]) {
	  case '"':
	       if (!insquote)
		    inquote = 1-inquote;
	       break;
	       
	  case '\'':
	       if (!inquote)
		    insquote = 1-insquote;
	       break;

	  case '&':
	  case '|':
	  case ';':
	  case '=':
	  case '?':
	  case '>':
	  case '(':
	  case ')':
	  case '{':
	  case '}':
	  case '[':
	  case ']':
	  case '^':
	       /*** Stuff that's okay if quoted.. ***/

	       if (!inquote && !insquote) {
		    LOGGopher(sockfd, "Possible Security Violation '%s'", cmd);
		    return(NULL);
	       }
	       
	       break;

	  case '!':
	  case '\\':
	  case '`':
	  case '\n':
	  case '$':
	       /*** Stuff that shouldn't be in there at all! **/

	       LOGGopher(sockfd, "Possible Security Violation '%s'", cmd);
	       return(NULL);

	       break;
	  }
     }

     theproc = popen(cmd, rw);
     if (theproc != NULL)
	  setvbuf(theproc, NULL, _IOLBF, 0);

     return(theproc);
}

/*
 * Set an environment variable 
 */

void
SetEnvironmentVariable(char *variable, char *value)
{
     char *tmpputenv;
     
     if (variable == NULL)
	  return;

     if (value == NULL)
	  value = "";

     tmpputenv = (char*) malloc(strlen(variable) + strlen(value) + 2);

     if (!tmpputenv)
	  return;

     /* SAFE as sprintf, but what the heck. -- jgoerzen */

     snprintf(tmpputenv, (strlen(variable) + strlen(value) + 2), 
	      "%s=%s", variable, value);
     putenv(tmpputenv);
     free(tmpputenv);
}

#ifndef NO_AUTHENTICATION
void
CheckAccess(int sockfd, Accesslevel testfor)
{
     AccessResult result;

     result = GDCaccess(Config, CurrentPeerName, CurrentPeerIP,
			     ActiveSessions, testfor);
     
     if (result == SITE_NOACCESS) {
	  Die(sockfd, 411, GDCgetBummerMsg(Config));
     } else if (result == SITE_TOOBUSY) {
	  Die(sockfd, 503, "Sorry, too busy now...");
     }
}

#endif
