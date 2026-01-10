#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void print_error(char *fmt) {
  fprintf(stderr, "%s\n", fmt);
  exit(EXIT_FAILURE);
}
