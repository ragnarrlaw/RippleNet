/* version v0.0.0 ct_tensor */

#include "ct.h"

#include <stdint.h>
#include <stdlib.h>

struct ct_tensor {
  void *data;
  ct_dtype_t dtype;
};

/* create tensor */
ct_tensor_t *ct_tensor_create(ct_dtype_t dtype, const ct_shape_t *shape) {
  return NULL;
}

/* borrow a tensor never owned */
void ct_tensor_retain(ct_tensor_t *t) {}

/* release a tensor */
void ct_tensor_release(ct_tensor_t *t) {}

/* return pointer to tensor data */
void *ct_tensor_data(ct_tensor_t *t);

/* return tensor data type */
ct_dtype_t ct_tensor_dtype(const ct_tensor_t *t) { return 0; }

/* return tensor shape */
const ct_shape_t *ct_tensor_shape(const ct_tensor_t *t) { return NULL; }
