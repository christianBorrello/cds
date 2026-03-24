#include "cds/vector.h"
#include "cds/common.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cds/algo.h"

#define INITIAL_CAPACITY 8

struct cds_vector {
  void  *data;          /* raw buffer */
  size_t size;          /* number of elements currently stored */
  size_t capacity;      /* number of elements that fit before realloc */
  size_t element_size;  /* size in bytes of a single element */
};

cds_vector *cds_vector_create(size_t size) {
  cds_vector *v = malloc(sizeof(struct cds_vector));
  if (!v) return NULL;
  if (is_sizet_overflow(size, INITIAL_CAPACITY)) {
    free(v);
    return NULL;
  }
  v->data = malloc(size * INITIAL_CAPACITY);
  if (!v->data) {
    free(v);
    return NULL;
  }
  v->capacity = INITIAL_CAPACITY;
  v->size = 0;
  v->element_size = size;
  return v;
}

cds_vector *cds_vector_from(const void *src, size_t n, size_t size) {
  if (!src || n == 0 || size == 0) return NULL;
  cds_vector *v = cds_vector_create(size);
  if (!v) return NULL;
  if (cds_vector_reserve(v, n) != CDS_OK) {
    cds_vector_destroy(v);
    return NULL;
  }
  memcpy(v->data, src, n * size);
  v->size = n;
  return v;
}

int cds_vector_reserve(cds_vector *v, size_t n) {
  if (!v || !v->data) return CDS_ERR_NULL;
  if (n <= v->capacity) return CDS_OK;
  if (is_sizet_overflow(n, v->element_size)) return CDS_ERR_ALLOC;
  void *new_data = realloc(v->data, n * v->element_size);
  if (!new_data) return CDS_ERR_ALLOC;
  v->data = new_data;
  v->capacity = n;
  return CDS_OK;
}

void cds_vector_destroy(cds_vector *v) {
  if (!v) return;
  free(v->data);
  free(v);
}

void *cds_vector_get(const cds_vector *v, size_t index) {
  if (!v || index >= v->size) return NULL;
  return (char *)v->data + (index * v->element_size);
}

int cds_vector_set(cds_vector *v, size_t index, const void *element) {
  if (!v || !element) return CDS_ERR_NULL;
  if (index >= v->size) return CDS_ERR_INDEX;
  memcpy((char *)v->data + (index * v->element_size), element, v->element_size);
  return CDS_OK;
}

int cds_vector_push(cds_vector *v, const void *element) {
  if (!v || !v->data || !element) return CDS_ERR_NULL;
  if (v->size == v->capacity) {
    if (is_sizet_overflow(2, v->capacity) ||
        is_sizet_overflow((v->capacity * 2), v->element_size))
      return CDS_ERR_ALLOC;
    void *new_data = realloc(v->data, v->capacity * 2 * v->element_size);
    if (!new_data) return CDS_ERR_ALLOC;
    v->data = new_data;
    v->capacity *= 2;
  }
  void *dest = (char *)v->data + (v->size * v->element_size);
  memcpy(dest, element, v->element_size);
  v->size++;
  return CDS_OK;
}

int cds_vector_pop(cds_vector *v, void *out) {
  if (!v) return CDS_ERR_NULL;
  if (v->size == 0) return CDS_ERR_INDEX;
  void *last = (char *)v->data + (v->size - 1) * v->element_size;
  if (out) memcpy(out, last, v->element_size);
  v->size--;
  if (v->size <= v->capacity / 4 && v->capacity / 2 >= INITIAL_CAPACITY) {
    void *new_data = realloc(v->data, (v->capacity / 2) * v->element_size);
    if (new_data) {
      v->data = new_data;
      v->capacity /= 2;
    }
  }
  return CDS_OK;
}

size_t cds_vector_size(const cds_vector *v) {
  if (!v) return 0;
  return v->size;
}

size_t cds_vector_capacity(const cds_vector *v) {
  if (!v) return 0;
  return v->capacity;
}

int cds_vector_sort(cds_vector *v, int (*cmp)(const void *, const void *)) {
  if (!v || !cmp) return CDS_ERR_NULL;
  return cds_sort(v->data, v->size, v->element_size, cmp);
}

void *cds_vector_bsearch(const cds_vector *v, const void *key,
                         int (*cmp)(const void *, const void *)) {
  if (!v || !key || !cmp) return NULL;
  return cds_bsearch(v->data, key, v->size, v->element_size, cmp);
}

bool cds_vector_is_sorted(const cds_vector *v,
                          int (*cmp)(const void *, const void *)) {
  if (!v || !cmp) return false;
  return cds_is_sorted(v->data, v->size, v->element_size, cmp);
}

void *cds_vector_lsearch(const cds_vector *v, const void *key,
                         int (*cmp)(const void *, const void *)) {
  if (!v || !key || !cmp) return NULL;
  return cds_lsearch(v->data, key, v->size, v->element_size, cmp);
}
