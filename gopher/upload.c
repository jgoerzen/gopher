#include "gopher.h"
#include "fileio.h"
#ifdef VMS
#include <stat.h>
#else
#include <sys/stat.h>
#endif

#ifdef HAVE_TIME_H
#  include <time.h>
#endif /* HAVE_TIME_H */

static char *DLnames[] = {
     "Zmodem",
     "Ymodem",
     "Xmodem-CRC",
     "Kermit",
     "Text",
     "WRQ - Reflection",
     NULL
};

static char *textorbin[] = {
     "Binary",
     "ASCII",
     NULL
	 
};

static char *ULtextcmds[] = {
     "rz ",
     "rb -a ",
     "rx -ac ",
     "kermit -q -r ",
     "false ",
     "unixlnk2",
     NULL
};

static char *ULbincmds[] = {
     "rz -b ",
     "rb ",
     "rx ",
     "kermit -q -r -i",
     "false ",
     "unxlink2",
     NULL
};


extern Requestitem *REQitemnew(void);

void
BuiltinUploadfile(void)
{
     Requestitem *ulmenu[6];
     char filename[80];
     boolean istext;
     int     ulprotocol;
     char    cmdline[512];
     boolean iszmodem;
     int     start, end;
     struct stat buf;

     filename[0] = '\0';


     ulmenu[0] = REQitemnew();
     ulmenu[0]->prompt = "Supply file information for Upload";
     ulmenu[0]->thing  = CUR_LABEL;

     ulmenu[1] = REQitemnew();
     ulmenu[1]->prompt = "";
     ulmenu[1]->thing = CUR_LABEL;
     
     ulmenu[2] = REQitemnew();
     ulmenu[2]->thing = CUR_CHOICE;
     ulmenu[2]->prompt = "Download Protocol";
     ulmenu[2]->choices = DLnames;
     ulmenu[2]->chooseitem = 0;

     ulmenu[3] = REQitemnew();
     ulmenu[3]->thing = CUR_CHOICE;
     ulmenu[3]->prompt = "Type of File";
     ulmenu[3]->choices = textorbin;
     ulmenu[3]->chooseitem = 0;

     ulmenu[4] = REQitemnew();
     ulmenu[4]->prompt = "Save file as";
     ulmenu[4]->thing = CUR_PROMPT;
     ulmenu[4]->stowage = filename;

     ulmenu[5] = NULL;

     if (!CURrequester(CursesScreen, "Upload File", ulmenu)) {
	  ulprotocol = ulmenu[2]->chooseitem;
	  istext     = ulmenu[3]->chooseitem;
	  iszmodem   = (ulprotocol == 0);

	  if (!iszmodem && filename[0] == '\0') {
	       CursesErrorMsg("You must supply a filename");
	       return;
	  }
	  
	  if (istext) {
	       strcpy(cmdline, ULtextcmds[ulprotocol]);
	  } else {
	       strcpy(cmdline, ULbincmds[ulprotocol]);
	  }
	  
	  if (!iszmodem) {
	       strcat(cmdline, filename);
	  }

	  CURexit(CursesScreen);
	  start = time(NULL);

	  if (ulprotocol == 4) { /* Text */
	       FileIO *fio;
	       char inputline[256];
	       
	       printf("Send your text now, enter CTRL-D or a single period to complete upload\n");
	       fio = FIOopenUFS(filename, O_WRONLY|O_CREAT, 0600);
	       if (fio == NULL) {
		    printf("Couldn't open the file %s for writing\n", filename);
		    CURenter(CursesScreen);
		    return;
	       }
	       while(fgets(inputline,sizeof(inputline), stdin)) {
		    ZapCRLF(inputline);
		    if (strcmp(inputline, ".") == 0) 
			 break;
		    FIOwritestring(fio, inputline);
	       }
	       FIOclose(fio);
	  } else {
	       printf("Executing the following command\n%s\n",cmdline);

	       if (FIOsystem(cmdline))
		    printf("Upload failed, sorry...\n");
	  }
	 
	  end = time(NULL);

	  if (stat(filename, &buf) < 0)
	       printf("Upload failed, try again...\n");
	  else {
	       printf(Gtxt("Upload complete. %d total bytes, %d bytes/sec\n",182),
		      (int)buf.st_size, ((int)buf.st_size)/(end-start));
	  }
	  printf(Gtxt("Press <RETURN> to continue",121));

	  fflush(stdout);
	  getchar();
	  CURenter(CursesScreen);
     }
     return;
}

     
