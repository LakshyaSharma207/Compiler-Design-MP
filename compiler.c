#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Symbol Table
typedef struct symbol{ 
    char name;
    char type[20];
}symbol;
symbol symbolTable[50];

// AST struct
typedef struct AstNode{
    char type[20];
    char data;
    struct AstNode *left;
    struct AstNode *right;
}AstNode;

AstNode* createNode(int);
void printAst(AstNode* node);
AstNode* prevnode = NULL; 
AstNode* headnode = NULL;

// Triples TAC struct
typedef struct Triple Triple;
typedef struct{
    char value;
    Triple *triplePtr;
} ArgUnion;

typedef struct Triple{
    int pos;
    char op;
    ArgUnion arg1;
    ArgUnion arg2;
    char result;
} Triple;

Triple myTriple[20];


Triple generateTriple(int, char, char, char, char);
void searchTriple();
void printTriple();

// General global variables
char arr[20], stack[20], temp[20];
int k=0, top=0, parse_flag = 0, triple_count = 0;

void printTable();
void insertTable();
void srParser();
void check(int);
void push(char);
void pop(int);
void codeGenerator();

int main()
{
    int op;
    while(1)
    {
        printf("\n\n+++++++++++++++++++++++++++++++++++\n");
        printf("1. Enter new expression (Lexical Phase)\n");
        printf("2. Print Symbol Table\n");
        printf("3. Shift Reduce Parsing\n");
        printf("4. Intermediate Code Generation\n");
        printf("5. Exit\n");
        printf("+++++++++++++++++++++++++++++++++++++++\n\n");
        printf("Enter your option = ");
        scanf("  %d", &op);
        
        if(op == 1)
        {
            insertTable();  
        }
        else if(op == 2)
        {
            if(strlen(arr) == 0)
            {
                printf("\nPlease Enter the expression first.\n");
            }
            else
            {
                printTable(); 
            }
        }
        else if(op == 3)
        {
            if(strlen(arr) == 0)
            {
                printf("\nPlease Enter the expression first.\n");
            }
            else
            {
                srParser(); 
            }
        }
        else if(op == 4)
        {
            if(strlen(stack) == 0 || parse_flag == 0)
            {
                printf("\nPlease Perform Parsing with correct input first.\n");
            }
            else
            {
                codeGenerator(); 
            }
        }
        else if(op == 5)
        {
            break;
        }
        else
        {
            printf("\nEnter valid option!!");
        }
    }
    
    printf("Exiting Program......");
    return 0;
}

void printTable()
{
    int i;
    printf("\n\nSymbol Table - \n");
    printf("Name\t Type\n");
    for(i = 0; i < k; i++) 
    {
        printf("%c\t %s\n", symbolTable[i].name, symbolTable[i].type);
    }
}

void insertTable()
{
    k = 0;
    int i=-1, j=0;
	char lst1[] = {'=', '/', '%', '*', '+', '-', '^', '!', '>', '<', ']', '[', '{', '}', '(', ')', '.'};

	
	// input expression to character array
    printf("\nInput string should follow these grammars -\n");
    printf("\nE->E+E\nE->E-E\nE->E*E\nE->E/E\nE->E%%E\nE->id | constant\n");
    printf("\nEnter the expression end it with $ = ");
    do{
        i+= 1;
        scanf(" %c", &arr[i]);
    }while(arr[i] != '$');

    //k = i+1;
    k = strlen(arr);
    printf("Given Expression:");
    for (i = 0; i < k; i += 1) 
    {
        printf("%c ", arr[i]);
    }

    int flag[k];
    for(i=0; i<k; i+=1)
    {
        flag[i] = 0;
    }
    // loop to build symbol table after identifying tokens
    for (i = 0; i < k; i+=1) 
    {
        for(j=0; j<18; j+=1)
        {
            if(flag[i] == 0)
            {
                if(arr[i] == lst1[j])
                {
                    symbolTable[i].name = arr[i];
                    strcpy(symbolTable[i].type, "Operator");
                    flag[i] = 1;
                    break;
                }
            }
        }
        if(flag[i] == 0)
        {
            if(arr[i] == '$')
            {
                symbolTable[i].name = '$';
                strcpy(symbolTable[i].type, "End_of_input");
                flag[i] = 1;
            }
            else if(isdigit(arr[i]))
            {
                symbolTable[i].name = arr[i];
                strcpy(symbolTable[i].type, "Constant");
                flag[i] = 1;
            }
            else
            {
                symbolTable[i].name = arr[i];
                strcpy(symbolTable[i].type, "Identifier");
                flag[i] = 1;
            }
        }
    }
}

