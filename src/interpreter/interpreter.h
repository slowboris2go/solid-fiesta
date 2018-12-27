// Contains structures and function declarations
#include "../uba/uba.h"


// The environment is a uba of pairs of function name and code output

// Contains meta data to execute function
typedef struct
{
    char* name;
    char* returnValue;
} functionMeta;

void interpret(uba* parseTree);