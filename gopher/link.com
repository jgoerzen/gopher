$ vg = 'f$verify(0)'
$!********************************************************************
$! lindner
$! 3.9
$! 1994/06/09 04:09:47
$! /home/mudhoney/GopherSrc/CVS/gopher+/gopher/link.com,v
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
$! link.com,v
$! Revision 3.9  1994/06/09  04:09:47  lindner
$! Fix for vms linking problem
$!
$! Revision 3.8  1994/05/06  02:21:31  lindner
$! Mods for TCPWARE on VMS
$!
$! Revision 3.7  1994/02/20  16:17:40  lindner
$! Add form object file
$!
$! Revision 3.6  1993/09/08  05:21:25  lindner
$! Add pager.c
$!
$! Revision 3.5  1993/08/25  02:58:56  lindner
$! Fix from Fote for vms make procedures
$!
$! Revision 3.4  1993/08/23  02:54:42  lindner
$! Fix for vmsopts
$!
$! Revision 3.3  1993/08/16  18:48:37  lindner
$! Alpha and DECC mods
$!
$! Revision 3.2  1993/08/05  03:24:39  lindner
$! Changes for CMUIP and NETLIB
$!
$! Revision 3.1.1.1  1993/02/11  18:02:59  lindner
$! Gopher+1.2beta release
$!
$! Revision 1.2  1993/01/07  22:54:37  lindner
$! Added download.obj
$!
$! Revision 1.1  1992/12/31  06:10:07  lindner
$! Initial revision
$!
$!
$!********************************************************************/
$!
$ IF P1 .EQS. ""
$ THEN
$    type/nopage sys$input:
     Usage:
	    @link UCX
	    @link WOLLONGONG
	    @link MULTINET
	    @link CMUIP
	    @link NETLIB
	    @link TCPWARE
$    exit
$ ENDIF
$ IF P2 .EQS. "DEBUG"
$ THEN
$  debug := "/debug/exe=gopher_debug"
$  objectlib := "libgopher_debug"
$ ELSE
$  debug := ""
$  objectlib := "libgopher"
$ ENDIF
$!
$ ON CONTROL_Y THEN GOTO CLEANUP
$ ON ERROR THEN GOTO CLEANUP
$!
$ IF P1 .eqs. "TCPWARE"
$ THEN
$    write sys$output "Linking client with UCX emulation..."
$ ENDIF
$    transport := "''P1'"
$!
$ IF f$trnlnm("VAXCMSG") .eqs. "DECC$MSG" .or. -
     f$trnlnm("DECC$CC_DEFAULT") .eqs. "/DECC"
$ THEN
$    optfile := "''transport'_DECC" ! DECC
$ ELSE
$    optfile := "''transport'_VAXC" ! VAXC
$ ENDIF
$!
$ IF P2 .eqs. "DEBUG
$ THEN
$ vgl = f$verify(1)
$ link'debug'-
       gopher.objd,manager.objd,globals.objd,ourutils.objd,cso.objd,-
       html2.objd,CURcurses.objd,gopherrc.objd,download.objd,pager.objd,-
       form.objd,upload.objd,[-.object]'objectlib'/lib,-
       sys$library:vaxccurse/lib,-
       sys$disk:[.vmsopts]'optfile'/opt
$ vgl = 'f$verify(0)'
$ ELSE
$ vgl = f$verify(1)
$ link'debug'-
       gopher.obj,manager.obj,globals.obj,ourutils.obj,cso.obj,-
       html2.obj,CURcurses.obj,gopherrc.obj,download.obj,pager.obj,-
       form.obj,upload.obj,[-.object]'objectlib'/lib,-
       sys$library:vaxccurse/lib,-
       sys$disk:[.vmsopts]'optfile'/opt
$ vgl = 'f$verify(0)'
$ ENDIF
$!
$ CLEANUP:
$    vgl = f$verify(vg)
$exit
