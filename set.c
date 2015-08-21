
/* vim: set ts=4 sw=4 expandtab: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "set.h"

int SET_ITEM_PRESENT[1] = {1};

void destroy_set(Set *set) {
    g_hash_table_destroy(set);
}

unsigned int set_length(Set *set) {
    return g_hash_table_size(set);
}


#if GLIB_MAJOR_VERSION < 2 || ( GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION < 40 )

gpointer* _fallback_g_hash_table_get_keys_as_array(GHashTable *hash_table, guint *len)
{
    GList *keys;
    GList *cur;
    gpointer *ret;
    unsigned int i, len2;

    keys = g_hash_table_get_keys(hash_table);
    len2 = g_list_length(keys);
    if ( len != NULL )
        *len = len2;

    ret = malloc(sizeof(char*) * (len2 + 1));

    cur = keys;

    for(i=0; i < len2; i++)
    {
        ret[i] = cur->data; //g_strdup(cur->data);
        cur = cur->next;
    }
    ret[len2] = NULL;

    return ret;
}

#endif


Set* file_to_set(const char *filename, NewEmptySetFunction newEmptySetFunction, SetAddItemFunction setAddItemFunction)
{
    char *contents;
    gpointer nextItem;
    gsize length;
    gboolean readSuccess;
    GError *error = NULL;
    Set *ret = newEmptySetFunction();

    readSuccess = g_file_get_contents(filename, &contents, &length, &error);

    if(!readSuccess)
    {
        fprintf(stderr, "%s\n", error->message);
        free(contents);
        return NULL;
    }

    nextItem = strtok(contents, "\n");
    while( nextItem )
    {
        setAddItemFunction(ret, nextItem);
        nextItem = strtok(NULL, "\n");
    }
    free(contents);

    return ret;
}

