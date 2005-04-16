$ vg = 'f$verify(0)'
$!********************************************************************
$! F.Macrides 15-Dec-1994  Enable DEBUG build via P2.
$! F.Macrides 01-Dec-1994  Initial version.
$!********************************************************************
$! MODULE: makefilt.com
$! compiling and linking script for gophfilt
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
$    write sys$output "Compiling gophfilt with DEBUG..."
$    debug := "/obj=gophfilt.objd/debug/noopt"
$ ELSE
$    debug := ""
$ ENDIF
$!
$ ON CONTROL_Y THEN GOTO CLEANUP
$ ON ERROR THEN GOTO CLEANUP
$!
$ IF P1 .eqs. "TCPWARE"
$ THEN
$    write sys$output "Compiling gophfilt with UCX emulation..."
$    transport := "UCX"
$ ELSE
$    transport := "''P1'"
$ ENDIF
$!
$ IF f$trnlnm("VAXCMSG") .eqs. "DECC$MSG" .or. -
     f$trnlnm("DECC$CC_DEFAULT") .eqs. "/DECC"
$ THEN
$  optfile := "''P1'_DECC"
$  vg1 = f$verify(1)
$! DECC:
$    define/nolog C$USER_INCLUDE [-],[-.object]
$    define/nolog DECC$USER_INCLUDE [-],[-.object]
$  vg1 = 'f$verify(0)'
$  IF transport .eqs. "UCX"
$  THEN
$    vg1 = f$verify(1)
$    cc/prefix=(all,except=(setlocale))-
       /warning=(disable=implicitfunc)-
       /define=('transport'=1,__VMS_CURSES)-
       'debug' gophfilt.c
$    vg1 = 'f$verify(0)'
$  ELSE
$    vg1 = f$verify(1)
$    cc/prefix=(all,except=(connect,gethostbyname,htons,inet_addr,-
			    setsockopt,socket,setlocale))-
        /warning=(disable=implicitfunc)-
        /define=('transport'=1,__VMS_CURSES)-
	'debug' gophfilt.c
$    vg1 = 'f$verify(0)'
$  ENDIF
$ ELSE
$  optfile := "''P1'_VAXC"
$  vg1 = f$verify(1)
$! VAXC:
$    cc/include=([-],[-.object])/define=('transport'=1)-
       'debug' gophfilt.c
$  vg1 = 'f$verify(0)'
$ ENDIF
$!
$ IF P2 .eqs. "DEBUG"
$ THEN
$ vg1 = f$verify(1)
$!
$ link/debug/exe=gophfilt_debug-
       gophfilt.objd,[-.object]libgopher_debug/lib,-
       sys$library:vaxccurse/lib,-
       sys$disk:[-.gopher.vmsopts]'optfile'/opt
$!
$ vg1 = 'f$verify(0)'
$ ELSE
$ vg1 = f$verify(1)
$!
$ link gophfilt.obj,[-.object]libgopher/lib,-
       sys$library:vaxccurse/lib,-
       sys$disk:[-.gopher.vmsopts]'optfile'/opt
$!
$ vg1 = 'f$verify(0)'
$ ENDIF
$!
$ CLEANUP:
$    vg1 = f$verify(vg)
$exit
