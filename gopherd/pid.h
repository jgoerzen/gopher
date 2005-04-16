/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/pid.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991,92,93,94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: pid.h
 * Abstraction of all PID routines
 *********************************************************************
 * Revision History:
 * $Log: pid.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.4  1994/07/31  05:16:42  lindner
 * One more bug fix..
 *
 * Revision 3.3  1994/07/31  03:46:14  lindner
 * Make PID routines optional
 *
 * Revision 3.2  1994/04/08  21:13:03  lindner
 * Fix for ansi C compilers
 *
 * Revision 3.1  1994/03/17  04:33:38  lindner
 * New pid routines
 *
 *
 *
 *********************************************************************/

/*
 * This is the interface for all the PID routines
 */

#ifndef NO_AUTHENTICATION

void PIDwrite( /* char* char* */);
int  PIDnumprocs( /* ... */);
void PIDdone(/* ... */);
void PIDclean();


#else /** NO_AUTHENTICATION **/

#  define PIDwrite(a,b) ;
#  define PIDnumprocs(a) (0)
#  define PIDdone(a,b) ;
#  define PIDclean(a) ;
#endif

#if defined(__STDC__) || defined(__stdc__)
 void PIDwritef(char *piddir, const char *fmt, ...);
#else
 void PIDwritef();
#endif
