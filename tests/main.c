#include <stdint.h>
#include <stdio.h>
#include "cds/common.h"
#include "cds/vector.h"
#include "cds/llist.h"
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
    ASSERT_EQ(cds_vector_count(v), 0);
    ASSERT_EQ(cds_vector_capacity(v), 8);

    printf("\n--- push ---\n");
    int values[] = {42, 7, 23, 1, 99, 15, 8, 56, 3, 31};
    size_t n = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(cds_vector_push(v, &values[i]), CDS_OK);
    }
    ASSERT_EQ(cds_vector_count(v), 10);
    ASSERT_EQ(cds_vector_capacity(v), 16);

    printf("\n--- get ---\n");
    int got;
    for (size_t i = 0; i < cds_vector_count(v); i++) {
        ASSERT_EQ(cds_vector_get(v, i, &got), CDS_OK);
        ASSERT_EQ(got, values[i]);
    }
    ASSERT_EQ(cds_vector_get(v, 999, &got), CDS_ERR_INDEX);
    ASSERT_EQ(cds_vector_get(v, 0, NULL), CDS_ERR_NULL);

    printf("\n--- set ---\n");
    int new_val = 100;
    ASSERT_EQ(cds_vector_set(v, 0, &new_val), CDS_OK);
    int check;
    cds_vector_get(v, 0, &check);
    ASSERT_EQ(check, 100);
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
    int found;
    int key = 56;
    ASSERT_EQ(cds_vector_lsearch(v, &key, cmp_int, &found), CDS_OK);
    ASSERT_EQ(found, 56);
    key = 999;
    ASSERT_EQ(cds_vector_lsearch(v, &key, cmp_int, &found), CDS_ERR_NOT_FOUND);
    key = 23;
    ASSERT_EQ(cds_vector_bsearch(v, &key, cmp_int, &found), CDS_OK);
    ASSERT_EQ(found, 23);
    key = 50;
    ASSERT_EQ(cds_vector_bsearch(v, &key, cmp_int, &found), CDS_ERR_NOT_FOUND);

    printf("\n--- pop ---\n");
    int popped;
    ASSERT_EQ(cds_vector_pop(v, &popped), CDS_OK);
    ASSERT_EQ(popped, 99);
    ASSERT_EQ(cds_vector_count(v), 9);
    ASSERT_EQ(cds_vector_pop(v, NULL), CDS_OK);
    ASSERT_EQ(cds_vector_count(v), 8);

    printf("\n--- pop on empty ---\n");
    cds_vector *empty_v = cds_vector_create(sizeof(int));
    ASSERT_EQ(cds_vector_pop(empty_v, NULL), CDS_ERR_EMPTY);
    cds_vector_destroy(empty_v);

    printf("\n--- empty ---\n");
    cds_vector *ev = cds_vector_create(sizeof(int));
    ASSERT_TRUE(cds_vector_empty(ev));
    int ev_val = 1;
    cds_vector_push(ev, &ev_val);
    ASSERT_TRUE(!cds_vector_empty(ev));
    cds_vector_destroy(ev);
    ASSERT_TRUE(cds_vector_empty(NULL));

    printf("\n--- clear ---\n");
    cds_vector *cv = cds_vector_create(sizeof(int));
    for (int i = 0; i < 20; i++) cds_vector_push(cv, &i);
    ASSERT_EQ(cds_vector_count(cv), 20);
    ASSERT_EQ(cds_vector_clear(cv), CDS_OK);
    ASSERT_EQ(cds_vector_count(cv), 0);
    ASSERT_TRUE(cds_vector_empty(cv));
    ASSERT_EQ(cds_vector_capacity(cv), 8);
    cds_vector_push(cv, &ev_val);
    ASSERT_EQ(cds_vector_count(cv), 1);
    cds_vector_destroy(cv);

    printf("\n--- destroy ---\n");
    cds_vector_destroy(v);

    printf("\n--- from ---\n");
    int src[] = {10, 20, 30, 40, 50};
    cds_vector *v2 = cds_vector_from(src, 5, sizeof(int));
    ASSERT_NOT_NULL(v2);
    ASSERT_EQ(cds_vector_count(v2), 5);
    ASSERT_TRUE(cds_vector_is_sorted(v2, cmp_int));
    for (size_t i = 0; i < 5; i++) {
        int val;
        cds_vector_get(v2, i, &val);
        ASSERT_EQ(val, src[i]);
    }
    cds_vector_destroy(v2);
}

