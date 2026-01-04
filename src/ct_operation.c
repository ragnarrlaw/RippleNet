#include "ct.h"

#include <stdlib.h>

ct_status_t ct_add(const ct_tensor_t *a, ct_tensor_t *b, ct_tensor_t **out) {
  return 0;
}

ct_status_t ct_mat_mul(const ct_tensor_t *a, ct_tensor_t *b,
                       ct_tensor_t **out) {
  return 0;
}

ct_tensor_t *ct_transpose(const ct_tensor_t *t) { return NULL; }

ct_status_t ct_reshape(const ct_tensor_t *t, const ct_shape_t *ns) {
  return CT_STATUS_SUCCESS;
}

ct_tensor_t *ct_slice(const ct_tensor_t *t) { return NULL; }
