/* v0.0.0 doesn't support many devices only CPU */
#ifndef CT_STORAGE_H
#define CT_STORAGE_H

#include "ct.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint32_t ct_storage_flags_t;

#define CT_STORAGE_OWNS_MEMORY (1u << 0) // If set, free(data) when ref_count==0
#define CT_STORAEG_PINNED (1u << 1)      // Is this Page-Locked (Pinned) memory?

// storage allocator fn type
typedef ct_status_t (*ct_allocator_fn)(ct_storage_t *s);
// storage de-allocator fn type
typedef void (*ct_deleter_fn)(ct_storage_t *s);

struct ct_storage {
  void *ptr;
  size_t size; /* total size in bytes e.g. (100 * sizeof(float)) */
  atomic_int ref_count;
  // ct_device_t device;
  ct_storage_flags_t flags;
  ct_allocator_fn alloc;
  ct_deleter_fn del;
};

ct_storage_t *ct_storage_create(size_t size);
ct_storage_t *ct_storage_retain(ct_storage_t *s);
void ct_storage_release(ct_storage_t *s);

ct_status_t ct_storage_cpu_alloc(ct_storage_t *s);
void ct_storage_cpu_delete(ct_storage_t *s);
// does nothing used for borrowed data
void ct_storage_noop_delete(ct_storage_t *s);

#endif // !CT_STORAGE_H
