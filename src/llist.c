#include "cds/llist.h"
#include "cds/common.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct cds_node cds_node;

struct cds_node
{
  void *value;
  cds_node *next;
};

struct cds_llist
{
  cds_node *head;
  size_t value_size;
  size_t count;
  cds_node *tail;
};


static cds_node *allocate_node(const void *value, size_t value_size)
{
  if (!value || value_size == 0) return NULL;
  cds_node *n = malloc(sizeof(cds_node));
  if (!n) return NULL;
  n->value = malloc(value_size);
  if (!n->value) 
  {
    free(n);
    return NULL;
  }
  memcpy(n->value, value, value_size);
  n->next = NULL;
  return n;
}

static void deallocate_node(cds_node *node)
{
  if (!node) return;
  free(node->value);
  free(node);
}

cds_llist *cds_llist_create(size_t value_size)
{
  if (value_size == 0) return NULL;
  cds_llist *ll = malloc(sizeof(cds_llist));
  if (!ll) return NULL;
  ll->head = NULL;
  ll->value_size = value_size;
  ll->count = 0;
  ll->tail = NULL;
  return ll;
}

int cds_llist_clear(cds_llist *ll)
{
  if (!ll) return CDS_ERR_NULL;
  cds_node *curr_node = ll->head;
  while (curr_node)
  {
    cds_node *tmp = curr_node;
    curr_node = curr_node->next;
    deallocate_node(tmp);
  }
  ll->head = NULL;
  ll->count = 0;
  ll->tail = NULL;
  return CDS_OK;
}

int cds_llist_destroy(cds_llist *ll)
{
  if (!ll) return CDS_OK;
  cds_llist_clear(ll);
  free(ll);
  return CDS_OK;
}

int cds_llist_prepend(cds_llist *ll, const void *value)
{
  if (!ll || !value) return CDS_ERR_NULL;
  cds_node *n = allocate_node(value, ll->value_size);
  if (!n) return CDS_ERR_ALLOC;
  cds_node *old_head = ll->head;
  ll->head = n;
  ll->head->next = old_head;
  if (!old_head) ll->tail = n;
  ll->count += 1;
  return CDS_OK;
}

int cds_llist_append(cds_llist *ll, const void *value)
{
  if (!ll || !value) return CDS_ERR_NULL;
  cds_node *n = allocate_node(value, ll->value_size);
  if (!n) return CDS_ERR_ALLOC;
  if (ll->tail) ll->tail->next = n;
  else ll->head = n;
  ll->tail = n;
  ll->count += 1;
  return CDS_OK;
}

bool cds_llist_empty(const cds_llist *ll)
{
  if (!ll) return true;
  return ll->count == 0;
}

size_t cds_llist_count(const cds_llist *ll)
{
  if (!ll) return 0;
  return ll->count;
}

int cds_llist_get(const cds_llist *ll, size_t index, void *out)
{
  if (!ll || !out) return CDS_ERR_NULL;
  if (index >= ll->count) return CDS_ERR_INDEX;
  cds_node *curr_node = ll->head;
  for (size_t j = 0; j < index; j++) 
  { 
    curr_node = curr_node->next;
  }
  memcpy(out, curr_node->value, ll->value_size);
  return CDS_OK;
}

int cds_llist_pop(cds_llist *ll, void *out)
{
  if (!ll) return CDS_ERR_NULL;
  if (ll->count == 0) return CDS_ERR_EMPTY;
  cds_node *old_head = ll->head;
  ll->head = ll->head->next;
  if (ll->count == 1) ll->tail = NULL;
  if (out) memcpy(out, old_head->value, ll->value_size);
  deallocate_node(old_head);
  ll->count -= 1;
  return CDS_OK;
}

int cds_llist_pop_back(cds_llist *ll, void *out)
{
  if (!ll) return CDS_ERR_NULL;
  if (ll->count == 0) return CDS_ERR_EMPTY;
  if (ll->count == 1)
  {
    if (out) memcpy(out, ll->tail->value, ll->value_size);
    deallocate_node(ll->tail);
    ll->count = 0;
    ll->head = NULL;
    ll->tail = NULL;
    return CDS_OK;
  }
  cds_node *new_tail = ll->head;
  for (size_t j = 0; j < ll->count - 2; j++)
  {
    new_tail = new_tail->next;
  }
  cds_node *old_tail = new_tail->next;
  ll->tail = new_tail;
  ll->tail->next = NULL;
  ll->count -= 1;
  if (out) memcpy(out, old_tail->value, ll->value_size);
  deallocate_node(old_tail);
  return CDS_OK;
}

