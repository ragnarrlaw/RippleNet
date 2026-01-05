/* v0.0.0 doesn't support many devices only CPU */
#include "ct_storage.h"
#include "ct.h"
#include <assert.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* only cpu-memory allocation is permitted */
ct_storage_t *ct_storage_create(size_t size) {
  ct_storage_t *ptr = (ct_storage_t *)calloc(1, sizeof(ct_storage_t));
  if (!ptr)
    return NULL;
  ptr->size = size;
  ptr->flags = 0;
  ptr->flags |= CT_STORAGE_OWNS_MEMORY;
  atomic_init(&ptr->ref_count, 1);
  ptr->alloc = &ct_storage_cpu_alloc;
  ptr->del = &ct_storage_cpu_delete;
  if (CT_STATUS_SUCCESS != ptr->alloc(ptr)) {
    free(ptr);
    return NULL;
  }
  return ptr;
}

ct_storage_t *ct_storage_wrap(void *mem, size_t size) {
  ct_storage_t *ptr = (ct_storage_t *)calloc(1, sizeof(ct_storage_t));
  if (!ptr)
    return NULL;
  ptr->size = size;
  ptr->flags = 0;
  ptr->flags &= (~(CT_STORAGE_OWNS_MEMORY));
  atomic_init(&ptr->ref_count, 1);
  ptr->alloc = &ct_storage_cpu_alloc;
  ptr->del = &ct_storage_noop_delete; // memory is not owned
  ptr->ptr = mem;
  return ptr;
}

ct_storage_t *ct_storage_retain(ct_storage_t *s) {
  assert(s != NULL && "Storage not allocated");
  atomic_fetch_add_explicit(&s->ref_count, 1, memory_order_relaxed);
  return s;
}

void ct_storage_release(ct_storage_t *s) {
  assert(s != NULL && "Storage not allocated");
  int old = atomic_fetch_sub_explicit(&s->ref_count, 1, memory_order_relaxed);
  if (old == 1) {
    if (s->flags & CT_STORAGE_OWNS_MEMORY)
      s->del(s);
  }
  free(s);
}

ct_status_t ct_storage_cpu_alloc(ct_storage_t *s) {
  assert(s != NULL && "Storage not allocated");
  if (s->size == 0)
    return CT_STATUS_SUCCESS;
  size_t alignment = 64;
  void *ptr = aligned_alloc(alignment, (s->size + alignment - 1) & ~(alignment - 1));
  if (!ptr)
    return CT_STATUS_ERROR_OOM;
  s->ptr = ptr;
  return CT_STATUS_SUCCESS;
}

void ct_storage_cpu_delete(ct_storage_t *s) {
  assert(s != NULL && "Storage not allocated");
  if (s->ptr) {
    free(s->ptr);
    s->ptr = NULL;
  }
}

// does nothing used for borrowed data
void ct_storage_noop_delete(ct_storage_t *s) {
  // do nothing -> when releasing burrowed data
}
