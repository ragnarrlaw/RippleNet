#ifndef CT_H
#define CT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdatomic.h>
#include <stdint.h>

typedef enum ct_status {
  CT_STATUS_SUCCESS = 0,
  CT_STATUS_ERROR_INVALID_ARGUMENT,
  CT_STATUS_ERROR_SHAPE_MISMATCH,
  CT_STATUS_ERROR_DTYPE_INVALID,
  CT_STATUS_ERROR_DTYPE_INCOMPATIBLE,
  CT_STATUS_ERROR_NO_MEMORY,
  CT_STATUS_ERROR_INTERNAL,
} ct_status_t;

/* INITIAL VERSIONS ONLY SUPPORT FLOAT 32 */
typedef enum ct_dtype {
  CT_DTYPE_FLOAT32 = 1,
  CT_DTYPE_FLOAT64,
  CT_DTYPE_INTEGER32,
} ct_dtype_t;

#define MAX_DIMS 8

// main structs
typedef struct ct_tensor ct_tensor_t;
typedef struct ct_storage ct_storage_t;

typedef struct {
  int32_t ndim;
  int64_t shape[MAX_DIMS];
} ct_shape_t;

ct_tensor_t *ct_tensor_create(ct_dtype_t dtype, const ct_shape_t *shape);
ct_dtype_t ct_tensor_dtype(const ct_tensor_t *t);
const ct_shape_t *ct_tensor_shape(const ct_tensor_t *t);
const ct_storage_t *ct_tensor_data(ct_tensor_t *t);

ct_status_t ct_add(const ct_tensor_t *a, ct_tensor_t *b, ct_tensor_t **out);
ct_status_t ct_mat_mul(const ct_tensor_t *a, ct_tensor_t *b, ct_tensor_t **out);
ct_tensor_t *ct_transpose(const ct_tensor_t *t);
ct_status_t ct_reshape(const ct_tensor_t *t, const ct_shape_t *ns);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !CT_H
