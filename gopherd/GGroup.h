
#include "STRstring.h"
#include "RBTree.h"
#include "AUTH.h"

struct GGroup_struct {
     String *groupname;
     RBTree *users;
};

typedef struct GGroup_struct GGroup;

#define GGsetGroupName(a,b) (STRset((a)->groupname, b))
#define GGgetGroupName(a)   (STRget((a)->groupname))


/** Protos..  **/
GGroup*     GGnew(void);
void        GGdestroy(GGroup *gg);
void        GGadduser(GGroup *gg, char *user);
GGroup*     GGnewFromFile(char *filename, char *groupname);
AUTHresult  GGtestuser(GGroup *gg, char *username);

/***********************************************************************
 * A tree of groups
 */

typedef RBTree GGroupTree;
GGroupTree *GGTnew(void);
void        GGTdestroy(GGroupTree *ggt);
boolean     GGTtest(char *group, char *user);
boolean     GGTfromDir(char *dirname);


