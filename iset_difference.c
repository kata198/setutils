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
    fprintf(stderr, "Usage: iset_difference [fileA] [fileB] (optional: fileC...)\n  Prints unique elements present in only fileA.\n All elements must be integers. For other types, use set_difference\n  A - B (- C...)\n");
}


int main(int argc, char* argv[])
{
    int *values;
    unsigned int i, numValues;
    Set *setA;
    Set **otherSets;
    unsigned int numOtherSets = argc-2;

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

    otherSets = malloc(sizeof(Set*) * numOtherSets);

    setA = file_to_int_set(argv[1]);
    if(!setA)
        return 127;
    for(i=0; i < numOtherSets; i++)
    {
        otherSets[i] = file_to_int_set(argv[i+2]);
        if(!otherSets[i])
            return 127;
    }

    Set *difference = int_set_difference_multi(setA, otherSets, numOtherSets);

    values = int_set_values(difference, &numValues);
    for(i=0; i < numValues; i++)
    {
        printf("%d\n", values[i]);
    }

    return 0;
}
