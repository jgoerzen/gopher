/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.2 $
 * $Date: 2000/12/20 01:19:20 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/waisgopher.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 1992 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: waisgopher.c
 * Routines to translate from gopher protocol to wais protocol
 *********************************************************************
 * Revision History:
 * $Log: waisgopher.c,v $
 * Revision 1.2  2000/12/20 01:19:20  jgoerzen
 * Added patches from David Allen <s2mdalle@titan.vcu.edu>
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.33  1995/10/05  03:29:11  lindner
 * Remove unused symbol
 *
 * Revision 3.32  1995/09/28  21:11:32  lindner
 * More fixes...
 *
 * Revision 3.31  1995/09/26  04:59:49  lindner
 * Fix abort routines, change to Die/Warn hash over access routines..
 *
 * Revision 3.30  1995/02/16  22:32:41  lindner
 * HTML icon support
 *
 * Revision 3.29  1995/02/01  21:41:48  lindner
 * Fix for FreeWAIS 0.4
 *
 * Revision 3.28  1995/01/04  17:36:31  lindner
 * Fix for freewais-sf on linux
 *
 * Revision 3.27  1994/11/05  03:20:02  lindner
 * Add logging, add period output to wais-gopher gateway
 *
 * Revision 3.26  1994/10/13  05:17:50  lindner
 * Compiler complaint fixes
 *
 * Revision 3.25  1994/07/19  20:23:44  lindner
 * Use local Locale.h
 *
 * Revision 3.24  1994/06/29  05:22:14  lindner
 * Add Gticket
 *
 * Revision 3.23  1994/05/02  07:41:17  lindner
 * Mods to use setlocale()
 *
 * Revision 3.22  1994/04/25  20:49:12  lindner
 * Fix for debug code
 *
 * Revision 3.21  1994/03/17  21:14:19  lindner
 * Fix for pid load limiting
 *
 * Revision 3.20  1994/03/17  04:11:56  lindner
 * Add Sockets.h
 *
 * Revision 3.19  1994/03/08  15:56:11  lindner
 * gcc -Wall fixes
 *
 * Revision 3.18  1994/01/21  03:38:35  lindner
 * Update function definitions (alanc)
 *
 * Revision 3.17  1994/01/03  18:04:47  lindner
 * bug fix..
 *
 * Revision 3.16  1993/12/30  04:12:04  lindner
 * Fix for number of arguments bug
 *
 * Revision 3.15  1993/11/02  06:02:41  lindner
 * HTML Mods
 *
 * Revision 3.14  1993/10/04  06:40:19  lindner
 * casting error
 *
 * Revision 3.13  1993/09/22  04:47:58  lindner
 * bug fix for waishits patch
 *
 * Revision 3.12  1993/09/22  04:30:27  lindner
 * Add option to conf.h for Max WAIS documents
 *
 * Revision 3.11  1993/09/20  23:15:09  lindner
 * Updates for multibyte wais docids
 *
 * Revision 3.10  1993/09/01  21:46:59  lindner
 * Fix problem with g0 client for wais gateway searches
 *
 * Revision 3.9  1993/08/11  02:27:43  lindner
 * Fix for wais gateway and Unix client
 *
 * Revision 3.8  1993/07/29  21:21:53  lindner
 * Fix for excess dots in text files
 *
 * Revision 3.7  1993/07/27  05:28:03  lindner
 * Mondo Debug overhaul from Mitra
 *
 * Revision 3.6  1993/07/26  15:31:12  lindner
 * mods for application/gopher-menu
 *
 * Revision 3.5  1993/07/23  03:16:34  lindner
 * Yet another fix for gplus
 *
 * Revision 3.4  1993/07/20  23:55:04  lindner
 * Fix gopher+ bug in gateway
 *
 * Revision 3.3  1993/07/07  19:37:18  lindner
 * *** empty log message ***
 *
 * Revision 3.2  1993/03/26  19:46:55  lindner
 * First crack at gopherplussing Indexing
 *
 * Revision 3.1.1.1  1993/02/11  18:02:53  lindner
 * Gopher+1.2beta release
 *
 * Revision 1.2  1993/01/30  23:57:44  lindner
 * Moved inputline parsing to gopherd.c
 *
 * Revision 1.1  1992/12/10  23:13:27  lindner
 * gopher 1.1 release
 *
 *
 *********************************************************************/


