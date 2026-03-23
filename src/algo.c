#include "cds/algo.h"
#include "cds/common.h"
#include <string.h>
#include <stdlib.h>

static void *elem_at(void *base, size_t i, size_t size) {
  return (char *)base + (i * size);
}

static void swap(void *base, size_t i, size_t j, size_t size) {
  unsigned char stack_buf[128];
  unsigned char *tmp = (size <= sizeof(stack_buf)) ? stack_buf : malloc(size);
  void *a = elem_at(base, i, size);
  void *b = elem_at(base, j, size);
  memcpy(tmp, a, size);
  memcpy(a, b, size);
  memcpy(b, tmp, size);
  if (tmp != stack_buf) free(tmp);
}
static size_t lomuto_partition(void *base, size_t size, size_t low, size_t high,
                               int (*cmp)(const void *, const void *)) {
  size_t mid = (low + high) / 2;
  void *low_ptr = elem_at(base, low, size);
  void *mid_ptr = elem_at(base, mid, size);
  void *high_ptr = elem_at(base, high, size);

  /* median-of-three pivot selection */
  if (cmp(low_ptr, mid_ptr) > 0)
    swap(base, low, mid, size);
  if (cmp(low_ptr, high_ptr) > 0)
    swap(base, low, high, size);
  if (cmp(mid_ptr, high_ptr) > 0)
    swap(base, mid, high, size);

  /* move pivot to the end */
  swap(base, mid, high, size);

  size_t i = low;
  size_t j = low;
  void *pivot = elem_at(base, high, size);

  while (j < high) {
    if (cmp(pivot, elem_at(base, j, size)) > 0) {
      swap(base, i, j, size);
      i++;
    }
    j++;
  }

  /* place pivot in its final position */
  swap(base, i, high, size);
  return i;
}

static void insertion_sort(void *base, size_t size, size_t n,
                           int (*cmp)(const void *, const void *)) {
  for (size_t i = 1; i < n; i++) {
    size_t j = i;
    while (j > 0 && cmp(elem_at(base, j - 1, size), elem_at(base, j, size)) > 0) {
      swap(base, j, j - 1, size);
      j--;
    }
  }
}

static void quicksort(void *base, size_t size, size_t low, size_t high,
                      int (*cmp)(const void *, const void *)) {
  while (low < high) {
    /* fall back to insertion sort for small subarrays */
    if (high - low < 16) {
      insertion_sort(elem_at(base, low, size), size, high - low + 1, cmp);
      return;
    }

    size_t p = lomuto_partition(base, size, low, high, cmp);

    /* recurse on the smaller half, iterate on the larger */
    if (p - low < high - p) {
      if (p > 0) quicksort(base, size, low, p - 1, cmp);
      low = p + 1;
    } else {
      quicksort(base, size, p + 1, high, cmp);
      high = (p > 0) ? p - 1 : 0;
    }
  }
}

int cds_sort(void *base, size_t n, size_t size,
             int (*cmp)(const void *, const void *)) {
  if (!base || n == 0 || size == 0 || !cmp) return CDS_ERR_NULL;
  if (n <= 1) return CDS_OK;
  quicksort(base, size, 0, n - 1, cmp);
  return CDS_OK;
}

void *cds_bsearch(const void *base, const void *key, size_t n, size_t size,
                  int (*cmp)(const void *, const void *)) {
  if (!base || !key || n == 0 || size == 0 || !cmp) return NULL;

  size_t low = 0;
  size_t high = n - 1;

  while (low < high) {
    size_t mid = (low + high) / 2;
    void *val = elem_at((void *)base, mid, size);
    int r = cmp(val, key);
    if (r == 0) return val;
    if (r < 0) low = mid + 1;
    else high = mid;
  }

  void *val = elem_at((void *)base, low, size);
  if (cmp(val, key) == 0) return val;
  return NULL;
}

bool cds_is_sorted(const void *base, size_t n, size_t size,
                   int (*cmp)(const void *, const void *)) {
  if (!base || n == 0 || size == 0 || !cmp) return false;

  for (size_t i = 0; i < n - 1; i++) {
    if (cmp(elem_at((void *)base, i, size),
            elem_at((void *)base, i + 1, size)) > 0)
      return false;
  }
  return true;
}

void *cds_lsearch(const void *base, const void *key, size_t n, size_t size,
                  int (*cmp)(const void *, const void *)) {
  if (!base || !key || n == 0 || size == 0 || !cmp) return NULL;

  for (size_t i = 0; i < n; i++) {
    const void *elem = elem_at((void *)base, i, size);
    if (cmp(elem, key) == 0)
      return (void *)elem;
  }
  return NULL;
}
