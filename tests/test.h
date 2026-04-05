#ifndef CDS_TEST_H
#define CDS_TEST_H

static int test_pass = 0;
static int test_fail = 0;

#define ASSERT_TRUE(expr)                                           \
  do                                                                \
  {                                                                 \
    if (expr)                                                       \
    {                                                               \
      test_pass++;                                                  \
    }                                                               \
    else                                                            \
    {                                                               \
      test_fail++;                                                  \
      printf("  FAIL: %s (%s:%d)\n", #expr, __FILE__, __LINE__);    \
    }                                                               \
  } while (0)

#define ASSERT_EQ(a, b)                                             \
  do {                                                              \
    long _val_a = (long)(a);                                        \
    long _val_b = (long)(b);                                        \
    if (_val_a == _val_b)                                           \
    {                                                               \
      test_pass++;                                                  \
    }                                                               \
    else                                                            \
    {                                                               \
      test_fail++;                                                  \
      printf("  FAIL: %s == %s (expected %ld, got %ld) (%s:%d)\n",  \
             #b, #a, _val_b, _val_a, __FILE__, __LINE__);           \
    }                                                               \
  } while (0)

#define ASSERT_NULL(ptr)                                            \
  do                                                                \
  {                                                                 \
    if (!ptr)                                                       \
    {                                                               \
      test_pass++;                                                  \
    }                                                               \
    else                                                            \
    {                                                               \
      test_fail++;                                                  \
      printf("  FAIL: %s expected NULL (%s:%d)\n",                  \
            #ptr, __FILE__, __LINE__);                              \
    }                                                               \
  } while (0)

#define ASSERT_NOT_NULL(ptr)                                        \
  do                                                                \
  {                                                                 \
    if (ptr)                                                        \
    {                                                               \
      test_pass++;                                                  \
    }                                                               \
    else                                                            \
    {                                                               \
      test_fail++;                                                  \
      printf("  FAIL: %s expected NOT NULL (%s:%d)\n",              \
            #ptr, __FILE__, __LINE__);                              \
    }                                                               \
  } while (0)

#define TEST_RESULTS()                                              \
  (                                                                 \
   printf("Tests: %d passed, %d failed.\n",test_pass, test_fail),   \
   (test_fail > 0) ? 1 : 0                                          \
  )

#endif
