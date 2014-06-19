/**
 * This file is part of essig.
 * 
 * Authors:
 * 	Damian Hofmann <2hofmann@inf>
 */
#ifndef ESSIG_UTIL_H
#define ESSIG_UTIL_H

#define FAIL(...) \
  fprintf(stderr, __VA_ARGS__); \
  exit(EXIT_FAILURE);

#define CHECK_ALLOC(p) \
  if((p) == NULL) \
  { \
    FAIL("Error: Could not allocate memory (at %s:%d)\n", __FILE__, __LINE__); \
  }

#define FAIL_UNLESS(cond) \
  if(!(cond)) \
  { \
    FAIL("Error: Check '" #cond "' failed\n"); \
  }

#ifndef NDEBUG
#define DEBUG_PRINTF(...) fprintf(stderr, "DEBUG:\t" __VA_ARGS__);
#else
#define DEBUG_PRINTF(...)
#endif

#define UNUSED(x) (void)(x);

#endif
