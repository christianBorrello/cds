#include "cds/common.h"
#include "cds/stack.h"
#include "cds/linked_list.h"
#include <stdlib.h>


struct cds_stack
{
  cds_linked_list *data;
};

cds_stack *cds_stack_create(size_t value_size)
{
  if (value_size == 0) return NULL;
  cds_stack *s = malloc(sizeof(*s));
  if (!s) return NULL;
  s->data = cds_linked_list_create(value_size);
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
  return cds_linked_list_clear(s->data);
}

int cds_stack_destroy(cds_stack *s)
{
  if (!s) return CDS_OK;
  cds_linked_list *tmp = s->data;
  free(s);
  return cds_linked_list_destroy(tmp);
}

bool cds_stack_empty(const cds_stack *s)
{
  if (!s) return true;
  return cds_linked_list_size(s->data) == 0;
}

size_t cds_stack_size(const cds_stack *s)
{
  if (!s) return 0;
  return cds_linked_list_size(s->data);
}

int cds_stack_peek(const cds_stack *s, void *out)
{
  if (!s) return CDS_ERR_NULL;
  return cds_linked_list_peek_head(s->data, out);
}

int cds_stack_pop(cds_stack *s, void *out)
{
  if (!s) return CDS_ERR_NULL;
  return cds_linked_list_remove_head(s->data, out);
}

int cds_stack_push(cds_stack *s, const void *value)
{
  if (!s) return CDS_ERR_NULL;
  return cds_linked_list_prepend(s->data, value);
}

