#ifndef UTIL_H
#define UTIL_H

typedef enum error_code {
  OUT_OF_MEMORY,
  DIMENSIONS_MISTMATCH,
  EXCEEDS_MAX_DIMS,
  INVALID_DEVICE_TYPE,
  INVALID_DATA_TYPE,
  EXCEEDED_MAX_THREADS,
  OUT_OF_BOUNDS_OPERATIONS,
} error_code;

/* for align_alloc to round up the size */
#define ALIGN_UP(size, align) (size + (align - 1)) & ~(align - 1)

#define ERROR_LOG(fmt, ...)                                                    \
  fprintf(stderr, "Error in file: %s@%d\n", __FILE__, __LINE__, ##__VA_ARGS_);

void print_error(char *fmt);

#endif // !UTIL_H
