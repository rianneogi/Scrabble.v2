#ifndef HASHING_H
#define HASHING_H

#include <stdio.h>
#include <string.h>

struct word
{
    char tempw[17];
};

unsigned long hash(char* str);
int createhashfile();
int searchhash(struct word s);

#endif