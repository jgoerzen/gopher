#include "GGroup.h"
#include "String.h"		/* For strcmp */
#include "Malloc.h"		/* For malloc */
#include <stdio.h>
#include "String.h"
#include "Dirent.h"
#include "Stat.h"

#ifndef NO_AUTHENTICATION

GGroup*
GGnew(void)
{
     GGroup *temp;

     temp = NewMEM(GGroup);

     temp->groupname = STRnew();
     temp->users     = RBTnew(strcmp, free);

     return(temp);
}


void
GGdestroy(GGroup *gg)
{
     STRdestroy(gg->groupname);
     RBTdestroy(gg->users);
}


void
GGadduser(GGroup *gg, char *user)
{
     RBTinsert(gg->users, user);
}

int
GGcmp(GGroup *gg1, GGroup *gg2)
{
     if (gg1 == NULL)
	  return(-1);
     if (gg2 == NULL)
	  return(1);
     return(strcmp(GGgetGroupName(gg1), GGgetGroupName(gg2)));
}

static void
GGfromFile(GGroup *gg, FILE *gfile)
{
     char myline[64]; /** Assume one user per line, for now.. **/
     char *cp;

     while (fgets(myline, sizeof(myline), gfile)) {
	  cp = strchr(myline, '\n');
	  if (cp != NULL)	/* Chop the LF off */
	       *cp = '\0';

	  GGadduser(gg, strdup(myline));
     }
}

  
GGroup*
GGnewFromFile(char *filename, char *groupname)
{
     FILE *gfile;
     GGroup *gg;

     gfile = fopen(filename, "r");
     gg    = GGnew();

     if (gfile == NULL || gg == NULL)
	  return(NULL);

     GGsetGroupName(gg, groupname);

     GGfromFile(gg, gfile);

     fclose(gfile);

     return(gg);
}



AUTHresult
GGtestuser(GGroup *gg, char *username)
{
     char *result;

     if (gg == NULL || gg->users == NULL)
	  return(AUTHRES_SYSERR);


     result = RBTfind(gg->users, username);
     
     if (result != NULL)
	  return(AUTHRES_OK);
     else
	  return(AUTHRES_NOGROUP);
}


/**********************************************************************
 * Functions for a tree of groups..
 */

GGroupTree *
GGTnew(void)
{
     return(RBTnew(GGcmp, GGdestroy));
}

void
GGTdestroy(GGroupTree *ggt)
{
     RBTdestroy(ggt);
}

GGroup *
GGTfindGG(GGroupTree *ggt, char *group)
{
     GGroup *result;
     GGroup *searchtemplate;

     searchtemplate = GGnew();
     GGsetGroupName(searchtemplate, group);

     result = (GGroup *) RBTfind(ggt, (char*)searchtemplate);
     
     return(result);

}

boolean
GGTtestuser(GGroupTree *ggt, char *group, char *user)
{
     GGroup *thegroup;
     
     thegroup = GGTfindGG(ggt, group);
     if (thegroup == NULL)
	  return(FALSE);

     return(GGtestuser(thegroup, user));
     
}

void
GGTinsert(GGroupTree *ggt, GGroup *gg)
{
     RBTinsert(ggt, (char*) gg);
}


/* Tree of groups to use in gopherd... */
static GGroupTree   *GlobalGroups = NULL; 


boolean
GGTfromDir(char *dirname)
{
     DIR                *thedir;
     struct dirent      *dp;
     GGroup             *group;
     struct stat        buf;

     if (chdir(dirname) < 0) {
 	  fprintf(stderr, "Cannot find group directory %s\n", dirname);
	  return(FALSE);
     }

     thedir = opendir(".");
     
     if (thedir == NULL)
	  return(FALSE);

     if (GlobalGroups == NULL)
	  GlobalGroups = GGTnew();

     
     for (dp = readdir(thedir); dp != NULL; dp = readdir(thedir)) {
	  /** Only do regular files.. **/
	  if (stat(dp->d_name, &buf) == 0 && S_ISREG(buf.st_mode)) {
	       
	       group = GGnewFromFile(dp->d_name, dp->d_name);
	       if (group == NULL)
		    return(FALSE);
	  
	       GGTinsert(GlobalGroups, group);
	  }
     }
     closedir(thedir);
     return(TRUE);
}


/*
 * Same as GGTtestuser, except uses static group tree ..
 */

boolean
GGTtest(char *group, char *user)
{
     return(GGTtestuser(GlobalGroups, group, user));
}
#endif
