#include "util.h"
#include <assert.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Supports 8 dimensions
 * CPU is the only device supported - GPU will be added later
 * Supports float -> 32 bits
 * Using assert -> Assert the Truth => put inside the assert(...) the condition
 * that MUST be true for the program to continue safely.
 * */

#define MAX_DIMS 8

typedef enum device_type { CPU, GPU } device_type;

/* flags related to storage struct */
typedef uint8_t storage_flags;
#define SF_OWNED_DATA (1u << 0)
#define SF_BORROWED_DATA (1u << 1)

/* flags related to matrix struct */
typedef uint8_t mat_flags;
#define MATF_CONTIGUOUS (1u << 0)
#define MATF_NON_CONTIGUOUS (1u << 1)
/* only perform gradient if this is enabled -> MATF_REQUIRES_GRAD */
#define MATF_REQIURES_GRAD (1u << 2)
#define MATF_IS_VIEW (1u << 3)

typedef float *(*storage_alloc_fn)(size_t size);
typedef void (*storage_dealloc_fn)(void *s);

typedef struct storage {
  float *data;
  size_t nelements;
  atomic_int refs;
  device_type device;
  storage_flags flags;
  storage_alloc_fn allocator;
  storage_dealloc_fn deallocator;
} storage;

typedef struct matrix {
  int ndims;
  float *data_ptr; // storage->data + offset where the data starts
  storage *data;
  device_type device;
  int dims[MAX_DIMS];
  int strides[MAX_DIMS];
  mat_flags flags;
} matrix;

// clang-format off
storage *storage_init(int nelements, device_type device);
// clang-format on

/*
 * based on device type, specific deallocators and allocators will be allocated
 */

float *storage_cpu_alloc(size_t size); // allocating CPU storage
void storage_cpu_dealloc(void *s);     // deallocating CPU storage
// clang-format off
void storage_no_op_dealloc(void *s); // if the data is not owned by the storage itself
storage *storage_wrap(void *ptr, size_t nelements, device_type device);
// clang-format on
storage *storage_retain(storage *s);
void storage_release(storage *s);
void storage_free(storage *s);

// clang-format off
matrix *matrix_init(int ndim, const int *dims, device_type device);
// clang-format on
void matrix_free(matrix *m);
matrix *matrix_warp(void *data, int ndims, const int *dims);
error_code matrix_add(const matrix *a, const matrix *b, matrix **out);
error_code matrix_sub(const matrix *a, const matrix *b, matrix **out);
error_code matrix_mul(const matrix *a, const matrix *b, matrix **out);
error_code matrix_add_vector(const matrix *a, const matrix *b, matrix **out);
error_code matrix_sub_vector(const matrix *a, const matrix *b, matrix **out);
const matrix *matrix_slice(const matrix *a, int ndims, int *dims);
void matrix_repr(const matrix *m);

// clang-format off
void calculate_strides_from_dims(int ndims, const int *dims, int *out_dims);
// clang-format on
size_t calculate_nelements_from_dims(int ndims, const int *dims);

/**************************************************************************************/

int main() {
  printf("RippleNet -> version: v%s\n", "0.0.0");
  int ndims = 3;
  int dims[MAX_DIMS] = {100, 28, 28};
  int out_dims[MAX_DIMS] = {0};
  calculate_strides_from_dims(ndims, dims, out_dims);
  printf("{");
  for (int i = 0; i < MAX_DIMS; i++) {
    printf("%d, ", out_dims[i]);
  }
  printf("}\n");
  return EXIT_SUCCESS;
}

// clang-format off
storage *storage_init(int nelements, device_type device) {
  // clang-format on
  assert(device == CPU && "Only CPU related allocations are permitted");
  assert(nelements > 0 && "Number of elements set to zero");
  float *ptr = (float *)calloc(nelements, sizeof(float));
  if (!ptr) {
    print_error("Storage for tensor data can not be allocated");
  }
  storage *s = (storage *)calloc(1, sizeof(storage));
  if (!s) {
    free(ptr);
    print_error("Storage can not be allocated");
  }
  s->data = ptr;
  s->nelements = nelements;
  s->device = device;
  s->refs = ATOMIC_VAR_INIT(1);
  s->flags = SF_OWNED_DATA;
  s->allocator = storage_cpu_alloc;
  s->deallocator = storage_cpu_dealloc;
  return s;
}

