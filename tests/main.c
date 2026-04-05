#include <stdint.h>
#include <stdio.h>
#include "cds/common.h"
#include "cds/vector.h"
#include "cds/linked_list.h"
#include "cds/stack.h"
#include "cds/queue.h"
#include "test.h"

static int cmp_int(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

static void test_vector(void)
{
    printf("\n========== VECTOR ==========\n");

    printf("\n--- create ---\n");
    cds_vector *invalid_v = cds_vector_create(SIZE_MAX);
    ASSERT_NULL(invalid_v);
    cds_vector *v = cds_vector_create(sizeof(int));
    ASSERT_NOT_NULL(v);
    ASSERT_EQ(cds_vector_size(v), 0);
    ASSERT_EQ(cds_vector_capacity(v), 8);

    printf("\n--- push ---\n");
    int values[] = {42, 7, 23, 1, 99, 15, 8, 56, 3, 31};
    size_t n = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(cds_vector_push(v, &values[i]), CDS_OK);
    }
    ASSERT_EQ(cds_vector_size(v), 10);
    ASSERT_EQ(cds_vector_capacity(v), 16);

    printf("\n--- get ---\n");
    for (size_t i = 0; i < cds_vector_size(v); i++) {
        int *val = cds_vector_get(v, i);
        ASSERT_NOT_NULL(val);
        ASSERT_EQ(*val, values[i]);
    }
    ASSERT_NULL(cds_vector_get(v, 999));

    printf("\n--- set ---\n");
    int new_val = 100;
    ASSERT_EQ(cds_vector_set(v, 0, &new_val), CDS_OK);
    int *check = cds_vector_get(v, 0);
    ASSERT_EQ(*check, 100);
    ASSERT_EQ(cds_vector_set(v, 999, &new_val), CDS_ERR_INDEX);
    cds_vector_set(v, 0, &values[0]);

    printf("\n--- reserve ---\n");
    ASSERT_TRUE(cds_vector_reserve(v, SIZE_MAX) != CDS_OK);
    ASSERT_EQ(cds_vector_reserve(v, 64), CDS_OK);
    ASSERT_EQ(cds_vector_capacity(v), 64);
    ASSERT_EQ(cds_vector_reserve(v, 8), CDS_OK);
    ASSERT_EQ(cds_vector_capacity(v), 64);

    printf("\n--- sort ---\n");
    ASSERT_TRUE(!cds_vector_is_sorted(v, cmp_int));
    ASSERT_EQ(cds_vector_sort(v, cmp_int), CDS_OK);
    ASSERT_TRUE(cds_vector_is_sorted(v, cmp_int));

    printf("\n--- search ---\n");
    int key = 56;
    ASSERT_NOT_NULL(cds_vector_lsearch(v, &key, cmp_int));
    key = 999;
    ASSERT_NULL(cds_vector_lsearch(v, &key, cmp_int));
    key = 23;
    ASSERT_NOT_NULL(cds_vector_bsearch(v, &key, cmp_int));
    key = 50;
    ASSERT_NULL(cds_vector_bsearch(v, &key, cmp_int));

    printf("\n--- pop ---\n");
    int popped;
    ASSERT_EQ(cds_vector_pop(v, &popped), CDS_OK);
    ASSERT_EQ(popped, 99);
    ASSERT_EQ(cds_vector_size(v), 9);
    ASSERT_EQ(cds_vector_pop(v, NULL), CDS_OK);
    ASSERT_EQ(cds_vector_size(v), 8);

    printf("\n--- destroy ---\n");
    cds_vector_destroy(v);

    printf("\n--- from ---\n");
    int src[] = {10, 20, 30, 40, 50};
    cds_vector *v2 = cds_vector_from(src, 5, sizeof(int));
    ASSERT_NOT_NULL(v2);
    ASSERT_EQ(cds_vector_size(v2), 5);
    ASSERT_TRUE(cds_vector_is_sorted(v2, cmp_int));
    for (size_t i = 0; i < 5; i++) {
        int *val = cds_vector_get(v2, i);
        ASSERT_EQ(*val, src[i]);
    }
    cds_vector_destroy(v2);
}

