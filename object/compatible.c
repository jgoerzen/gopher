/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2001/01/17 21:16:35 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/object/compatible.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: compatible.c
 * Compatibility routines
 *********************************************************************
 * Revision History:
 * $Log: compatible.c,v $
 * Revision 1.2  2001/01/17 21:16:35  jgoerzen
 * More psinrtf -> snprintf changes
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.19  1996/01/04  18:25:48  lindner
 * Updates for autoconf
 *
 * Revision 3.18  1995/09/25  22:18:20  lindner
 * Next update
 *
 * Revision 3.17  1995/09/25  22:07:22  lindner
 * Ansification
 *
 * Revision 3.16  1995/02/06  22:14:22  lindner
 * Remove const
 *
 * Revision 3.15  1994/12/15  17:30:16  lindner
 * A replacement fgetpwent
 *
 * Revision 3.14  1994/07/21  22:18:45  lindner
 * Add putenv() compat code
 *
 * Revision 3.13  1994/04/25  03:34:34  lindner
 * Put back alpha patch (Fote)
 *
 * Revision 3.12  1994/04/08  20:05:55  lindner
 * gcc -Wall fixes
 *
 * Revision 3.11  1994/04/01  04:44:28  lindner
 * Fix for alpha VMS
 *
 * Revision 3.10  1994/03/30  21:38:39  lindner
 * Remove some VMS code
 *
 * Revision 3.9  1994/03/17  04:38:45  lindner
 * VMS weird directory routines
 *
 * Revision 3.8  1994/03/08  03:24:09  lindner
 * Waitpid for vms
 *
 * Revision 3.7  1993/10/27  18:51:10  lindner
 * Updates for VMS files/records
 *
 * Revision 3.6  1993/09/03  03:26:39  lindner
 * Better VMS tempnam() implementation
 *
 * Revision 3.5  1993/06/22  05:53:17  lindner
 * Added getdtablesize() option
 *
 * Revision 3.4  1993/04/15  21:36:30  lindner
 * Emulation of geteuid calls for HPs
 *
 * Revision 3.3  1993/03/18  22:27:46  lindner
 * better portable tempnam()
 *
 * Revision 3.2  1993/02/19  21:33:27  lindner
 * Gopher1.2b2 release
 *
 * Revision 3.1.1.1  1993/02/11  18:03:05  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.4  1993/01/17  03:46:12  lindner
 * Fixed tempnam for VMS
 *
 * Revision 1.3  1993/01/08  23:13:55  lindner
 * Added more VMS mods from jqj
 *
 *
 *********************************************************************/


/*
 * Some functions that aren't implemented on every machine on the net
 *
 * definitions should be in the form "NO_FNNAME"
 * compatible.h looks at preprocessor symbols and automatically defines
 * many of the NO_FNNAME options
 *
 */

#include "String.h"
#include <stdio.h>
#include "Malloc.h"  /*** For NULL ***/
#include "compatible.h"

/*** For machines that don't have strstr ***/

#if !defined(HAVE_STRSTR)

char *
Gopher_strstr(char *host_name, char *cp)
{
     int i, j;

     for (i = 0; i < strlen(host_name); i++) {
          j = strncmp(host_name+i, cp, strlen(cp));
          if (j == 0)
               return(host_name+i);
     }
     return(NULL);
}
#endif

#if !defined(HAVE_VPRINTF)

#include <varargs.h>
vsprintf(va_alist)
  va_dcl
{
        ;
}

vfprintf(va_alist)
  va_dcl
{
        ;
}


#endif

#if !defined(HAVE_TEMPNAM)
/* A tip of the hat to the developers of elm 2.4pl17, from whence 
   the non-VMS portion of this routine comes.  
*/
/* and a tempnam for temporary files */
static int cnt = 0;