int cds_llist_insert(cds_llist *ll, const void *value, size_t index)
{
  if (!ll || !value) return CDS_ERR_NULL;
  if (index > ll->count) return CDS_ERR_INDEX;
  cds_node **link = &ll->head;
  size_t i = 0;
  while (*link && i < index) 
  {
    link = &((*link)->next);
    i++;
  }
  cds_node *new_node = allocate_node(value, ll->value_size);
  if (!new_node) return CDS_ERR_ALLOC;
  new_node->next = *link;
  *link = new_node;
  if (index == ll->count) ll->tail = new_node;
  ll->count++;
  return CDS_OK;
}

int cds_llist_remove(cds_llist *ll, const void *value, int(*cmp)(const void *, const void *)) 
{
  if (!ll || !cmp) return CDS_ERR_NULL;
  if (ll->count == 0) return CDS_ERR_EMPTY;
  cds_node **link = &ll->head;
  cds_node *last_node = NULL;
  while (*link)
  {
    cds_node *curr_node = *link;
    if (cmp(curr_node->value, value) == 0) 
    {
      *link = curr_node->next;
      if (curr_node == ll->tail) ll->tail = last_node;
      deallocate_node(curr_node);
      ll->count--;
      return CDS_OK;
    }
    else 
    {
      last_node = curr_node;
      link = &curr_node->next;
    }
  }
  return CDS_ERR_NOT_FOUND;
}

int cds_llist_purge(cds_llist *ll, const void *value, int(*cmp)(const void *, const void *)) 
{
  if (!ll || !cmp) return CDS_ERR_NULL;
  if (ll->count == 0) return CDS_ERR_EMPTY;
  bool removed_nodes = false;
  cds_node **link = &ll->head;
  cds_node *last_node = NULL;
  while (*link)
  {
    cds_node *curr_node = *link;
    if (cmp(curr_node->value, value) == 0)
    {
      *link = curr_node->next;
      deallocate_node(curr_node);
      ll->count--;
      removed_nodes = true;
    }
    else  
    {
      last_node = curr_node;
      link = &curr_node->next;
    }
  }
  ll->tail = last_node;
  return removed_nodes ? CDS_OK : CDS_ERR_NOT_FOUND;
}

int cds_llist_contains(const cds_llist *ll, const void *value, int(*cmp)(const void *, const void *))
{
  if (!ll || !value || !cmp) return CDS_ERR_NULL;
  if (ll->count == 0) return CDS_ERR_EMPTY;
  cds_node *curr_node = ll->head;
  while (curr_node)
  {
    if (cmp(curr_node->value, value) == 0) return CDS_OK;
    curr_node = curr_node->next;
  }
  return CDS_ERR_NOT_FOUND;
}

int cds_llist_peek(const cds_llist *ll, void *out)
{
  if (!ll || !out) return CDS_ERR_NULL;
  if (ll->count == 0) return CDS_ERR_EMPTY;
  memcpy(out, ll->head->value, ll->value_size);
  return CDS_OK;
}

int cds_llist_back(const cds_llist *ll, void *out)
{
  if (!ll || !out) return CDS_ERR_NULL;
  if (ll->count == 0) return CDS_ERR_EMPTY;
  memcpy(out, ll->tail->value, ll->value_size);
  return CDS_OK;
}

int cds_llist_find(const cds_llist *ll, const void *value, 
    int(*cmp)(const void *, const void *), void *out)
{
  if (!ll || !value || !out) return CDS_ERR_NULL;
  cds_node *curr_node = ll->head;
  while (curr_node)
  {
    if (cmp(curr_node->value, value) == 0)
    {
      memcpy(out, curr_node->value, ll->value_size);
      return CDS_OK;
    }
    curr_node = curr_node->next;
  }
  return CDS_ERR_NOT_FOUND;
}
