/*
 * Copyright (c) 2015 - 2017 Timothy Savannah All Rights Reserved
 *   Licensed under terms of LGPLv3
*/

#ifndef _SET_H
#define _SET_H

#include <glib.h>

/*
 * A Set is a highly-optimized (O(1) search, etc) unordered assembly. All Sets herein use char* as their data type. A copy is made of each key stored, so you must use destroy_set(set) to free the set, and you can modify/free the keys you've added without interfering with the set.
 */


/**************************\
*           DATA           *
\**************************/

/* Set - The Set type. Actually only uses the keys portion of the glib Hash Table.
 */
typedef GHashTable Set;

/* SET_ITEM_PRESENT  (Internal) - A constant pointer used to signify a value held at a point. */
extern int SET_ITEM_PRESENT[1];


typedef void (*SetAddItemFunction)(Set *set, char *item);
typedef Set* (*NewSetFunction)(void*);

typedef Set* (*NewEmptySetFunction)(void);


/*************************\
*          COMMON         *
\*************************/

/* destroy_set - Destroy a set and free all items */
void destroy_set(Set *set);

/* set_length - Returns the length of the given set */
unsigned int set_length(Set *set);

/* file_to_set - Reads a file and creates a set, one item per line. This is meant to be inherited by a specific set type */
Set* file_to_set(const char *filename, NewEmptySetFunction newEmptySetFunction, SetAddItemFunction setAddItemFunction);

/*************************\
*        STRING SET       *
\*************************/

/* new_str_set - Create a new Set to hold strings. initialValues is NULL for an empty set, or a set of original values.
 *  Values are copied when added into the set. initialValues should be unique, if there are non-unique values, the copy will be leaked.
 *  It is not checked here for performance reasons. str_set_add does check and does not copy if a duplicate value is attempted to be added*/
Set* new_str_set(char **initialValues);

Set *new_empty_str_set(void);

/* str_set_add - Add a string to a set. */
void str_set_add(Set *set, char *value);

/* str_set_remove - Remove a string from a set */
void str_set_remove(Set *set, char *value);

/* str_set_contains - Returns 0/1 if the set contains a given value */
int str_set_contains(Set *set, char *value);

/* str_set_values - Return a NULL-terminated list of all strings within the set. len, if non-NULL, will afterwhich contain the set length. */
char **str_set_values(Set *set, unsigned int *len);

/* str_set_intersection - Returns a new set as the intersection of A & B (in A and also in B) */
Set* str_set_intersection(Set *setA, Set *setB);

/* set_set_intersection_multi - Returns a new set as the intersection of A and a list of other sets. */
Set* str_set_intersection_multi(Set *setA, Set **otherSets, unsigned int numOtherSets);

/* str_set_difference - Returns a new set of the difference of A - B (in A but not in B) */
Set* str_set_difference(Set *setA, Set *setB);

/* str_set_difference_multi - Return a new set of the difference of A - B - (...) (in A but not in any other sets) */
Set *str_set_difference_multi(Set *setA, Set **otherSets, unsigned int numOtherSets);

/* str_set_union - Returns a new set of the union of A U B (in A or B) */
Set* str_set_union(Set *setA, Set *setB);

/* str_set_union_multi - Returns a new set of the union of A U B (U C..) (in A or B (or C...)). This is same as str_set_union_all, but with a signature matching the other set functions. */
Set* str_set_union_multi(Set *setA, Set **otherSets, unsigned int numOtherSets);

/* str_set_union_all - Returns a set of the union of all sets. */
Set *str_set_union_all(Set **sets, unsigned int numSets);


#define file_to_str_set(filename) (file_to_set(filename, (NewEmptySetFunction) new_empty_str_set, (SetAddItemFunction)str_set_add))

/*************************\
*      INTEGER  SET       *
\*************************/

/* new_int_set - Create a new Set to hold integers. initialValues is NULL for an empty set, or a set of original values.
 *  Values are copied when added into the set. initialValues should be unique, if there are non-unique values, the copy will be leaked.
 *  It is not checked here for performance reasons. int_set_add does check and does not copy if a duplicate value is attempted to be added*/
Set* new_int_set(int *initialValues, unsigned int numInitialValues);

/* new_empty_int_set - Creates an empty int set */
Set *new_empty_int_set(void);

/* int_set_add - Add an integer to a set. */
void int_set_add(Set *set, int value);

/* int_set_add_str - Convert a string to an integer and add to set */
void int_set_add_str(Set *set, char *value);

/* int_set_remove - Remove an integer from a set */
void int_set_remove(Set *set, int value);

/* int_set_contains - Returns 0/1 if the set contains a given value */
int int_set_contains(Set *set, int value);

/* int_set_values - Return a list of all integers within the set. Because 0 is a valid integer, this is not null-terminated and contains a "length" parameter to hold the length. */
int *int_set_values(Set *set, unsigned int *len);

/* int_set_intersection - Returns a new set as the intersection of A & B (in A and also in B) */
Set* int_set_intersection(Set *setA, Set *setB);

/* set_set_intersection_multi - Returns a new set as the intersection of A and a list of other sets. */
Set* int_set_intersection_multi(Set *setA, Set **otherSets, unsigned int numOtherSets);

/* int_set_difference - Returns a new set of the difference of A - B (in A but not in B) */
Set* int_set_difference(Set *setA, Set *setB);

/* int_set_difference_multi - Return a new set of the difference of A - B - (...) (in A but not in any other sets) */
Set *int_set_difference_multi(Set *setA, Set **otherSets, unsigned int numOtherSets);

/* int_set_union - Returns a new set of the union of A U B (in A or B) */
Set* int_set_union(Set *setA, Set *setB);

/* int_set_union_multi - Returns a new set of the union of A U B (U C..) (in A or B (or C...)). This is same as int_set_union_all, but with a signature matching the other set functions. */
Set* int_set_union_multi(Set *setA, Set **otherSets, unsigned int numOtherSets);

/* int_set_union_all - Returns a set of the union of all sets. */
Set *int_set_union_all(Set **sets, unsigned int numSets);

#define file_to_int_set(filename) (file_to_set(filename, (NewEmptySetFunction) new_empty_int_set, (SetAddItemFunction)int_set_add_str))

/*************************\
*        COMPAT           *
\*************************/

#if GLIB_MAJOR_VERSION < 2 || ( GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION < 40 )

//#warning GLIB IS OLD
gpointer *_fallback_g_hash_table_get_keys_as_array(GHashTable *hash_table, guint *len);
#define g_hash_table_get_keys_as_array(x, y) _fallback_g_hash_table_get_keys_as_array(x, y)
#endif


#endif