/* Wais to gopher gateway code.

   Paul Lindner, March 1992
   <lindner@boombox.micro.umn.edu>
*/

/* WIDE AREA INFORMATION SERVER SOFTWARE:
   No guarantees or restrictions.  See the readme file for the full standard
   disclaimer.
   
   Brewster@think.com
*/

#if defined(WAISSEARCH)   /*** Only compile this if we have WAIS ***/

#include "gopherd.h"
#include "command.h"
#include "Debug.h"
#include "serverutil.h"

#if defined(_AIX)
#define ANSI_LIKE
#endif

#include "Locale.h"
#include <ctype.h>
#include <ui.h>
#include <docid.h>

#define MAIN
#define sockets_h
#include "wais.h"
#include "Sockets.h"

#define WAISSEARCH_DATE "Fri Sep 13 1991"
#define DOCIDSIZE 5120


/*** Need for freewais-sf on some platforms ***/
#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif



/**** Needed by wais-8-b5... Ick... ***/

char *sdir=NULL;
char *cdir=NULL;


/**** For FreeWAIS 0.4 ****/
#ifdef FREEWAIS_0_4
#  define init_connection   FW_init_connection
#  define close_connection  FW_close_connection
#endif

/* encode a variable length integer - used by gopher_to_docid */
static unsigned char* 
integer_encode(cp,value) 
  register unsigned char* cp;
  register unsigned int value;
{
     char buf[100];
     char* bp = buf-1;

     if(value < 128) {
	  *cp++=(unsigned char)value;
	  return cp;
     }
     
     while(value >127) {
	  *++bp = (unsigned char)(value&0x7f);
	  value >>=7;
     }
     *cp++=(unsigned char)(value | 0x80);
     while(bp > (buf)) {
	  *cp++=(*bp--) | 0x80;
     }
     *cp++=*bp;
     return cp;
}



/**** Used by something in the source reading routines. ***/
void
PrintStatus(char *str)
{
     return;
}

void find_value(source, key, value, value_size)
  char *source, *key, *value;
  int value_size;
{
  char ch;
  long position = 0;  /* position in value */
  char *pos =strstr(source, key); /* address into source */

  value[0] = '\0';		/* initialize to nothing */

  if(NULL == pos)
    return;

  pos = pos + strlen(key);
  ch = *pos;
  /* skip leading quotes and spaces */
  while((ch == '\"') || (ch == ' ')) {
    pos++; ch = *pos;
  }
  for(position = 0; pos < source + strlen(source); pos++){
    if((ch = *pos) == ' ') {
      value[position] = '\0';
      return;
    }
    value[position] = ch;
    position++;
    if(position >= value_size){
      value[value_size - 1] = '\0';
      return;
    }
  }
  value[position] = '\0';
}

/****/

void ZapTabs(in)
  char *in;
{
     /** replace tabs with a space... **/
     while (*in != '\0') {
	  if (*in == '\t')
	       *in = ' ';
	  in ++;
     }
}

void
Send_binary_record(record, sockfd)
  WAISDocumentText *record;
  int sockfd;
{
     writen(sockfd, record->DocumentText->bytes, record->DocumentText->size);
}


/*** Modified from ../ir/ui.c to add \r\n at the ends of the line... ***/
void
Mydisplay_text_record_completely(record,quote_string_quotes, sockfd)
  WAISDocumentText *record;
  boolean quote_string_quotes;
  int sockfd;
{
     long count;
     unsigned char ch;
     unsigned char output[512];
     int outputptr = 0;

     for(count = 0; count < record->DocumentText->size; count++){
	  
	  ch = (unsigned char)record->DocumentText->bytes[count];
	  
	  switch (ch) {

	  case 27:
	       /* then we have an escape code */
	       /* if the next letter is '(' or ')', then ignore two letters */
	       if('(' == record->DocumentText->bytes[count + 1] ||
		  ')' == record->DocumentText->bytes[count + 1])
		    count += 1;             /* it is a term marker */
	       else count += 4;		/* it is a paragraph marker */
	       break;
	  case '\t':
	       output[outputptr++] = ch;
	       break;
	  case '\r':
	       if ('\n' == record->DocumentText->bytes[count + 1])
		    break;
	  case '\n':
	       Debugmsg("Got a return\n");
	       output[outputptr++] = '\r';
	       output[outputptr++] = '\n';
	       break;
	  default:
	       if (isprint(ch)){
		    if(quote_string_quotes && ch == '"') {
			 output[outputptr++] = '/';
			 output[outputptr++] = '/';
		    }
		    output[outputptr++] = ch;
	       } 
	  }

	  if (outputptr >500) {
	       output[outputptr++] = '\0';
	       writestring(sockfd, output);
	       outputptr = 0;
	  }
     }


     /*** Write out the rest of the buffer...  ***/

     output[outputptr++] = '\0';
     writestring(sockfd, output);
}



