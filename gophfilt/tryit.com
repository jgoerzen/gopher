$ v = 'f$verify(0)'
$!********************************************************************
$! F.Macrides 15-Dec-1994  Enable DEBUG build via P2.
$! F.Macrides 01-Dec-1994  Initial version.
$!********************************************************************
$! MODULE: tryit.com
$! Example DCL script for use with gophfilt
$!********************************************************************/
$ set noon
$ IF f$type(gophfilt) .eqs. ""
$ THEN
$ v = f$verify(1)
$!***************************************************
$! define gophfilt as a foreign command:
$!
$! gophfilt :== $device[directory]gophfilt.exe
$!
$ v = 'f$verify(0)'
$ exit
$!
$ ENDIF
$ v = f$verify(1)
$!**************************************************
$! Pass filename as P1 if you want output to a file:
$!
$ if P1 .nes. "" then define sys$output "''P1'"
$!
$!************************
$! Try it with qualifiers:
$!
$ gophfilt -t 0 -p "0/Information About Gopher/about"-
           -h gopher.tc.umn.edu -s 70
$!
$!*******************
$! Try it with a URL:
$!
$ gophfilt "gopher://gopher.tc.umn.edu:70/00/Information%20About%20Gopher/about"
$!
$!*******************************************************
$! Try it with tabbed stream input from the command file:
$!
$ gophfilt
0	0/Information About Gopher/about	gopher.tc.umn.edu	70
$!
$ v = 'f$verify(0)'
$ exit
