/*
upper->row->column->value
1->x->x->X

total number of grid cells:81+72=153
*/
#include <stdio.h>
int index_to_variable(int number,int index);
inline int pij_to_variable(int number,int position,int row,int column);
inline int variable_getPos(int variable);
inline int variable_getRow(int variable);
void transform_sudoku_to_satFile(int* sdk);
