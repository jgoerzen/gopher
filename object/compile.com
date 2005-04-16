$ vo = 'f$verify(0)'
$!********************************************************************
$! lindner
$! 3.2
$! 1993/05/18 15:06:34
$! /home/mudhoney/GopherSrc/CVS/gopher+/object/compile.com,v
$! Exp
$!
$! Paul Lindner, University of Minnesota CIS.
$!
$! Copyright 1991, 1992 by the Regents of the University of Minnesota
$! see the file "Copyright" in the distribution for conditions of use.
$!********************************************************************
$! MODULE: compile.com
$! compiling script for VMS
$!*********************************************************************
$! Revision History:
$! compile.com,v
$! Revision 3.2  1993/05/18  15:06:34  lindner
$! Fixed CVS/RCS comment leaders...
$!
$! Revision 3.1  1993/05/05  18:44:07  lindner
$! new VMS files
$!
$! Revision 1.1  1992/12/31  05:19:12  lindner
$! Initial revision
$!
$!
$!********************************************************************/
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
$    write sys$output "Compiling library with DEBUG..."
$    debug := "/debug/noopt"
$ ELSE
$  debug := ""
$ ENDIF
$!
$ ON CONTROL_Y THEN GOTO CLEANUP
$ ON ERROR THEN GOTO CLEANUP
$!
$ IF P1 .eqs. "TCPWARE"
$ THEN
$    write sys$output "Compiling library with UCX emulation..."
$    transport := "UCX"
$ ELSE
$    transport := "''P1'"
$ ENDIF
$!
$ IF f$trnlnm("VAXCMSG") .eqs. "DECC$MSG" .or. -
     f$trnlnm("DECC$CC_DEFAULT") .eqs. "/DECC"
$!
$ THEN
$ vo1 = f$verify(1)
$! DECC:
$ vo1 = 'f$verify(0)'
$  IF transport .eqs. "UCX"
$  THEN
$ vo1 = f$verify(1)
$    cc := cc/prefix=(all,except=(setlocale))-
             /warning=(disable=implicitfunc)-
             /define=('transport'=1,__VMS_CURSES)'debug'
$ vo1 = 'f$verify(0)'
$  ELSE
$ vo1 = f$verify(1)
$    cc := cc/prefix=(all,except=(connect,gethostbyname,htons,inet_addr,-
                                  setsockopt,socket,setlocale))-
             /warning=(disable=implicitfunc)-
             /define=('transport'=1,__VMS_CURSES)'debug'
$ vo1 = 'f$verify(0)'
$  ENDIF
$ ELSE
$ vo1 = f$verify(1)
$! VAXC:
$    cc := cc/define=('transport'=1)'debug'
$ vo1 = 'f$verify(0)'
$ ENDIF
$ vo1 = f$verify(1)
$!
$ cc compatible.c
$ cc daarray.c
$ cc gdgopherdir.c
$ cc gsgopherobj.c
$ cc strstring.c
$ cc util.c
$ cc blblock.c
$ cc VIews.c
$ cc Sockets.c
$ cc Debug.c
$ cc url.c
$ cc fileio.c
$ cc locale.c
$!
$ cc getopt.c
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
$    vo1 = f$verify(vo)
$exit
