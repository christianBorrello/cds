#include <stdio.h>
#include "cds/common.h"
#include "cds/vector.h"

int cmp_int(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

int main(void)
{
    printf("=== cds_vector_create ===\n");
    cds_vector *v = cds_vector_create(sizeof(int));
    if (!v) {
        printf("FAIL: cds_vector_create returned NULL\n");
        return 1;
    }
    printf("OK: vector created (size=%zu, capacity=%zu)\n",
           cds_vector_size(v), cds_vector_capacity(v));

    printf("\n=== cds_vector_push ===\n");
    int values[] = {42, 7, 23, 1, 99, 15, 8, 56, 3, 31};
    size_t n = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < n; i++) {
        int result = cds_vector_push(v, &values[i]);
        printf("push %d -> %s\n", values[i], result == CDS_OK ? "OK" : "FAIL");
    }
    printf("after pushes: size=%zu, capacity=%zu\n",
           cds_vector_size(v), cds_vector_capacity(v));

    printf("\n=== cds_vector_get ===\n");
    for (size_t i = 0; i < cds_vector_size(v); i++) {
        int *val = cds_vector_get(v, i);
        if (val) printf("  v[%zu] = %d\n", i, *val);
    }
    int *out_of_bounds = cds_vector_get(v, 999);
    printf("get out of bounds -> %s\n", out_of_bounds == NULL ? "OK (NULL)" : "FAIL");

    printf("\n=== cds_vector_set ===\n");
    int new_val = 100;
    int set_result = cds_vector_set(v, 0, &new_val);
    printf("set v[0] = 100 -> %s\n", set_result == CDS_OK ? "OK" : "FAIL");
    int *check = cds_vector_get(v, 0);
    printf("verify v[0] = %d (expected 100)\n", check ? *check : -1);
    set_result = cds_vector_set(v, 999, &new_val);
    printf("set out of bounds -> %s\n", set_result == CDS_ERR_INDEX ? "OK (ERR_INDEX)" : "FAIL");
    cds_vector_set(v, 0, &values[0]);

    printf("\n=== cds_vector_reserve ===\n");
    int res_result = cds_vector_reserve(v, 64);
    printf("reserve 64 -> %s (capacity=%zu)\n",
           res_result == CDS_OK ? "OK" : "FAIL", cds_vector_capacity(v));
    res_result = cds_vector_reserve(v, 8);
    printf("reserve 8 (smaller, no-op) -> %s (capacity=%zu)\n",
           res_result == CDS_OK ? "OK" : "FAIL", cds_vector_capacity(v));

    printf("\n=== cds_vector_is_sorted (before sort) ===\n");
    printf("is_sorted -> %s\n", cds_vector_is_sorted(v, cmp_int) ? "true" : "false");

    printf("\n=== cds_vector_lsearch (unsorted) ===\n");
    int key = 56;
    int *found = cds_vector_lsearch(v, &key, cmp_int);
    printf("lsearch for %d -> %s\n", key, found ? "FOUND" : "NOT FOUND");
    key = 999;
    found = cds_vector_lsearch(v, &key, cmp_int);
    printf("lsearch for %d -> %s\n", key, found ? "FOUND" : "NOT FOUND");

    printf("\n=== cds_vector_sort ===\n");
    printf("before sort: ");
    for (size_t i = 0; i < cds_vector_size(v); i++) {
        int *val = cds_vector_get(v, i);
        if (val) printf("%d ", *val);
    }
    printf("\n");
    int sort_result = cds_vector_sort(v, cmp_int);
    printf("sort -> %s\n", sort_result == CDS_OK ? "OK" : "FAIL");
    printf("after sort:  ");
    for (size_t i = 0; i < cds_vector_size(v); i++) {
        int *val = cds_vector_get(v, i);
        if (val) printf("%d ", *val);
    }
    printf("\n");

    printf("\n=== cds_vector_is_sorted (after sort) ===\n");
    printf("is_sorted -> %s\n", cds_vector_is_sorted(v, cmp_int) ? "true" : "false");

    printf("\n=== cds_vector_bsearch ===\n");
    key = 23;
    found = cds_vector_bsearch(v, &key, cmp_int);
    printf("bsearch for %d -> %s\n", key, found ? "FOUND" : "NOT FOUND");
    key = 50;
    found = cds_vector_bsearch(v, &key, cmp_int);
    printf("bsearch for %d -> %s\n", key, found ? "FOUND" : "NOT FOUND");

    printf("\n=== cds_vector_pop ===\n");
    int popped;
    if (cds_vector_pop(v, &popped) == CDS_OK)
        printf("popped: %d\n", popped);
    printf("after pop: size=%zu\n", cds_vector_size(v));
    if (cds_vector_pop(v, NULL) == CDS_OK)
        printf("pop with NULL out: OK (size=%zu)\n", cds_vector_size(v));

    printf("\n=== cds_vector_destroy ===\n");
    cds_vector_destroy(v);
    printf("OK: vector destroyed\n");

    printf("\n=== cds_vector_from ===\n");
    int src[] = {10, 20, 30, 40, 50};
    cds_vector *v2 = cds_vector_from(src, 5, sizeof(int));
    if (!v2) {
        printf("FAIL: cds_vector_from returned NULL\n");
        return 1;
    }
    printf("OK: vector from array (size=%zu, capacity=%zu)\n",
           cds_vector_size(v2), cds_vector_capacity(v2));
    printf("contents: ");
    for (size_t i = 0; i < cds_vector_size(v2); i++) {
        int *val = cds_vector_get(v2, i);
        if (val) printf("%d ", *val);
    }
    printf("\n");
    printf("is_sorted -> %s\n", cds_vector_is_sorted(v2, cmp_int) ? "true" : "false");
    cds_vector_destroy(v2);
    printf("OK: vector destroyed\n");

    return 0;
}
