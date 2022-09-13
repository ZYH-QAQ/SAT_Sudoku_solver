#include "CNF.h"
void AbStack::push(ClauseElement* e)
{
    this->stack[this->stackTop++]=e;
}
ClauseElement* AbStack::pop()
{
    return this->stack[--this->stackTop];
}
ClauseElement* AbStack::peek()    
{    
    return this->stack[this->stackTop-1];    
} 
void IntStack::push(int e)
{
    this->stack[this->stackTop++]=e;
}
int IntStack::pop()
{
    return this->stack[--this->stackTop];
}
int IntStack::peek()    
{    
    return this->stack[this->stackTop-1];    
} 