/* modified from Jonny G's version in ui/question.c */
void showDiags(d)
  diagnosticRecord **d;
{
     long i;
     
     for (i = 0; d[i] != NULL; i++) {
	  if (d[i]->ADDINFO != NULL) {
	       printf("Code: %s, %s\n", d[i]->DIAG, d[i] ->ADDINFO);
	  }
     }
}



int
acceptable(foo)
  char foo;
{
     if (foo == '\t' || foo == '\r' || foo == '\n' || foo == '\0')
	  return(0);
     else if (!isprint(foo))
	  return(0);
     else
	  return(1);
}


static char * hex = "0123456789ABCDEF";

/*
 * DocId_to_Gopher transforms a docid into a character string suitable for
 * transmission
 */

char *DocId_to_Gopher(docid, docsize)
  any *docid;
  int docsize;
{
     static char GopherString[DOCIDSIZE];
     char *q = GopherString;
     char *p;
     int l, i;
          
     /** First lets stick on the size of the document first **/

     sprintf(GopherString, "%d", docsize);
     q += strlen(GopherString);
     *q++ = ':';

      for (p=docid->bytes; (p < docid->bytes + docid->size) && q<&GopherString[DOCIDSIZE];) {
	  if (*p >= 10) {
	       ; /* Bad thing happened, can't understand docid, punt */
	       return(NULL);
	  }
	  
	  *q++ = (*p++) + '0';   /* Record Type */
	  *q++ = '=';            /* Seperate */

         /* patch by ses: l might be a multibyte integer, so deal with it*/
	  l = 0;  
	  while(1) {
	       int c;
	       c = *p++;
	       if(c &~0x7f) {
		    l |= (c&0x7f);
		    l <<= 7;
	       } else {
		    l |= c;
		    break;
	       }
	  }



	  for (i=0; i<l; i++, p++) {
	       if (acceptable(*p)==0) {
		    *q++ = '%';
		    *q++ = hex[(*p) >> 4];
		    *q++ = hex[(*p) & 15];
	       }
	       else *q++ = *p;
	  }
	  *q++ = ';';
     }
     *q++ = 0;
     return(GopherString);
}


 
int 
hex_to_int( c)
  char c;
{
     if(isdigit(c)) {
	  return c - '0';
     } else {
	  return (tolower(c) - 'a')+10;
     }
}


/*
 * Gstring is a name produced by DocID_to_Gopher
 */

any *Gopher_to_DocId(Gstring, DocSize)
  char *Gstring;
  int *DocSize;
{
     static any docid;
     char *outptr;
     char *inptr; 
     char *sor;
     char *eqptr;  
     int size; 
     int type;
     char tmp[DOCIDSIZE];

     /* Strip off the Document size first.... */
     
     inptr = strchr(Gstring, ':');
     if (inptr == NULL) 
	  return(NULL);

     *DocSize = atoi(Gstring);
     
     Gstring = inptr +1;

     if(docid.bytes) {
	  free(docid.bytes);
     }
     docid.bytes = s_malloc(DOCIDSIZE);
     docid.size=DOCIDSIZE;
     sor = docid.bytes;

     while(1) {
	  if(*Gstring == 0) {
	       break; /* we've finished */
	  }
	  outptr = tmp;
	  type = (*Gstring++)-'0'; /* get the field id*/
	  Gstring++;               /* skip over the '=' sign*/
	  eqptr = Gstring;
	  size = 0;                /* set the size to 0 */
	  while(1) {
	       if(*Gstring == ';') {
		    Gstring++;
		    break;
	       }
	       if(*Gstring == '%') {
		    
		    Gstring++;
		    *outptr = hex_to_int(*Gstring++);
		    *outptr <<= 4;
		    *outptr += hex_to_int(*Gstring++);
	       } else {
		    *outptr = *Gstring++;
	       }
	       size++;
	       outptr++;
	  }
	  if(DEBUG) {
	       printf("type = %d, size = %d\n",type,size);
	  }
	  *sor++ = type; /* set the type identifer*/
	  sor = (char*)integer_encode(sor,size);
	  bcopy(tmp,sor,size);
	  sor += size;
     }	  

     docid.size = sor - docid.bytes;
     return(&docid);

}
		    


