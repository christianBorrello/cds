#ifndef CDS_VECTOR_H
#define CDS_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

/* Opaque type — internals hidden in vector.c */
typedef struct cds_vector cds_vector;

/* Create a new empty vector with elements of the given size in bytes.
 *
 * Returns: pointer to the new vector, or 
 *          NULL if allocation fails. */
cds_vector *cds_vector_create(size_t size);

/* Create a new vector and populate it with n elements copied from src.
 *
 * Returns: pointer to the new vector, or 
 *          NULL if allocation fails or src is NULL. */
cds_vector *cds_vector_from(const void *src, size_t n, size_t size);

/* Pre-allocate space for at least n elements.
 * Does nothing if the current capacity is already sufficient.
 *
 * Returns: CDS_OK on success, 
 *          CDS_ERR_NULL if v is NULL,
 *          CDS_ERR_ALLOC if allocation fails. */
int cds_vector_reserve(cds_vector *v, size_t n);

/* Destroy the vector and free all associated memory.
 * Safe to call with v == NULL (no-op). */
void cds_vector_destroy(cds_vector *v);

/* Get a pointer to the element at the given index.
 *
 * Returns: pointer to the element, or 
 *          NULL if v is NULL or index is out of bounds. */
void *cds_vector_get(const cds_vector *v, size_t index);

/* Overwrite the element at the given index with a copy of element.
 *
 * Returns: CDS_OK on success, 
 *          CDS_ERR_NULL if v or element is NULL,
 *          CDS_ERR_INDEX if index is out of bounds. */
int cds_vector_set(cds_vector *v, size_t index, const void *element);

/* Append a copy of element to the end of the vector.
 * The vector grows automatically if capacity is reached.
 *
 * Returns: CDS_OK on success, 
 *          CDS_ERR_NULL if v or element is NULL,
 *          CDS_ERR_ALLOC if allocation fails. */
int cds_vector_push(cds_vector *v, const void *element);

/* Remove the last element from the vector.
 * If out is not NULL, the removed element is copied into it.
 *
 * Returns: CDS_OK on success, 
 *          CDS_ERR_NULL if v is NULL,
 *          CDS_ERR_INDEX if the vector is empty. */
int cds_vector_pop(cds_vector *v, void *out);

/* Returns: the number of elements in the vector, or 0 if v is NULL. */
size_t cds_vector_size(const cds_vector *v);

/* Returns: the current capacity of the vector, or 0 if v is NULL. */
size_t cds_vector_capacity(const cds_vector *v);

/* Sort the vector in-place using the given comparator.
 *
 * Returns: CDS_OK on success, CDS_ERR_NULL if v or cmp is NULL. */
int cds_vector_sort(cds_vector *v, int (*cmp)(const void *, const void *));

/* Search for key in a sorted vector using binary search.
 * The vector must be sorted according to cmp before calling.
 *
 * Returns: pointer to the matching element, or NULL if not found. */
void *cds_vector_bsearch(const cds_vector *v, const void *key, int (*cmp)(const void *, const void *));

/* Check whether the vector is sorted according to cmp.
 *
 * Returns: true if sorted, false otherwise or if v is NULL. */
bool cds_vector_is_sorted(const cds_vector *v, int (*cmp)(const void *, const void *));

/* Search for key in the vector using linear search.
 * Works on both sorted and unsorted vectors.
 *
 * Returns: pointer to the first matching element, or NULL if not found. */
void *cds_vector_lsearch(const cds_vector *v, const void *key, int (*cmp)(const void *, const void *));

#endif /* CDS_VECTOR_H */
