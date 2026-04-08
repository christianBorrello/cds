#include "cds/common.h"
#include "cds/queue.h"
#include "cds/llist.h"
#include <stdlib.h>

struct cds_queue
{
  cds_llist *data;
};


cds_queue *cds_queue_create(size_t value_size)
{
  if (value_size == 0) return NULL;
  cds_queue *q = malloc(sizeof(*q));
  if (!q) return NULL;
  q->data = cds_llist_create(value_size);
  if (!(q->data))
  {
    free(q);
    return NULL;
  }
  return q;
}

int cds_queue_clear(cds_queue *q)
{
  if (!q) return CDS_ERR_NULL;
  return cds_llist_clear(q->data);
}

int cds_queue_destroy(cds_queue *q)
{
  if (!q) return CDS_OK;
  cds_llist *tmp = q->data;
  free(q);
  return cds_llist_destroy(tmp);
}

bool cds_queue_empty(const cds_queue *q)
{
  if (!q) return true;
  return cds_llist_count(q->data) == 0;
}

size_t cds_queue_count(const cds_queue *q)
{
  if (!q) return 0;
  return cds_llist_count(q->data);
}

int cds_queue_enq(cds_queue *q, const void *value)
{
  if (!q) return CDS_ERR_NULL;
  return cds_llist_append(q->data, value);
}

int cds_queue_deq(cds_queue *q, void *out)
{
  if (!q) return CDS_ERR_NULL;
  return cds_llist_pop(q->data, out);
}

int cds_queue_peek(const cds_queue *q, void *out)
{
  if (!q) return CDS_ERR_NULL;
  return cds_llist_peek(q->data, out);
}

