/**
 * This file is part of essig.
 *
 * Authors:
 *  Damian Hofmann <2hofmann@inf>
 */
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "essig.h"
#include "util.h"

static const char *usage = "Usage: %s <input>\n";

static const unsigned int default_step_count = 1000000000;
static const unsigned int default_drop_count = 100000;
static const double default_max_dist  = 0.01; // Å
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

static double parse_double(const char *src)
{
  return strtod(src, NULL); // FIXME handle errors
}

int main(int argc, char *argv[])
{
  int opt;
  FILE *molecule_input;
  char *filename = "essig_protoniert.mol";
  unsigned int step_count = default_step_count;
  unsigned int drop_count = default_drop_count;
  double max_dist = default_max_dist;
  double max_angle = default_max_angle;

  while((opt = getopt(argc, argv, "n:d:D:R:")) != -1)
  {
    switch(opt)
    {
      case 'n':
        step_count = parse_number(optarg);
        FAIL_UNLESS(step_count > 0);
        break;
      case 'd':
        drop_count = parse_number(optarg);
        FAIL_UNLESS(drop_count > 0);
        break;
      case 'D':
        max_dist = parse_double(optarg);
        FAIL_UNLESS(max_dist > 0);
        FAIL_UNLESS(max_dist < INFINITY);
        break;
      case 'R':
        max_angle = parse_double(optarg);
        FAIL_UNLESS(max_angle > 0);
        FAIL_UNLESS(max_angle < INFINITY);
        break;
      default:
        FAIL(usage, argv[0]);
    }
  }

  srand48(time(NULL));

  Molecule *molecule = molecule_new();

  molecule_input = fopen(filename, "r");
  if(molecule_input == NULL)
  {
    fprintf(stderr,"Could not open file : %s\n", filename);
    exit(EXIT_FAILURE);
  }
  molecule_read_from_file(molecule, molecule_input);
  fclose(molecule_input);

  const time_t t = time(NULL);
  struct tm *td = localtime(&t);
  char str_time[100];
  strftime(str_time, sizeof str_time, "%Y-%m-%d %H:%M:%S", td);

  errno = 0;
  char hostname[100];
  if(gethostname(hostname, 100) != 0)
  {
    strcpy(hostname, "unknown host");
  }

  DEBUG_PRINTF("Starting simulation at %s on %s:\n"
               "  step count\t= %u\n"
               "  drop_count\t= %u\n"
               "  max_dist\t= %f Å\n"
               "  max_angle\t= %f rad\n"
               "  RT ratio\t= %f\n"
               "  temperature\t= %f K\n",
               str_time,
               hostname,
               step_count,
               drop_count,
               max_dist,
               max_angle,
               default_rotation_translation_ratio,
               default_temperature);

  run_simulation(molecule, 1, step_count, drop_count, max_dist, max_angle,
                 default_rotation_translation_ratio,default_temperature, true);
  
  molecule_free(molecule);
  return EXIT_SUCCESS;
}
