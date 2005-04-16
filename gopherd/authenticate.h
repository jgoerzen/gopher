/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.1 $
 * $Date: 2000/08/19 00:28:56 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/authenticate.h,v $
 *
 * Paul Lindner, University of Minnesota DCS.
 *
 * Copyright 1991, 92, 93, 94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: authenticate.h
 * protos for des encryption routines
 *********************************************************************
 * Revision History:
 * $Log: authenticate.h,v $
 * Revision 1.1  2000/08/19 00:28:56  jgoerzen
 * Initial revision
 *
 * Revision 3.1  1995/09/25  22:27:25  lindner
 * new header file
 *
 *
 *********************************************************************/


char *GDESencrypt(char *user, char *ip, char *key, char *cleartext);
char *GDESdecrypt(char *user, char *ip, char *key, char *codedtext);
