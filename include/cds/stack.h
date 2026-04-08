#ifndef CDS_STACK_H
#define CDS_STACK_H

#include <stddef.h>
#include <stdbool.h>

/* Opaque type - internals hidden in stack.c
 *
 * Not thread-safe. Callers must synchronize concurrent access */
typedef struct cds_stack cds_stack;

/* ── Lifecycle ─────────────────────────────────────────────────────── */

/* Create a new empty stack with elements of the given size in bytes — O(1)
 *
 * Returns: pointer to the new stack, or
 *          NULL if allocation fails or value_size is 0 */
cds_stack *cds_stack_create(size_t value_size);

/* Remove all elements from the stack without destroying it — O(n)
 *
 * Returns: CDS_ERR_NULL if s is NULL,
 *          CDS_OK       on success */
int cds_stack_clear(cds_stack *s);

/* Destroy the stack and free all associated memory — O(n)
 *
 * Safe to call with s = NULL (no-op) */
int cds_stack_destroy(cds_stack *s);

/* ── Access ────────────────────────────────────────────────────────── */

/* Returns: true if the stack is empty or s is NULL, false otherwise — O(1) */
bool cds_stack_empty(const cds_stack *s);

/* Returns: the number of elements in the stack, or 0 if s is NULL — O(1) */
size_t cds_stack_count(const cds_stack *s);

/* Copy the top element into out without removing it — O(1)
 *
 * Returns: CDS_ERR_NULL  if s or out is NULL,
 *          CDS_ERR_EMPTY if the stack is empty,
 *          CDS_OK        on success */
int cds_stack_peek(const cds_stack *s, void *out);

/* ── Operations ────────────────────────────────────────────────────── */

/* Push a copy of value onto the top of the stack — O(1)
 *
 * Returns: CDS_ERR_NULL  if s or value is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_stack_push(cds_stack *s, const void *value);

/* Remove the top element and optionally copy it into out — O(1)
 *
 * out may be NULL to discard the value.
 *
 * Returns: CDS_ERR_NULL  if s is NULL,
 *          CDS_ERR_EMPTY if the stack is empty,
 *          CDS_OK        on success */
int cds_stack_pop(cds_stack *s, void *out);

#endif /* CDS_STACK_H */