char *tempnam(char *dir, char *pfx)
{
	char space[512];
	char *newspace;

#ifdef VMS
	if (dir == NULL) {
		dir = "sys$scratch:";
	} else if (*dir == '\0') {
		dir = "sys$scratch:";
	}
	
	if (pfx == NULL) {
		pfx = "gopher.$";
	} else if (*pfx == '\0') {
		pfx = "gopher.$";
	}

	snprintf(space, sizeof(space),
		 "%s%s%d%d", dir, pfx, getpid(), cnt);
#else
	if (dir == NULL) {
		dir = "/usr/tmp";
	} else if (*dir == '\0') {
		dir = "/usr/tmp";
	}
	
	if (pfx == NULL) {
		pfx = "";
	}

	snprintf(space, sizeof(space),
		 "%s/%s%d.%d", dir, pfx, getpid(), cnt);
#endif
	cnt++;
	
	newspace = (char *)malloc(strlen(space) + 1);
	if (newspace != NULL) {
		strcpy(newspace, space);
	}
	return newspace;
}
#endif

#if !defined(HAVE_STRDUP)
char *goph_strdup(char *str)
{
        int len;
        char *temp;

        if (str == NULL) return(NULL);
        len = strlen(str);

        temp = (char *) malloc(sizeof(char) * len + 1);

        strcpy(temp, str);
        return(temp);
}
#endif

#if !defined(HAVE_TZSET)
void
tzset()
{
     ;
}
#endif

#if !defined(HAVE_STRCASECMP)
/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Modified for use on VMS by Earl Fogel, University of Saskatchewan
 * Computing Services, January 1992
 */

typedef unsigned char u_char;

/*
 * This array is designed for mapping upper and lower case letter
 * together for a case independent comparison.  The mappings are
 * based upon ascii character sequences.
 */
static u_char charmap[] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
	'\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
	'\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
	'\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
	'\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
	'\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
	'\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
	'\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
	'\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
	'\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
	'\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
	'\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
	'\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
	'\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
	'\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
	'\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
	'\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
	'\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
	'\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};

int
strcasecmp(const char *s1, const char *s2)
{
	register const u_char *cm = charmap,
			*us1 = (const u_char *)s1,
			*us2 = (const u_char *)s2;

	while (cm[*us1] == cm[*us2++])
		if (*us1++ == '\0')
			return (0);
	return (cm[*us1] - cm[*--us2]);
}

int
strncasecmp(const char *s1, const char *s2, size_t n)
{
	if (n != 0) {
		register const u_char *cm = charmap,
				*us1 = (const u_char *)s1,
				*us2 = (const u_char *)s2;

		do {
			if (cm[*us1] != cm[*us2++])
				return (cm[*us1] - cm[*--us2]);
			if (*us1++ == '\0')
				break;
		} while (--n != 0);
	}
	return (0);
}

#endif

#if defined(NEED_GETDTABLESIZE)
int getdtablesize()
{
     struct rlimit rlp;
     
     rlp.rlim_cur = rlp.rlim_max = RLIM_INFINITY;
     
     if (getrlimit( RLIMIT_NOFILE, &rlp ) )
	  return(-1);
     fds = rlp.rlim_cur;
}
#endif

#if defined(VMS)
/* In all versions of VMS, fopen() and open() are needlessly inefficient.
 * Define jacket routines to do file opens with more sensible parameters
 * than the VAXCRTL default.
 * [Should we really be doing this for EVERY fopen() and open()?]
 */
#ifdef fopen
#undef fopen
#endif
#ifdef open
#undef open
#endif

FILE *fopen_VAR (char *name, char *mode)
{
    return fopen ( name, mode, "rfm=var","rat=cr","mbc=32" );
}

FILE *fopen_FIX (char *name, char *mode)
{
    return fopen ( name, mode, "rfm=fix","mrs=512","mbc=32" );
}

FILE *fopen_VMSopt (char *name, char *mode)
{
    return fopen ( name, mode, "mbc=32" );
}

int open_VMSopt (char *name, int flags, unsigned int mode)
{
    return  open ( name, flags, mode, "mbc=32");
}

#endif


#if defined(VMS) || defined(NO_WAITPID_WAIT3)
#  include "Wait.h"

pid_t waitpid(pid_t pid, int *status, int options)
{
     int zepid;

     while ((zepid = wait(status)) != pid && pid != -1)
	  /** Should check and make sure process exited... **/
	  ;

     return(zepid);
}
#endif     