/*-----------------------------------------------------------------*/

/* modified from tracy shen's version in wutil.c
 * displays either a text record of a set of headlines.
 */
void
display_search_response(response, hostname, port, dbname, SourceName, 
			sockfd, view, isgplus)

  SearchResponseAPDU *response;
  char               *hostname, *port, *dbname, *SourceName;
  int                sockfd;
  char               *view;
  boolean            isgplus;
{
     WAISSearchResponse  *info;
     long i, k;
     GopherObj *gs = NULL;
     GopherDirObj *gd = NULL;
     char gopherpath[1024];

     gs = GSnew();
     gd = GDnew(64);

     if ( response->DatabaseDiagnosticRecords != 0 ) {
	  info = (WAISSearchResponse *)response->DatabaseDiagnosticRecords;
	  i =0; 
		  
	  if (info->Diagnostics != NULL)
	       showDiags(info->Diagnostics);
	  
	  if ( info->DocHeaders != 0 ) {
	       k =0;
	       while (info->DocHeaders[k] != 0 ) {
		    i++;
		    ZapCRLF(info->DocHeaders[k]->Headline);
		    ZapTabs(info->DocHeaders[k]->Headline);
		    
		    GSinit(gs);

		    GSsetTitle(gs, info->DocHeaders[k]->Headline);
		    
		    sprintf(gopherpath, "waisdocid:%s:%s:%s",
			    SourceName,
			    info->DocHeaders[k]->Types[0], 
			    DocId_to_Gopher(info->DocHeaders[k]->DocumentID,
					    info->DocHeaders[k]->DocumentLength));
		    GSsetPath(gs, gopherpath);
		    GSsetHost(gs, Zehostname);
		    GSsetPort(gs, GopherPort);

		    if (strcmp(info->DocHeaders[k]->Types[0], "TEXT") == 0) {
			 GSsetType(gs, A_FILE);
		    }
		    else if (strcmp(info->DocHeaders[k]->Types[0],"GIF")==0) {
			 GSsetType(gs, A_GIF);
		    }
		    else if (strcmp(info->DocHeaders[k]->Types[0],"PICT")==0) {
			 GSsetType(gs, A_IMAGE);
		    }
		    else if (strcmp(info->DocHeaders[k]->Types[0],"WSRC")==0) {
			 GSsetType(gs, A_INDEX);
		    }
		    else
			 GSsetType(gs, A_UNIXBIN);
		    
		    GDaddGS(gd, gs);
		    k++;
	       }
	       
	       if (view == NULL)
		    GDtoNet(gd, sockfd, GSFORM_G0, Gticket, NULL);
	       else {
		    if (isgplus)
			 GSsendHeader(sockfd, -1);

		    if (strcasecmp(view, "application/gopher+-menu")==0)
			 GDplustoNet(gd, sockfd, NULL, Gticket);
		    else
			 GDtoNet(gd, sockfd, GSFORM_G0, Gticket, NULL);
	       }

	  }

	  if ( info->Text != 0 ) {
	       k =0;
	       while ((info->Text[k] != 0) ) {
		    i++;
		    printf("\n    Text record %2d, ", i);
		    Mydisplay_text_record_completely( info->Text[k++], false, sockfd);
	       }
	  }
     }
     writestring(sockfd, ".\r\n");
}


/*** Find server, database, and service
 *** This routine recognizes the hack where the filename is
 *** (server database service)
 ***/
