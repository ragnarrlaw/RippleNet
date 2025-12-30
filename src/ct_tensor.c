/* version v0.0.0 ct_tensor */
#include "ct.h"

#include <stdint.h>
#include <stdlib.h>

typedef uint32_t ct_flags_t;

#define CT_FLAG_OWNS_DATA (1u << 0)
#define CT_FLAG_BORROWED (1u << 1)
#define CT_FLAG_CONTIGUOUS (1u << 2)
#define CT_FLAG_READONLY (1u << 3)
#define CT_FLAG_CUSTOM_FREE (1u << 4)

struct ct_tensor {
  void *data;
  ct_dtype_t dtype;
  int32_t num_dim;
  int64_t *shape;
  int64_t *stride;
  ct_flags_t flags;
};

/* create tensor */
ct_tensor_t *ct_tensor_create(ct_dtype_t dtype, const ct_shape_t *shape) {
  return NULL;
}

/* return pointer to tensor data */
void *ct_tensor_data(ct_tensor_t *t);

/* return tensor data type */
ct_dtype_t ct_tensor_dtype(const ct_tensor_t *t) { return 0; }

/* return tensor shape */
const ct_shape_t *ct_tensor_shape(const ct_tensor_t *t) { return NULL; }
