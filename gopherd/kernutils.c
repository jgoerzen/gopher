/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/kernutils.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: kernutils.c
 * Routines to implement kernel specific stuff
 *********************************************************************
 * Revision History:
 * $Log: kernutils.c,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.10  1995/09/25  22:10:57  lindner
 * blobbo..
 *
 * Revision 3.9  1994/07/31  04:39:06  lindner
 * Fix bug in HP-UX load avg. calculation
 *
 * Revision 3.8  1994/04/25  20:49:09  lindner
 * Fix for debug code
 *
 * Revision 3.7  1994/03/17  04:11:23  lindner
 * VMS update
 *
 * Revision 3.6  1994/01/06  06:46:21  lindner
 * Fix for HP-UX 8.0 systems
 *
 * Revision 3.5  1993/09/11  04:39:07  lindner
 * Fixes so that load ave works on command line
 *
 * Revision 3.4  1993/09/01  21:46:14  lindner
 * Load ave code for hp-ux and apollo domain
 *
 * Revision 3.3  1993/07/27  05:27:52  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.2  1993/06/11  16:52:33  lindner
 * Added VMS load code
 *
 * Revision 3.1.1.1  1993/02/11  18:02:56  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.1  1992/12/21  20:46:12  lindner
 * Initial revision
 *
 *
 *********************************************************************/


#include "gopherd.h"
#include "Debug.h"

#define LOADSYMBOL "_avenrun"    /* should work with most Unix flavors */

#define WHICHLOAD  2             /* 0 ==  1 min average           */
                                 /* 1 ==  5 min average           */
                                 /* 2 == 15 min average           */


#ifndef MAXLOAD
#    define MAXLOAD 10.0
#endif


#if !defined(LOADRESTRICT)

int LoadTooHigh() 
{
  return(0);
}

#else /* LOADRESTRICT */

double maxload;
double sysload = 0.0;

int 
LoadTooHigh(void)
{
     int status;
     
     if (!maxload) maxload = MAXLOAD;

     status = getload();
     if (status == -1)
	  Debugmsg("Error getting Load information!!\n");
     ;
     Debug("getload returns %d\n",status);
     return(status);
}

/*----------------*/
#ifdef VMS
#define KU_DONE

extern volatile in noshare vaxc$errno_stv;

int getload(void)
{
     double sysload = 0.0;
    /* In VMS with Multinet, we'll assign a channel to $$VMS_LOAD_AVERAGE,
	and read in 36 bytes into the structure _averages.		    */
     struct {
	  float   load_avg[3];    /*	1 min, 5 min, 15 min	*/
	  float   prio_avg[3];    /*	1 min, 5 min, 15 min	*/
	  float   queue_avg[3];   /*	1 min, 5 min, 15 min	*/
     }  _averages;
     
     FILE	*lav;

     if (GDCgetMaxLoad(Config)==0.0)
	  return(0);

     if ((lav=fopen("$$VMS_LOAD_AVERAGE","r"))==NULL) {
	  Debug("Cannot access load averages...%s",
		strerror(EVMSERR,vaxc$errno));
	  Debug("%s",vaxc$errno_stv?"/ ":"");
	  Debug("%s",vaxc$errno_stv?strerror(EVMSERR,vaxc$errno_stv):"");
	  return(0);
     }
     if (fread((char *)&_averages,sizeof(_averages),1,lav)==0) {
	  Debugmsg("Cannot access load averages...");
	  fclose(lav);
	  return(0);
     }
     if((sysload = _averages.load_avg[WHICHLOAD]) > GDCgetMaxLoad(Config)) {
	  Debug("System maxload exceeded (currently %f)\n",sysload);
	  fclose(lav);
	  return(1);
     }
     fclose(lav);
     return(0);
}
#endif

/*--------------------------------------------------*/
#if defined(hpux)
#define KU_DONE
#include <sys/pstat.h>

int 
getload()
{
     struct pst_dynamic ps;
     double sysload;
     
     pstat_getdynamic(&ps, sizeof(ps), 0, 0);
     switch (WHICHLOAD) {
     case 0: 
	  sysload = ps.psd_avg_1_min;
	  break;

     case 1: 
	  sysload = ps.psd_avg_5_min;
	  break;

     case 2: 
	  sysload = ps.psd_avg_15_min;
	  break;
     }
     if (sysload > maxload) {
	  Debug("System maxload exceeded (currently %f)\n", sysload);
	  return(1);
     }
     return(0);
}
#endif
/*---------------------------------------------*/
#if defined(apollo)
#define KU_DONE
int 
getload()
{
     long avenrun[3];
     
     proc1_$get_loadav(avenrun);
     
     if((sysload = (((double) avenrun[WHICHLOAD]) / 65536.0)) > maxload) {
	  Debug("System maxload exceeded (currently %f)\n",sysload);
	  return(1);
     }
     return(0);
}
#endif
/*-----------------------------------------*/
#if defined(sun)
#define KU_DONE

double atof();
#include <nlist.h>
#include <kvm.h>
#define X_AVENRUN 0
long avenrun[3];
kvm_t * kd;
struct nlist nl[] = { {LOADSYMBOL}, {""}, };

int getload()
{
     if ((kd = kvm_open(NULL, NULL, NULL, O_RDONLY, NULL)) == NULL) 
	  return(-1);
     if (kvm_nlist(kd, nl) != 0) 
	  return(-1);
     if(nl[X_AVENRUN].n_type == 0) 
	  return(-1);
     if(kvm_read(kd,nl[X_AVENRUN].n_value,avenrun,sizeof(avenrun)) 
	!= sizeof(avenrun)) 
	  return(-1);
     if((sysload = (((double) avenrun[WHICHLOAD]) / FSCALE)) > maxload) {
	  Debug("System maxload exceeded (currently %f)\n",sysload);
	  return(1);
     }
     return(0);
}
#endif

/*-----------------------------*/
#ifndef KU_DONE
#include <sys/param.h>
#include <nlist.h>
#include <sys/fixpoint.h>
#undef open
#undef fopen
struct  nlist nl[] = { { "avenrun" },
#define X_AVENRUN       0
		       { 0 },
		       };
int     kmem;
fix     avenrun[3];

int 
getload()
{
     if ((kmem = open("/dev/kmem", 0)) < 0)
	  return(-1);
     nlist("/vmunix", nl);
     if (nl[0].n_type==0)
	  return(-1);
     lseek(kmem, (long)nl[X_AVENRUN].n_value, 0);
     read(kmem, avenrun, sizeof(avenrun));
     sysload = FIX_TO_DBL(avenrun[WHICHLOAD]);
     if(sysload > maxload) {
	  Debug("System maxload exceeded (currently %.2f)\n",sysload);
	  return(1);
     }
     return(0);
}
#endif
#endif /* LOADRESTRICT */     
