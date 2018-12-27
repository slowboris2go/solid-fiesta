// Contains function prototypes
#pragma once

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include"../uba/uba.h"
#include"../tokenizer/tokenizer.h"

// The code tree is an array of parse entries

typedef enum
{
    LITERAL,
    LIST
} entryType;

typedef struct
{
    entryType type;
    union
    {
        char* literal;
        uba* list;
    };
} parseEntry;

uba* parse(uba* tokens, size_t *index);

bool parenCheck(uba* tokens, size_t* lineNumber);
