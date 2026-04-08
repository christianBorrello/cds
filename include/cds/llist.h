#ifndef CDS_LLIST_H
#define CDS_LLIST_H

#include <stddef.h>
#include <stdbool.h>

/* Opaque type - internals hidden in llist.c
 *
 * Not thread-safe. Callers must synchronize concurrent access */
typedef struct cds_llist cds_llist;

/* ── Lifecycle ─────────────────────────────────────────────────────── */

/* Create a new empty linked list with elements of the given size in bytes — O(1)
 *
 * Returns: pointer to the new llist, or
 *          NULL if allocation fails or value_size is 0 */
cds_llist *cds_llist_create(size_t value_size);

/* Remove all elements from the list without destroying the list itself — O(n)
 *
 * Returns: CDS_ERR_NULL if ll is NULL,
 *          CDS_OK       on success */
int cds_llist_clear(cds_llist *ll);

/* Destroy the llist and free all memory — O(n)
 *
 * Safe to call with ll = NULL (no-op) */
int cds_llist_destroy(cds_llist *ll);

/* ── Access ────────────────────────────────────────────────────────── */

/* Returns: true if the list is empty or ll is NULL, false otherwise — O(1) */
bool cds_llist_empty(const cds_llist *ll);

/* Returns: the number of elements in the list, or 0 if ll is NULL — O(1) */
size_t cds_llist_count(const cds_llist *ll);

/* Copy the front value into out without removing it — O(1)
 *
 * Returns: CDS_ERR_NULL  if ll or out is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_llist_peek(const cds_llist *ll, void *out);

/* Copy the back value into out without removing it — O(1)
 *
 * Returns: CDS_ERR_NULL  if ll or out is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_llist_back(const cds_llist *ll, void *out);

/* Copy the value at the given index into out — O(n)
 *
 * Returns: CDS_ERR_NULL  if ll or out is NULL,
 *          CDS_ERR_INDEX if index >= count,
 *          CDS_OK        on success */
int cds_llist_get(const cds_llist *ll, size_t index, void *out);

/* ── Insertion ─────────────────────────────────────────────────────── */

/* Insert a value at the head of the list — O(1)
 *
 * Returns: CDS_ERR_NULL  if ll or value is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_llist_prepend(cds_llist *ll, const void *value);

/* Insert a value at the tail of the list — O(1)
 *
 * Returns: CDS_ERR_NULL  if ll or value is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_llist_append(cds_llist *ll, const void *value);

/* Insert a value at the given index — O(n)
 *
 * index == 0 prepends, index == count appends.
 *
 * Returns: CDS_ERR_NULL  if ll or value is NULL,
 *          CDS_ERR_INDEX if index > count,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_llist_insert(cds_llist *ll, const void *value, size_t index);

/* ── Removal ───────────────────────────────────────────────────────── */

/* Pop the front element and optionally copy its value into out — O(1)
 *
 * out may be NULL to discard the value.
 *
 * Returns: CDS_ERR_NULL  if ll is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_llist_pop(cds_llist *ll, void *out);

/* Pop the back element and optionally copy its value into out — O(n)
 *
 * out may be NULL to discard the value.
 * O(n) because the singly-linked list must traverse to find the
 * second-to-last element.
 *
 * Returns: CDS_ERR_NULL  if ll is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_llist_pop_back(cds_llist *ll, void *out);

/* Remove the first element matching value according to cmp — O(n)
 *
 * cmp(a, b) must return 0 when a and b are equal.
 *
 * Returns: CDS_ERR_NULL      if ll or cmp is NULL,
 *          CDS_ERR_EMPTY     if the list is empty,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            on success */
int cds_llist_remove(cds_llist *ll, const void *value, int(*cmp)(const void *, const void *));

/* Purge all elements matching value according to cmp — O(n)
 *
 * cmp(a, b) must return 0 when a and b are equal.
 *
 * Returns: CDS_ERR_NULL      if ll or cmp is NULL,
 *          CDS_ERR_EMPTY     if the list is empty,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            if at least one element was removed */
int cds_llist_purge(cds_llist *ll, const void *value, int(*cmp)(const void *, const void *));

/* ── Search ────────────────────────────────────────────────────────── */

/* Check whether the list contains an element matching value according to cmp — O(n)
 *
 * cmp(a, b) must return 0 when a and b are equal.
 *
 * Returns: CDS_ERR_NULL      if ll, value, or cmp is NULL,
 *          CDS_ERR_EMPTY     if the list is empty,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            if a match exists */
int cds_llist_contains(const cds_llist *ll, const void *value, int(*cmp)(const void *, const void *));

/* Find the first element matching value according to cmp and copy it into out — O(n)
 *
 * cmp(a, b) must return 0 when a and b are equal.
 *
 * Returns: CDS_ERR_NULL      if ll, value, or out is NULL,
 *          CDS_ERR_NOT_FOUND if no match is found,
 *          CDS_OK            on success */
int cds_llist_find(const cds_llist *ll, const void *value, int(*cmp)(const void *, const void *), void *out);

#endif /* CDS_LLIST_H */
