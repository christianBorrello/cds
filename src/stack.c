#include "cds/common.h"
#include "cds/stack.h"
#include "cds/llist.h"
#include <stdlib.h>


struct cds_stack
{
  cds_llist *data;
};

cds_stack *cds_stack_create(size_t value_size)
{
  if (value_size == 0) return NULL;
  cds_stack *s = malloc(sizeof(*s));
  if (!s) return NULL;
  s->data = cds_llist_create(value_size);
  if (!s->data)
  {
    free(s);
    return NULL;
  }
  return s;
}

int cds_stack_clear(cds_stack *s)
{
  if (!s) return CDS_ERR_NULL;
  return cds_llist_clear(s->data);
}

int cds_stack_destroy(cds_stack *s)
{
  if (!s) return CDS_OK;
  cds_llist *tmp = s->data;
  free(s);
  return cds_llist_destroy(tmp);
}

bool cds_stack_empty(const cds_stack *s)
{
  if (!s) return true;
  return cds_llist_count(s->data) == 0;
}

size_t cds_stack_count(const cds_stack *s)
{
  if (!s) return 0;
  return cds_llist_count(s->data);
}

int cds_stack_peek(const cds_stack *s, void *out)
{
  if (!s) return CDS_ERR_NULL;
  return cds_llist_peek(s->data, out);
}

int cds_stack_pop(cds_stack *s, void *out)
{
  if (!s) return CDS_ERR_NULL;
  return cds_llist_pop(s->data, out);
}

int cds_stack_push(cds_stack *s, const void *value)
{
  if (!s) return CDS_ERR_NULL;
  return cds_llist_prepend(s->data, value);
}

