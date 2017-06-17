/*
 * Copyright (c) 2015 - 2017 Timothy Savannah All Rights Reserved
 *   Licensed under terms of LGPLv3
*/

/* vim: set ts=4 sw=4 expandtab: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

void usage(void)
{
    fprintf(stderr, "Usage: iset_union [fileA] [fileB] (fileC...)\n  Prints the union of all sets.\n All elements must be integers. For strings, use set_union.\n  A U B (U C...)\n\n");
}

int main(int argc, char* argv[])
{
    int *values;
    unsigned int i;
    Set **sets;
    unsigned int numSets = argc-1;
    unsigned int numValues;

    if(argc < 3)
    {
        usage();
        return 1;
    }
    for(i=1; i < argc; i++)
    {
        if(strcmp("--help", argv[i]) == 0)
        {
            usage();
            return 1;
        }
    }

    sets = malloc(sizeof(Set*) * numSets);

    for(i=0; i < numSets; i++)
    {
        sets[i] = file_to_int_set(argv[i+1]);
        if(!sets[i])
            return 127;
    }
        
    Set *sunion = int_set_union_all(sets, numSets);

    for(i=0; i < numSets; i++)
        destroy_set(sets[i]);

    values = int_set_values(sunion, &numValues);
    for(i=0; i < numValues; i++)
    {
        printf("%d\n", values[i]);
    }
    free(values);
    destroy_set(sunion);

    return 0;
}
