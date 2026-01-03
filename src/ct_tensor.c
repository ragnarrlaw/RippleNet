/* version v0.0.0 ct_tensor */
#include "ct.h"
#include "ct_flags.h"
#include "ct_storage.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int64_t calculate_size(const ct_shape_t *shape);
static void calculate_strides(const ct_shape_t *shape, int64_t *strides_out);

struct ct_tensor {
  ct_storage_t *data;
  ct_dtype_t dtype;
  int32_t ndim;
  int64_t shape[MAX_DIMS];
  int64_t stride[MAX_DIMS];
  ct_flags_t flags;
};

ct_tensor_t *ct_tensor_create(ct_dtype_t dtype, const ct_shape_t *shape) {
  if (dtype != CT_DTYPE_FLOAT32) {
    fprintf(stderr, "Global restriction: Only FLOAT32 supported in v0.0.0\n");
    return NULL;
  }
  return NULL;
}

ct_storage_t *ct_tensor_data(ct_tensor_t *t);

ct_dtype_t ct_tensor_dtype(const ct_tensor_t *t) { return 0; }

const ct_shape_t *ct_tensor_shape(const ct_tensor_t *t) { return NULL; }

/* calculate the total size using shape */
static int64_t calculate_size(const ct_shape_t *shape) {
  assert(shape != NULL && "Shape must be allocated before");
  if (shape->ndim <= 0)
    return 0;
  int64_t n_elements = 1;
  for (int32_t i = 0; i < shape->ndim; i++) {
    n_elements *= shape->shape[i];
  }
  return n_elements;
}

/* calculate default element strides not byte strides (contiguous C-order is
 * assumed) */
static void calculate_strides(const ct_shape_t *shape, int64_t *strides_out) {
  assert(shape != NULL && "Shape allocate shape before the call");
  assert(shape->ndim > MAX_DIMS && "#MAX_DIMS is 8 in v0.0.0");
  if (shape->ndim <= 0)
    return;
  int64_t acc = 1;
  for (int32_t i = shape->ndim - 1; i > -1; i--) {
    strides_out[i] = acc;
    acc *= shape->shape[i];
  }
}
