/********************************************************************
 * $Author: jgoerzen $
 * $Revision: 1.3 $
 * $Date: 2001/01/17 21:48:05 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/form.c,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 92, 93, 94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: form.c
 * Form definition and management functions
 *********************************************************************
 * Revision History:
 * $Log: form.c,v $
 * Revision 1.3  2001/01/17 21:48:05  jgoerzen
 * Many fixes and tune-ups.  Now compiles cleanly with -Wall -Werror!
 *
 * Revision 1.2  2001/01/03 22:28:24  s2mdalle
 * Code cleanups, compiler warning fixes
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.9  1995/11/03  21:18:17  lindner
 * ANSIfication
 *
 * Revision 3.8  1995/05/01  03:42:48  lindner
 * Fixes for NetBSD
 *
 * Revision 3.7  1994/07/03  23:10:57  lindner
 * fix forms..
 *
 * Revision 3.6  1994/05/19  14:07:24  lindner
 * use fast malloc on VMS VAXC
 *
 * Revision 3.5  1994/04/01  02:24:56  lindner
 * Fix return types
 *
 * Revision 3.4  1994/03/08  15:55:05  lindner
 * gcc -Wall fixes
 *
 * Revision 3.3  1994/03/04  23:39:50  lindner
 * Fix for log entries
 *
 * Revision 3.2  1994/03/04  23:36:32  lindner
 * bug fixes.
 *
 * Revision 3.1  1994/02/20  16:15:44  lindner
 * New form definition and management functions
 *
 *
 *********************************************************************/

#include "form.h"

#include "String.h"
#include "GSgopherobj.h"
#include "BLblock.h"
#include "Malloc.h"
#include "CURcurses.h"
#include "gopher.h"

/** Make a new item... **/

ITEM *
ITEMnew(void)
{
     ITEM *temp;

     temp = (ITEM*) malloc(sizeof(ITEM));

     temp->type     = ITEM_UNINIT;
     temp->label    = STRnew();
     temp->response = STRnew();

     temp->chooseitem = -1;
     temp->choices  = STAnew(3);

     return(temp);
}


/*
 * Clear out a specific item 
 */

void
ITEMinit(ITEM *item)
{
     item->type = ITEM_UNINIT;

     STRinit(item->label);
     STRinit(item->response);
     STAinit(item->choices);
     item->chooseitem=0;
}

/*
 * Copy an item
 */

ITEM*
ITEMcpy(ITEM *dest, ITEM *orig)
{
     dest->type  = orig->type;
     STRset(dest->label, STRget(orig->label));
     STRset(dest->response, STRget(orig->response));

     dest->chooseitem = orig->chooseitem;
     STAcpy(dest->choices, orig->choices);

     return(dest);
}

/*
 * Nuke a defined ITEM
 */

void
ITEMdestroy(ITEM *item)
{
     STRdestroy(item->label);
     STRdestroy(item->response);

     STAdestroy(item->choices);
     free(item);
}

/*
 * Add an item to the list of choices..
 */

void
ITEMpushChoice(ITEM *item, char *choice)
{
     String *str;

     str = STRnew();

     STRset(str, choice);
     STApush(item->choices, str);
     
     STRdestroy(str);
}



/*************************************************************************
 * Form definition routines..  Pretty swanky..
 */

