#include "cds/vector.h"
#include "cds/common.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cds/algo.h"

#define INITIAL_CAPACITY 8

struct cds_vector
{
  void  *data;          /* raw buffer */
  size_t count;         /* number of values currently stored */
  size_t capacity;      /* number of values that fit before realloc */
  size_t value_size;    /* size in bytes of a single value */
};

cds_vector *cds_vector_create(size_t value_size)
{
  if (value_size == 0) return NULL;
  cds_vector *v = malloc(sizeof(struct cds_vector));
  if (!v) return NULL;
  if (is_sizet_overflow(value_size, INITIAL_CAPACITY)) {
    free(v);
    return NULL;
  }
  v->data = malloc(value_size * INITIAL_CAPACITY);
  if (!v->data) {
    free(v);
    return NULL;
  }
  v->capacity = INITIAL_CAPACITY;
  v->count = 0;
  v->value_size = value_size;
  return v;
}

cds_vector *cds_vector_from(const void *src, size_t n, size_t value_size)
{
  if (!src || n == 0 || value_size == 0) return NULL;
  cds_vector *v = cds_vector_create(value_size);
  if (!v) return NULL;
  if (cds_vector_reserve(v, n) != CDS_OK) {
    cds_vector_destroy(v);
    return NULL;
  }
  memcpy(v->data, src, n * value_size);
  v->count = n;
  return v;
}

int cds_vector_reserve(cds_vector *v, size_t n)
{
  if (!v || !v->data) return CDS_ERR_NULL;
  if (n <= v->capacity) return CDS_OK;
  if (is_sizet_overflow(n, v->value_size)) return CDS_ERR_ALLOC;
  void *new_data = realloc(v->data, n * v->value_size);
  if (!new_data) return CDS_ERR_ALLOC;
  v->data = new_data;
  v->capacity = n;
  return CDS_OK;
}

int cds_vector_destroy(cds_vector *v)
{
  if (!v) return CDS_OK;
  free(v->data);
  free(v);
  return CDS_OK;
}

int cds_vector_get(const cds_vector *v, size_t index, void *out)
{
  if (!v || !out) return CDS_ERR_NULL;
  if (index >= v->count) return CDS_ERR_INDEX;
  void *p_val = (char *)v->data + (index * v->value_size);
  memcpy(out, p_val, v->value_size);
  return CDS_OK;
}

int cds_vector_set(cds_vector *v, size_t index, const void *value)
{
  if (!v || !value) return CDS_ERR_NULL;
  if (index >= v->count) return CDS_ERR_INDEX;
  memcpy((char *)v->data + (index * v->value_size), value, v->value_size);
  return CDS_OK;
}

int cds_vector_push(cds_vector *v, const void *value)
{
  if (!v || !v->data || !value) return CDS_ERR_NULL;
  if (v->count == v->capacity) {
    if (is_sizet_overflow(2, v->capacity) ||
        is_sizet_overflow((v->capacity * 2), v->value_size))
      return CDS_ERR_ALLOC;
    void *new_data = realloc(v->data, v->capacity * 2 * v->value_size);
    if (!new_data) return CDS_ERR_ALLOC;
    v->data = new_data;
    v->capacity *= 2;
  }
  void *dest = (char *)v->data + (v->count * v->value_size);
  memcpy(dest, value, v->value_size);
  v->count++;
  return CDS_OK;
}

int cds_vector_pop(cds_vector *v, void *out)
{
  if (!v) return CDS_ERR_NULL;
  if (v->count == 0) return CDS_ERR_EMPTY;
  void *last = (char *)v->data + (v->count - 1) * v->value_size;
  if (out) memcpy(out, last, v->value_size);
  v->count--;
  if (v->count <= v->capacity / 4 && v->capacity / 2 >= INITIAL_CAPACITY) {
    void *new_data = realloc(v->data, (v->capacity / 2) * v->value_size);
    if (new_data) {
      v->data = new_data;
      v->capacity /= 2;
    }
  }
  return CDS_OK;
}

bool cds_vector_empty(const cds_vector *v)
{
  if (!v) return true;
  return v->count == 0;
}

size_t cds_vector_count(const cds_vector *v)
{
  if (!v) return 0;
  return v->count;
}

int cds_vector_clear(cds_vector *v)
{
  if (!v) return CDS_ERR_NULL;
  v->count = 0;
  if (v->capacity > INITIAL_CAPACITY)
  {
    void *new_data = realloc(v->data, INITIAL_CAPACITY * v->value_size);
    if (new_data)
    {
      v->data = new_data;
      v->capacity = INITIAL_CAPACITY;
    }
  }
  return CDS_OK;
}

size_t cds_vector_capacity(const cds_vector *v)
{
  if (!v) return 0;
  return v->capacity;
}

int cds_vector_sort(cds_vector *v, 
    int (*cmp)(const void *, const void *))
{
  if (!v || !cmp) return CDS_ERR_NULL;
  return cds_sort(v->data, v->count, v->value_size, cmp);
}

int cds_vector_bsearch(const cds_vector *v, const void *key,
    int (*cmp)(const void *, const void *), void *out)
{
  if (!v || !key || !cmp) return CDS_ERR_NULL;
  return cds_bsearch(v->data, key, v->count, v->value_size, cmp, out);

}

bool cds_vector_is_sorted(const cds_vector *v,
    int (*cmp)(const void *, const void *))
{
  if (!v || !cmp) return false;
  return cds_is_sorted(v->data, v->count, v->value_size, cmp);
}

int cds_vector_lsearch(const cds_vector *v, const void *key,
    int (*cmp)(const void *, const void *), void *out)
{
  if (!v || !key || !cmp) return CDS_ERR_NULL;
  return cds_lsearch(v->data, key, v->count, v->value_size, cmp, out);
}
