/*
 * Copyright (c) 2015 - 2017 Timothy Savannah All Rights Reserved
 *   Licensed under terms of LGPLv3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

void usage(void)
{
	fprintf(stderr, "Usage: set_intersection [fileA] [fileB] (optional, fileC...)\n  Prints a list of the elements common in both fileA and fileB (and any additionals).\n A & B (& C...)\n");
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

	Set *intersection = str_set_intersection_multi(setA, otherSets, numOtherSets);

	values = str_set_values(intersection, NULL);
	for(i=0; values[i] != NULL; i++)
	{
		printf("%s\n", values[i]);
	}

	return 0;
}