// clang-format off
storage *storage_wrap(void *ptr, size_t nelements, device_type device) {
  // clang-format on
  assert(device != CPU && "Only CPU related allocations are permitted");
  assert(!nelements && "Number of dimensions set to zero");
  storage *s = (storage *)calloc(1, sizeof(storage));
  s->data = ptr;
  s->nelements = nelements;
  s->device = device;
  s->refs = ATOMIC_VAR_INIT(0);
  s->flags = SF_BORROWED_DATA;
  s->allocator = storage_cpu_alloc;
  s->deallocator = storage_no_op_dealloc;
  return s;
  return NULL;
}

storage *storage_retain(storage *s) {
  assert(s != NULL && "Storage not allocated");
  atomic_fetch_add(&s->refs, 1);
  return s;
}

void storage_free(storage *s) {
  assert(s != NULL && "Freeing a freed pointer");
  int old = atomic_fetch_sub(&s->refs, 1);
  if (old == 1) {
    s->deallocator(s);
  }
}

float *storage_cpu_alloc(size_t nelements) {
  float *block = (float *)calloc(nelements, sizeof(float));
  return block ? block : NULL;
}

void storage_cpu_dealloc(void *s) {
  if (s) {
    free(s);
  }
  s = NULL;
}

void storage_no_op_dealloc(void *s) {
  // for pre-allocated data, do nothing
}

// clang-format off
matrix *matrix_init(int ndims, const int *dims, device_type device) {
  // clang-format on
  assert(device != CPU && "Only CPU related allocations are permitted");
  assert(ndims > 0 && "Number of dimensions should be either 1 or greater");
  assert(ndims < MAX_DIMS && "Number of dimensions must be less than MAX_DIMS");
  assert(!dims && "Dimensions set to NULL");
  size_t nelements = calculate_nelements_from_dims(ndims, dims);
  storage *s_ptr = storage_init(nelements, CPU);
  if (!s_ptr) {
    print_error("Matrix storage cannot be allocated");
  }
  matrix *m_ptr = (matrix *)calloc(1, sizeof(matrix));
  if (!m_ptr) {
    print_error("Matrix cannot be allocated");
  }
  calculate_strides_from_dims(ndims, dims, m_ptr->strides);
  m_ptr->ndims = ndims;
  memcpy(m_ptr->dims, dims, MAX_DIMS);
  m_ptr->data_ptr = s_ptr->data;
  m_ptr->flags = MATF_CONTIGUOUS;
  return m_ptr;
}

void matrix_free(matrix *m) {}

// clang-format off
void calculate_strides_from_dims(int ndims, const int *dims, int *out_dims) {
  // clang-format on
  assert(ndims > 0 && "Number of dimensions should be greater that zero");
  assert(dims != NULL && "Dimensions set to NULL");
  assert(out_dims != NULL && "Output dimensions set to NULL");
  int acc = 1;
  memset((void *)out_dims, 0, MAX_DIMS);
  for (int i = ndims - 1; i > -1; i--) {
    out_dims[i] = acc;
    acc *= dims[i];
  }
}

size_t calculate_nelements_from_dims(int ndims, const int *dims) {
  assert(ndims > 0 && "Number of dimensions should be greater that zero");
  assert(dims != NULL && "Dimensions set to NULL");
  int nelems = 1;
  for (int i = 0; i < ndims; i++) {
    nelems *= dims[i];
  }
  return nelems;
}

error_code matrix_add(const matrix *a, const matrix *b, matrix **out) {
  return NULL;
}

error_code matrix_sub(const matrix *a, const matrix *b, matrix **out) {
  return NULL;
}

error_code matrix_mul(const matrix *a, const matrix *b, matrix **out) {
  return NULL;
}

const matrix *matrix_slice(const matrix *a, int ndims, int *dims) {
  return NULL;
}

error_code matrix_add_vector(const matrix *a, const matrix *b, matrix **out) {
  return NULL;
}

error_code matrix_sub_vector(const matrix *a, const matrix *b, matrix **out) {
  return NULL;
}

void matrix_repr(const matrix *m) {
  assert(!m && "Matrix is not allocated");
}