static void test_linked_list(void)
{
    printf("\n========== LINKED LIST ==========\n");

    printf("\n--- create ---\n");
    ASSERT_NULL(cds_linked_list_create(0));
    cds_linked_list *ll = cds_linked_list_create(sizeof(int));
    ASSERT_NOT_NULL(ll);
    ASSERT_EQ(cds_linked_list_size(ll), 0);

    printf("\n--- prepend / append ---\n");
    int ll_vals[] = {10, 20, 30, 40, 50};
    cds_linked_list_append(ll, &ll_vals[2]);   /* [30] */
    cds_linked_list_prepend(ll, &ll_vals[1]);  /* [20, 30] */
    cds_linked_list_prepend(ll, &ll_vals[0]);  /* [10, 20, 30] */
    cds_linked_list_append(ll, &ll_vals[3]);   /* [10, 20, 30, 40] */
    cds_linked_list_append(ll, &ll_vals[4]);   /* [10, 20, 30, 40, 50] */
    ASSERT_EQ(cds_linked_list_size(ll), 5);

    printf("\n--- get ---\n");
    int ll_out;
    for (size_t i = 0; i < cds_linked_list_size(ll); i++) {
        cds_linked_list_get(ll, i, &ll_out);
        ASSERT_EQ(ll_out, ll_vals[i]);
    }
    ASSERT_EQ(cds_linked_list_get(ll, 999, &ll_out), CDS_ERR_INDEX);
    ASSERT_EQ(cds_linked_list_get(ll, 0, NULL), CDS_ERR_NULL);

    printf("\n--- remove_head ---\n");
    cds_linked_list_remove_head(ll, &ll_out);  /* [20, 30, 40, 50] */
    ASSERT_EQ(ll_out, 10);
    ASSERT_EQ(cds_linked_list_size(ll), 4);
    cds_linked_list_get(ll, 0, &ll_out);
    ASSERT_EQ(ll_out, 20);

    printf("\n--- remove_tail ---\n");
    cds_linked_list_remove_tail(ll, &ll_out);  /* [20, 30, 40] */
    ASSERT_EQ(ll_out, 50);
    ASSERT_EQ(cds_linked_list_size(ll), 3);

    printf("\n--- remove_head/tail with NULL out ---\n");
    int tmp = 10;
    cds_linked_list_prepend(ll, &tmp);  /* [10, 20, 30, 40] */
    ASSERT_EQ(cds_linked_list_remove_head(ll, NULL), CDS_OK);
    ASSERT_EQ(cds_linked_list_size(ll), 3);

    printf("\n--- remove_head/tail on empty ---\n");
    cds_linked_list *empty_ll = cds_linked_list_create(sizeof(int));
    ASSERT_EQ(cds_linked_list_remove_head(empty_ll, NULL), CDS_ERR_EMPTY);
    ASSERT_EQ(cds_linked_list_remove_tail(empty_ll, NULL), CDS_ERR_EMPTY);
    cds_linked_list_destroy(empty_ll);

    printf("\n--- remove_head/tail single element ---\n");
    cds_linked_list *single_ll = cds_linked_list_create(sizeof(int));
    int single_val = 99;
    cds_linked_list_append(single_ll, &single_val);
    cds_linked_list_remove_head(single_ll, &ll_out);
    ASSERT_EQ(ll_out, 99);
    ASSERT_EQ(cds_linked_list_size(single_ll), 0);
    cds_linked_list_append(single_ll, &single_val);
    cds_linked_list_remove_tail(single_ll, &ll_out);
    ASSERT_EQ(ll_out, 99);
    ASSERT_EQ(cds_linked_list_size(single_ll), 0);
    cds_linked_list_destroy(single_ll);

    printf("\n--- insert_at ---\n");
    /* list is [20, 30, 40] */
    int insert_val = 25;
    cds_linked_list_insert_at(ll, &insert_val, 1);  /* [20, 25, 30, 40] */
    cds_linked_list_get(ll, 1, &ll_out);
    ASSERT_EQ(ll_out, 25);
    ASSERT_EQ(cds_linked_list_size(ll), 4);
    ASSERT_EQ(cds_linked_list_insert_at(ll, &insert_val, 99), CDS_ERR_INDEX);

    printf("\n--- contains ---\n");
    int search_key = 30;
    ASSERT_EQ(cds_linked_list_contains(ll, &search_key, cmp_int), CDS_OK);
    search_key = 999;
    ASSERT_EQ(cds_linked_list_contains(ll, &search_key, cmp_int), CDS_ERR_NOT_FOUND);

    printf("\n--- remove (by value) ---\n");
    /* list is [20, 25, 30, 40] */
    int remove_val = 25;
    ASSERT_EQ(cds_linked_list_remove(ll, &remove_val, cmp_int), CDS_OK);
    ASSERT_EQ(cds_linked_list_size(ll), 3);
    ASSERT_EQ(cds_linked_list_contains(ll, &remove_val, cmp_int), CDS_ERR_NOT_FOUND);
    remove_val = 999;
    ASSERT_EQ(cds_linked_list_remove(ll, &remove_val, cmp_int), CDS_ERR_NOT_FOUND);

    /* remove head by value */
    remove_val = 20;
    cds_linked_list_remove(ll, &remove_val, cmp_int);  /* [30, 40] */
    cds_linked_list_get(ll, 0, &ll_out);
    ASSERT_EQ(ll_out, 30);

    /* remove tail by value */
    remove_val = 40;
    cds_linked_list_remove(ll, &remove_val, cmp_int);  /* [30] */
    ASSERT_EQ(cds_linked_list_size(ll), 1);

    printf("\n--- remove_all ---\n");
    cds_linked_list *dup_ll = cds_linked_list_create(sizeof(int));
    int dup_vals[] = {5, 10, 5, 20, 5};
    for (size_t i = 0; i < 5; i++) {
        cds_linked_list_append(dup_ll, &dup_vals[i]);
    }
    int dup_target = 5;
    ASSERT_EQ(cds_linked_list_remove_all(dup_ll, &dup_target, cmp_int), CDS_OK);
    ASSERT_EQ(cds_linked_list_size(dup_ll), 2);
    cds_linked_list_get(dup_ll, 0, &ll_out);
    ASSERT_EQ(ll_out, 10);
    cds_linked_list_get(dup_ll, 1, &ll_out);
    ASSERT_EQ(ll_out, 20);
    dup_target = 999;
    ASSERT_EQ(cds_linked_list_remove_all(dup_ll, &dup_target, cmp_int), CDS_ERR_NOT_FOUND);

    /* remove_all everything */
    dup_target = 10;
    cds_linked_list_remove_all(dup_ll, &dup_target, cmp_int);
    dup_target = 20;
    cds_linked_list_remove_all(dup_ll, &dup_target, cmp_int);
    ASSERT_EQ(cds_linked_list_size(dup_ll), 0);
    cds_linked_list_destroy(dup_ll);

    printf("\n--- destroy ---\n");
    cds_linked_list_destroy(ll);
    ASSERT_EQ(cds_linked_list_destroy(NULL), CDS_OK);

    printf("\n--- NULL safety ---\n");
    ASSERT_EQ(cds_linked_list_size(NULL), 0);
}

