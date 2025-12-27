/* tensor v0.0.1 */

#include "ct.h"

#include <stdatomic.h>
#include <stdint.h>

typedef uint32_t ct_flags_t;

#define CT_FLAG_OWNS_DATA (1u << 0)
#define CT_FLAG_BORROWED (1u << 1)
#define CT_FLAG_CONTIGUOUS (1u << 2)
#define CT_FLAG_READONLY (1u << 3)
#define CT_FLAG_CUSTOM_FREE (1u << 4)

struct ct_tensor {
  void *data;
  ct_dtype_t dtype;
  int32_t ndim;
  int64_t *shape;
  int64_t *stride;
  ct_flags_t flags;
  atomic_int refcount;
  void *allocator_ctx;
  void (*free_fn)(void *ptr, void *allocator_ctx);
};

ct_tensor_t *ct_tensor_create(ct_context_t *ctx, ct_dtype_t dtype,
                              const ct_shape_t *shape) {
  return NULL;
}

ct_tensor_t *ct_tensor_wrap(ct_context_t *ctx, void *data, ct_dtype_t dtype,
                            const ct_shape_t *shape) {
  return NULL;
}

void ct_tensor_retain(ct_tensor_t *t) {}
void ct_tensor_release(ct_tensor_t *t) {}
void *ct_tensor_data(ct_tensor_t *t) { return NULL; }
ct_dtype_t ct_tensor_dtype(const ct_tensor_t *t) { return 0; }
const ct_shape_t *ct_tensor_shape(const ct_tensor_t *t) { return NULL; }