FORM*
FORMfromASK(GopherObj *gs)
{
     int Asknum;
     Blockobj *bl;
     char askline[256];
     char *defaultval;
#if 0
     int i;
     char ** responses = NULL;
     ITEM *item;
#endif /* 0 */
     FORM *form;

     GSgetginfo(gs, TRUE);
	  
     bl = GSfindBlock(gs, "ASK");

     if (bl == NULL)
	  return(NULL);

     form = FORMnew(BLgetNumLines(bl));

     for (Asknum=0; Asknum <BLgetNumLines(bl); Asknum++) {
	  char *askprompt, *cp;

	  strcpy(askline, BLgetLine(bl, Asknum));

	  /*** find the type of question ***/
	  askprompt = strchr(askline, ':');

	  if (askprompt == NULL) {
	       /* Empty line crashes CURRequest unless do this*/
	       FORMaddLabel(form, "");
	       continue;
	  }
	  *(askprompt+1) = '\0';
	  askprompt+=2;
	  
	  /*** Zap the tabs, and load it up.. ***/
	  cp = strchr(askprompt, '\t');
	  if (cp != NULL) {
	       defaultval = cp+1;
	       *cp = '\0';
	  } else
	       defaultval = NULL;

	  
	  if (strncasecmp(askline, "Note:", 5) == 0)
	       FORMaddLabel(form, askprompt);

	  else if (strncasecmp(askline, "Choose:", 7) == 0) {
	       int cnum = 0;
	       char **choices;
	       
	       choices = (char**) malloc(sizeof(char*)*100);

	       /*** add list of choices to struct **/
	       while ((cp = strchr(defaultval, '\t')) != NULL) {
		    *cp = '\0';
		    choices[cnum++] = defaultval;
		    defaultval = cp+1;
	       }
	       if (defaultval != NULL)
		    choices[cnum++] = strdup(defaultval);

	       choices[cnum++] = NULL;
	       
	       FORMaddChoice(form, askprompt, choices, 0);
	  }

	  else if (strncasecmp(askline, "Select:", 7) == 0) {
	       int chooseitem = 0;

	       cp = strrchr(askprompt, ':');
	       if (cp != NULL) {
		    *cp = '\0';
		    cp++;
		    if (*cp == '1')
			 chooseitem = 1;
	       }
	       
	       FORMaddSelect(form, askprompt, chooseitem);
	  }
	  else if (strncasecmp(askline, "AskP:", 5)==0)
	       FORMaddPasswd(form, askprompt, defaultval);
	  else if (strncasecmp(askline, "AskL:", 5)==0)
	       FORMaddLong(form, askprompt, defaultval);
	  else if (strncasecmp(askline, "Choosef:", 8) == 0)
	       FORMaddFilechoice(form, askprompt, defaultval);
	  else
	       FORMaddPrompt(form, askprompt, defaultval);
     }

     return(form);
}

char**
FORMgetAskdata(GopherObj *gs, FORM *form, int Asknum)
{
     char **responses = NULL;
     int i;
     ITEM *item;


     if (!CURform(CursesScreen, GSgetTitle(gs), form)) {
	  int respnum = 0;

	  responses = (char**) malloc(sizeof(char*)*(1+Asknum));

	  for (i=0; i <Asknum; i++) {
	       item = FORMgetEntry(form, i);

	       switch (ITEMgetType(item)) {
	       case ITEM_LONG:
		    responses[respnum++] = strdup("1");
		    responses[respnum++] = strdup(ITEMgetResponse(item));
		    break;

	       case ITEM_SELECT:
		    if (ITEMgetChoice(item) == 0)
			 responses[respnum++] = strdup("0");
		    else
			 responses[respnum++] = strdup("1");

		    break;

	       case ITEM_CHOICE:
		    responses[respnum++] = strdup(ITEMgetChoiceNum(item, ITEMgetChoice(item)));
		    break;
	       case ITEM_PROMPT:
	       case ITEM_PASSWD:
		    responses[respnum++] = strdup(ITEMgetResponse(item));
               case ITEM_UNINIT: /* Unhandled cases... */
               case ITEM_LABEL:
               case ITEM_FILENAME:
                 break;
	       }
	  }
	  responses[respnum++] = NULL;
     }
     /*** Free memory ***/
     FORMdestroy(form);

     return(responses);
}


boolean
FORMaddLabel(FORM *form, char *label)
{
     ITEM *item;

     item = ITEMnew();

     ITEMsetType(item, ITEM_LABEL);
     ITEMsetLabel(item, label);
     
     FORMpush(form, (char *) item);

     ITEMdestroy(item);

     return(TRUE);
}     


boolean
FORMaddPrompt(FORM *form, char *prompt, char *defval)
{
     ITEM *item;

     item = ITEMnew();
     ITEMsetType(item, ITEM_PROMPT);
     if (prompt != NULL)
	  ITEMsetLabel(item, prompt);
     else
	  ITEMsetLabel(item, "");

     if (defval != NULL)
	  ITEMsetResponse(item, defval);
     else 
	  ITEMsetResponse(item, "");

     FORMpush(form, (char *)item);

     ITEMdestroy(item);

     return(TRUE);
}     

boolean
FORMaddPasswd(FORM *form, char *prompt, char *defval)
{
     FORMaddPrompt(form, prompt, defval);
     ITEMsetType(FORMgetEntry(form, FORMgetTop(form)-1), ITEM_PASSWD);

     return(TRUE);
}     

boolean
FORMaddFilechoice(FORM *form, char *prompt, char *defval)
{
     FORMaddPrompt(form, prompt, defval);
     ITEMsetType(FORMgetEntry(form, FORMgetTop(form)-1), ITEM_FILENAME);

     return(TRUE);
}     


boolean
FORMaddLong(FORM *form, char *prompt, char *defval)
{
     FORMaddPrompt(form, prompt, defval);
     ITEMsetType(FORMgetEntry(form, FORMgetTop(form)-1), ITEM_LONG);

     return(TRUE);
}     

/*
 * add a multiple choice item..
 */

