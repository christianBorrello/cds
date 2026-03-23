#ifndef CDS_ALGO_H
#define CDS_ALGO_H

#include <stddef.h>
#include <stdbool.h>

/* Sort an array in-place using the given comparator.
 *
 * Returns: CDS_OK on success,
 *          CDS_ERR_NULL if base or cmp is NULL. */
int cds_sort(void *base, size_t n, size_t size,
             int (*cmp)(const void *, const void *));

/* Search for key in a sorted array using binary search.
 * The array must be sorted according to cmp before calling.
 *
 * Returns: pointer to the matching element, or NULL if not found. */
void *cds_bsearch(const void *base, const void *key, size_t n, size_t size,
                  int (*cmp)(const void *, const void *));

/* Check whether an array is sorted according to cmp.
 *
 * Returns: true if sorted, false otherwise or if base is NULL. */
bool cds_is_sorted(const void *base, size_t n, size_t size,
                   int (*cmp)(const void *, const void *));

/* Search for key in an array using linear search.
 * Works on both sorted and unsorted arrays.
 *
 * Returns: pointer to the first matching element, or NULL if not found. */
void *cds_lsearch(const void *base, const void *key, size_t n, size_t size,
                  int (*cmp)(const void *, const void *));

#endif /* CDS_ALGO_H */
