
#ifndef _SET_H
#define _SET_H

#include <glib.h>

/*
 * A Set is a highly-optimized (O(1) search, etc) unordered assembly. All Sets herein use char* as their data type. A copy is made of each key stored, so you must use destroy_str_set(set) to free the set, and you can modify/free the keys you've added without interfering with the set.
 */

/* Set - The Set type. Actually only uses the keys portion of the glib Hash Table.
 */
typedef GHashTable Set;

/* SET_ITEM_PRESENT  (Internal) - A constant pointer used to signify a value held at a point. */
extern int SET_ITEM_PRESENT[1];

/* new_str_set - Create a new Set to hold strings. initialValues is NULL for an empty set, or a set of original values.
 *  Values are copied when added into the set. initialValues should be unique, if there are non-unique values, the copy will be leaked.
 *  It is not checked here for performance reasons. str_set_add does check and does not copy if a duplicate value is attempted to be added*/
Set *new_str_set(char **initialValues);

/* destroy_str_set - Destroy a set and free all keys */
void destroy_str_set(Set *set);

/* str_set_add - Add a string to a set. */
void str_set_add(Set *set, char *value);

/* str_set_remove - Remove a string from a set */
void str_set_remove(Set *set, char *value);

/* str_set_values - Return a NULL-terminated list of all strings within the set */
char **str_set_values(Set *set);

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

/* filename_to_set - Reads a file and creates a set, one item per line. */
Set *filename_to_set(const char *filename);

#endif