void
MyReadSource(Moo, filename, sockfd)
  Source Moo;
  char   *filename;
  int sockfd;
{
     FILE *dotsrc;
     char  *cp;
     if (filename == NULL)
	  Die(sockfd, 404, "No Filename!");

     if (*filename == '(' && filename[strlen(filename)-1] == ')') {
	  cp = strchr(filename, ' ');
	  if (cp==NULL)
	       Die(sockfd, 404, "malformed filename");
	  *cp = '\0';
	  strcpy(Moo->server, filename+1);
	  *cp = ' ';
  
	  filename = cp+1;
	  cp = strchr(filename, ' ');
	  if (cp==NULL)
	       Die(sockfd, 404, "malformed filename");
	  *cp = '\0';
	  strcpy(Moo->database, filename);
	  *cp = ' ';

	  filename = cp +1;
	  filename[strlen(filename)] = '\0';
	  strcpy(Moo->service, filename);
     }
     else {
	  dotsrc = rfopen(filename, "r");
	  if (dotsrc == NULL)
	       Die(sockfd, 404, "File '%s' does not exist", filename);

	  ReadSource(Moo, dotsrc);
	  fclose(dotsrc);
     }
}
	  
	  
		 



#define MAX_KEYWORDS_LENGTH 1000
#define MAX_SERVER_LENGTH 1000
#define MAX_DATABASE_LENGTH 1000
#define MAX_SERVICE_LENGTH 1000
#ifndef WAISMAXHITS
#define WAISMAXHITS 40
#endif

/******************************************************************/

void SearchRemoteWAIS(sockfd, SourceName, cmd, view)
  int sockfd;
  char *SourceName;
  CMDobj *cmd;
  char *view;
{
     char *keywords = CMDgetSearch(cmd);
     char* request_message = NULL; /* arbitrary message limit */
     char* response_message = NULL; /* arbitrary message limit */
     long request_buffer_length;	/* how of the request is left */
     SearchResponseAPDU  *query_response;
     SearchResponseAPDU  *retrieval_response;
     char server_name[MAX_SERVER_LENGTH + 1];	
     char service[MAX_SERVICE_LENGTH + 1];
     char database[MAX_DATABASE_LENGTH + 1];
     FILE *connection;
     Source Moo;
     boolean isgplus = CMDisGplus(cmd);


     server_name[0] = '\0';  /* null it out */
     database[0] = '\0';     /* null it out */
     service[0] = '\0';      /* null it out */

     /**
      ** Next load up the name of the source...
      */
     
     
     if (keywords == NULL) {
	  /** An error occured, probably old client software... **/
	  writestring(sockfd, ".\r\n");
     } 
     
     Moo = (Source)malloc(sizeof(_Source));
     bzero(Moo, sizeof(_Source));

     MyReadSource(Moo, SourceName, sockfd);
     strcpy(server_name, Moo->server);
     strcpy(database, Moo->database);
     strcpy(service, Moo->service);
     
     if (server_name[0] == 0)
	  connection = NULL;
     
     else if ((connection=connect_to_server(server_name,atoi(service))) == NULL) 
	  Die(sockfd, 504, "Error openning connection to %s via service %s.", 
	      server_name, service);
     
     request_message = (char*)s_malloc((size_t)MAX_MESSAGE_LEN * sizeof(char));
     response_message = (char*)s_malloc((size_t)MAX_MESSAGE_LEN * sizeof(char));
     {
	  char userInfo[256];

	  sprintf(userInfo, "waisgopher, from host: %s", Zehostname);

	  init_connection(request_message, response_message,
			  MAX_MESSAGE_LEN,
			  connection,
			  userInfo);
     }
     request_buffer_length = MAX_MESSAGE_LEN; /* how of the request is left */
     
     if(NULL ==
	generate_search_apdu(request_message + HEADER_LENGTH, 
			     &request_buffer_length, 
			     keywords, database, NULL, WAISMAXHITS))
	  Die(sockfd, 504, "request too large");
     
     
     if(0 ==
	interpret_message(request_message, 
			  MAX_MESSAGE_LEN - request_buffer_length, 
			  response_message,
			  MAX_MESSAGE_LEN,
			  connection,
			  false	/* true verbose */
			  )) { /* perhaps the server shut down on us, let's see: */
	  if ( connection != NULL) {
	       fclose(connection);
	       if ((connection=connect_to_server(server_name,atoi(service))) == NULL) 
	       {
		    Die(sockfd, 504, "Error openning connection to %s via service %s.\n",
			server_name, service);
	       }

	       if(0 ==
		  interpret_message(request_message, 
				    MAX_MESSAGE_LEN - request_buffer_length, 
				    response_message,
				    MAX_MESSAGE_LEN,
				    connection,
				    false /* true verbose */
				    ))
		    Die(sockfd, 504, "really couldn't deliver message");
	  }
	  else
	       Die(sockfd, 504, "returned message too large");
     }
     
     readSearchResponseAPDU(&query_response, response_message + HEADER_LENGTH);
     
     LOGGopher(sockfd, "search %s for %s", database, keywords);

     display_search_response(query_response, server_name, service, database, SourceName, sockfd, view, isgplus);
     
     if (retrieval_response) {
	  freeWAISSearchResponse(query_response->DatabaseDiagnosticRecords);
	  freeSearchResponseAPDU( query_response);
     } else {
	  /** retrieval response was null **/;
     }
     /*** close it down ***/
     
     close_connection(connection);
     
     s_free(request_message);
     s_free(response_message);
     
}




