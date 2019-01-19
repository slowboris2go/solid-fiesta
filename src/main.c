// This file will contain the macro expander code.

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

#include "tokenizer/tokenizer.h"
#include "interpreter/interpreter.h"

// debug function
void printTokens(uba* tokens)
{
    for(size_t i = 0; i < ubaSize(tokens); ++i)
    {
        TokenType* token = ubaIndex(tokens, i);
        printf("%s\n", token->token);
    }
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Usage: lcc filename\n");
        return 0;
    }
    // Read file completely
    // We accomplish this by finding size and allocating space and reading.
    FILE * fd;
    fd = fopen(argv[1], "r");

    fseek(fd, 0L, SEEK_END);
    size_t fileSize = ftell(fd);
    fseek(fd, 0L, SEEK_SET);

    // Read file into buffer
    uint8_t* buffer = calloc(fileSize, sizeof(uint8_t));
    fread(buffer, fileSize, 1, fd);
    
    // first tokenize input
    uba* tokens = NULL;
    if(!runTokenizer(buffer, fileSize, &tokens))
    {
        // We got an error tokenizing that has been reported
        return 1;
    }
    //printTokens(tokens);

    // Free the file buffer since it is no longer useful
    free(buffer);

    // Now that we parsed it so lets run it
    interpret(tokens);

    return 0;
}