/*
 * This is an ugly hack for now, since we only use strftime in one
 * place..... so far..
 */

#if !defined(HAVE_STRFTIME)
#  include <time.h>


int strftime(char *buf, int bufsize, char *fmt, struct tm *tm)
{
     sprintf(buf, "%4d%02d%02d%02d%02d%02d\n",
	     tm->tm_year, tm->tm_mon+1,tm->tm_mday,
	     tm->tm_hour, tm->tm_mon, tm->tm_sec);
}

#endif
	     

/*
 * This code was stolen from cnews.  Modified to make "newenv" static so
 * that realloc() can be used on subsequent calls to avoid memory leaks.
 *
 * We only need this if Configure said there isn't a putenv() in libc.
 */

#ifndef HAVE_PUTENV /*{*/

/* peculiar return values */
#define WORKED 0
#define FAILED 1

int
putenv(char *var)			/* put var in the environment */
{
     register char **envp;
     register int oldenvcnt;
     extern char **environ;
     static char **newenv = NULL;
     
     /* count variables, look for var */
     for (envp = environ; *envp != 0; envp++) {
	  register char *varp = var, *ep = *envp;
	  register int namesame;
	  
	  namesame = 0;
	  for (; *varp == *ep && *varp != '\0'; ++ep, ++varp)
	       if (*varp == '=')
		    namesame = 1;
	  if (*varp == *ep && *ep == '\0')
	       return WORKED;	/* old & new var's are the same */
	  if (namesame) {
	       *envp = var;	/* replace var with new value */
	       return WORKED;
	  }
     }
     oldenvcnt = envp - environ;
     
     /* allocate new environment with room for one more variable */
     if (newenv == NULL)
	  newenv = (char **)malloc((unsigned)((oldenvcnt+1+1)*sizeof(*envp)));
     else
	  newenv = (char **)realloc((char *)newenv, (unsigned)((oldenvcnt+1+1)*sizeof(*envp)));
     if (newenv == NULL)
	  return FAILED;
     
     /* copy old environment pointers, add var, switch environments */
     (void) bcopy((char *)environ, (char *)newenv, oldenvcnt*sizeof(*envp));
     newenv[oldenvcnt] = var;
     newenv[oldenvcnt+1] = NULL;
     environ = newenv;
     return WORKED;
}

#endif /* NO_PUTENV */

#ifndef HAVE_FGETPWENT
struct passwd *
fgetpwent(FILE *f)
{
     static char input[256];
     static struct passwd *p  = NULL;
     char *cp, *cp2;

     if (p == NULL) {
	  p = (struct passwd *) malloc(sizeof(struct passwd));
     }
     
     if (fgets(input, 256, f) == NULL)
	  return(NULL);

     /* Username */
     cp = strchr(input, ':');
     if (cp == NULL) return(NULL);

     *cp = '\0'; cp++;
     p->pw_name = input;
     
     /* Password */
     cp2 = strchr(cp, ':');
     if (cp2 == NULL) return(NULL);
     *cp2 = '\0'; cp2++;
     p->pw_passwd = cp;

     /* UID */
     cp = strchr(cp2, ':');
     if (cp == NULL) return(NULL);
     
     *cp = '\0'; cp++;
     p->pw_uid = atoi(cp2);

     /* GID */
     cp2 = strchr(cp, ':');
     if (cp2 == NULL) return(NULL);
     *cp2 = '\0'; cp2++;
     p->pw_gid = atoi(cp);

     /* GECOS */
     cp = strchr(cp2, ':');
     if (cp == NULL) return(NULL);
     *cp = '\0'; cp++;
     p->pw_gecos = cp2;

     /* Home dir */
     cp2 = strchr(cp, ':');
     if (cp2 == NULL) return(NULL);
     *cp2 = '\0'; cp2++;
     p->pw_dir = cp;

     /* Shell */
     p->pw_shell = cp2;

     return(p);
}
#endif /* NO_FGETPWENT */

