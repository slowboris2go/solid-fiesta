// Implementation of an unbounded array

#include "uba.h"


uba* ubaNew(size_t capacity)
{
    uba* result = malloc(sizeof(uba));
    result->capacity = capacity;
    result->length = 0;
    result->data = calloc(capacity, sizeof(char*));
}

void ubaAdd(uba* self, void* element)
{
    // Double size of data if full
    if(self->length == self->capacity)
    {
        void** newData = calloc(self->capacity*2, sizeof(void*));
        memcpy(newData, self->data, self->capacity*sizeof(void*));
        free(self->data);
        self->data = newData;
        self->capacity = 2*self->capacity;
    }
    // Add element
    self->data[self->length] = element;
    self->length++;
}

void* ubaIndex(uba* self, size_t idx)
{
    return self->data[idx];
}

void ubaFree(uba* self)
{
    free(self->data);
    free(self);
}

size_t ubaSize(uba* self)
{
    return self->length;
}