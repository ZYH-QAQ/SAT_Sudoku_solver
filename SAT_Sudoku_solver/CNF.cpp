#include "CNF.h"
extern CNF cnf;






void CNF::readFile(char* filename,int& varNum,int& clsNum)
{
    FILE* fp = fopen(filename,"r");
    char buffer[1024];
    while(fgets(buffer,1024,fp)[0]=='c');
    varNum=0;clsNum=0;
    if(buffer[0]=='p')
    {
        int i=0;
        for(;buffer[i]>'9'||buffer[i]<'0';i++);
        for(;buffer[i]!=' ';i++)
        {
            varNum=varNum*10+(int)(buffer[i]-'0');
        }
        i++;
        for(;buffer[i]!='\0'&&buffer[i]!='\r'&&buffer[i]!='\n'&&buffer[i]!=' ';i++)
        {
            clsNum=clsNum*10+(int)(buffer[i]-'0');
        }
    }
    for(int i=0;i<clsNum;i++)
    {
        newClause();
        int variable;
        do
        {
            fscanf(fp,"%d",&variable);
            if(variable!=0)
            {
                addToClause(variable);
            }
        } while (variable!=0);
        
    }
}
void CNF::printCNF()
{
    int count=0;
    printf("head");
    for(ClauseElement* e=this->head->e1;e!=0;e=e->e1)
    {
        count++;
        printf("\n  |\n  |\nnum=%d",e->variable);
        for(ClauseElement* e2=e->e2;e2!=0;e2=e2->e1)
        {
            printf(" --> %d",e2->variable);
        }
    }
    printf("\nClauseNum=%d",count);
    printf("\n\n");
}
void CNF::newClause()
{
    ClauseElement* e=new ClauseElement();
    e->e1=this->head->e1;
    e->e2=0;
    e->isFirst=true;
    e->variable=0;
    this->head->e1=e;
    e->e0=this->head;
    if(e->e1) e->e1->e0=e;
}
void CNF::retrieve(ClauseElement* e)
{
    if(e->isFirst)
    {
        e->e0->e1=e;
        if(e->e1) e->e1->e0=e;
//cnf.printCNF();
//system("cls");
    }
    else
    {
        if(!e->e2->isFirst)
        e->e2->e1=e;
        else e->e2->e2=e;
        if(e->e1) e->e1->e2=e;
        e->e0->variable++;
    }
}
ClauseElement* CNF::judgeEachClause(ClauseElement* aim,int variable,int& status)
{
    status=0;
    ClauseElement* returner=0;
    for(ClauseElement* e =aim->e2;e;e=e->e1)
    {
        if(e->variable==variable)
        {
            status=1;
            return aim;
        }
        else if(e->variable==-variable)
        {
            status=-1;
            returner=e;
        }
    }
    return returner;
}
void CNF::unemployClause(ClauseElement* aim,AbStack& abstack)
{
    if(aim==0)
    {
        throw std::runtime_error("You cannot unemploy a clause from an empty CNF.\n");
    }
    aim->e0->e1=aim->e1;
    if(aim->e1) aim->e1->e0=aim->e0;
    abstack.push(aim);
}
void CNF::unemployVariable(ClauseElement* e,AbStack& abstack)
{
    e->e0->variable--;
    if(!e->e2->isFirst)
    e->e2->e1=e->e1;
    else e->e2->e2=e->e1;
    if(e->e1) e->e1->e2=e->e2;
    abstack.push(e);
}
void CNF::addToClause(int variable,ClauseElement* aim)
{
    ClauseElement* e=new ClauseElement();
    e->variable=variable;
    aim->variable++;
    e->e1=aim->e2;
    if(e->e1) e->e1->e2=e;
    e->e2=aim;
    aim->e2=e;
    e->e0=aim;
}
void CNF::addToClause(int variable)
{
    ClauseElement* e=new ClauseElement();
    e->variable=variable;
    this->head->e1->variable++;
    e->e1=this->head->e1->e2;
    if(this->head->e1->e2)
        e->e1->e2=e;
    e->e2=this->head->e1;
    this->head->e1->e2=e;
    e->e0=this->head->e1;
}
bool CNF::ifEmpty()
{
    if(this->head->e1==0) return 1;
    else return 0;
}
bool CNF::ifEmptyClause()
{
    for(ClauseElement* e=this->head->e1; e; e=e->e1)
    {
        if(e->variable==0) return true;
    }
    return false;
}

