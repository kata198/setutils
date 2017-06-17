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

Set* new_empty_str_set(void)
{
    return g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}

Set* new_str_set(char **initialValues) {
    /* The value is the reused pointer SET_ITEM_PRESENT, we don't free it. The keys, however, are always copies and we do free them.
     */
    Set *set = new_empty_str_set();
    if(initialValues != NULL)
    {
        unsigned int i;
        for(i=0; initialValues[i] != NULL; i++)
        {
            g_hash_table_insert(set, g_strdup(initialValues[i]), SET_ITEM_PRESENT);
        }

    }
    return set;
}

Set* copy_str_set(Set *set)
{
    return new_str_set(str_set_values(set, NULL));
}

void str_set_add(Set *set, char *value) {
    /* Only insert if value is not already present. There would still only be one entry, but because of copying pointer
     *   we would have a leak. Could use g_hash_table_replace, but that's more expensive than just checking, because it would check-copy-free instead of just check.
     */
    if(!str_set_contains(set, value))
        g_hash_table_insert(set, g_strdup(value), SET_ITEM_PRESENT);
}

void str_set_remove(Set *set, char *value) {
    g_hash_table_remove(set, value);
}

int str_set_contains(Set *set, char *value) {
    return !!(g_hash_table_lookup(set, value) != NULL);
}


char** str_set_values(Set *set, unsigned int *len) {
    return (char **)g_hash_table_get_keys_as_array(set, len);
}

Set* str_set_intersection(Set *setA, Set *setB)
{
    char **setAValues;
    unsigned int setALen;
    unsigned int i;
    Set *ret = new_str_set(NULL);

    setAValues = (char **)g_hash_table_get_keys_as_array(setA, &setALen);
    for(i=0; i < setALen; i++)
    {
        if(str_set_contains(setB, setAValues[i]))
            str_set_add(ret, setAValues[i]);
    }

    return ret;
}
Set* str_set_intersection_multi(Set *setA, Set **otherSets, unsigned int numOtherSets)
{
    if(numOtherSets == 1)
        return str_set_intersection(setA, otherSets[0]);

    char **setAValues;
    char *thisValue;
    unsigned int setALen;
    unsigned int setAIdx, otherSetNo;
    unsigned int matchesAll;
    Set *ret = new_str_set(NULL);

    setAValues = (char **)g_hash_table_get_keys_as_array(setA, &setALen);
    for(setAIdx=0; setAIdx < setALen; setAIdx++)
    {
        thisValue = setAValues[setAIdx];
        matchesAll = 1;  
        for(otherSetNo=0; otherSetNo<numOtherSets; otherSetNo++)
        {
            if(!str_set_contains(otherSets[otherSetNo], thisValue))
            {
                matchesAll = 0;
                break;
            }
        }
        if(matchesAll)
            str_set_add(ret, thisValue);
    }

    return ret;
}

Set* str_set_difference(Set *setA, Set *setB)
{
    char **setAValues;
    unsigned int setALen;
    unsigned int i;
    Set *ret = new_str_set(NULL);

    setAValues = (char **)g_hash_table_get_keys_as_array(setA, &setALen);
    for(i=0; i < setALen; i++)
    {
        if(!str_set_contains(setB, setAValues[i]))
            str_set_add(ret, setAValues[i]);
    }

    return ret;
}

Set *str_set_difference_multi(Set *setA, Set **otherSets, unsigned int numOtherSets)
{
    if(numOtherSets == 1)
        return str_set_difference(setA, otherSets[0]);

    char **setAValues;
    char *thisValue;
    unsigned int setALen;
    unsigned int setAIdx, otherSetNo;
    unsigned int matchesAny;
    Set *ret = new_str_set(NULL);

    setAValues = (char **)g_hash_table_get_keys_as_array(setA, &setALen);
    for(setAIdx=0; setAIdx < setALen; setAIdx++)
    {
        thisValue = setAValues[setAIdx];
        matchesAny = 0;
        for(otherSetNo=0; otherSetNo<numOtherSets; otherSetNo++)
        {
            if(str_set_contains(otherSets[otherSetNo], thisValue))
            {
                matchesAny = 1;
                break;
            }
        }
        if(!matchesAny)
            str_set_add(ret, thisValue);
    }

    return ret;
}

Set* str_set_union(Set *setA, Set *setB)
{
    char **setBValues;
    unsigned int setBLen;
    unsigned int i;
    Set *ret = copy_str_set(setA);

    setBValues = (char **)g_hash_table_get_keys_as_array(setB, &setBLen);
    for(i=0; i < setBLen; i++)
    {
        str_set_add(ret, setBValues[i]);
    }

    return ret;
}

Set* str_set_union_all(Set **sets, unsigned int numSets)
{
    switch(numSets)
    {
        case 0:
            return new_str_set(NULL);
            break;
        case 1:
            return copy_str_set(sets[0]);
            break;
        case 2:
            return str_set_union(sets[0], sets[1]);
            break;
        default:
            return str_set_union_multi(sets[0], sets+1, numSets-1);
            break;
    }
}
            

Set* str_set_union_multi(Set *setA, Set **otherSets, unsigned int numOtherSets)
{
    char **setValues;
    unsigned int otherSetNo;
    Set *ret = copy_str_set(setA);
    for(otherSetNo=0; otherSetNo<numOtherSets; otherSetNo++)
    {
        setValues = str_set_values(otherSets[otherSetNo], NULL);
        while(*setValues)
        {
            str_set_add(ret, *setValues);
            setValues += 1;
        }
    }
    return ret;

}