static void test_llist(void)
{
    printf("\n========== LLIST ==========\n");

    printf("\n--- create ---\n");
    ASSERT_NULL(cds_llist_create(0));
    cds_llist *ll = cds_llist_create(sizeof(int));
    ASSERT_NOT_NULL(ll);
    ASSERT_EQ(cds_llist_count(ll), 0);

    printf("\n--- prepend / append ---\n");
    int ll_vals[] = {10, 20, 30, 40, 50};
    cds_llist_append(ll, &ll_vals[2]);   /* [30] */
    cds_llist_prepend(ll, &ll_vals[1]);  /* [20, 30] */
    cds_llist_prepend(ll, &ll_vals[0]);  /* [10, 20, 30] */
    cds_llist_append(ll, &ll_vals[3]);   /* [10, 20, 30, 40] */
    cds_llist_append(ll, &ll_vals[4]);   /* [10, 20, 30, 40, 50] */
    ASSERT_EQ(cds_llist_count(ll), 5);

    printf("\n--- get ---\n");
    int ll_out;
    for (size_t i = 0; i < cds_llist_count(ll); i++) {
        cds_llist_get(ll, i, &ll_out);
        ASSERT_EQ(ll_out, ll_vals[i]);
    }
    ASSERT_EQ(cds_llist_get(ll, 999, &ll_out), CDS_ERR_INDEX);
    ASSERT_EQ(cds_llist_get(ll, 0, NULL), CDS_ERR_NULL);

    printf("\n--- pop ---\n");
    cds_llist_pop(ll, &ll_out);  /* [20, 30, 40, 50] */
    ASSERT_EQ(ll_out, 10);
    ASSERT_EQ(cds_llist_count(ll), 4);
    cds_llist_get(ll, 0, &ll_out);
    ASSERT_EQ(ll_out, 20);

    printf("\n--- pop_back ---\n");
    cds_llist_pop_back(ll, &ll_out);  /* [20, 30, 40] */
    ASSERT_EQ(ll_out, 50);
    ASSERT_EQ(cds_llist_count(ll), 3);

    printf("\n--- pop/pop_back with NULL out ---\n");
    int tmp = 10;
    cds_llist_prepend(ll, &tmp);  /* [10, 20, 30, 40] */
    ASSERT_EQ(cds_llist_pop(ll, NULL), CDS_OK);
    ASSERT_EQ(cds_llist_count(ll), 3);

    printf("\n--- pop/pop_back on empty ---\n");
    cds_llist *empty_ll = cds_llist_create(sizeof(int));
    ASSERT_EQ(cds_llist_pop(empty_ll, NULL), CDS_ERR_EMPTY);
    ASSERT_EQ(cds_llist_pop_back(empty_ll, NULL), CDS_ERR_EMPTY);
    cds_llist_destroy(empty_ll);

    printf("\n--- pop/pop_back single element ---\n");
    cds_llist *single_ll = cds_llist_create(sizeof(int));
    int single_val = 99;
    cds_llist_append(single_ll, &single_val);
    cds_llist_pop(single_ll, &ll_out);
    ASSERT_EQ(ll_out, 99);
    ASSERT_EQ(cds_llist_count(single_ll), 0);
    cds_llist_append(single_ll, &single_val);
    cds_llist_pop_back(single_ll, &ll_out);
    ASSERT_EQ(ll_out, 99);
    ASSERT_EQ(cds_llist_count(single_ll), 0);
    cds_llist_destroy(single_ll);

    printf("\n--- insert ---\n");
    /* list is [20, 30, 40] */
    int insert_val = 25;
    cds_llist_insert(ll, &insert_val, 1);  /* [20, 25, 30, 40] */
    cds_llist_get(ll, 1, &ll_out);
    ASSERT_EQ(ll_out, 25);
    ASSERT_EQ(cds_llist_count(ll), 4);
    ASSERT_EQ(cds_llist_insert(ll, &insert_val, 99), CDS_ERR_INDEX);

    printf("\n--- contains ---\n");
    int search_key = 30;
    ASSERT_EQ(cds_llist_contains(ll, &search_key, cmp_int), CDS_OK);
    search_key = 999;
    ASSERT_EQ(cds_llist_contains(ll, &search_key, cmp_int), CDS_ERR_NOT_FOUND);

    printf("\n--- remove (by value) ---\n");
    /* list is [20, 25, 30, 40] */
    int remove_val = 25;
    ASSERT_EQ(cds_llist_remove(ll, &remove_val, cmp_int), CDS_OK);
    ASSERT_EQ(cds_llist_count(ll), 3);
    ASSERT_EQ(cds_llist_contains(ll, &remove_val, cmp_int), CDS_ERR_NOT_FOUND);
    remove_val = 999;
    ASSERT_EQ(cds_llist_remove(ll, &remove_val, cmp_int), CDS_ERR_NOT_FOUND);

    /* remove head by value */
    remove_val = 20;
    cds_llist_remove(ll, &remove_val, cmp_int);  /* [30, 40] */
    cds_llist_get(ll, 0, &ll_out);
    ASSERT_EQ(ll_out, 30);

    /* remove tail by value */
    remove_val = 40;
    cds_llist_remove(ll, &remove_val, cmp_int);  /* [30] */
    ASSERT_EQ(cds_llist_count(ll), 1);

    printf("\n--- purge ---\n");
    cds_llist *dup_ll = cds_llist_create(sizeof(int));
    int dup_vals[] = {5, 10, 5, 20, 5};
    for (size_t i = 0; i < 5; i++) {
        cds_llist_append(dup_ll, &dup_vals[i]);
    }
    int dup_target = 5;
    ASSERT_EQ(cds_llist_purge(dup_ll, &dup_target, cmp_int), CDS_OK);
    ASSERT_EQ(cds_llist_count(dup_ll), 2);
    cds_llist_get(dup_ll, 0, &ll_out);
    ASSERT_EQ(ll_out, 10);
    cds_llist_get(dup_ll, 1, &ll_out);
    ASSERT_EQ(ll_out, 20);
    dup_target = 999;
    ASSERT_EQ(cds_llist_purge(dup_ll, &dup_target, cmp_int), CDS_ERR_NOT_FOUND);

    /* remove_all everything */
    dup_target = 10;
    cds_llist_purge(dup_ll, &dup_target, cmp_int);
    dup_target = 20;
    cds_llist_purge(dup_ll, &dup_target, cmp_int);
    ASSERT_EQ(cds_llist_count(dup_ll), 0);
    cds_llist_destroy(dup_ll);

    printf("\n--- destroy ---\n");
    cds_llist_destroy(ll);
    ASSERT_EQ(cds_llist_destroy(NULL), CDS_OK);

    printf("\n--- empty ---\n");
    cds_llist *el = cds_llist_create(sizeof(int));
    ASSERT_TRUE(cds_llist_empty(el));
    int el_val = 1;
    cds_llist_append(el, &el_val);
    ASSERT_TRUE(!cds_llist_empty(el));
    cds_llist_destroy(el);
    ASSERT_TRUE(cds_llist_empty(NULL));

    printf("\n--- NULL safety ---\n");
    ASSERT_EQ(cds_llist_count(NULL), 0);
}

