// Tokenizes the file into words

#include "tokenizer.h"

// Create copy of a portion of a string to add to uba
char* makeStringCopy(char* source, size_t length)
{
    char* result = malloc(length+1); // don't forget null terminator
    strncat(result, source, length);
    return result;
}

// Checks for whitespace
bool isWhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Exemption is made for double quotes for strings
bool isDoubleQuotes(char c)
{
    return c == '"';
}

TokenType* makeToken(char* token, size_t lineNumber)
{
    TokenType* tokenStruct = malloc(sizeof(TokenType));
    tokenStruct->token = token;
    tokenStruct->lineNumber = lineNumber;
    return tokenStruct;
}

// Handles the TOKEN_DISCARD state
void discardHandler(Tokenizer* tokenizer)
{
    do
    {
        // Reached the end of the string
        if(tokenizer->index >= tokenizer->codeSize)
        {
            tokenizer->state = TOKEN_DONE;
            tokenizer->errorStatus = TOKEN_ERROR_NONE;
            break;
        }

        char currentChar = tokenizer->code[tokenizer->index];

        // Discard whitespace
        if(isWhitespace(currentChar))
        {
            if(currentChar == '\n')
            {
                tokenizer->lineNumber++;
            }
            tokenizer->index++;
            break;
        }

        // Whether this is the start of a string
        if(isDoubleQuotes(currentChar))
        {
            tokenizer->state = TOKEN_STRING;
            tokenizer->tokenStart = &tokenizer->code[tokenizer->index];
            tokenizer->tokenSize = 1;
            tokenizer->index++;
            break;
        }

        // If we got here it is not an exception so we start reading the token
        tokenizer->state = TOKEN_READ;
        tokenizer->tokenStart = &tokenizer->code[tokenizer->index];
        tokenizer->tokenSize = 1;
        tokenizer->index++;


    } while(0);
}

void readHandler(Tokenizer* tokenizer)
{
    do
    {
        // Reached the end of the string
        if(tokenizer->index >= tokenizer->codeSize)
        {
            tokenizer->state = TOKEN_DONE;
            tokenizer->errorStatus = TOKEN_ERROR_NONE;
            // Add token we were building up
            char* tokenString = makeStringCopy(tokenizer->tokenStart, tokenizer->tokenSize);
            TokenType* token = makeToken(tokenString, tokenizer->lineNumber);
            ubaAdd(tokenizer->result, token);
            break;
        }

        char currentChar = tokenizer->code[tokenizer->index];

        // Discard whitespace
        if(isWhitespace(currentChar))
        {
            tokenizer->state = TOKEN_DISCARD;
            if(currentChar == '\n')
            {
                tokenizer->lineNumber++;
            }
            // Add token we were building up
            char* tokenString = makeStringCopy(tokenizer->tokenStart, tokenizer->tokenSize);
            TokenType* token = makeToken(tokenString, tokenizer->lineNumber);
            ubaAdd(tokenizer->result, token);
            tokenizer->index++;
            break;
        }

        // Whether this is the start of a string
        if(isDoubleQuotes(currentChar))
        {
            tokenizer->state = TOKEN_STRING;
            // Add the currently built token
            char* tokenString = makeStringCopy(tokenizer->tokenStart, tokenizer->tokenSize);
            TokenType* token = makeToken(tokenString, tokenizer->lineNumber);
            ubaAdd(tokenizer->result, token);
            tokenizer->tokenStart = &tokenizer->code[tokenizer->index];
            tokenizer->tokenSize = 1;
            tokenizer->index++;
            break;
        }

        // Read a normal character so increment index and token size
        tokenizer->tokenSize++;
        tokenizer->index++;
    } while(0);
}

void stringHandler(Tokenizer* tokenizer)
{
    do
    {
        // Reached the end of the string
        if(tokenizer->index >= tokenizer->codeSize)
        {
            tokenizer->state = TOKEN_DONE;
            tokenizer->errorStatus = TOKEN_ERROR_UNCLOSED_STRING;
            printf("%lu: Unclosed quotation mark\n", tokenizer->lineNumber);           
            break;
        }

        char currentChar = tokenizer->code[tokenizer->index];

        // Whether this is the start of a string
        if(isDoubleQuotes(currentChar))
        {
            tokenizer->state = TOKEN_DISCARD;
            // Add the currently built token including this double quote
            tokenizer->tokenSize++;
            char* tokenString = makeStringCopy(tokenizer->tokenStart, tokenizer->tokenSize);
            TokenType* token = makeToken(tokenString, tokenizer->lineNumber);
            ubaAdd(tokenizer->result, token);
            tokenizer->index++;
            break;
        }

        // If we get this character we ignore the next character assuming its special
        if(currentChar == '\\')
        {
            tokenizer->state = TOKEN_STRING_IGNORE;
            tokenizer->tokenSize++;
            tokenizer->index++;
            break;
        }

        // if we get a new line don't forget to increment the line number
        if(currentChar == '\n')
        {
            tokenizer->lineNumber++;
            tokenizer->tokenSize++;
            tokenizer->index++;
            break;
        }

        // Read a normal character so increment index and token size
        tokenizer->tokenSize++;
        tokenizer->index++;
    } while(0);    
}

// Handles case where we are parsing a string and received the "\" string
void stringIgnoreHandler(Tokenizer* tokenizer)
{
    do
    {
        // Reached the end of the string
        if(tokenizer->index >= tokenizer->codeSize)
        {
            tokenizer->state = TOKEN_DONE;
            tokenizer->errorStatus = TOKEN_ERROR_UNCLOSED_STRING;
            printf("%lu: Unclosed quotation mark\n", tokenizer->lineNumber);           
            break;
        }

        // Read a normal character so increment index and token size
        tokenizer->tokenSize++;
        tokenizer->index++;
    } while(0);        
}


// State machine used to process the characters into tokens
bool runTokenizer(char* code, size_t size, uba** tokens)
{
    Tokenizer tokenizer = {0};
    tokenizer.result = ubaNew(10);
    tokenizer.state = TOKEN_DISCARD;
    // Setup the function pointers for each state
    tokenizer.stateHandler[TOKEN_DISCARD] = discardHandler;
    tokenizer.stateHandler[TOKEN_READ] = readHandler;
    tokenizer.stateHandler[TOKEN_STRING] = stringHandler;
    tokenizer.stateHandler[TOKEN_STRING_IGNORE] = stringIgnoreHandler;

    // Setup arguments that are passed in
    tokenizer.code = code;
    tokenizer.codeSize = size;
    tokenizer.index = 0;
    tokenizer.lineNumber = 1;

    while (tokenizer.state != TOKEN_DONE)
    {
        tokenizer.stateHandler[tokenizer.state](&tokenizer);
    }

    *tokens = tokenizer.result;

    // Check for errors
    if(tokenizer.errorStatus == TOKEN_ERROR_NONE)
    {
        return true;
    }
    return false;
}

