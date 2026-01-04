#ifndef CT_ERRORS_H
#define CT_ERRORS_H

#include <stdio.h>

#define LOG_ERROR(msg)                                                         \
  fprintf(stderr, "[ERROR] %s (%s:%d): %s\n", __func__, __FILE__, __LINE__,    \
          msg);

/* USE WITH CARE
 * returns immediately, doesn't wait for resources to be cleaned up, prone to
 * memory leaks */
#define CT_TRY(x)                                                              \
  do {                                                                         \
    CT_STATUS_T __s = (x);                                                     \
    if (__s != CT_STATUS_SUCCESS) {                                            \
      fprintf(stderr, "[TRACE] %s:%d in %s()\n", __FILE__, __LINE__,           \
              __func__);                                                       \
      return __s;                                                              \
    }                                                                          \
  } while (0)

#define CT_TRY_GOTO(x)                                                         \
  do {                                                                         \
    CT_STATUS_T __s = (x);                                                     \
    if (__s != CT_STATUS_SUCCESS) {                                            \
      fprintf(stderr, "[TRACE] %s:%d in %s()\n", __FILE__, __LINE__,           \
              __func__);                                                       \
      goto cleanup;                                                            \
    }                                                                          \
  } while (0)

#endif // !CT_ERRORS_H
