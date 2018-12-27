
// Includes functionality to turn file string into an array tree.

#include "parser.h"

bool parenCheck(uba* tokens, size_t* lineNumber)
{
    size_t size = ubaSize(tokens);
    ssize_t result = 0;
    for(size_t idx = 0; idx < size; ++idx)
    {
        TokenType* token = ubaIndex(tokens, idx);
        if (strcmp("(", token->token) == 0)
        {
            result++;
        }
        if(strcmp(")", token->token) == 0)
        {
            result--;
        }

        if(result < 0)
        {
            *lineNumber = token->lineNumber;
            return false;
        }
    }
    if(result > 0)
    {
        return false;
    }
    return true;
}


/**
 * This function takes the tokenized input and turns it into a code tree
 * depending on the parens.
 * 
 * Builds an unbounded array of parseEntrys using recursion
 */
uba* parse(uba* tokens, size_t *index)
{
    // TODO empty token case
    uba* result = ubaNew(10);
    size_t size = ubaSize(tokens);

    while(*index < size)
    {
        TokenType* tokenStruct = ubaIndex(tokens, *index);
        char* token = tokenStruct->token;
        if(strcmp(token, "(") == 0)
        {
            // Create structure
            parseEntry* element = malloc(sizeof(parseEntry));
            element->type = LIST;

            
            *index += 1;
            element->list = parse(tokens, index);
            ubaAdd(result, element);
            // Note that index is updated by the function call
        }
        else if(strcmp(token, ")") == 0)
        {
            *index += 1;
            return result;
        }
        else
        {
            // We are dealing with a word
            parseEntry* element = malloc(sizeof(parseEntry));
            element->type = LITERAL;
            element->literal = token; // TODO make this a copy so we can free tokens

            ubaAdd(result, element);
            *index += 1;
        }
    }

    return result;
}