boolean
FORMaddChoice(FORM *form, char *prompt, char **choices, int defval)
{
     ITEM *item;
     int i=0;

     FORMaddPrompt(form, prompt, "");
     item = FORMgetEntry(form, FORMgetTop(form)-1);
     
     ITEMsetChoice(item, defval);
     ITEMsetType(item, ITEM_CHOICE);

     while (choices[i] != NULL) {
	  ITEMpushChoice(item, choices[i]);
	  i++;
     }

     return(TRUE);
}


boolean
FORMaddSelect(FORM *form, char *prompt, int defval)
{
     ITEM *item;

     FORMaddPrompt(form, prompt, "");
     item = FORMgetEntry(form, FORMgetTop(form)-1);
     
     ITEMsetChoice(item, defval);

     ITEMpushChoice(item, "No");
     ITEMpushChoice(item, "Yes");
     ITEMsetType(item, ITEM_SELECT);

     return(TRUE);
}



/*
 * Display a form and allow a user to fill it out.
 */

int
CURform(CursesObj *cur, char *Wintitle, FORM *form)
{
     WINDOW *tempwin;
     int  i,j;
     int  totalprompts;
     int  numprompts=0;
     int  maxpromptwidth =0;
     int  currentfield = 0;
     int  maxlength = COLS-7;
     char TrimmedTitle[128];
     ITEM *item;
     int  ch;
     int  numforms = 1;
     int  currentform = 0;
     int  pagesize = LINES-8;
     int  thisformdone = FALSE;


     /** Find the number of prompts... and the max width***/
     for (totalprompts = 0; totalprompts < FORMgetTop(form); totalprompts++) {
	  item = FORMgetEntry(form, totalprompts);
	  
	  /*** Skip non editable prompts ***/
	  if (item != NULL && ITEMgetType(item) != ITEM_LABEL) {
	       if (strlen(ITEMgetPrompt(item)) > maxpromptwidth)
		    maxpromptwidth = strlen(ITEMgetPrompt(item));
	  }

	  if (totalprompts != 0  && (totalprompts % pagesize) == 0)
	       numforms ++;
     }

     if (numforms > 1)
	  numprompts = pagesize;
     else
	  numprompts = totalprompts;

     if (numprompts == 0) {
	  return(-1);
     }
     
     maxlength -= (maxpromptwidth+1);
     tempwin = newwin(6 + numprompts, COLS-2, (LINES-(6+numprompts))/2,1);
     CURwenter(cur,tempwin);

     while (currentform < numforms) {
	  if (currentform == (numforms-1)) {
	       numprompts = totalprompts - (pagesize * currentform);
	  } else {
	       numprompts = pagesize ;
	  }
	  wstandend(tempwin);
	  CURbox(cur,tempwin, 6+numprompts, COLS-2);

	  currentfield = 0;

	  /*** Add the window title, centered ***/
	  if (Wintitle != NULL) {
	       /** Trim window title to fit in the window **/
	       if (strlen(Wintitle) > COLS-2) {
		    strncpy(TrimmedTitle, Wintitle, COLS-2);
		    TrimmedTitle[COLS-5] = '.';
		    TrimmedTitle[COLS-4] = '.';
		    TrimmedTitle[COLS-3] = '.';
		    TrimmedTitle[COLS-2] = '\0';
	       }
	       else
		    strcpy(TrimmedTitle, Wintitle);
	       
	       /** Put the title, bold **/
	       wmove(tempwin, 0,(COLS -2  - strlen(TrimmedTitle))/2);
	       wstandout(tempwin);
	       waddstr(tempwin, TrimmedTitle);
	       wstandend(tempwin);
	  }
	  
	  /** Add the prompts and typing area **/
	  for (i=0; i <numprompts; i++) {
	       
	       item = FORMgetEntry(form, i + currentform*pagesize);
	       
	       wmove(tempwin, 2+i, 2);
	       waddstr(tempwin, ITEMgetPrompt(item));
	       
	       switch (ITEMgetType(item)) {
		    
	       case ITEM_LABEL:
		    break;
		    
	       case ITEM_SELECT:
	       case ITEM_CHOICE:
		    /** Add the default **/
		    wmove(tempwin, 2+i, maxpromptwidth +4);
		    waddstr(tempwin, ITEMgetChoiceNum(item, ITEMgetChoice(item)));
		    
		    break;
	       default:
		    
		    /** Add the black space for the stowage, **/
		    /** and the stowage, if it exists        **/
	            wmove(tempwin, 2+i, maxpromptwidth +4);
		    wstandout(tempwin);
		    
		    if (ITEMgetType(item) == ITEM_PASSWD) {
			 int numchars = strlen(ITEMgetResponse(item));
			 
			 for (j=0; j<numchars; j++)
			      waddch(tempwin, '*');
		    } else
			 waddstr(tempwin, ITEMgetResponse(item));
		    
		    for (j=strlen(ITEMgetResponse(item))+maxpromptwidth+4; 
			 j< COLS-6; j++) {
			 waddch(tempwin, ' ');
		    }
		    wstandend(tempwin);
	       } /* End switch */
	  } /* End for */
	  
	  /** Add the labels, centered **/
	  wmove(tempwin, 3 + numprompts, (COLS-63)/2);
	  CURbutton(cur, tempwin, "Switch Fields: TAB", FALSE);
	  
	  CURbutton(cur, tempwin, "Cancel: ^G", FALSE);
	  waddch(tempwin, ' ');
	  CURbutton(cur, tempwin, "Erase: ^U", FALSE);
	  waddch(tempwin, ' ');
	  CURbutton(cur, tempwin, "Accept: Enter", FALSE);
	  
	  touchwin(tempwin);
	  wrefresh(tempwin);
	  
	  thisformdone = FALSE;

	  while (!thisformdone) {
	       boolean hidden;
	       int     oldchoice;
	       
	       item = FORMgetEntry(form, currentfield + currentform*pagesize);
	       
	       if (ITEMgetType(item) == ITEM_PASSWD)
		    hidden = TRUE;
	       else
		    hidden = FALSE;
	       
	       wmove(tempwin, 2+currentfield, maxpromptwidth +4);
	       
	       wrefresh(tempwin);
	       
	       if (ITEMgetType(item) == ITEM_CHOICE || 
		   ITEMgetType(item) == ITEM_SELECT) {
		    int choice = ITEMgetChoice(item);
		    int done = FALSE;
		    
		    wmove(tempwin, 4+numprompts, (COLS-22)/2);
		    CURbutton(cur, tempwin, "Cycle Values: SPACE", FALSE);
		    wmove(tempwin, 2+currentfield, maxpromptwidth +
			  strlen(ITEMgetChoiceNum(item, choice)) +4);
		    
		    wrefresh(tempwin);
		    
		    
		    while (!done) {
			 ch = CURgetch(cur);
			 oldchoice = choice;
			 if (ch == ' ')
			      choice ++;
			 else
			      done = TRUE;
			 
			 if (choice == ITEMgetNumChoices(item))
			      choice = 0;
			 
			 /*** Erase old choice.. ***/
			 wmove(tempwin, 2+currentfield, maxpromptwidth + 4);
			 
			 for (i=strlen(ITEMgetChoiceNum(item, oldchoice)); i>0; i--)
			      waddch(tempwin, ' ');
			 
			 wmove(tempwin, 2+currentfield, maxpromptwidth + 4);
			 
			 waddstr(tempwin, ITEMgetChoiceNum(item,choice));
			 ITEMsetChoice(item, choice);
			 wrefresh(tempwin);
		    } /* End while */
		    wmove(tempwin, 4+numprompts, (COLS-22)/2);
		    waddstr(tempwin, "                       ");
		    
	       } else if (ITEMgetType(item) == ITEM_LABEL)
		    ch = '\t';
	       else {
		    char tmpbuffer[256];
		    tmpbuffer[0] = '\0';

		    if (ITEMgetResponse(item) != NULL)
			 strcpy(tmpbuffer, ITEMgetResponse(item));

		    ch = CURwgetstr(cur,tempwin,ITEMgetResponse(item),
				    maxlength, hidden);

		    ITEMsetResponse(item, tmpbuffer);
	       } /* End else */

	       switch (ch) {
		    
	       case '\t':
	       case KEY_DOWN:
		    /*** Move to another field ***/
		    do {
			 currentfield = (currentfield +1) % numprompts;
		    } while (currentfield == numprompts || 
			     ITEMgetType(FORMgetEntry(form, currentfield + 
						      currentform * pagesize))
			     == ITEM_LABEL);
		    break;
		    
	       case KEY_UP:
		    do {
			 currentfield--;
			 if (currentfield <0)
			      currentfield = numprompts-1;
		    } while (ITEMgetType(FORMgetEntry(form, currentfield +
						      currentform * pagesize)) 
			      == ITEM_LABEL);
		    
		    break;
		    
	       case '\007':
	       case -1:
		    /*** Cancel ***/
		    delwin(tempwin);
		    return(-1);
		    
	       case '\n':
		    currentform++;
		    if (currentform == numforms) {
			 delwin(tempwin);
			 return(0);
		    } else {
			 wclear(tempwin);
			 thisformdone = TRUE;
		    }
		    break;
	       } /* End switch */
	  } /* End while */
     } /* End while */
     return 0;
} /* End CURform */