static void test_stack(void)
{
    printf("\n========== STACK ==========\n");

    printf("\n--- create ---\n");
    ASSERT_NULL(cds_stack_create(0));
    cds_stack *s = cds_stack_create(sizeof(int));
    ASSERT_NOT_NULL(s);
    ASSERT_EQ(cds_stack_size(s), 0);
    ASSERT_TRUE(cds_stack_empty(s));

    printf("\n--- push ---\n");
    int vals[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(cds_stack_push(s, &vals[i]), CDS_OK);
    }
    ASSERT_EQ(cds_stack_size(s), 5);
    ASSERT_TRUE(!cds_stack_empty(s));

    printf("\n--- peek ---\n");
    int out;
    ASSERT_EQ(cds_stack_peek(s, &out), CDS_OK);
    ASSERT_EQ(out, 50);
    ASSERT_EQ(cds_stack_size(s), 5);

    printf("\n--- pop (LIFO order) ---\n");
    int expected[] = {50, 40, 30, 20, 10};
    for (size_t i = 0; i < 5; i++) {
        cds_stack_pop(s, &out);
        ASSERT_EQ(out, expected[i]);
    }
    ASSERT_EQ(cds_stack_size(s), 0);
    ASSERT_TRUE(cds_stack_empty(s));

    printf("\n--- pop/peek on empty ---\n");
    ASSERT_EQ(cds_stack_pop(s, &out), CDS_ERR_EMPTY);
    ASSERT_EQ(cds_stack_peek(s, &out), CDS_ERR_EMPTY);

    printf("\n--- pop with NULL out ---\n");
    cds_stack_push(s, &vals[0]);
    cds_stack_push(s, &vals[1]);
    ASSERT_EQ(cds_stack_pop(s, NULL), CDS_OK);
    ASSERT_EQ(cds_stack_size(s), 1);

    printf("\n--- clear ---\n");
    cds_stack_push(s, &vals[2]);
    cds_stack_push(s, &vals[3]);
    ASSERT_EQ(cds_stack_clear(s), CDS_OK);
    ASSERT_EQ(cds_stack_size(s), 0);
    ASSERT_TRUE(cds_stack_empty(s));

    printf("\n--- push after clear ---\n");
    int reuse_val = 99;
    cds_stack_push(s, &reuse_val);
    cds_stack_peek(s, &out);
    ASSERT_EQ(out, 99);

    printf("\n--- destroy ---\n");
    cds_stack_destroy(s);

    printf("\n--- NULL safety ---\n");
    ASSERT_EQ(cds_stack_size(NULL), 0);
    ASSERT_TRUE(cds_stack_empty(NULL));
    ASSERT_EQ(cds_stack_push(NULL, &reuse_val), CDS_ERR_NULL);
    ASSERT_EQ(cds_stack_pop(NULL, &out), CDS_ERR_NULL);
    ASSERT_EQ(cds_stack_peek(NULL, &out), CDS_ERR_NULL);
}

