/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.3 $
 * $Date: 2002/01/08 21:35:49 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/conf.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: conf.h
 * More configuration parameters.
 *********************************************************************
 * Revision History:
 * $Log: conf.h,v $
 * Revision 1.3  2002/01/08 21:35:49  jgoerzen
 * Many changes:
 *  Revved the version number
 *  updated greeting
 *  updated copyright notices
 *
 * Revision 1.2  2000/12/20 01:19:15  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.2  2000/12/18 11:05:07  mdallen
 * Removed ~50 instances of nested comments for commented out #define's
 * 
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.23  1995/02/25  06:45:33  lindner
 * Add write time out
 *
 * Revision 3.22  1995/01/04  18:35:18  lindner
 * Added Danish as a default, more hlp and hlp_secure comments
 *
 * Revision 3.21  1994/12/07  23:54:20  lindner
 * mods for default language behavior on VMS
 *
 * Revision 3.20  1994/08/19  17:01:44  lindner
 * Fixes from Alan
 *
 * Revision 3.19  1994/07/18  19:04:40  lindner
 * Features from Alan and JL for Language Definitions and Gripes
 *
 * Revision 3.18  1994/06/09  04:06:28  lindner
 * F.Macrides 27-May-1994 Added option to allow 'd'elete only for
 * bookmarks via a DELETE_BOOKMARKS_ONLY compilation symbol.
 *
 * Added option to not read maps from the user rc file (i.e., only from
 * the system rc file) in SecureMode or NoShellMode, via the compilation
 * symbol SECURE_MAPS_GLOBALRC_ONLY.
 *
 * Added info about the NOMAIL compilation symbol in the VMS section.
 *
 * Put back -force_html %s for the lynx command in the VMS section
 * (really *is* needed with the current text/html code when foo.html
 * files are supplied by a gopher+ server from it's own data tree; other
 * Web browsers don't have that switch, and won't work right with gopher+
 * servers until they can use the gopher+ extra stuff to determine the
 * mime type.
 *
 * Revision 3.17  1994/05/11  02:48:16  lindner
 * fix for VMS gopherprint defines
 *
 * Revision 3.16  1994/04/14  17:03:02  lindner
 * fix for html command
 *
 * Revision 3.15  1994/01/20  06:43:51  lindner
 * text/html viewer support for lynx 2.1 &  CERN's www-linemode client
 *
 * Revision 3.14  1994/01/14  16:24:22  lindner
 * Added anonymous ftp type 'f' option
 *
 * Revision 3.13  1993/11/03  03:36:35  lindner
 * Mod for variable records
 *
 * Revision 3.12  1993/10/13  16:46:51  lindner
 * Updates for %s on defaults, vms mods
 *
 * Revision 3.11  1993/09/22  04:30:31  lindner
 * Add option to conf.h for Max WAIS documents
 *
 * Revision 3.10  1993/09/11  07:08:50  lindner
 * Mucho stuff for VMS, callable HTML stuff
 *
 * Revision 3.9  1993/08/28  04:59:03  lindner
 * Moved GLOBALRC definition to conf.h for VMS
 *
 * Revision 3.8  1993/08/19  20:32:59  lindner
 * add default remoterc, change read timeout to 1 minute
 *
 * Revision 3.7  1993/08/12  06:35:08  lindner
 * Don't override CONF_FILE definition, use mail instead of /bin/mail for VMS
 *
 * Revision 3.6  1993/08/04  22:07:42  lindner
 * Use /bin/mail instead of ucbmail
 *
 * Revision 3.5  1993/07/27  05:35:30  lindner
 * reading material for VMS, dead code removal
 *
 * Revision 3.4  1993/04/15  22:08:51  lindner
 * Remote user mods (Mitra)
 *
 * Revision 3.3  1993/03/18  23:11:16  lindner
 * 1.2b3 release
 *
 * Revision 3.2  1993/02/19  21:25:03  lindner
 * Updated pager command for gopher+ stuff.
 *
 * Revision 3.1.1.1  1993/02/11  18:02:49  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.7  1993/02/09  22:49:34  lindner
 * Fixes for new mapping thing
 *
 * Revision 1.6  1993/01/08  23:04:48  lindner
 * Changed TN3270_COMMAND for Multinet
 *
 * Revision 1.5  1992/12/31  05:32:43  lindner
 * Added mods for VMS
 *
 * Revision 1.4  1992/12/22  21:45:26  lindner
 * Fixed bug with that zcat code I just added...
 *
 * Revision 1.3  1992/12/21  20:27:25  lindner
 * Added #ifdef to make zcat changable..
 *
 * Revision 1.2  1992/12/13  05:56:32  lindner
 * Added options for connection time-out code in the server (mtm)
 *
 * Revision 1.1  1992/12/11  19:01:58  lindner
 * Gopher1.1 Release
 *
 *********************************************************************/

/*
 * Defaults for the client program
 * On startup the client will contact either the gopher server
 * CLIENT1_HOST or CLIENT2_HOST randomly.
 *
 * Set CLIENT2_PORT to 0 if you only want one root machine
 */

#define CLIENT1_HOST "gopherproject.org"
#define CLIENT1_PATH "/Software/Gopher"
#define CLIENT1_PORT 70

#define CLIENT2_HOST "nonexistant"
#define CLIENT2_PATH "nonexistant"
#define CLIENT2_PORT 0

/*
define CLIENT1_HOST "gopher.tc.umn.edu"
define CLIENT2_HOST "gopher2.tc.umn.edu"

define CLIENT1_PORT 70
define CLIENT2_PORT  70
*/

/*
 * Defaults for the client's Gopher server aFTP gateway.
 *
 * On command 'f' the client will prompt for an aFTP Host
 *  and create a directory for it via the gateway.
 * You can optionally enter a selector for a particular
 *  directory on the aFTP Host, and the gateway will return
 *  that instead of the Host's root directory.
 *
 * Set AFTP_HOST to your Gopher server with the gateway implemented
 *
 * Set AFTP_PORT to its port number
 */

#define AFTP_HOST "gopher-gw.micro.umn.edu"

#define AFTP_PORT 70

/*
 * Default language for client when multiple language views are available
 *
 * (This is a gopher+ language code - see object/VIews.c for more examples)
 */
#define DEFAULT_LANG	"En_US"	    /* English (US) */
/* #define DEFAULT_LANG	"Da_DK"	    Danish */
/* #define DEFAULT_LANG	"De_DE"	    German */
/* #define DEFAULT_LANG	"En_GB"	    English (UK) */
/* #define DEFAULT_LANG	"Es_ES"	    Spanish */
/* #define DEFAULT_LANG	"Fr_FR"	    French */
/* #define DEFAULT_LANG "It_IT"	    Italian */
/* #define DEFAULT_LANG	"Jp_JP"	    Japanese */
/* #define DEFAULT_LANG "No_NO"	    Norwegian */
/* #define DEFAULT_LANG	"Sv_SE"	    Swedish */


/*
 * Define this if you want the 'd'elete command restricted to bookmarks
 */
/* #define DELETE_BOOKMARKS_ONLY */


/*
 * Define this if you want only the system rc file read for maps when
 * the client is invoked in SecureMode or NoShellMode (bookmarks in the
 * user account's gopherrc will still be read.
 */
/* #define SECURE_MAPS_GLOBALRC_ONLY */

/*
 * Define this if you want the spawn to shell command disabled regardless
 * of whether SecureMode or NoShellMode are set.
 */
/* #define NEVERSPAWN */

/*
 * Define this if you want to disable access to the Options menus under all
 * circumstances.
 */
/* #define NEVERSETOPTIONS */

/*
 * Define this if you want the link's tuple and URL decription (as created
 * by decribe_gopher() for the '=' command) inserted at the tops of gripe
 * messages.
 */
#define DESCRIBE_GOPHER_GRIPE /* */

/*
 * Define this if you want a To: prompt containing the administrator's
 * address included for the gripe command.  If included, the user can
 * modify it to have the gripe sent to a different address.
 */
/* #define MODIFIABLE_GRIPE_TO */

/*
 * Define this if you want gripes disabled in SecureMode and NoShellMode
 */
/* #define NOGRIPE_SECURE */

/* Define LOCAL_GRIPE_ADMINISTRATOR and use your site's Gopher Administrator
 * address if you want gripes sent there for Gopher0 servers (or Gopher+
 * servers without Admin info for the link).  If DOMAIN_FOR_LOCAL_GRIPES is
 * also defined, only items on hosts with domain names containing that string
 * will generate mail to your LOCAL_GRIPE_ADMINISTRATOR.  If
 * LOCAL_GRIPE_ADMINISTRATOR is not defined, or the item being griped about
 * isn't on a server in the DOMAIN_FOR_LOCAL_GRIPES, the user will get a
 * screen message that the link's administrator is unknown (unless
 * MODIFIABLE_GRIPE_TO is on, in which case the address field is set to "").
 */
/* #define LOCAL_GRIPE_ADMINISTRATOR "<GopherAdmin@host.domain>" */
/* #define DOMAIN_FOR_LOCAL_GRIPES   "host.domain" */
/* #define DOMAIN_FOR_LOCAL_GRIPES   CLIENT1_HOST  */

/*
 * Override some defaults for various platforms
 */

#if defined(sun)
#define PLAY_COMMAND "play -v 40 -"
#endif


#if defined(NeXT)
#define NO_VPRINTF
#define PLAY_COMMAND "play -v 40 -"
#endif


#if defined(_SEQUENT_)
#define PRINTER_COMMAND "lp"
#endif


#if defined(VMS)
/*
 * VMS systems use VARIABLE length records for text files and
 * FIXED 512 records for binary files that are saved or cached.
 * Comment this out if you want to use stream_LF format instead.
 * The Bookmark (sys$login:gopherrc.) and configuration
 * (GopherP_Dir:gopher.rc and GopherP_Dir:gopherremote.rc, see
 * below) files are stream_LF regardless of how this program
 * logical is set.
 */
#define VMSRecords /* */

/*
 * Define this if you have an old version of VAXC/OpenVMS
 * which lacks the mktime() function.
 */
/* #define NO_MKTIME */

/*
 * The "builtin" pager is the default VMS utility for displaying text.
 * Alternatively, define "TPU" for invoking callable TPU.  Its /READ_ONLY
 * and /NOJOURNAL qualifiers are added internally by the software.
 * A still better alternative is to acquire MOST, optimized for Gopher and
 * C SWING, via anonymous FTP from narnia.memst.edu.  Define it with the -n
 * and +s switches.
 */
#define PAGER_COMMAND "builtin" /* */
/* #define PAGER_COMMAND "TPU/NOINI/COM=GopherP_Dir:GOPHER.TPU %s" */
/* #define PAGER_COMMAND "most -n +s %s" */

/*
 * MAIL_COMMAND is the program logical for the mail verb. 
 * MAIL_ADRS is the argument for an sprintf() command that can add
 * "prefix%""ADDRESS""" to the Internet mail address given by the
 * user.  It is structured for PMDF's IN%"INTERNET_ADDRESS"
 * scheme.  The %s is replaced with the address given by the
 * user.  No conversion will be done if a DECNET or simple
 * VMS MAIL address is given.  The default definition of "%s" for
 * MAIL_ADRS does not provide translation.  If you want to use
 * PMDF's prefix of IN%, Message Exchange's (MX's) prefix of MX%,
 * MultiNet's prefix of SMTP%,  or Wollongong's prefix of WINS%,
 * comment out the default definition of MAIL_ADRS and uncomment
 * the appropriate line below to your selected system.
 */
#define MAIL_COMMAND "mail"
#define MAIL_ADRS "%s" /* */
/* #define MAIL_ADRS "\"IN%%\"\"%s\"\"\""    */
/* #define MAIL_ADRS "\"MX%%\"\"%s\"\"\""    */
/* #define MAIL_ADRS "\"WINS%%\"\"%s\"\"\""  */
/* #define MAIL_ADRS "\"SMTP%%\"\"%s\"\"\""  */

/*
 * Use MultiNet's command verb for telnet and tn3270, so that it can
 * co-exist with other TCPIP transports on VMS systems.
 */
#if defined(MULTINET)
#   define TELNET_COMMAND "multinet telnet"
#   define TN3270_COMMAND "multinet telnet/tn3270"
#else
#   define TELNET_COMMAND "telnet"
#   define TN3270_COMMAND "tn3270"
#endif

/*
 * The Printer Command may be setup to use a command procedure to save the
 * file before printing it. This avoids the problem of Gopher removing the
 * temporary file before it can be printed. For example set up a command
 * procedure in a public place (e.g., GopherP_Dir:GOPHERPRINT.COM) containing:
 *        $ file="sys$scratch:gopher_"+f$extract(21,2,f$time())+".tmp"
 *        $ copy 'p1' 'file'
 *        $ print 'f$trnlnm("GOPHERQUEUE")' /delete/noidentify 'file'
 *  (Note that the logical GOPHERQUEUE can be used to set options like
 *   default queue name or form type eg DEFINE/JOB GOPHERQUEUE "/queue=que1")
 * and then define the Printer Command appropriately
 */
#define PRINTER_COMMAND "print %s" /* */
/* #define PRINTER_COMMAND "@GopherP_Dir:GOPHERPRINT %s" */

/*
 * Sounds are not implemented on VMS.
 * They can only be 's'aved or 'D'ownloaded.
 */
#define PLAY_COMMAND "- none -"

/*
 * If IMAGE_COMMAND is defined as "- none -"
 * a print command is not added to its default map,
 * and images can only be 's'aved or 'D'ownloaded.
 */
#define IMAGE_COMMAND "xv %s" /* */
/* #define IMAGE_COMMAND "- none -" */

/*
 * The builtin HTML browser is not yet functional.
 *
 * Lynx is a Curses-based HTML browser, available from ftp2.cc.ukans.edu,
 * which was initially designed to use Gopher as its server and still works
 * well with this software (on both VMS and Unix platforms).
 *
 * The WWW Line-Mode browser is avaialble from info.cern.ch
 *
 */

#define HTML_COMMAND "- none -"     
/* #define HTML_COMMAND "lynx -force_html %s" lynx 2.2 or greater */
/* #define HTML_COMMAND "www"                 WWW Line-Mode client */

/*
 * Point these to the default configuration files for view command maps:
 *    $ define/system "GopherP_Dir" "device:[directory]"
 * Make sure the files have commands mapped appropriately for VMS,
 * and have any maps which don't apply commented out.
 * 
 */
#define GLOBALRC "GopherP_Dir:gopher.rc"
#define REMOTERC "GopherP_Dir:gopherremote.rc"

/*
 * Define the *device* where shared images of message files will be
 * found.  Since these are architecture-specific, __ALPHA versions
 * need to be different than VAX versions, so if you have both on your
 * cluster and need a client for each of them, you'll need to compile
 * code for each, with a different directory for each one.  The ":.EXE"
 * *must* be present; the device specification may be changed (no directory
 * allowed).
 */
#ifdef __ALPHA
#define GOPHERMSGS_DEV "GopherP_Dir:.EXE" /* */
#else
#define GOPHERMSGS_DEV "GopherP_Dir:.EXE" /* */
#endif

/*
 * Define the default language to use when LC_MESSAGES selects multiple
 * message files.  If the default language is one of the multiple message
 * files selected, then no choice is offered to start the client (as in
 * the case when no message files or only one is detected), but the list
 * of possible languages is retained and can be invoked with the L keystroke.
 *
 * If multiple message files are selected but no default language is defined,
 * a menu of available languages always will be displayed at startup of the
 * gopher client.
 *
 * Note this is the language code imbedded in the default message file's 
 * *filename* -- it is not necessarily a gopher+ language code as in the
 * DEFAULT_LANG definition above.
 *
 * See [.GOPHER.LOCALES]README.VMS for more information.
 */
#define GOPHERMSGS_DEFAULT "EN_US"	 /* English_US	*/
/* #define GOPHERMSGS_DEFAULT "DE_DE"	 German_DE	*/ 
/* #define GOPHERMSGS_DEFAULT "DK"	 Danish	*/
/* #define GOPHERMSGS_DEFAULT "ES_ES"	 Spanish_ES	*/
/* #define GOPHERMSGS_DEFAULT "ES_MX"	 Spanish_MX	*/
/* #define GOPHERMSGS_DEFAULT "IT"	 Italian	*/
/* #define GOPHERMSGS_DEFAULT "FR"	 French	*/
/* #define GOPHERMSGS_DEFAULT "PIGLATIN" Oy-vay	*/
/* #define GOPHERMSGS_DEFAULT "SV"	 Swedish	*/

/*
 * Point this to the on-line Gopher+ help file.  It will be used if
 * a message catalog is not implemented, or if the implemented message
 * catalog does not yet have a help file for the corresponding language.
 */
#define GOPHERHELP "GopherP_Dir:gopher.hlp"

/*
 * Define this and point it to an edited version of the gopher.hlp file
 * which has the commands disabled in SecureMode deleted.  Otherwise, the
 * file pointed to by GOPHERHELP will be used.  If any message catalogs are
 * to be implemented and they have help files for the corresponding language,
 * edit those, and make sure each message number 224 (226 for Unix) points to
 * the correct help file location and name, so that it will be used instead.
 */
/* #define GOPHERHELP_SECURE "GopherP_Dir:gopher.hlp_secure" */

/*
 * Define this if you want to disable mailing of documents in SecurMode (-s)
 * and NoShellMode (-S).  Printing and saving to disk are always disabled for
 * those ("remote user") modes.
 */
/* #define NOMAIL */

/*
 * Client logging via syslog() code has been ported to VMS systems
 * with MULTINET.  See [.doc]clientlogging.vms for more information.
 * Check out the code in [.gopher]ourutils.c if you think you might
 * be able to port it to other TCPIP packages.
 *
 * Define CLIENT_LOGGER to log client requests issued to servers.
 * Define TELNET_TRACE to trace and log telnet connections by the client.
 */
/* #define CLIENT_LOGGER */
/* #define TELNET_TRACE  */

#endif /* VMS (Have you noticed how verbose VMSers tend to be?!?!?) */



/*
 * Now set the parameters, only if not set above... 
 */
#ifndef PAGER_COMMAND
#define PAGER_COMMAND "builtin"
#endif

#ifndef MAIL_COMMAND
#define MAIL_COMMAND "/bin/mail"
#endif

#ifndef TELNET_COMMAND
#define TELNET_COMMAND "telnet"
#endif

#ifndef TN3270_COMMAND
#define TN3270_COMMAND "tn3270"
#endif

#ifndef PRINTER_COMMAND
#define PRINTER_COMMAND "lpr"
#endif

#ifndef PLAY_COMMAND
#define PLAY_COMMAND "/bin/false"
#endif

#ifndef MIME_COMMAND
#define MIME_COMMAND "metamail -P" 
#endif

#ifndef IMAGE_COMMAND 
/* #define IMAGE_COMMAND "xloadimage -fork %s"*/
#define IMAGE_COMMAND "xv %s"
#endif

#ifndef HTML_COMMAND
/*** See the VMS HTML section for info on obtaining lynx or www ***/
#define HTML_COMMAND "- none -"		      /* */
/* #define HTML_COMMAND "lynx -force_html %s" lynx 2.1 or greater */
/* #define HTML_COMMAND "www"                 WWW Line-Mode client */
#endif

#ifndef REMOTERC
#define REMOTERC "/usr/local/lib/gopherrc.remote"
#endif

/****************** gopherd configuration ***********************/

/*
 * The maximum number of hits to return from a query to a
 * WAIS index.
 */

#define WAISMAXHITS 40

/*
 * The load average at which to restrict connections
 */

#define MAXLOAD 10.0

/*
 * Return type for signal()
 */

#define SIGRETTYPE void

/*
 * Timeout for network reads (1 minute)
 */

#define READTIMEOUT (1 * 60)

/*
 * Timeout for network writes (3 minute)
 */

#define WRITETIMEOUT (3 * 60)


/* We need to define this since inetd.conf can only have a few
   arguments, and we need lots of them */

#if !defined(CONF_FILE)
#  define CONF_FILE	"/usr/local/etc/gopherd.conf"
#endif
