$ v = 'f$verify(0)'
$!********************************************************************
$ IF P1 .EQS. ""
$ THEN
$    type/nopage sys$input:
     Usage:
	    @make UCX
	    @make WOLLONGONG
	    @make MULTINET
	    @make CMUIP
            @make NETLIB
            @make TCPWARE
$    exit
$ ENDIF
$!
$ ON CONTROL_Y THEN GOTO CLEANUP
$ ON ERROR THEN GOTO CLEANUP
$ v1 = f$verify(1)
$ set default [.object]
$ v1 = 'f$verify(0)'
$ @compile 'P1' 'P2'
$ @link 'P1' 'P2'
$!
$ v1 = f$verify(1)
$ set default [-.gopher]
$ v1 = 'f$verify(0)'
$ @compile 'P1' 'P2'
$ @link 'P1' 'P2'
$!
$ v1 = f$verify(1)
$ set default [-.gophfilt]
$ v1 = 'f$verify(0)'
$ @make_gophfilt 'P1' 'P2'
$ IF P2 .eqs. "DEBUG"
$ THEN
$ v1 = f$verify(1)
$ set default [-]
$ v1 = 'f$verify(0)'
$ goto CLEANUP
$ ENDIF
$!
$ v1 = f$verify(1)
$ set default [-.gopher.locales]
$ v1 = 'f$verify(0)'
$ @make
$!
$ v1 = f$verify(1)
$ set default [-.-]
$ v1 = 'f$verify(0)'
$!
$ CLEANUP:
$ v1 = f$verify(v)
$exit
