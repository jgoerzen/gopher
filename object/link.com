$ vo = 'f$verify(0)'
$!
$ ON CONTROL_Y THEN GOTO CLEANUP
$ ON ERROR THEN GOTO CLEANUP
$!
$ IF P2 .eqs. "DEBUG
$ THEN
$ vo1 = f$verify(1)
$ library/create libgopher_debug compatible.objd,daarray.objd,-
    gdgopherdir.objd,gsgopherobj.objd,locale.objd,-
    strstring.objd,blblock.objd,views.objd,util.objd,-
    getopt.objd,sockets.objd,Debug.objd,url.objd,fileio.objd
$!
$ vo1 = 'f$verify(0)'
$ ELSE
$ vo1 = f$verify(1)
$ library/create libgopher compatible.obj,daarray.obj,-
    gdgopherdir.obj,gsgopherobj.obj,locale.obj,-
    strstring.obj,blblock.obj,views.obj,util.obj,-
    getopt.obj,sockets.obj,Debug.obj,url.obj,fileio.obj
$!
$ vo1 = 'f$verify(0)'
$ ENDIF
$ CLEANUP:
$    vo1 = f$verify(vo)
$exit
