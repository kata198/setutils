/*
 * Copyright (c) 2015 - 2017 Timothy Savannah All Rights Reserved
 *   Licensed under terms of LGPLv3
*/

/* vim: set ts=4 sw=4 expandtab: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "set.h"

#define INTVAL(_x) GINT_TO_POINTER(_x)

Set *new_empty_int_set(void) {
    return g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
}

Set* new_int_set(int *initialValues, unsigned int numInitialValues) {
    /* The value is the reused pointer SET_ITEM_PRESENT, we don't free it. The keys, however, are always copies and we do free them.
     */
    Set *set = new_empty_int_set();
    if(initialValues != NULL && numInitialValues > 0)
    {
        unsigned int i;
        for(i=0; i < numInitialValues; i++)
        {
            g_hash_table_insert(set, INTVAL(initialValues[i]), SET_ITEM_PRESENT);
        }

    }
    return set;
}

Set* copy_int_set(Set *set)
{
    unsigned int setLen;
    int *values = int_set_values(set, &setLen);
    return new_int_set(values, setLen);
}

void int_set_add(Set *set, int value) {
    /* Only insert if value is not already present. There would still only be one entry, but because of copying pointer
     *   we would have a leak. Could use g_hash_table_replace, but that's more expensive than just checking, because it would check-copy-free instead of just check.
     */
    if(!int_set_contains(set, value))
        g_hash_table_insert(set, INTVAL(value), SET_ITEM_PRESENT);
}

void int_set_add_str(Set *set, char *value) {
    int valueI = atoi(value);
    int_set_add(set, valueI);
}

void int_set_remove(Set *set, int value) {
    g_hash_table_remove(set, INTVAL(value));
}

int int_set_contains(Set *set, int value) {
    return !!(g_hash_table_lookup(set, INTVAL(value)) != NULL);
}

int* int_set_values(Set *set, unsigned int *len) {
    int *ret;
    unsigned int lenVal, i;

    lenVal = set_length(set);
    if(len != NULL)
        *len = lenVal;
    if(lenVal == 0)
    {
        /* What to do when empty set? I guess return NULL. */
        return NULL;
    }
    ret = malloc(sizeof(int) * lenVal);
    GList *keys = g_hash_table_get_keys(set);
    GList *curKey = keys;
    for(i=0; !!(curKey); i++, curKey=curKey->next)
    {
        ret[i] = GPOINTER_TO_INT(curKey->data);
    }

    return ret;
}

Set* int_set_intersection(Set *setA, Set *setB)
{
    int *setAValues;
    unsigned int setALen;
    unsigned int i;
    Set *ret = new_int_set(NULL, 0);
    
    setAValues = int_set_values(setA, &setALen);
    for(i=0; i < setALen; i++)
    {
        if(int_set_contains(setB, setAValues[i]))
            int_set_add(ret, setAValues[i]);
    }
    free(setAValues);

    return ret;
}
Set* int_set_intersection_multi(Set *setA, Set **otherSets, unsigned int numOtherSets)
{
    if(numOtherSets == 1)
        return int_set_intersection(setA, otherSets[0]);

    int *setAValues;
    int thisValue;
    unsigned int setALen;
    unsigned int setAIdx, otherSetNo;
    unsigned int matchesAll;
    Set *ret = new_int_set(NULL, 0);

    setAValues = int_set_values(setA, &setALen);
    for(setAIdx=0; setAIdx < setALen; setAIdx++)
    {
        thisValue = setAValues[setAIdx];
        matchesAll = 1;  
        for(otherSetNo=0; otherSetNo<numOtherSets; otherSetNo++)
        {
            if(!int_set_contains(otherSets[otherSetNo], thisValue))
            {
                matchesAll = 0;
                break;
            }
        }
        if(matchesAll)
            int_set_add(ret, thisValue);
    }
    free(setAValues);

    return ret;
}

Set* int_set_difference(Set *setA, Set *setB)
{
    int *setAValues;
    unsigned int setALen;
    unsigned int i;
    Set *ret = new_int_set(NULL, 0);

    setAValues = int_set_values(setA, &setALen);
    for(i=0; i < setALen; i++)
    {
        if(!int_set_contains(setB, setAValues[i]))
            int_set_add(ret, setAValues[i]);
    }
    free(setAValues);

    return ret;
}

Set *int_set_difference_multi(Set *setA, Set **otherSets, unsigned int numOtherSets)
{
    if(numOtherSets == 1)
        return int_set_difference(setA, otherSets[0]);

    int *setAValues;
    int thisValue;
    unsigned int setALen;
    unsigned int setAIdx, otherSetNo;
    int matchesAny;
    Set *ret = new_int_set(NULL, 0);

    setAValues = int_set_values(setA, &setALen);
    for(setAIdx=0; setAIdx < setALen; setAIdx++)
    {
        thisValue = setAValues[setAIdx];
        matchesAny = 0;
        for(otherSetNo=0; otherSetNo<numOtherSets; otherSetNo++)
        {
            if(int_set_contains(otherSets[otherSetNo], thisValue))
            {
                matchesAny = 1;
                break;
            }
        }
        if(!matchesAny)
            int_set_add(ret, thisValue);
    }
    free(setAValues);

    return ret;
}

Set* int_set_union(Set *setA, Set *setB)
{
    int *setBValues;
    unsigned int setBLen;
    unsigned int i;
    Set *ret = copy_int_set(setA);

    setBValues = int_set_values(setB, &setBLen);
    for(i=0; i < setBLen; i++)
    {
        int_set_add(ret, setBValues[i]);
    }
    free(setBValues);

    return ret;
}

Set* int_set_union_all(Set **sets, unsigned int numSets)
{
    switch(numSets)
    {
        case 0:
            return new_int_set(NULL, 0);
            break;
        case 1:
            return copy_int_set(sets[0]);
            break;
        case 2:
            return int_set_union(sets[0], sets[1]);
            break;
        default:
            return int_set_union_multi(sets[0], sets+1, numSets-1);
            break;
    }
}
            

Set* int_set_union_multi(Set *setA, Set **otherSets, unsigned int numOtherSets)
{
    int *setValues;
    unsigned int otherSetNo;
    unsigned int numValues;
    unsigned int i;
    Set *ret = copy_int_set(setA);
    for(otherSetNo=0; otherSetNo<numOtherSets; otherSetNo++)
    {
        setValues = int_set_values(otherSets[otherSetNo], &numValues);
        for(i=0; i < numValues; i++)
        {
            int_set_add(ret, setValues[i]);
        }
        free(setValues);
    }

    return ret;

}

