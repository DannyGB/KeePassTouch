#include "arrayextensions.h"

ArrayExtensions::ArrayExtensions()
{
}

vector<char> ArrayExtensions::toVector(char* source, unsigned int size)
{
    vector<char> destination;
    for(unsigned int i = 0; i<size;i++) {
        destination.push_back(source[i]);
    }

    return destination;
}
