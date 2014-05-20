#ifndef ESSIG_UTIL_H
#define ESSIG_UTIL_H

#define CHECK_ALLOC(p) \
if((p) == NULL) \
{ \
  fprintf(stderr, "Error: Could not allocate memory (at %s:%d)\n", \
          __FILE__, __LINE__); \
  exit(EXIT_FAILURE); \
}

#endif