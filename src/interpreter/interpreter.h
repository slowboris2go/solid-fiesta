#pragma once

// Contains structures and function declarations
#include "../uba/uba.h"
#include "../tokenizer/tokenizer.h"
#include <stdint.h>
#include <stdio.h>

// The state of the interpreter
typedef struct 
{
    size_t programIndex;
} InterpreterState;


void interpret(uba* parseTree);