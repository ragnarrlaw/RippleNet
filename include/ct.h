/* public api for ct v0.0.1 */
#ifndef CT_H
#define CT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// standard error codes & definitions
typedef int32_t ct_status_t;
#define CT_OK 0
#define CT_ERR_INVALID_ARG 1
#define CT_ERR_SHAPE 2
#define CT_ERR_DTYPE 3
#define CT_ERR_NOMEM 4
#define CT_ERR_NOMEM 4
#define CT_ERR_INTERNAL 5

// data types
typedef int32_t ct_dtype_t;
#define CT_F32 0
#define CT_F64 1
#define CT_I32 2

// main structs
typedef struct ct_context ct_context_t;
typedef struct ct_tensor ct_tensor_t;

#ifdef WITH_MAX_DIM_LIMIT
#define MAX_DIMS 8
typedef struct {
  int32_t ndim;
  int64_t shape[MAX_DIMS];
  int64_t stride[MAX_DIMS];
} ct_shape_t;
#else
typedef struct {
  int32_t ndim;
  int64_t *shape;
  int64_t *stride;
} ct_shape_t;
#endif // WITH_MAX_DIM_LIMIT

// ct_context
ct_context_t *ct_context_create(void);
void ct_context_destroy(ct_context_t *ctx);
const char *ct_last_error(ct_context_t *ctx);

// ct_tensor
ct_tensor_t *ct_tensor_create(ct_context_t *ctx, ct_dtype_t dtype,
                              const ct_shape_t *shape);
ct_tensor_t *
ct_tensor_wrap(ct_context_t *ctx, void *data, ct_dtype_t dtype,
               const ct_shape_t *shape); /* borrow a tensor never owned */
void ct_tensor_retain(ct_tensor_t *t);
void ct_tensor_release(ct_tensor_t *t);
void *ct_tensor_data(ct_tensor_t *t);
ct_dtype_t ct_tensor_dtype(const ct_tensor_t *t);
const ct_shape_t *ct_tensor_shape(const ct_tensor_t *t);

// ct_tensor ops (eager)
ct_status_t ct_add(ct_context_t *ctx, const ct_tensor_t *a, ct_tensor_t *b,
                   ct_tensor_t **out);
ct_status_t ct_mat_mul(ct_context_t *ctx, const ct_tensor_t *a, ct_tensor_t *b,
                       ct_tensor_t **out);

// ct_tensor ops (lazy)
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !CT_H
