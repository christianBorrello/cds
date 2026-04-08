#ifndef CDS_ALGO_H
#define CDS_ALGO_H

#include <stddef.h>
#include <stdbool.h>

/* Raw-array algorithms. These operate on contiguous memory buffers
 * and are used internally by container modules.
 *
 * Not thread-safe. Callers must synchronize concurrent access */

/* Sort an array in-place using the given comparator — O(n log n)
 *
 * Returns: CDS_ERR_NULL if base or cmp is NULL,
 *          CDS_OK       on success */
int cds_sort(void *base, size_t n, size_t size,
             int (*cmp)(const void *, const void *));

/* Check whether an array is sorted according to cmp — O(n)
 *
 * Returns: true if sorted, false otherwise or if base is NULL */
bool cds_is_sorted(const void *base, size_t n, size_t size,
                   int (*cmp)(const void *, const void *));

/* Search for key in a sorted array using binary search — O(log n)
 *
 * Precondition: the array must be sorted according to cmp.
 * If the array is not sorted, the result is unspecified.
 *
 * Returns: CDS_ERR_NULL      if base, key, cmp, or out is NULL,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            on success */
int cds_bsearch(const void *base, const void *key, size_t n, size_t size,
                int (*cmp)(const void *, const void *), void *out);

/* Search for key in an array using linear search — O(n)
 * Works on both sorted and unsorted arrays
 *
 * Returns: CDS_ERR_NULL      if base, key, cmp, or out is NULL,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            on success */
int cds_lsearch(const void *base, const void *key, size_t n, size_t size,
                int (*cmp)(const void *, const void *), void *out);

#endif /* CDS_ALGO_H */
