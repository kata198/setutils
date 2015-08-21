
/* vim: set ts=4 sw=4 expandtab: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "set.h"

int SET_ITEM_PRESENT[1] = {1};

Set* new_str_set(char **initialValues) {
    /* The value is the reused pointer SET_ITEM_PRESENT, we don't free it. The keys, however, are always copies and we do free them.
     */
    Set *set = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    unsigned int i;
    if(initialValues != NULL)
    {
        for(i=0; initialValues[i] != NULL; i++)
        {
            g_hash_table_insert(set, g_strdup(initialValues[i]), SET_ITEM_PRESENT);
        }

    }
    return set;
}

void destroy_str_set(Set *set) {
    g_hash_table_destroy(set);
}

Set* copy_str_set(Set *set)
{
    return new_str_set(str_set_values(set));
}

void str_set_add(Set *set, char *value) {
    /* Only insert if value is not already present. There would still only be one entry, but because of copying pointer
     *   we would have a leak. Could use g_hash_table_replace, but that's more expensive than just checking, because it would check-copy-free instead of just check.
     */
    if(g_hash_table_lookup(set, value) == NULL)
        g_hash_table_insert(set, g_strdup(value), SET_ITEM_PRESENT);
}

void str_set_remove(Set *set, char *value) {
    g_hash_table_remove(set, value);
}


#if GLIB_MAJOR_VERSION < 2 || ( GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION < 40 )

char **_fallback_g_hash_table_get_keys_as_array(GHashTable *hash_table, guint *len)
{
    GList *keys;
    GList *cur;
    char **ret;
    unsigned int i, len2;

    keys = g_hash_table_get_keys(hash_table);
    len2 = g_list_length(keys);
    if ( len != NULL )
        *len = len2;

    ret = malloc(sizeof(char*) * (*len + 1));

    cur = keys;

    for(i=0; i < len2; i++)
    {
        ret[i] = g_strdup(cur->data);
        cur = cur->next;
    }

    return ret;
}
#define g_hash_table_get_keys_as_array(x, y) _fallback_g_hash_table_get_keys_as_array(x, y)


#endif


char** str_set_values(Set *set) {
    unsigned int junk;

    return (char **)g_hash_table_get_keys_as_array(set, &junk);
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
        if(g_hash_table_lookup(setB, setAValues[i]) != NULL)
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
            if(g_hash_table_lookup(otherSets[otherSetNo], thisValue) == NULL)
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
        if(g_hash_table_lookup(setB, setAValues[i]) == NULL)
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
            if(g_hash_table_lookup(otherSets[otherSetNo], thisValue) != NULL)
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
    Set *ret = new_str_set((char **)g_hash_table_get_keys_as_array(setA, &setBLen));

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
        setValues = str_set_values(otherSets[otherSetNo]);
        while(*setValues)
        {
            str_set_add(ret, *setValues);
            setValues += 1;
        }
    }
    return ret;

}

Set *filename_to_set(const char *filename)
{
    char *contents;
    char *nextItem;
    gsize length;
    gboolean readSuccess;
    GError *error = NULL;
    Set *ret = new_str_set(NULL);

    /* TODO: Check Error? */
    readSuccess = g_file_get_contents(filename, &contents, &length, &error);

    if(!readSuccess)
    {
        fprintf(stderr, "%s\n", error->message);
        return NULL;
    }

    nextItem = strtok(contents, "\n");
    while( nextItem )
    {
        str_set_add(ret, nextItem);
        nextItem = strtok(NULL, "\n");
    }

    return ret;
}

