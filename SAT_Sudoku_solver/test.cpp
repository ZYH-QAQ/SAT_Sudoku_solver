#pragma GCC optimize(2)
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "chooseVariable.h"
#include <windows.h>
#include "printResult.h"
#include <cstdlib>
DWORD start, end; //time
int loopCount=0;
int varNum,clsNum;
int strategy=1;
AbStack abStack;
IntStack intStack;
//bool result[3000];
double procedure=0;//store the percentage of procedure;
int depth_callNum[13]={0};
bool results[3000];
bool DPLL(CNF& cnf,int depth,VPLhandler& vpl,long long dxpdepth,double procedure);
bool DPLL_v2(int depth,int variable_currently,VPLhandler& vpl,CNF* cnf,long long dxpdepth,double procedure);
bool examine(CNF& test)
{
    for(ClauseElement* e=test.head->e1;e;e=e->e1)
    {
        bool judge=0;
        for(ClauseElement* e1=e->e2;e1;e1=e1->e1)
        {
            int abs=e1->variable>0?e1->variable:-e1->variable;
            if(results[abs]&&(e1->variable==abs)||!results[abs]&&(e1->variable!=abs))
            {judge=1;break;}
        }
        if(judge==0) 
        return false;
    }
    return true;
}
void normalize()
{
    for(int i=0;i<12;i++)
    {
        depth_callNum[i+1]=2*(depth_callNum[i]-1)>depth_callNum[i+1]?2*(depth_callNum[i]-1):depth_callNum[i+1];
    }
}
void printSDK(int* sdk)
{
    printf("\n");
    printf("-----------------\n");
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<9;j++)
        {
            if(sdk[81+9*i+j]!=0)
            printf("%d ",sdk[81+9*i+j]);
            else printf("  ");
        }
        printf("\n");
    }
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<6;j++)
        {
            if(sdk[81+54+6*i+j]!=0)
            printf("%d ",sdk[81+54+6*i+j]);
            else printf("  ");
        }
        for(int j=0;j<9;j++)
        {
            if(sdk[9*i+j]!=0)
            printf("%d ",sdk[9*i+j]);
            else printf("  ");
        }
        printf("\n");
    }
    for(int i=3;i<9;i++)
    {
        printf("            ");
        for(int j=0;j<9;j++)
        {
            if(sdk[9*i+j]!=0)
            printf("%d ",sdk[9*i+j]);
            else printf("  ");
        }
        printf("\n");
    }
    printf("            -----------------\n");
}
void index_to_pij(int index,int& pos,int& row,int& col)
{
    if(index<81)
    {
        pos=0;
        row=index/9;
        col=index%9;
        return;
    }
    index-=81;
    pos=1;
    if(index<54)
    {
        row=index/9;
        col=index%9;
        return;
    }
    index-=54;
    row=index/6+6;
    col=index%6;
    return;
}
int pij_to_index(int pos,int row,int col)
{
    if(!(pos==1&&row>=6))
    return pos*81+row*9+col;
    else if(col<6)
    return 81+54+6*(row-6)+col;
    else
    return (row-6)*9+col-6;
}
bool ifNotValid(int number,int index,int* sdk)
{
    int pos,row,col;
    index_to_pij(index,pos,row,col);
    for(int i=0;i<9;i++)
    {
        if(sdk[pij_to_index(pos,row,(col+i)%9)]==number||sdk[pij_to_index(pos,(row+i)%9,col)]==number) return true;
    }
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(sdk[pij_to_index(pos,row/3*3+i,col/3*3+j)]==number) return true;
        }
    }
    return false;
}
int main()
{
srand(start);
while(true)
{
system("cls");
printf("1.SAT\n2.sudoku\n0.exit\n\n");
int ab=0;
scanf("%d",&ab);
if(ab==0) break;
if(ab==1)
{
    //#pragma omp parallel
    system("cls");
    printf("Please input filename: \n");
    char fname[100],fresultName[100];
    scanf("%s",fname);
    strcpy(fresultName,fname);
    strcat(fname,".cnf");strcat(fresultName,".res");
    {
        FILE* ffp;
        if((ffp=fopen(fname,"r"))==0)
        {
            fclose(ffp);
            printf("File not found.\n");
            getchar();getchar();
            continue;
        }
        fclose(ffp);
    }
    printf("Which strategy would you like to use?\n");
    printf("1.Shortest clause.\n2.The first variable that appeared.\n3.The most appeared variable.\n4.Variables in order of appearing times from the beginning.\n5.The first variable that appeared (before optimized)\n");
    scanf("%d",&strategy);
    CNF cnf,test;
    cnf.readFile(fname,varNum,clsNum);
    test.readFile(fname,varNum,clsNum);
    //cnf.printCNF();
    VPLhandler vpl=VPLhandler(cnf,varNum);
    start = GetTickCount();
    bool finalResult=DPLL(cnf,1,vpl,2,0);
    end = GetTickCount();
    FILE* fresult=fopen(fresultName,"w");
    if(finalResult)
    {
        printf("\nexamine= %d",examine(test));
        fprintf(fresult,"s 1\n");
        fprintf(fresult,"v");
        for(int i=0;i<varNum;i++)
        {
            if(results[i]) fprintf(fresult," %d",i+1);
            else fprintf(fresult," %d",-i-1);
        }
        fprintf(fresult,"\n");
    }
    else if(loopCount==-1) {printf("\ntime exceeded\n");fprintf(fresult,"s -1\n");}
    else {printf("\nfalse\n");fprintf(fresult,"s 0\n");}
    printf("\ntime= %lld ms\n",end-start);
    printf("    = %.3f s\n\n",(end-start)/1000.0);
    fprintf(fresult,"t %d",end-start);
    fclose(fresult);
    printf("memory releasing...\n");
    cnf.deleteTree();test.deleteTree();
    printf("release complete");
    //printf("filled=%d\n\n",count);//should be 153
    //print_result(results); 
    getchar();
    getchar();
}
if(ab==2)
{
    system("cls");
    int sdk[200]={
    8 ,1 ,9 ,4 ,7 ,5 ,3 ,2, 6,
    2, 7, 4, 3, 9 ,6 ,1, 5 ,8,
    6, 3, 5, 1, 8, 2, 4, 9, 7,
    9, 4, 7, 8, 2 ,3, 6, 1, 5,
    3, 6, 2, 5, 4 ,1, 7, 8, 9,
    1, 5, 8, 7, 6, 9, 2, 3, 4,
    4, 8, 1, 2, 5, 7, 9, 6, 3,
    7, 9, 3, 6, 1, 8, 5, 4, 2,
    5, 2, 6, 9, 3, 4, 8, 7, 1,

    8, 6, 7, 5, 2, 9, 1, 4, 3,
    1, 3, 5, 7, 6, 4, 9, 2, 8,
    4, 2, 9, 1, 3, 8, 7, 5, 6,
    7, 4, 8, 9, 5, 2, 3 ,6, 1,
    3, 1, 6, 4, 8, 7, 5, 9, 2,
    9, 5, 2, 3, 1, 6, 4, 8, 7,
    6, 7, 3, 2, 4, 5,
    5, 8, 1, 6, 9, 3,
    2, 9, 4, 8, 7, 1
    };
    int sdk_answer[200]={
    8 ,1 ,9 ,4 ,7 ,5 ,3 ,2, 6,
    2, 7, 4, 3, 9 ,6 ,1, 5 ,8,
    6, 3, 5, 1, 8, 2, 4, 9, 7,
    9, 4, 7, 8, 2 ,3, 6, 1, 5,
    3, 6, 2, 5, 4 ,1, 7, 8, 9,
    1, 5, 8, 7, 6, 9, 2, 3, 4,
    4, 8, 1, 2, 5, 7, 9, 6, 3,
    7, 9, 3, 6, 1, 8, 5, 4, 2,
    5, 2, 6, 9, 3, 4, 8, 7, 1,

    8, 6, 7, 5, 2, 9, 1, 4, 3,
    1, 3, 5, 7, 6, 4, 9, 2, 8,
    4, 2, 9, 1, 3, 8, 7, 5, 6,
    7, 4, 8, 9, 5, 2, 3 ,6, 1,
    3, 1, 6, 4, 8, 7, 5, 9, 2,
    9, 5, 2, 3, 1, 6, 4, 8, 7,
    6, 7, 3, 2, 4, 5,
    5, 8, 1, 6, 9, 3,
    2, 9, 4, 8, 7, 1
    };
    int HoleNum=80;
    printf("Please fill in the hole number: \n");
    scanf("%d", &HoleNum);
    printf("Generating sudoku...\n");
    int holecnt=0;
    bool vis[500];
    for(int i=0;i<200;i++) vis[i]=0;
    while(holecnt<HoleNum)
    {
        int tar=rand()%153;
        if(vis[tar]==1) continue;
        else
        {
            vis[tar]=1;
            int Num=sdk[tar];
            bool sat=0;
            for(int testnum=1;testnum<=9;testnum++)
            {
                if(testnum==Num) continue;
                if(ifNotValid(testnum,tar,sdk)) continue;
                sdk[tar]=testnum;
                transform_sudoku_to_satFile(sdk);
                char fname[50]={'1','1','4','5','1','4','.','t','e','m','p','\0'};
                CNF cnf;VPLhandler vpl=VPLhandler(cnf,varNum);
                cnf.readFile(fname,varNum,clsNum);
                sat=DPLL_v2(1,0,vpl,&cnf,2,0);
                //cnf.deleteTree();
                if(sat==1) break;
            }
            if(sat==1)
            {
                sdk[tar]=Num;
                continue;
            }
            else
            {
                sdk[tar]=0;
                holecnt++;
                continue;
            }
        }
    }//dig complete
while(true)
{
    system("cls");
    printSDK(sdk);
    printf("\n\nPlease input your answers in format (1:upper 0:lower) (row) (column) (number), 0 0 0 0 for the answer\n");
    int pos,row,column,num;
    scanf("%d %d %d %d",&pos,&row,&column,&num);
    if(row==0&&column==0&&num==0)
    {
        system("cls");
        printf("Try next time!\n");
        printSDK(sdk_answer);
        break;
    }
    if(pos!=0&&pos!=1||row<1||row>9||column<1||column>9||num<1||num>9)
    {
        printf("InValid Input!\n");
        Sleep(1200);
        continue;
    }
    int index_ref=pij_to_index(pos,row-1,column-1);
    if(sdk_answer[index_ref]==num)
    {
        printf("correct!\n");
        Sleep(1200);
        sdk[index_ref]=num;
        holecnt--;
    }
    else
    {
        printf("incorrect!\n");
        Sleep(1200);
    }
    if(holecnt==0)
    {
        system("cls");
        printf("Congratulations!\n");
        printSDK(sdk);
        break;
    }
}
    getchar();
    getchar();
}
}
}
bool DPLL(CNF& cnf,int depth,VPLhandler& vpl,long long dxpdepth,double procedure)
{
    if(loopCount++>100)
    {
        DWORD current = GetTickCount();
        if(current-start>300000)//time limit 5 min
        {
            loopCount=-1;
            return false;
        }
        else loopCount=0;
    }
    fprintf(stderr,"\rprocedure=%lf%%",procedure*100);
    if(cnf.ifEmpty()) 
    return true;
    else if(cnf.ifEmptyClause())
    return false;
    int variable;
    int count=0;
    while((variable=cnf.getSingleClause())!=114514)
    {

        count++;
        CNF::imposeSCR(cnf,results,variable,abStack,intStack);
        if(cnf.ifEmpty()) 
            return true;
        else if(cnf.ifEmptyClause())
        {
            for(int i=0;i<count;i++)
            {
                CNF::backTrace(intStack,abStack);
            }
            return false;
        }
    }
    //get v
    int variable_1;
    if(strategy==5)
    {
        if(loopCount>100)
        variable_1=vpl.getFirst(cnf);
        else
        variable_1=cnf.head->e1->e2->variable;
    }
    else if(strategy==4)
    variable_1=vpl.getFixedMVariable();
    else if(strategy==3)
    variable_1=vpl.getMVariable(cnf,varNum);
    else if(strategy==2)
    variable_1=cnf.head->e1->e2->variable;
    else
    variable_1=vpl.getSVariable(cnf);
    CNF::imposeSCR(cnf,results,variable_1,abStack,intStack);
    if(DPLL(cnf,depth+1,vpl,dxpdepth*2,procedure)) 
        {
            if(depth==1) cnf.originize(abStack,intStack);
            return true;
        }
    else if(loopCount==-1)
        return false;
    if(depth<=32) procedure+=1.0/dxpdepth;
    CNF::backTrace(intStack,abStack);
    CNF::imposeSCR(cnf,results,-variable_1,abStack,intStack);
    if(DPLL(cnf,depth+1,vpl,dxpdepth*2,procedure)) 
        {
            if(depth==1) cnf.originize(abStack,intStack);
            return true;
        }
    else if(loopCount==-1)
        return false;
    CNF::backTrace(intStack,abStack);
    for(int i=0;i<count;i++)
    {
        CNF::backTrace(intStack,abStack);
    }
    vpl.backTrace();

    return false;
}
bool DPLL_v2(int depth,int variable_currently,VPLhandler& vpl,CNF* cnf,long long dxpdepth,double procedure)
{
    //cnf.printCNF();
    //system("cls");
    //procedure=depth_callNum[12]/32.0/16.0/16.0;
    //if(depth<=14 && depth>1) depth_callNum[depth-2]++;
    //normalize();
    //fprintf(stderr,"\rprocedure=%f%%",procedure*100);
    int variable;
    while((variable=cnf->getSingleClause())!=114514)
    {
        CNF::imposeSCR(*cnf,results,variable);
        if(cnf->ifEmpty()) 
            return true;
        else if(cnf->ifEmptyClause())
        {
            //cnf->deleteTree();
            return false;
        }
    }
    //get v
    //int variable_1=vpl.getMVariable(*cnf,varNum);
    //int variable_1=cnf->head->e1->e2->variable;
    int variable_1=vpl.getSVariable(*cnf);
    DWORD start, end; 
    start = GetTickCount();
    CNF* cnf_replica_1=cnf->copyTree();
    end = GetTickCount();
    cnf_replica_1->newClause();
    cnf_replica_1->addToClause(variable_1);
    if(DPLL_v2(depth+1,variable_1,vpl,cnf_replica_1,dxpdepth*2,procedure)) 
        return true;
    //if(depth<=24) procedure+=1.0/dxpdepth;
    cnf_replica_1->deleteTree();//先删了一个，再生成第二个
    //CNF* cnf_replica_2=cnf->copyTree();
    CNF* cnf_replica_2=cnf;
    //dupCount++;
    cnf_replica_2->newClause();
    cnf_replica_2->addToClause(-variable_1);
    if(DPLL_v2(depth+1,-variable_1,vpl,cnf_replica_2,dxpdepth*2,procedure)) 
        return true;
    //cnf_replica_2->deleteTree();

    //vpl.backTrace();

    return false;
}