void srParser()
{
    int i;
    printf("\n The given grammar is -\n");
    printf("\n E->E+E\n E->E-E\n E->E*E\n E->E/E\n E->E%%E\n E->id | constant\n");
    printf("\n\t  Shift Reduce table");
    printf("\nstack\t\tinput symbol\t\taction");
    printf("\n $\t\t%s\t\t\t--", arr);
    strcpy(temp, arr);
    top=0;
    prevnode = NULL; 
    headnode = NULL;

    // loop to construct Parsing Table
    for(i = 0; i < k; i+=1) 
    {
        if(symbolTable[i].name == '$')
        {
            check(i);
        }
        else
        {
            push(symbolTable[i].name);
            stack[top] = '\0';
            temp[i] = ' ';
            printf("\n $%s\t\t%s\t\t\tSHIFT %c", stack, temp, symbolTable[i].name);
            check(i);
        }
    }
}

// function to check stack for handle
void check(int i) 
{
    int flag = 0, j;

    // Reduce identifier to E
    if (strcmp(symbolTable[i].type, "Identifier") == 0 || strcmp(symbolTable[i].type, "Constant") == 0) 
    {
        pop(1);
        push('E');
        printf("\n $%s\t\t%s\t\t\tE->%c", stack, temp, symbolTable[i].name);
        flag = 1;
    }
    // no further reduction
    if (strcmp(symbolTable[i].type, "Operator") == 0) 
    {
        flag = 1;
        AstNode* tempnode = createNode(i);
        if(prevnode != NULL) 
        {
            prevnode->right = tempnode;
        }
        if(headnode == NULL) 
        {
            headnode = tempnode;
        }
        prevnode = tempnode;
    }
    // Reduce expression to E
    if (strcmp(stack, "E*E") == 0 || strcmp(stack, "E/E") == 0 || strcmp(stack, "E%E") == 0 || strcmp(stack, "E+E") == 0 || strcmp(stack, "E-E") == 0) 
    {
        pop(3);
        flag = 1;
        push('E');
        printf("\n $%s\t\t%s\t\t\tE%cE", stack, temp, symbolTable[i-1].name);
    }

    // flag check for string validity
    if(flag == 0) 
    {
        if(strcmp(stack, "E") == 0 && symbolTable[i].name == '$')
        {
            printf("\n $%s\t\t%s\t\t\tACCEPT", stack, temp);
            printf("\n\nString is accepted and can go further for intermediate code generation!");
            parse_flag = 1;
            printf("\n\nAST for the input is: ");
            printAst(headnode);
        }
        else
        {
            printf("\n $%s\t\t%s\t\t\tREJECT", stack, temp);
            printf("\n\nEnter a new valid string!!");
        }
    }
}

void push(char x)
{
    if(top>=k)
    {
        printf("\n\tSTACK is over flow");
    }
    else
    {
        stack[top] = x;
        top += 1;
    }
}
void pop(int y)
{
    if (top <=- 1)
    {
        printf("\n\t Stack is under flow");
    }
    else
    {
        int i;
        for (i = 0; i < y; i +=1)
        {
            stack[top] = '\0';
            top -=1 ;
        }
    }
}