static void test_stack(void)
{
    printf("\n========== STACK ==========\n");

    printf("\n--- create ---\n");
    ASSERT_NULL(cds_stack_create(0));
    cds_stack *s = cds_stack_create(sizeof(int));
    ASSERT_NOT_NULL(s);
    ASSERT_EQ(cds_stack_count(s), 0);
    ASSERT_TRUE(cds_stack_empty(s));

    printf("\n--- push ---\n");
    int vals[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(cds_stack_push(s, &vals[i]), CDS_OK);
    }
    ASSERT_EQ(cds_stack_count(s), 5);
    ASSERT_TRUE(!cds_stack_empty(s));

    printf("\n--- peek ---\n");
    int out;
    ASSERT_EQ(cds_stack_peek(s, &out), CDS_OK);
    ASSERT_EQ(out, 50);
    ASSERT_EQ(cds_stack_count(s), 5);

    printf("\n--- pop (LIFO order) ---\n");
    int expected[] = {50, 40, 30, 20, 10};
    for (size_t i = 0; i < 5; i++) {
        cds_stack_pop(s, &out);
        ASSERT_EQ(out, expected[i]);
    }
    ASSERT_EQ(cds_stack_count(s), 0);
    ASSERT_TRUE(cds_stack_empty(s));

    printf("\n--- pop/peek on empty ---\n");
    ASSERT_EQ(cds_stack_pop(s, &out), CDS_ERR_EMPTY);
    ASSERT_EQ(cds_stack_peek(s, &out), CDS_ERR_EMPTY);

    printf("\n--- pop with NULL out ---\n");
    cds_stack_push(s, &vals[0]);
    cds_stack_push(s, &vals[1]);
    ASSERT_EQ(cds_stack_pop(s, NULL), CDS_OK);
    ASSERT_EQ(cds_stack_count(s), 1);

    printf("\n--- clear ---\n");
    cds_stack_push(s, &vals[2]);
    cds_stack_push(s, &vals[3]);
    ASSERT_EQ(cds_stack_clear(s), CDS_OK);
    ASSERT_EQ(cds_stack_count(s), 0);
    ASSERT_TRUE(cds_stack_empty(s));

    printf("\n--- push after clear ---\n");
    int reuse_val = 99;
    cds_stack_push(s, &reuse_val);
    cds_stack_peek(s, &out);
    ASSERT_EQ(out, 99);

    printf("\n--- destroy ---\n");
    cds_stack_destroy(s);

    printf("\n--- NULL safety ---\n");
    ASSERT_EQ(cds_stack_count(NULL), 0);
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
    ASSERT_EQ(cds_queue_count(q), 0);
    ASSERT_TRUE(cds_queue_empty(q));

    printf("\n--- enq ---\n");
    int vals[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(cds_queue_enq(q, &vals[i]), CDS_OK);
    }
    ASSERT_EQ(cds_queue_count(q), 5);
    ASSERT_TRUE(!cds_queue_empty(q));

    printf("\n--- peek ---\n");
    int out;
    ASSERT_EQ(cds_queue_peek(q, &out), CDS_OK);
    ASSERT_EQ(out, 10);
    ASSERT_EQ(cds_queue_count(q), 5);

    printf("\n--- deq (FIFO order) ---\n");
    int expected[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < 5; i++) {
        cds_queue_deq(q, &out);
        ASSERT_EQ(out, expected[i]);
    }
    ASSERT_EQ(cds_queue_count(q), 0);
    ASSERT_TRUE(cds_queue_empty(q));

    printf("\n--- deq/peek on empty ---\n");
    ASSERT_EQ(cds_queue_deq(q, &out), CDS_ERR_EMPTY);
    ASSERT_EQ(cds_queue_peek(q, &out), CDS_ERR_EMPTY);

    printf("\n--- deq with NULL out ---\n");
    cds_queue_enq(q, &vals[0]);
    cds_queue_enq(q, &vals[1]);
    ASSERT_EQ(cds_queue_deq(q, NULL), CDS_OK);
    ASSERT_EQ(cds_queue_count(q), 1);
    cds_queue_deq(q, &out);
    ASSERT_EQ(out, 20);

    printf("\n--- clear ---\n");
    cds_queue_enq(q, &vals[2]);
    cds_queue_enq(q, &vals[3]);
    ASSERT_EQ(cds_queue_clear(q), CDS_OK);
    ASSERT_EQ(cds_queue_count(q), 0);
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
    ASSERT_EQ(cds_queue_count(NULL), 0);
    ASSERT_TRUE(cds_queue_empty(NULL));
    ASSERT_EQ(cds_queue_enq(NULL, &reuse_val), CDS_ERR_NULL);
    ASSERT_EQ(cds_queue_deq(NULL, &out), CDS_ERR_NULL);
    ASSERT_EQ(cds_queue_peek(NULL, &out), CDS_ERR_NULL);
}

int main(void)
{
    test_vector();
    test_llist();
    test_stack();
    test_queue();
    printf("\n");
    return TEST_RESULTS();
}
