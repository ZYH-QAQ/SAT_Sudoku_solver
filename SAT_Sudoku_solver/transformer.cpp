#include "transformer.h"
int index_to_variable(int number,int index)
{
    if(index<81)
    {
        int row=index/9;
        int column=index%9;
        return row*100+column*10+number;
    }
    index-=81;
    if(index<54)
    {
        int row=index/9;
        int column=index%9;
        return 1000+row*100+column*10+number;
    }
    index-=54;
    int row=6+index/6;
    int column=index%6;
    return 1000+row*100+column*10+number;
}
inline int pij_to_variable(int number,int position,int row,int column)
{
    return (position==1&&row>=6&&column>=6)?(row-6)*100+(column-6)*10+number:position*1000+row*100+column*10+number;
}
inline int variable_getPos(int variable)
{
    return variable/1000;
}
inline int variable_getRow(int variable)
{
    return (variable%1000)/100;
}
void transform_sudoku_to_satFile(int* sdk)
{
    FILE* f=fopen("114514.temp","w");
    int num_of_variables=153*9,num_of_clauses=0;
    for(int i=0; i<153;i++)
    {
        if(sdk[i]!=0) num_of_clauses+=9;
        num_of_clauses+=36;
    }
    num_of_clauses+=18*9*2+17*9+18*36*2*9;//each box/column/row contains at least a set of 1-9 ensures no repetition.
    fprintf(f,"p cnf %d %d\n",num_of_variables,num_of_clauses);
    
    
    //row
    for(int pos=0;pos<2;pos++)
        for(int row = 0; row <9;row++)
            for(int num=1;num<=9;num++)
            {
                for(int j=0;j<9;j++)
                {
                    for(int k=j+1;k<9;k++)
                    {
                        fprintf(f,"%d %d 0\n",-pij_to_variable(num,pos,row,j),-pij_to_variable(num,pos,row,k));
                    }
                }
                
            }
    //col
    for(int pos=0;pos<2;pos++)
        for(int col = 0; col <9;col++)
            for(int num=1;num<=9;num++)
            {
                for(int j=0;j<9;j++)
                {
                    for(int k=j+1;k<9;k++)
                    {
                        fprintf(f,"%d %d 0\n",-pij_to_variable(num,pos,j,col),-pij_to_variable(num,pos,k,col));
                    }
                }
                
            }
    
    //row_condition
    for(int pos=0;pos<2;pos++)
        for(int row = 0; row <9;row++)
            for(int num=1;num<=9;num++)
            {
                for(int column=0;column<9;column++)
                    fprintf(f,"%d ",pij_to_variable(num,pos,row,column));
                fprintf(f,"0\n");
            }

    //column_condition
    for(int pos=0;pos<2;pos++)
        for(int column = 0; column <9;column++)
            for(int num=1;num<=9;num++)
            {
                for(int row=0;row<9;row++)
                    fprintf(f,"%d ",pij_to_variable(num,pos,row,column));
                fprintf(f,"0\n");
            }
    //boxes in pos 0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            for(int num=1;num<=9;num++)
            {
                for(int subrow=0;subrow<3;subrow++)
                    for(int subcolumn=0;subcolumn<3;subcolumn++)
                    {
                        int row=i*3+subrow;
                        int column=j*3+subcolumn;
                        fprintf(f,"%d ",pij_to_variable(num,0,row,column));
                    }
                fprintf(f,"0\n");
            }
    //boxes is pos 1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        {
            if(i==2&&j==2) break;
            for(int num=1;num<=9;num++)
            {
                for(int subrow=0;subrow<3;subrow++)
                    for(int subcolumn=0;subcolumn<3;subcolumn++)
                    {
                        int row=i*3+subrow;
                        int column=j*3+subcolumn;
                        fprintf(f,"%d ",pij_to_variable(num,1,row,column));
                    }
                fprintf(f,"0\n");
            }
        }
    //create single clauses for restraint condition
    for(int i=0; i<153;i++)
    {
        if(sdk[i]!=0)
        {
            fprintf(f,"%d 0\n",index_to_variable(sdk[i],i));
            for(int j=1;j<=9;j++)
            {
                if(sdk[i]!=j)
                fprintf(f,"%d 0\n",-index_to_variable(j,i));
            }
        }
        
        {
            for(int n1=1;n1<=9;n1++)
            {
                for(int n2=n1+1;n2<=9;n2++)
                {
                    fprintf(f,"%d %d 0\n",-index_to_variable(n1,i),-index_to_variable(n2,i));
                }
            }
        }
    }
    
    fclose(f);
}