$ vg = 'f$verify(0)'
$!********************************************************************
$! wilkinson/macrides
$! 1.5
$! 1994/12/18
$!
$! J.Lance Wilkinson, Penn State University C&IS - LCS.
$! F. Macrides, Worcester Foundation for Expermimental Biology
$!********************************************************************
$! MODULE: message.com
$! compiling script for VMS "internationalized" messages
$!*********************************************************************
$! Revision History:
$! message.com,v
$! Revision 1.5  1994/12/18 macrides
$! Pass the facility values to MESSAGE.TPU as incremental values in the range
$!	of 0 - 2047.  This guarantees unique values for up to the maximum of
$!	2048 shared image message files.
$!
$! Revision 1.4  1994/7/14  wilkinson
$! Made MAKE.COM search out all .msg files and process them rather than hard-
$!	coding specific .msg files.
$!
$! Revision 1.3  1994/7/13  wilkinson/macrides
$! Firmed up AXP support
$!
$! Revision 1.2  1994/7/12  wilkinson
$! Attempted AXP support
$!
$! Revision 1.1  1994/5/31  wilkinson
$! Initial revision
$!
$!********************************************************************/
$!
$ ON CONTROL_Y THEN GOTO CLEANUP
$ ON ERROR THEN GOTO CLEANUP
$!
$ vg1 = f$verify(1)
$!
$!	Preprocess the .MSG files into VMS message files.
$!
$ vg1 = 'f$verify(0)'
$ if f$getsyi("HW_MODEL").ge.1024
$ then
$   vg1 = f$verify(1)
$   platform   := "AXP"
$   preprocess := "edit/tpu/nosection/nodisplay/command=message.tpu"
$!
$   vg1 = 'f$verify(0)'
$ else
$   vg1 = f$verify(1)
$   platform   := "VAX"
$   preprocess := "edit/tpu/nosection/nodisplay/command=message.tpu"
$   vg1 = 'f$verify(0)'
$ endif
$
$ facility = 0
$ prep_loop:
$   file = f$search("*.msg")
$   if file .eqs. "" .or. facility .ge. 2048 then goto done_prep
$   filename := 'f$parse(file,,,"name")
$   initstring = "''platform'"'facility
$   vg1 = f$verify(1)
$!
$   preprocess/init='initstring/output='filename.vms$msg 'filename.msg
$   vg1 = 'f$verify(0)'
$   facility = facility + 1
$   goto prep_loop
$ done_prep:
$ purge/nolog *.vms$msg
$ purge/nolog *.opt
$ vg1 = f$verify(1)
$!
$!	Compile the resultant .VMS$MSG files into VMS message files.
$!
$ vg1 = 'f$verify(0)'
$ msgs_loop:
$   file = f$search("*.vms$msg")
$   if file .eqs. "" then goto done_msgs
$   filename := 'f$parse(file,,,"name")
$   vg1 = f$verify(1)
$   message 'filename.vms$msg
$   vg1 = 'f$verify(0)'
$   goto msgs_loop
$ done_msgs:
$ purge/nolog *.obj
$ vg1 = f$verify(1)
$!
$!	Create non-executable message files.  These .EXE files should
$!		be copied from [.gopher.locales] to GopherP_Dir or the
$!		location referenced by GOPHERMSGS_DEV in conf.h
$!
$ vg1 = 'f$verify(0)'
$ link_loop:
$   file = f$search("*.obj")
$   if file .eqs. "" then goto done_link
$   filename := 'f$parse(file,,,"name")
$   vg1 = f$verify(1)
$   link/sharable=gopher_msg_'filename.exe 'filename.obj, 'filename.opt/opt
$   vg1 = 'f$verify(0)'
$   goto link_loop
$ done_link:
$ purge/nolog gopher_msg_*.exe
$ delete/nolog *.obj;*
$ delete/nolog *.vms$msg;*
$ delete/nolog *.opt;*
$!
$ vg1 = 'f$verify(0)'
$ CLEANUP:
$    vg1 = f$verify(vg)
$exit
