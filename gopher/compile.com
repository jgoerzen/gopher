$ vg = 'f$verify(0)'
$!********************************************************************
$! lindner
$! 3.9
$! 1993/09/21 01:48:04
$! /home/mudhoney/GopherSrc/CVS/gopher+/gopher/compile.com,v
$! Exp
$!
$! Paul Lindner, University of Minnesota CIS.
$!
$! Copyright 1991, 1992 by the Regents of the University of Minnesota
$! see the file "Copyright" in the distribution for conditions of use.
$!********************************************************************
$! MODULE: compile.com
$! compiling script for VMS gopher client
$!*********************************************************************
$! Revision History:
$! compile.com,v
$! Revision 3.9  1993/09/21  01:48:04  lindner
$! Fix for alpha and curses
$!
$! Revision 3.8  1993/09/08  05:21:24  lindner
$! Add pager.c
$!
$! Revision 3.7  1993/08/25  02:58:53  lindner
$! Fix from Fote for vms make procedures
$!
$! Revision 3.6  1993/08/19  20:22:21  lindner
$! mods for openvms
$!
$! Revision 3.5  1993/08/16  18:48:33  lindner
$! Alpha and DECC mods
$!
$! Revision 3.4  1993/08/05  03:24:37  lindner
$! Changes for CMUIP and NETLIB
$!
$! Revision 3.3  1993/06/22  06:12:53  lindner
$! mods for DEC C
$!
$! Revision 3.2  1993/05/20  06:02:51  lindner
$! updates for VMS compatibility
$!
$! Revision 3.1.1.1  1993/02/11  18:02:56  lindner
$! Gopher+1.2beta release
$!
$! Revision 1.2  1993/01/07  22:51:06  lindner
$! Added download.c
$!
$! Revision 1.1  1992/12/31  06:08:20  lindner
$! Initial revision
$!
$!
$!********************************************************************/
$!
$ IF P1 .EQS. ""
$ THEN
$    type/nopage sys$input:
     Usage:
	    @compile UCX
	    @compile WOLLONGONG
	    @compile MULTINET
	    @compile CMUIP
	    @compile NETLIB
	    @compile TCPWARE
$    exit
$ ENDIF
$ IF P2 .EQS. "DEBUG"
$ THEN
$    write sys$output "Compiling client with DEBUG..."
$    debug := "/debug/noopt"
$ ELSE
$    debug := ""
$ ENDIF
$!
$ ON CONTROL_Y THEN GOTO CLEANUP
$ ON ERROR THEN GOTO CLEANUP
$!
$ IF P1 .eqs. "TCPWARE"
$ THEN
$    write sys$output "Compiling client with UCX emulation..."
$    transport := "UCX"
$ ELSE
$    transport := "''P1'"
$ ENDIF
$!
$ IF f$trnlnm("VAXCMSG") .eqs. "DECC$MSG" .or. -
     f$trnlnm("DECC$CC_DEFAULT") .eqs. "/DECC"
$ THEN
$ vg1 = f$verify(1)
$! DECC:
$    define/nolog C$USER_INCLUDE [-],[-.object]
$    define/nolog DECC$USER_INCLUDE [-],[-.object]
$ vg1 = 'f$verify(0)'
$  IF transport .eqs. "UCX"
$  THEN
$ vg1 = f$verify(1)
$    cc := cc/prefix=(all,except=(setlocale))-
             /warning=(disable=implicitfunc)-
             /define=('transport'=1,__VMS_CURSES)'debug'
$ vg1 = 'f$verify(0)'
$  ELSE
$ vg1 = f$verify(1)
     cc := cc/prefix=(all,except=(connect,gethostbyname,htons,inet_addr,-
                                  setsockopt,socket,setlocale))-
             /warning=(disable=implicitfunc)-
             /define=('transport'=1,__VMS_CURSES)'debug'
$ vg1 = 'f$verify(0)'
$  ENDIF
$ ELSE
$ vg1 = f$verify(1)
$! VAXC:
$    cc := cc/include=([-],[-.object])-
	     /define=('transport'=1)'debug'
$ vg1 = 'f$verify(0)'
$ ENDIF
$ vg1 = f$verify(1)
$!
$ cc cso.c
$ cc curcurses.c
$ cc globals.c
$ cc gopher.c
$ cc gopherrc.c
$ cc html2.c
$ cc manager.c
$ cc ourutils.c
$ cc subprocs.c
$ cc download.c
$ cc pager.c
$ cc form.c
$ cc upload.c
$!
$ vg1 = 'f$verify(0)'
$ IF P2 .eqs. "DEBUG"
$ THEN
$    vg1 = f$verify(1)
$    rename/nolog/noconf *.obj; *.objd
$    vg1 = 'f$verify(0)'
$ ENDIF
$!
$ CLEANUP:
$    vg1 = f$verify(vg)
$exit