int CNF::getSingleClause()
{
    for(ClauseElement* e=this->head->e1;e;e=e->e1)
    {
        if(e->variable==1) return e->e2->variable;
    }
    return 114514;
}
void CNF::backTrace(IntStack& intstack,AbStack& abstack)
{
    int numOfClausesFromDtoC=intstack.pop();
    for(int i=0;i<numOfClausesFromDtoC;i++)
    {
        ClauseElement* e=abstack.pop();
        retrieve(e);
    }
}
void CNF::imposeSCR(CNF& cnf,bool* results,int variable,AbStack& abstack,IntStack& intstack)
{
    ClauseElement * eNext=cnf.head->e1;
    int abandon=0,status;
    bool isfound=false;
    for(ClauseElement* e=cnf.head->e1;e;e=eNext)
    {
        eNext=e->e1;
        ClauseElement* temp=judgeEachClause(e,variable,status);
        if(status==1)
        {
            unemployClause(temp,abstack);
            isfound=true;
            abandon++;
        }
        else if(status==-1)
        {
            
            unemployVariable(temp,abstack);
            isfound=true;
            abandon++;
        }
    }
    
    intstack.push(abandon);
    int absVariable=variable>0?variable:-variable;
    if(isfound)
    results[absVariable]=(absVariable==variable);
}
void unemployClause_(ClauseElement* aim)
{
    if(aim==0)
    {
        throw std::runtime_error("You cannot unemploy a clause from an empty CNF.\n");
    }
    aim->e0->e1=aim->e1;
    if(aim->e1) aim->e1->e0=aim->e0;
    ClauseElement* enext;
    for(ClauseElement*e=aim->e2;e;e=enext)
    {
        enext=e->e1;
        delete(e);
    }
    delete(aim);
}
void unemployVariable_(ClauseElement* e)
{
    e->e0->variable--;
    if(!e->e2->isFirst)
    e->e2->e1=e->e1;
    else e->e2->e2=e->e1;
    if(e->e1) e->e1->e2=e->e2;
    delete(e);
}
void CNF::imposeSCR(CNF& cnf,bool* results,int variable)
{
    ClauseElement * eNext=cnf.head->e1;
    int abandon=0,status;
    bool isfound=false;
    for(ClauseElement* e=cnf.head->e1;e;e=eNext)
    {
        eNext=e->e1;
        ClauseElement* temp=judgeEachClause(e,variable,status);
        if(status==1)
        {
            unemployClause_(temp);
            isfound=true;
            abandon++;
        }
        else if(status==-1)
        {
            
            unemployVariable_(temp);
            isfound=true;
            abandon++;
        }
    }
    int absVariable=variable>0?variable:-variable;
    if(isfound)
    results[absVariable]=(absVariable==variable);
}
bool CNF::judgeStorage()
{
    for(ClauseElement* e=this->head->e1; e; e=e->e1)
    {
        int num=e->variable;
        for(ClauseElement* e2=e->e2; e2; e2=e2->e1)
        {
            num--;
            if(e2->e2==0) return false;
        }
        if(num!=0) return false;
    }
    return true;
}
CNF* CNF::copyTree()
{
    CNF* newTree=new CNF();
    for(ClauseElement* e=this->head->e1;e;e=e->e1)
    {
        newTree->newClause();
        for(ClauseElement* e2=e->e2;e2;e2=e2->e1)
        {
            newTree->addToClause(e2->variable);
        }
    }
    return newTree;
}
void CNF::deleteTree()
{
    if(this->head==0) return;
    ClauseElement* enext;
    for(ClauseElement* e=this->head->e1;e;e=enext)
    {
        enext=e->e1;
        ClauseElement* e2next;
        for(ClauseElement* e2=e->e2;e2;e2=e2next)
        {
            e2next=e2->e1;
            delete(e2);
        }
        delete(e);
    }
    this->head=0;
}
void CNF::originize(AbStack& abstack,IntStack& intstack)
{
    while(intstack.stackTop>0)
    {
        int numOfClausesFromDtoC=intstack.pop();
        for(int i=0;i<numOfClausesFromDtoC;i++)
        {
            ClauseElement* e=abstack.pop();
            retrieve(e);
        }
    }
}