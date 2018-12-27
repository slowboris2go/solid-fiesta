// Provides unbounded array functionality
#pragma once

#include<stdlib.h>
#include<string.h>

typedef struct
{
    size_t capacity; // Length of the data array
    size_t length; // Length of allocated data
    void** data; // An array of strings
} uba;


uba* ubaNew(size_t capacity);

void ubaAdd(uba* self, void* element);


void* ubaIndex(uba* self, size_t idx);

void ubaFree(uba* self);

size_t ubaSize(uba* self);
