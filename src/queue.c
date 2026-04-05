#include "cds/common.h"
#include "cds/queue.h"
#include "cds/linked_list.h"
#include <stdlib.h>

struct cds_queue
{
  cds_linked_list *data;
};


cds_queue *cds_queue_create(size_t value_size)
{
  if (value_size == 0) return NULL;
  cds_queue *q = malloc(sizeof(*q));
  if (!q) return NULL;
  q->data = cds_linked_list_create(value_size);
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
  return cds_linked_list_clear(q->data);
}

int cds_queue_destroy(cds_queue *q)
{
  if (!q) return CDS_OK;
  cds_linked_list *tmp = q->data;
  free(q);
  return cds_linked_list_destroy(tmp);
}

bool cds_queue_empty(const cds_queue *q)
{
  if (!q) return true;
  return cds_linked_list_size(q->data) == 0;
}

size_t cds_queue_size(const cds_queue *q)
{
  if (!q) return 0;
  return cds_linked_list_size(q->data);
}

int cds_queue_enq(cds_queue *q, const void *value)
{
  if (!q) return CDS_ERR_NULL;
  return cds_linked_list_append(q->data, value);
}

int cds_queue_deq(cds_queue *q, void *out)
{
  if (!q) return CDS_ERR_NULL;
  return cds_linked_list_remove_head(q->data, out);
}

int cds_queue_peek(const cds_queue *q, void *out)
{
  if (!q) return CDS_ERR_NULL;
  return cds_linked_list_peek_head(q->data, out);
}