static void test_queue(void)
{
    printf("\n========== QUEUE ==========\n");

    printf("\n--- create ---\n");
    ASSERT_NULL(cds_queue_create(0));
    cds_queue *q = cds_queue_create(sizeof(int));
    ASSERT_NOT_NULL(q);
    ASSERT_EQ(cds_queue_size(q), 0);
    ASSERT_TRUE(cds_queue_empty(q));

    printf("\n--- enq ---\n");
    int vals[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(cds_queue_enq(q, &vals[i]), CDS_OK);
    }
    ASSERT_EQ(cds_queue_size(q), 5);
    ASSERT_TRUE(!cds_queue_empty(q));

    printf("\n--- peek ---\n");
    int out;
    ASSERT_EQ(cds_queue_peek(q, &out), CDS_OK);
    ASSERT_EQ(out, 10);
    ASSERT_EQ(cds_queue_size(q), 5);

    printf("\n--- deq (FIFO order) ---\n");
    int expected[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        cds_queue_deq(q, &out);
        ASSERT_EQ(out, expected[i]);
    }
    ASSERT_EQ(cds_queue_size(q), 0);
    ASSERT_TRUE(cds_queue_empty(q));

    printf("\n--- deq/peek on empty ---\n");
    ASSERT_EQ(cds_queue_deq(q, &out), CDS_ERR_EMPTY);
    ASSERT_EQ(cds_queue_peek(q, &out), CDS_ERR_EMPTY);

    printf("\n--- deq with NULL out ---\n");
    cds_queue_enq(q, &vals[0]);
    cds_queue_enq(q, &vals[1]);
    ASSERT_EQ(cds_queue_deq(q, NULL), CDS_OK);
    ASSERT_EQ(cds_queue_size(q), 1);
    cds_queue_deq(q, &out);
    ASSERT_EQ(out, 20);

    printf("\n--- clear ---\n");
    cds_queue_enq(q, &vals[2]);
    cds_queue_enq(q, &vals[3]);
    ASSERT_EQ(cds_queue_clear(q), CDS_OK);
    ASSERT_EQ(cds_queue_size(q), 0);
    ASSERT_TRUE(cds_queue_empty(q));

    printf("\n--- enq after clear ---\n");
    int reuse_val = 99;
    cds_queue_enq(q, &reuse_val);
    cds_queue_peek(q, &out);
    ASSERT_EQ(out, 99);

    printf("\n--- destroy ---\n");
    cds_queue_destroy(q);
    ASSERT_EQ(cds_queue_destroy(NULL), CDS_OK);

    printf("\n--- NULL safety ---\n");
    ASSERT_EQ(cds_queue_size(NULL), 0);
    ASSERT_TRUE(cds_queue_empty(NULL));
    ASSERT_EQ(cds_queue_enq(NULL, &reuse_val), CDS_ERR_NULL);
    ASSERT_EQ(cds_queue_deq(NULL, &out), CDS_ERR_NULL);
    ASSERT_EQ(cds_queue_peek(NULL, &out), CDS_ERR_NULL);
}

int main(void)
{
    test_vector();
    test_linked_list();
    test_stack();
    test_queue();
    printf("\n");
    return TEST_RESULTS();
}
