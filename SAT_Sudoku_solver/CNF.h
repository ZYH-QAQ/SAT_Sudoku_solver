#include <malloc.h>
#include <stdio.h>
#include <stdexcept>
#include <string>
struct ClauseElement{
    int variable=0;
    struct ClauseElement* e0=0;//point to the previous row;//pointer to the clausehead;
    struct ClauseElement* e1=0;//point to the next row or next element in the same row;
    struct ClauseElement* e2=0;//for the first element,point to the next element in the same row,else point to the previous element in the same row
    bool isFirst=0;
};
struct AbStack{
    ClauseElement* stack[100000]={0};
    int stackTop=0;
    void push(ClauseElement* e);
    ClauseElement* pop();
    ClauseElement* peek();
};
struct IntStack{
    int stack[100000]={0};
    int stackTop=0;
    void push(int e);
    int pop();
    int peek();
};
struct CNF{
ClauseElement* head=new ClauseElement();
void readFile(char* filename,int& varNum,int& clsNum);
void printCNF();
void newClause();//creat an empty clause;
//e contains the original information from the original clause;
//this function puts e back to its original clause;
static void retrieve(ClauseElement* e);//adopt an existing clause;
//judge if the clause has the variable: return 1, doesn't have the variable: return 0, has -variable: return -1;
static ClauseElement* judgeEachClause(ClauseElement* aim,int variable,int& status);
//abandon a clause to abstack;
static void unemployClause(ClauseElement* e,AbStack& abstack);//unemploy an existing clause;
//abandon a variable to abstack;
static void unemployVariable(ClauseElement* e,AbStack& abstack);
static void Abandon(ClauseElement* e,AbStack& abstack);//transfer a clause from cnf to dnf;
static void addToClause(int variable,ClauseElement* aim);
void addToClause(int variable);
//judge whether the CNF is empty
bool ifEmpty();
//judge whether the CNF has an empty clause
bool ifEmptyClause();
//return the singleClause variable;
int getSingleClause();
static void backTrace(IntStack& intstack,AbStack& abstack);
static void imposeSCR(CNF& cnf,bool* results,int variable,AbStack& abstack,IntStack& intstack);
static void imposeSCR(CNF& cnf,bool* results,int variable);
bool judgeStorage();
void deleteTree();
CNF* copyTree();
void originize(AbStack& abstack,IntStack& intstack);
};











