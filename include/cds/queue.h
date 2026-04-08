#ifndef CDS_QUEUE_H
#define CDS_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

/* Opaque type - internals hidden in queue.c
 *
 * Not thread-safe. Callers must synchronize concurrent access */
typedef struct cds_queue cds_queue;

/* ── Lifecycle ─────────────────────────────────────────────────────── */

/* Create a new empty queue with elements of the given size in bytes — O(1)
 *
 * Returns: pointer to the new queue, or
 *          NULL if allocation fails or value_size is 0 */
cds_queue *cds_queue_create(size_t value_size);

/* Remove all elements from the queue without destroying it — O(n)
 *
 * Returns: CDS_ERR_NULL if q is NULL,
 *          CDS_OK       on success */
int cds_queue_clear(cds_queue *q);

/* Destroy the queue and free all associated memory — O(n)
 *
 * Returns: CDS_ERR_NULL if q is NULL,
 *          CDS_OK       on success */
int cds_queue_destroy(cds_queue *q);

/* ── Access ────────────────────────────────────────────────────────── */

/* Returns: true if the queue is empty or q is NULL, false otherwise — O(1) */
bool cds_queue_empty(const cds_queue *q);

/* Returns: the number of elements in the queue, or 0 if q is NULL — O(1) */
size_t cds_queue_count(const cds_queue *q);

/* Copy the front element into out without removing it — O(1)
 *
 * Returns: CDS_ERR_NULL  if q or out is NULL,
 *          CDS_ERR_EMPTY if the queue is empty,
 *          CDS_OK        on success */
int cds_queue_peek(const cds_queue *q, void *out);

/* ── Operations ────────────────────────────────────────────────────── */

/* Add a copy of value to the back of the queue — O(1)
 *
 * Returns: CDS_ERR_NULL  if q or value is NULL,
 *          CDS_ERR_ALLOC if allocation fails,
 *          CDS_OK        on success */
int cds_queue_enq(cds_queue *q, const void *value);

/* Remove the front element and optionally copy it into out — O(1)
 *
 * out may be NULL to discard the value.
 *
 * Returns: CDS_ERR_NULL  if q is NULL,
 *          CDS_ERR_EMPTY if the queue is empty,
 *          CDS_OK        on success */
int cds_queue_deq(cds_queue *q, void *out);

#endif /* CDS_QUEUE_H */
