#ifndef CDS_LINKED_LIST_H
#define CDS_LINKED_LIST_H

#include <stddef.h>

/* Opaque type - internals hidden in linked_list.c */
typedef struct cds_linked_list cds_linked_list;

/* ── Lifecycle ─────────────────────────────────────────────────────── */

/* Create a new empty linked list with elements of the given size in bytes
 *
 * Returns: pointer to the new linked list, or
 *          NULL if allocation fails or element_size is 0 */
cds_linked_list *cds_linked_list_create(size_t element_size);

/* Remove all nodes from the list without destroying the list itself
 *
 * Returns: CDS_ERR_NULL if ll is NULL,
 *          CDS_OK       on success */
int cds_linked_list_clear(cds_linked_list *ll);

/* Destroy the linked list and free all nodes
 *
 * Safe to call with ll = NULL (no-op) */
int cds_linked_list_destroy(cds_linked_list *ll);

/* ── Access ────────────────────────────────────────────────────────── */

/* Returns: the number of nodes in the list, or 0 if ll is NULL */
size_t cds_linked_list_size(const cds_linked_list *ll);

/* Copy the head value into out without removing it — O(1)
 *
 * Returns: CDS_ERR_NULL  if ll or out is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_linked_list_peek_head(const cds_linked_list *ll, void *out);

/* Copy the tail value into out without removing it — O(1)
 *
 * Returns: CDS_ERR_NULL  if ll or out is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_linked_list_peek_tail(const cds_linked_list *ll, void *out);

/* Copy the value at the given index into out — O(n)
 *
 * Returns: CDS_ERR_NULL  if ll or out is NULL,
 *          CDS_ERR_INDEX if index >= size,
 *          CDS_OK        on success */
int cds_linked_list_get(const cds_linked_list *ll, size_t index, void *out);

/* ── Insertion ─────────────────────────────────────────────────────── */

/* Insert a node at the head of the list
 *
 * Returns: CDS_ERR_NULL  if ll or value is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_linked_list_prepend(cds_linked_list *ll, const void *value);

/* Insert a node at the tail of the list
 *
 * Returns: CDS_ERR_NULL  if ll or value is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_linked_list_append(cds_linked_list *ll, const void *value);

/* Insert a node at the specified index
 *
 * index == 0 prepends, index == size appends.
 *
 * Returns: CDS_ERR_NULL  if ll or value is NULL,
 *          CDS_ERR_INDEX if index > size,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_linked_list_insert_at(cds_linked_list *ll, const void *value, size_t index);

/* ── Removal ───────────────────────────────────────────────────────── */

/* Remove the head node and optionally copy its value into out
 *
 * out may be NULL to discard the value.
 *
 * Returns: CDS_ERR_NULL  if ll is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_linked_list_remove_head(cds_linked_list *ll, void *out);

/* Remove the tail node and optionally copy its value into out
 *
 * out may be NULL to discard the value.
 *
 * Returns: CDS_ERR_NULL  if ll is NULL,
 *          CDS_ERR_EMPTY if the list is empty,
 *          CDS_OK        on success */
int cds_linked_list_remove_tail(cds_linked_list *ll, void *out);

/* Remove the first node whose value matches according to cmp
 *
 * cmp(a, b) must return 0 when a and b are equal.
 *
 * Returns: CDS_ERR_NULL      if ll or cmp is NULL,
 *          CDS_ERR_EMPTY     if the list is empty,
 *          CDS_ERR_NOT_FOUND if no node matches,
 *          CDS_OK            on success */
int cds_linked_list_remove(cds_linked_list *ll, const void *value, int(*cmp)(const void *, const void *));

/* Remove all nodes whose value matches according to cmp
 *
 * cmp(a, b) must return 0 when a and b are equal.
 *
 * Returns: CDS_ERR_NULL      if ll or cmp is NULL,
 *          CDS_ERR_EMPTY     if the list is empty,
 *          CDS_ERR_NOT_FOUND if no node matches,
 *          CDS_OK            if at least one node was removed */
int cds_linked_list_remove_all(cds_linked_list *ll, const void *value, int(*cmp)(const void *, const void *));

/* ── Search ────────────────────────────────────────────────────────── */

/* Check whether the list contains a node matching value according to cmp
 *
 * cmp(a, b) must return 0 when a and b are equal.
 *
 * Returns: CDS_ERR_NULL      if ll, value, or cmp is NULL,
 *          CDS_ERR_EMPTY     if the list is empty,
 *          CDS_ERR_NOT_FOUND if no node matches,
 *          CDS_OK            if a matching node exists */
int cds_linked_list_contains(const cds_linked_list *ll, const void *value, int(*cmp)(const void *, const void *));

#endif /* CDS_LINKED_LIST_H */
