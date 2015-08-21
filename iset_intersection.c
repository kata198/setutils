
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

void usage(void)
{
	fprintf(stderr, "Usage: iset_intersection [fileA] [fileB] (optional, fileC...)\n  Prints a list of the elements common in both fileA and fileB (and any additionals).\n  All items must be integers, use set_intersection for all other types.\n A & B (& C...)\n");
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

	Set *intersection = int_set_intersection_multi(setA, otherSets, numOtherSets);

	values = int_set_values(intersection, &numValues);
	for(i=0; i < numValues; i++)
	{
		printf("%d\n", values[i]);
	}

	return 0;
}
