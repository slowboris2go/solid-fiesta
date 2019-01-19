#pragma once

#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<stdio.h>

#include "../uba/uba.h"


typedef struct
{
    char* token;
    size_t lineNumber;
} TokenType;

// Indicates current state of the tokenizer
typedef enum
{
    TOKEN_DONE,
    TOKEN_DISCARD,
    TOKEN_READ,
    TOKEN_STRING,
    TOKEN_STRING_IGNORE,
    TOKEN_NUM_STATES
} TokenizerStateEnum;

// Indicates error tokenizing
typedef enum
{
    TOKEN_ERROR_NONE,
    TOKEN_ERROR_UNCLOSED_STRING
} TokenizerErrorEnum;

// All state variables needed to run the tokenizer
typedef struct T
{
    TokenizerStateEnum state; // Current state of the tokenizer
    void (*stateHandler[TOKEN_NUM_STATES])(struct T*);
    char* code; // Pointer to code string
    size_t codeSize; // Length of code string
    size_t index; // Current character to look at
    size_t lineNumber; // Line number in code
    char* tokenStart; // Index to the start of the token
    size_t tokenSize; // Current size of token
    uba* result; // Pointer to resulting token stream.
    TokenizerErrorEnum errorStatus; // Indicates whether there was an error parsing or not.
} Tokenizer;

char* makeStringCopy(char* source, size_t length);

bool isWhitespace(char c);

bool isParens(char c);

bool runTokenizer(char* code, size_t codeLength, uba** tokens);
