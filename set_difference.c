
/* vim: set ts=4 sw=4 expandtab: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

void usage(void)
{
    fprintf(stderr, "Usage: set_difference [fileA] [fileB] (optional: fileC...)\n  Prints unique elements present in only fileA.\nA - B (- C...)\n");
}


int main(int argc, char* argv[])
{
    char **values;
    unsigned int i;
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

    setA = file_to_str_set(argv[1]);
    if(!setA)
        return 127;
    for(i=0; i < numOtherSets; i++)
    {
        otherSets[i] = file_to_str_set(argv[i+2]);
        if(!otherSets[i])
            return 127;
    }

    Set *difference = str_set_difference_multi(setA, otherSets, numOtherSets);

    values = str_set_values(difference, NULL);
    for(i=0; values[i] != NULL; i++)
    {
        printf("%s\n", values[i]);
    }

    return 0;
}
