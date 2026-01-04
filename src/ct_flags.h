#ifndef CT_FLAGS_H
#define CT_FLAGS_H

#include <stdint.h>

typedef uint32_t ct_flags_t;

#define CT_FLAG_OWNS_DATA (1u << 0)
#define CT_FLAG_BORROWED (1u << 1)
#define CT_FLAG_CONTIGUOUS (1u << 2)
#define CT_FLAG_READONLY (1u << 3)
#define CT_FLAG_CUSTOM_FREE (1u << 4)

#endif // !CT_FLAGS_H
