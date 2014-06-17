/**
 * This file is part of essig.
 *
 * Authors:
 *  Damian Hofmann <2hofmann@inf>
 */
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#include "essig.h"
#include "util.h"

static const char *usage = "Usage: %s <input>\n";

static const unsigned int default_step_count = 1000000000;
static const unsigned int default_drop_count = 10000;
static const double default_max_dist  = 0.05; // Å
static const double default_max_angle = 1; // rad
static const double default_rotation_translation_ratio = 1;
static const double default_temperature = 293.2; // K

static long int parse_number(const char *src)
{
  long int val;
  char *endptr = NULL;
  errno = 0;

  val = strtol(src, &endptr, 10);

  if((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
     || (errno != 0 && val == 0))
  {
    FAIL("Error while reading l, m, n: %s", strerror(errno));
  }
  return val;
}

int main(int argc, char *argv[])
{
  int opt;
  unsigned int step_count = default_step_count;
  unsigned int drop_count = default_drop_count;
  while((opt = getopt(argc, argv, "n:d:")) != -1)
  {
    switch(opt)
    {
      case 'n':
        step_count = parse_number(optarg);
        assert(step_count > 0);
        break;
      case 'd':
        drop_count = parse_number(optarg);
        break;
      default:
        FAIL(usage, argv[0]);
    }
  }

  srand48(time(NULL));

  Molecule *molecule = molecule_new();
  molecule_read_from_file(molecule, NULL);

  run_simulation(molecule, 1, step_count, drop_count,
                 default_max_dist, default_max_angle, default_rotation_translation_ratio,
                 default_temperature, true);

  molecule_free(molecule);
  return EXIT_SUCCESS;
}

