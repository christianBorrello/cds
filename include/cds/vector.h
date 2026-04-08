#ifndef CDS_VECTOR_H
#define CDS_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

/* Opaque type - internals hidden in vector.c
 *
 * Not thread-safe. Callers must synchronize concurrent access */
typedef struct cds_vector cds_vector;

/* ── Lifecycle ─────────────────────────────────────────────────────── */

/* Create a new empty vector with elements of the given size in bytes — O(1)
 *
 * Returns: pointer to the new vector, or
 *          NULL if allocation fails or value_size is 0 */
cds_vector *cds_vector_create(size_t value_size);

/* Create a new vector and populate it with n elements copied from src — O(n)
 *
 * Returns: pointer to the new vector, or
 *          NULL if allocation fails or src is NULL */
cds_vector *cds_vector_from(const void *src, size_t n, size_t value_size);

/* Remove all elements from the vector without destroying it — O(1)
 * Shrinks the internal buffer back to initial capacity
 *
 * Returns: CDS_ERR_NULL if v is NULL,
 *          CDS_OK       on success */
int cds_vector_clear(cds_vector *v);

/* Destroy the vector and free all associated memory — O(1)
 *
 * Safe to call with v = NULL (no-op) */
int cds_vector_destroy(cds_vector *v);

/* ── Access ────────────────────────────────────────────────────────── */

/* Returns: true if the vector is empty or v is NULL, false otherwise — O(1) */
bool cds_vector_empty(const cds_vector *v);

/* Returns: the number of elements in the vector, or 0 if v is NULL — O(1) */
size_t cds_vector_count(const cds_vector *v);

/* Returns: the current capacity of the vector, or 0 if v is NULL — O(1) */
size_t cds_vector_capacity(const cds_vector *v);

/* Copy the element at the given index into out — O(1)
 *
 * Returns: CDS_ERR_NULL  if v or out is NULL,
 *          CDS_ERR_INDEX if index is out of bounds,
 *          CDS_OK        on success */
int cds_vector_get(const cds_vector *v, size_t index, void *out);

/* Overwrite the element at the given index with a copy of value — O(1)
 *
 * Returns: CDS_ERR_NULL  if v or value is NULL,
 *          CDS_ERR_INDEX if index is out of bounds,
 *          CDS_OK        on success */
int cds_vector_set(cds_vector *v, size_t index, const void *value);

/* ── Insertion ─────────────────────────────────────────────────────── */

/* Append a copy of value to the end of the vector — amortized O(1)
 * The vector grows automatically if capacity is reached
 *
 * Returns: CDS_ERR_NULL  if v or value is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_vector_push(cds_vector *v, const void *value);

/* Pre-allocate space for at least n elements — O(n) when growing
 * Does nothing if the current capacity is already sufficient
 *
 * Returns: CDS_ERR_NULL  if v is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_vector_reserve(cds_vector *v, size_t n);

/* ── Removal ───────────────────────────────────────────────────────── */

/* Remove the last element and optionally copy its value into out — O(1)
 *
 * out may be NULL to discard the value.
 *
 * Returns: CDS_ERR_NULL  if v is NULL,
 *          CDS_ERR_EMPTY if the vector is empty,
 *          CDS_OK        on success */
int cds_vector_pop(cds_vector *v, void *out);

/* ── Sort & Search ─────────────────────────────────────────────────── */

/* Sort the vector in-place using the given comparator — O(n log n)
 *
 * Returns: CDS_ERR_NULL if v or cmp is NULL,
 *          CDS_OK       on success */
int cds_vector_sort(cds_vector *v, 
    int (*cmp)(const void *, const void *));

/* Check whether the vector is sorted according to cmp — O(n)
 *
 * Returns: true if sorted, false otherwise or if v is NULL */
bool cds_vector_is_sorted(const cds_vector *v, 
    int (*cmp)(const void *, const void *));

/* Search for key in a sorted vector using binary search — O(log n)
 *
 * Precondition: the vector must be sorted according to cmp.
 * If the vector is not sorted, the result is unspecified.
 *
 * Returns: CDS_ERR_NULL      if v, key, cmp, or out is NULL,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            on success */
int cds_vector_bsearch(const cds_vector *v, const void *key,
    int (*cmp)(const void *, const void *), void *out);

/* Search for key in the vector using linear search — O(n)
 * Works on both sorted and unsorted vectors
 *
 * Returns: CDS_ERR_NULL      if v, key, cmp, or out is NULL,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            on success */
int cds_vector_lsearch(const cds_vector *v, const void *key,
    int (*cmp)(const void *, const void *), void *out);

#endif /* CDS_VECTOR_H */