/*******************************************************/

void
Fetchdocid(sockfd, cmd)
  int    sockfd;
  CMDobj *cmd;
{
     char *inputline = CMDgetSelstr(cmd) + 10;
     any *docid;
     int DocLen;
     int count;
     char* request_message = NULL; /* arbitrary message limit */
     char* response_message = NULL; /* arbitrary message limit */
     long request_buffer_length;	/* how of the request is left */
     char server_name[MAX_SERVER_LENGTH + 1];	
     char service[MAX_SERVICE_LENGTH + 1];
     char database[MAX_DATABASE_LENGTH + 1];
     SearchResponseAPDU  *retrieval_response;
     FILE *connection;
     char *SourceName;
     Source Moo;
     char *DocIdString;
     char *type;
     char *searchwords;
     boolean senddotcrlf = false;
	      
     server_name[0] = '\0';  /* null it out */
     database[0] = '\0';     /* null it out */
     service[0] = '\0';      /* null it out */


     DocIdString = strchr(inputline, ':');
     if (DocIdString == NULL) {
	  Die(sockfd, 404, "Malformed docid");
	  return;
     }
     else {
	  *DocIdString = '\0';
	  DocIdString++;
	  type = DocIdString;
	  DocIdString = strchr(type, ':') + 1;
	  *(DocIdString-1) = '\0';
	  searchwords = strrchr(DocIdString, '\t');
	  if (searchwords != NULL) {
	       *(searchwords) = '\0';
	       searchwords++;
	  }
     }

     SourceName = inputline;
     
     Moo = (Source)malloc(sizeof(_Source));
     bzero(Moo, sizeof(_Source));

     MyReadSource(Moo, SourceName);

     strcpy(server_name, Moo->server);
     strcpy(database, Moo->database);
     strcpy(service, Moo->service);
     
     if (server_name[0] == 0)
	  connection = NULL;


     else if ((connection=connect_to_server(server_name,atoi(service))) == NULL) 
	  
	  Die(sockfd, 504, "Error openning connection to %s via service %s.", 
	      server_name, service);

     request_message = (char*)s_malloc((size_t)MAX_MESSAGE_LEN * sizeof(char));
     response_message = (char*)s_malloc((size_t)MAX_MESSAGE_LEN * sizeof(char));

     docid = Gopher_to_DocId(DocIdString, &DocLen);
     
     /*** First let's transform the first word into a docid ***/
     /*** What we need from net:  DocumentLength, Types: (TEXT!), docid, **/
     
     for(count = 0; 
	 count * CHARS_PER_PAGE <
	 DocLen;
	 count++){
	  
	  request_buffer_length = MAX_MESSAGE_LEN; /* how of the request is left */
	  if(0 ==
	     generate_retrieval_apdu(request_message + HEADER_LENGTH,
				     &request_buffer_length, 
				     docid, 
				     CT_byte,
				     count * CHARS_PER_PAGE,
				     MIN((count + 1) * CHARS_PER_PAGE,
					 DocLen),
				     type,
				     database
				     ))
	       panic("request too long");
	  
	  if(0 ==
	     interpret_message(request_message, 
			       MAX_MESSAGE_LEN - request_buffer_length, 
			       response_message,
			       MAX_MESSAGE_LEN,
			       connection,
			       false /* true verbose */	
			       )) { /* perhaps the server shut down on us, let's see: */
	       if ( connection != NULL) {
		    fclose(connection);
		    if ((connection=connect_to_server(server_name,atoi(service))) == NULL) 
		    {
			 fprintf (stderr, "Error openning connection to %s via service %s.\n",
				  server_name, service);
			 gopherd_exit(-1);
		    }
		    if(0 ==
		       interpret_message(request_message, 
					 MAX_MESSAGE_LEN - request_buffer_length, 
					 response_message,
					 MAX_MESSAGE_LEN,
					 connection,
					 false /* true verbose */
					 ))
			 panic("really couldn't deliver message");
	       }
	       else
		    panic("returned message too large");
	  }
	  
	  readSearchResponseAPDU(&retrieval_response, 
				 response_message + HEADER_LENGTH);
	  
	  /* display_search_response(retrieval_response); the general thing */
	  if(NULL == ((WAISSearchResponse *)retrieval_response->DatabaseDiagnosticRecords)->Text){
	       display_search_response(retrieval_response);
	       panic("No text was returned");
	  }

	  if (strncmp(type, "TEXT", 4) == 0) {
	       int k;
	       k = 0;
	       Debugmsg("Displaying text\n");
	       while (((WAISSearchResponse *)retrieval_response->DatabaseDiagnosticRecords)->Text[k] != 0) {
		    Mydisplay_text_record_completely
			 (((WAISSearchResponse *)retrieval_response->DatabaseDiagnosticRecords)->Text[k++], false, sockfd);
	       }
	       senddotcrlf = true;
	  }
	  else if (strncmp(type, "WSRC", 4) == 0)  {
	      char searchline[512];
	      char tempfile[64];
	      int  tempdotsrc;
	      Source Moo;

	      /*** It's a search! **/
	      Debugmsg("Searching!\n");
	      
	      /*** Dump out the contents of the text record on the server ***/
	      /*** then call SearchRemoteWAIS(), then delete file ***/
	      rchdir("/");
	      mkdir("tmp",0755);
	      sprintf(tempfile, "/tmp/waissearch.%d", getpid());
	      tempdotsrc = ropen(tempfile, O_RDWR|O_CREAT,0755);
	      if (tempdotsrc < 0) {
		   writestring(sockfd, "3couldn't make temp file\r\n.\r\n");
		   return;
	      }
	      Mydisplay_text_record_completely
		   (((WAISSearchResponse *)retrieval_response->DatabaseDiagnosticRecords)->Text[0], false, tempdotsrc);
	      close(tempdotsrc);

	      Moo = (Source) malloc(sizeof(_Source));
	      bzero(Moo, sizeof(_Source));

	      MyReadSource(Moo, tempfile);

	      sprintf(searchline, "(%s %s %s)\t%s", Moo->server, Moo->database, Moo->service, searchwords);

	      Debug("waisgopher %s\n", searchline);
	      rchdir("/");
	      SearchRemoteWAIS(sockfd, searchline, cmd, "");
	      
	      unlink(tempfile);
	 }
	  else {
	       int recs =0;

	       Debugmsg("Displaying Binary\n");
	       while (1) {
		    if (((WAISSearchResponse*)retrieval_response->DatabaseDiagnosticRecords)->Text[recs] == NULL)
			 break;
		    Send_binary_record(((WAISSearchResponse *)retrieval_response->DatabaseDiagnosticRecords)->Text[recs++], sockfd);
	       }
	  }
     }

     if (senddotcrlf)
	  writestring(sockfd, ".\r\n");


     freeWAISSearchResponse( retrieval_response->DatabaseDiagnosticRecords); 
     freeSearchResponseAPDU( retrieval_response);

     /*** close it down ***/

     close_connection(connection);
     
     s_free(request_message);
     s_free(response_message);

}

#else /* defined(WAISSEARCH) */

#include "command.h"

void
SearchRemoteWAIS(sockfd, SourceName, cmd, view)
  int sockfd;
  char   *SourceName;
  CMDobj *cmd;
  char   *view;
{
     Die(sockfd, 502, "No wais stuff in the server!!");
}


void
Fetchdocid(sockfd, cmd)
  int sockfd;
  CMDobj *cmd;
{
     Die(sockfd, 502, "No wais stuff in the server!!");
}

#endif
