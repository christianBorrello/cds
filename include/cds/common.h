#ifndef CDS_COMMON_H
#define CDS_COMMON_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


/* Return codes */
#define CDS_OK             0
#define CDS_ERR_ALLOC     -1
#define CDS_ERR_INDEX     -2
#define CDS_ERR_NULL      -3
#define CDS_ERR_NOT_FOUND -4
#define CDS_ERR_EMPTY     -5

static inline bool is_sizet_overflow(size_t a, size_t b) { return a != 0 && b > SIZE_MAX/a; }

#endif /* CDS_COMMON_H */
