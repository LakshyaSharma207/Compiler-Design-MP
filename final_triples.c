// the logic and data structure behind triple representation. I hope it's not wrong even if not completely correct
#include <stdio.h>
#include <string.h>

typedef struct Triple Triple;

typedef struct {
    char value;
    Triple *triplePtr;
} ArgUnion;

struct Triple {
    int pos;
    char op;
    ArgUnion arg1;
    ArgUnion arg2;
    char result;
};

Triple myTriple[20];
int triple_count = 0;

Triple generateTriple(int, char, char, char, char);
void searchTriple();
void printTriple();

int main() 
{
    myTriple[0] = generateTriple(1, '+', 'b', 'c', 'Z');
    myTriple[1] = generateTriple(2, '*', 'a', 'Z', 'Y');
    searchTriple();
    printTriple();
    return 0;
}

Triple generateTriple(int pos, char op, char arg1, char arg2, char result) {
    Triple newTriple = {pos, op, {.value = arg1}, {.value = arg2}, result};
    triple_count+=1;
    return newTriple;
}

void printTriple() 
{
    int i;
    for(i=0; i<triple_count; i+=1)
    {
        if(myTriple[i].arg1.triplePtr != NULL) 
        {
            printf("(%d, %c, (%d), ", myTriple[i].pos, myTriple[i].op, myTriple[i].arg1.triplePtr->pos);
        }
        else
        {
            printf("(%d, %c, %c, ", myTriple[i].pos, myTriple[i].op, myTriple[i].arg1.value);
        } 
        if(myTriple[i].arg2.triplePtr != NULL) 
        {
            printf("(%d))\n", myTriple[i].arg2.triplePtr->pos);
        }
        else
        {
            printf("%c)\n", myTriple[i].arg2.value);
        } 
    }
}

void searchTriple() 
{
    int i;
    for(i=0; i<triple_count-1; i+=1)
    {
        if(myTriple[i].result == myTriple[triple_count-1].arg1.value)
        {
            myTriple[triple_count-1].arg1.triplePtr = &myTriple[i];
        }
        else if(myTriple[i].result == myTriple[triple_count-1].arg2.value)
        {
            myTriple[triple_count-1].arg2.triplePtr = &myTriple[i];
        }
    }
}