// function to create node
AstNode* createNode(int i) 
{
    if(strcmp(symbolTable[i].type, "Constant") == 0)
    {
        AstNode* node = malloc(sizeof(AstNode));
        strcpy(node->type, symbolTable[i].type);
        node->data = symbolTable[i].name;
        node->left = node->right = NULL;
        return node;
    }
    else if(strcmp(symbolTable[i].type, "Identifier") == 0)
    {
        AstNode* node = malloc(sizeof(AstNode));
        strcpy(node->type, symbolTable[i].type);
        node->data = symbolTable[i].name;
        node->left = node->right = NULL;
        return node;
    }
    else if(strcmp(symbolTable[i].type, "Operator") == 0) 
    {
        AstNode* node = malloc(sizeof(AstNode));
        strcpy(node->type, symbolTable[i].type);
        node->data = symbolTable[i].name;
        node->left = createNode(i-1);
        node->right = createNode(i+1);
        return node;
    }
    return NULL;
}

// Function to print the AST
void printAst(AstNode* node) 
{
    if (node == NULL) return;

    if(strcmp(node->type, "Constant") == 0 || strcmp(node->type, "Identifier") == 0) 
    {
        printf("%c", node->data);
        free(node);
    } 
    if(strcmp(node->type, "Operator") == 0) 
    {
        printf("%c", node->data);
        printf("(");
        printAst(node->left);
        printf(", ");
        printAst(node->right);
        printf(")");
        free(node);
    }
}

void codeGenerator()
{
    int i, j, check, no_op=0, pos=0;
    char result='Z';
    
    for(i=0; i<k; i+=1)
    {
        if(strcmp(symbolTable[i].type, "Operator") == 0)
        {
            no_op++;
        }
    }

    strcpy(temp, arr);
    printf("\nIntermediate code Quadraples- \n");
    printf("\topr\top1\top2\tresult\n");
    for(check = 1; check < no_op; check += 1)
    {
        for(i = 0; i < k; i += 1)
        {
            if(temp[i] == '/' || temp[i] == '*' || temp[i] == '%')
            {
                printf("(%d)\t%c\t%c\t%c\t %c\n",check, temp[i], temp[i - 1], temp[i + 1], result);
                myTriple[triple_count] = generateTriple(check, temp[i], temp[i-1], temp[i+1], result);
                searchTriple();
                temp[i - 1] = result; 
                result--;
    
                for(j = i; j<k-pos ; j+=1)
                {
                    temp[j] = temp[j + 2]; 
                }
                break;
            }
            else if(temp[i] == '+' || temp[i] == '-')
            {
                printf("(%d)\t%c\t%c\t%c\t %c\n",check, temp[i], temp[i - 1], temp[i + 1], result);
                myTriple[triple_count] = generateTriple(check, temp[i], temp[i-1], temp[i+1], result);
                searchTriple();
                temp[i - 1] = result; 
                result--;
    
                for(j = i; j<k-pos; j+=1)
                {
                    temp[j] = temp[j + 2]; 
                }
                break;
            }
        }
    }
    printf("(%d)\t%c\t%c\t%c\t %c\n",check, temp[1], temp[0], temp[2], result);
    myTriple[triple_count] = generateTriple(check, temp[1], temp[0], temp[2], result);
    
    searchTriple();
    printf("\n\nIndirect Triple Representation - \n");
    printf("\topr\top1\top2\n");
    printTriple();
}

// returns initialized Triple
Triple generateTriple(int pos, char op, char arg1, char arg2, char result) 
{
    Triple newTriple = {pos, op, {.value = arg1}, {.value = arg2}, result};
    triple_count+=1;
    return newTriple;
}

// searches for common result in arg and assigns pointer to it
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

// prints triples stored in struct
void printTriple() 
{
    int i;
    for(i=0; i<triple_count; i+=1)
    {
        if(myTriple[i].arg1.triplePtr != NULL) 
        {
            printf("(%d)\t%c\t(%d) ", myTriple[i].pos, myTriple[i].op, myTriple[i].arg1.triplePtr->pos);
        }
        else
        {
            printf("(%d)\t%c\t%c ", myTriple[i].pos, myTriple[i].op, myTriple[i].arg1.value);
        } 
        if(myTriple[i].arg2.triplePtr != NULL) 
        {
            printf("\t(%d)\n", myTriple[i].arg2.triplePtr->pos);
        }
        else
        {
            printf("\t%c\n", myTriple[i].arg2.value);
        } 
    }
} 
