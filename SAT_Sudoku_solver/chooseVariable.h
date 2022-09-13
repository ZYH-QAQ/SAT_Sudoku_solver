#include "CNF.h"
#include <string.h>
struct VPLhandler{
    int variables_priority_list[20000];
    //bool variables_used[1000]={0};
    int var_cursor=0;
    VPLhandler(CNF& cnf,int varNum);
    int getVariable();
    void backTrace();
    void print(int varNum);
    int getMVariable(CNF& cnf,int varNum);
    int getSVariable(CNF& cnf);
    int getFirst(CNF& cnf);
    int getFixedMVariable();
};
