/* public api for ct v0.0.0 -> single thread version */
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
#define CT_ERR_INTERNAL 5

// data types
typedef int32_t ct_dtype_t;
#define CT_F32 0
#define CT_F64 1
#define CT_I32 2

// main structs
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

/* create tensor */
ct_tensor_t *ct_tensor_create(ct_dtype_t dtype, const ct_shape_t *shape);
/* borrow a tensor never owned */
void ct_tensor_retain(ct_tensor_t *t);
/* release a tensor, if owner */
void ct_tensor_release(ct_tensor_t *t);
/* return pointer to tensor data */
void *ct_tensor_data(ct_tensor_t *t);
/* return tensor data type */
ct_dtype_t ct_tensor_dtype(const ct_tensor_t *t);
/* return tensor shape */
const ct_shape_t *ct_tensor_shape(const ct_tensor_t *t);

ct_status_t ct_add(const ct_tensor_t *a, ct_tensor_t *b, ct_tensor_t **out);
ct_status_t ct_mat_mul(const ct_tensor_t *a, ct_tensor_t *b, ct_tensor_t **out);

// ct_tensor ops (lazy)
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !CT_H
