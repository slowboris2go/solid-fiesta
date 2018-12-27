// Implements basic interpreter

#include "interpreter.h"
#include "../parser/parser.h"


char* functionCall(uba* environment, char* name)
{
    size_t size = ubaSize(environment);
    for(size_t i = 0; i < size; ++i)
    {
        functionMeta* function = ubaIndex(environment, i);
        if(strcmp(name, function->name) == 0)
        {
            return function->returnValue;
        }
    }
    // Found nothing
    return NULL;
}


void interpretCall(uba* environment, uba* call)
{
    size_t size = ubaSize(call);

    if(size != 1)
    {
        printf("Error encountered2\n");
    }
    else
    {
        parseEntry* entry = ubaIndex(call, 0);
        if(entry->type != LITERAL)
        {
            printf("Error encountered4\n");
            return;
        }
        char* valueToWrite = functionCall(environment, entry->literal);
        if(valueToWrite == NULL) // Found no such function
        {
            printf("Error encountered3\n");
            return;
        }
        printf("%s", valueToWrite);
    }
}

void interpretDef(uba* environment, uba* def)
{
    size_t size = ubaSize(def);

    if(size != 4)
    {
        printf("Error occurred5\n");
    }
    else
    {
        parseEntry* entry = ubaIndex(def, 1); 
        char* name = entry->literal;

        entry = ubaIndex(def, 3);
        char* returnValue = entry->literal;

        functionMeta* function = malloc(sizeof(functionMeta));
        function->name = name;
        function->returnValue = returnValue;
        // TODO make fresh copies of the string so can free the parse tree

        ubaAdd(environment, function);
    }
}


void interpretList(uba* environment, uba* list)
{
    size_t size = ubaSize(list);

    if(size == 0)
    {
        printf("Error encountered7\n");
    }
    else
    {
        parseEntry* entry = ubaIndex(list, 0);
        if(strcmp("macro", entry->literal) == 0)
        {
            interpretDef(environment, list);
        }
        else
        {
            interpretCall(environment, list);
        }
    }
    
}

// This is the highest level call so a list of instructions
void interpret(uba* parseTree)
{
    uba* environment = ubaNew(10);

    size_t size = ubaSize(parseTree);
    for(size_t i = 0; i < size; ++i)
    {
        parseEntry* entry = ubaIndex(parseTree, i);
        if(entry->type = LITERAL)
        {
            printf("Error occured1\n");
            return;
        }
        interpretList(environment, entry->list);
    }
    printf("\n");
}