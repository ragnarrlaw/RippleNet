#ifndef CT_STORAGE_H
#define CT_STORAGE_H

#include "ct.h"
#include "ct_flags.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

struct ct_storage {
  void *ptr;
  size_t size; /* total size in bytes e.g. (100 * sizeof(float)) */
  atomic_int ref_count;
  ct_flags_t flags;
};

ct_storage_t *storage_create(size_t size);
void storage_retain(ct_storage_t *s);
void storage_release(ct_storage_t *s);

#endif // !CT_STORAGE_H
