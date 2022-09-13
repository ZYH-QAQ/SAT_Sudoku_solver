#include <stdio.h>
#include "transformer.h"
void print_result(bool* result)
{
    for(int row=0; row<6;row++)
    {
        for(int col=0; col<9;col++)
        {
            for(int num=1; num<=9;num++)
            {
                if(result[pij_to_variable(num,1,row,col)]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        printf("\n");
    }
    for(int row=6; row<9;row++)
    {
        for(int col=0; col<9;col++)
        {
            for(int num=1; num<=9;num++)
            {
                if(result[pij_to_variable(num,1,row,col)]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        for(int col=3;col<9;col++)
        {
            for(int num=1; num<=9;num++)
            {
                if(result[pij_to_variable(num,0,row-6,col)]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        printf("\n");
    }

    for(int row=3; row<9;row++)
    {
        printf("            ");
        for(int col=0; col<9;col++)
        {
            
            for(int num=1; num<=9;num++)
            {
                if(result[pij_to_variable(num,0,row,col)]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        printf("\n");
    }
    
}
void print_result_2(bool* result)
{
    for(int row=1; row<=6;row++)
    {
        for(int col=1; col<=9;col++)
        {
            for(int num=1; num<=9;num++)
            {
                if(result[1000+100*row+10*col+num]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        printf("\n");
    }
    for(int row=7; row<=9;row++)
    {
        for(int col=1; col<=9;col++)
        {
            for(int num=1; num<=9;num++)
            {
                if(result[1000+100*row+10*col+num]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        for(int col=4;col<=9;col++)
        {
            for(int num=1; num<=9;num++)
            {
                if(result[2000+100*row+10*col+num]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        printf("\n");
    }

    for(int row=4; row<=9;row++)
    {
        printf("            ");
        for(int col=1; col<=9;col++)
        {
            
            for(int num=1; num<=9;num++)
            {
                if(result[2000+100*row+10*col+num]==1)
                {
                    printf("%d ",num);
                    break;
                }
            }
        }
        printf("\n");
    }
    
}
void r2r(bool* result2,bool* result)
{
    for(int pos=1;pos<=2;pos++)
    {
        for(int row=1; row<=9;row++)
        {
            for(int col=1; col<=9;col++)
            {
                for(int num=1; num<=9;num++)
                {
                    if(result2[pos*1000+100*row+10*col+num]==1)
                    {
                        result[(2-pos)*1000+100*(row-1)+10*(col-1)+num]=1;
                        break;
                    }
                }
            }
        }
    }
}