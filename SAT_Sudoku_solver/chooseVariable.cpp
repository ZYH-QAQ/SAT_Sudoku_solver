#include "chooseVariable.h"
#include <windows.h>
VPLhandler::VPLhandler(CNF& cnf,int varNum)
{
    int* countEachVariable=(int*)malloc(sizeof(int)*varNum*3);
    memset(countEachVariable,0,sizeof(int)*varNum);
    for(ClauseElement* e=cnf.head->e1;e;e=e->e1)
    {
        for(ClauseElement* e1=e->e2;e1;e1=e1->e1)
        {
            countEachVariable[e1->variable>0?e1->variable:-e1->variable]++;
        }
    }
    for(int i=0;i<varNum;i++)
    {
        int max=0,maxIndex=1;
        for(int j=1;j<=varNum;j++)
        {
            if(countEachVariable[j]>max)
            {
                max=countEachVariable[j];
                maxIndex=j;
            }
        }
        countEachVariable[maxIndex]=0;
        this->variables_priority_list[i]=maxIndex;
    }
}
int VPLhandler::getVariable()
{
    return this->variables_priority_list[this->var_cursor++];
}
int VPLhandler::getMVariable(CNF& cnf,int varNum)
{
    int countEachVariable[40000];
    memset(countEachVariable,0,sizeof(int)*40000);
    int count=0;
    for(ClauseElement* e=cnf.head->e1;e;e=e->e1)
    {
        count++;
        if(count==125)
        {
            int a=1;
        }
        for(ClauseElement* e1=e->e2;e1;e1=e1->e1)
        {
            countEachVariable[e1->variable>0?e1->variable:-e1->variable]++;
        }
    }

        int max=0,maxIndex=1;
        for(int j=1;j<=varNum;j++)
        {
            if(countEachVariable[j]>max)
            {
                max=countEachVariable[j];
                maxIndex=j;
            }
        }
    return maxIndex;
}
int VPLhandler::getSVariable(CNF& cnf)
{
    int s_size=INT_MAX,s_var=0;
    for(ClauseElement* e=cnf.head->e1;e;e=e->e1)
    {
        if(e->variable<s_size)
        {
            s_size=e->variable;
            s_var=e->e2->variable;
        }
    }
    return s_var;
}
int VPLhandler::getFirst(CNF& cnf)
{
    Sleep(40);
    return cnf.head->e1->e2->variable;
}
int VPLhandler::getFixedMVariable()
{
    return this->variables_priority_list[this->var_cursor++];
}
void VPLhandler::backTrace()
{
    this->var_cursor--;
}
void VPLhandler::print(int Varnum)
{
    for(int i=0; i<Varnum; i++)
    {
        printf("%d ",this->variables_priority_list[i]);
    }
    printf("\n");
}