// Implements basic interpreter

#include "interpreter.h"



uint32_t execute(InterpreterState* state, uba* tokens)
{
    size_t size = ubaSize(tokens);

    if(state->programIndex >= size)
    {
        printf("Error encountered tryint to read word %lu!\n", state->programIndex);
        return 0;
    }

    TokenType* token = ubaIndex(tokens, state->programIndex);

    if(strcmp(token->token, "+") == 0)
    {
        // Get the value of the next two statements and add them
        state->programIndex++;
        uint32_t param1 = execute(state, tokens);
        uint32_t param2 = execute(state, tokens);
        return param1 + param2;
    }
    else
    {
        // Assume a number
        uint32_t number = atoi(token->token);
        state->programIndex++;
        return number;
    }
    
}

// This is the highest level call so a list of instructions
void interpret(uba* tokens)
{
    InterpreterState state = {0};
    uint32_t result = execute(&state, tokens);

    // Assume no errors

    printf("%u\n", result);
}