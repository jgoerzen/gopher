/********************************************************************
 * $Author: s2mdalle $
 * $Revision: 1.2 $
 * $Date: 2001/01/03 22:29:00 $
 * $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopher/form.h,v $
 * $State: Exp $
 *
 * Paul Lindner, University of Minnesota CIS.
 *
 * Copyright 1991, 92, 93, 94 by the Regents of the University of Minnesota
 * see the file "Copyright" in the distribution for conditions of use.
 *********************************************************************
 * MODULE: form.h
 * Abstraction of form definition and management functions
 *********************************************************************
 * Revision History:
 * $Log: form.h,v $
 * Revision 1.2  2001/01/03 22:29:00  s2mdalle
 * Compiler warning fixes, includes, etc.
 *
 * Revision 1.1.1.1  2000/08/19 00:28:56  jgoerzen
 * Import from UMN Gopher 2.3.1 after GPLization
 *
 * Revision 3.3  1995/11/03  21:18:17  lindner
 * ANSIfication
 *
 * Revision 3.2  1994/07/03  23:10:58  lindner
 * fix forms..
 *
 * Revision 3.1  1994/02/20  16:15:46  lindner
 * New form definition and management functions
 *
 *
 *********************************************************************/

#include "String.h"
#include "DAarray.h"
#include "STAarray.h"
#include "boolean.h"
#include "CURcurses.h"

/*
 * An ITEM is a single UI control type thing..  Examples include labels,
 * choose items, etc..
 */

typedef enum {ITEM_UNINIT, ITEM_LABEL, ITEM_PROMPT, ITEM_CHOICE,
	      ITEM_SELECT, ITEM_PASSWD, ITEM_LONG, ITEM_FILENAME} 
ITEMtype;

typedef struct ITEM_struct {
     ITEMtype type;
     String   *label;
     String   *response;

     int      chooseitem;
     StrArray *choices;
} ITEM;


#define ITEMsetPrompt(f,l)    (STRset((f)->label,(l)))
#define ITEMgetPrompt(f)      (STRget((f)->label))

#define ITEMgetType(f)       ((f)->type)
#define ITEMsetType(f,t)     ((f)->type = (t))

#define ITEMsetLabel(f,l)    (STRset((f)->label,(l)))
#define ITEMgetLabel(f)      (STRget((f)->label))

#define ITEMgetResponse(f)   (STRget((f)->response))
#define ITEMsetResponse(f,r) (STRset((f)->response,(r)))

#define ITEMgetChoice(f)     ((f)->chooseitem)
#define ITEMsetChoice(f,c)   ((f)->chooseitem=(c))

#define ITEMgetChoiceNum(f,i)   (STAgetText((f)->choices,i))
#define ITEMgetNumChoices(i) (STAgetTop((i)->choices))  

ITEM *ITEMnew(void);
void  ITEMinit(ITEM *item);
ITEM *ITEMcpy(ITEM *dest, ITEM *orig);
void  ITEMdestroy(ITEM *item);
void  ITEMpushChoice(ITEM *item, char *choice);

/********************************************************************
 * 
 * A FORM is an array of items
 *
 */

typedef DynArray FORM;
#define FORMnew(a)        (DAnew((a),ITEMnew,ITEMinit,ITEMdestroy,ITEMcpy))
#define FORMinit(a)       (DAinit((a)))
#define FORMgetTop(a)     (DAgetTop(a))
#define FORMgetEntry(a,b) (ITEM*)(DAgetEntry(a,b))

#define FORMpush(a,b)     (DApush((DynArray*)(a),(b)))
#define FORMpop(a)        (FORM*)(DApop(a))
#define FORMdestroy(a)    (DAdestroy(a))
#define FORMcpy(a,b)      (DAcpy(a,b))

int CURform(CursesObj *cur, char *Wintitle, FORM *form);

boolean FORMaddLabel(FORM *form, char *label);
boolean FORMaddPrompt(FORM *form, char *prompt, char *defval);
boolean FORMaddPasswd(FORM *form, char *prompt, char *defval);
boolean FORMaddFilechoice(FORM *form, char *prompt, char *defval);
boolean FORMaddLong(FORM *form, char *prompt, char *defval);
boolean FORMaddChoice(FORM *form, char *prompt, char **choices, int defval);
boolean FORMaddSelect(FORM *form, char *prompt, int defval);
char    **FORMdoAskBlock();



