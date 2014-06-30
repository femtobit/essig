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


static void read_molecule(Molecule *mol, const char *filename)
{
  FILE *infile;

  infile = fopen(filename, "r");
  if(infile == NULL)
  {
    FAIL("Error: Could not open file '%s'\n", filename);
  }
  molecule_read_from_file(mol, infile);
  fclose(infile);
}


int main(int argc, char *argv[])
{
  unsigned int step_count = 1000000000;
  unsigned int drop_count = 100000;
  double rotation_translation_ratio = 1.0;
  double max_dist     = 0.2;   // Å
  double max_angle    = 0.7;   // rad
  double temperature  = 293.2; // K
  bool output_intermediate = false;
  const char *filename = "essig_protoniert.mol";
  const char *second_filename = NULL;

  int opt;
  while((opt = getopt(argc, argv, "n:d:D:A:T:R:f:F:v:")) != -1)
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
      case 'A':
        max_angle = parse_double(optarg);
        FAIL_UNLESS(max_angle > 0);
        FAIL_UNLESS(max_angle < INFINITY);
        break;
      case 'T':
        temperature = parse_double(optarg);
        FAIL_UNLESS(temperature != INFINITY
                    && temperature != -INFINITY);
        FAIL_UNLESS(!isnan(temperature));
        break;
      case 'R':
        rotation_translation_ratio = parse_double(optarg);
        FAIL_UNLESS(rotation_translation_ratio >= 0.0
                    && rotation_translation_ratio <= 1.0);
        break;
      case 'f':
        filename = optarg;
        break;
      case 'F':
        second_filename = optarg;
        break;
      case 'v':
        output_intermediate = true;
        break;
      default:
        FAIL(usage, argv[0]);
    }
  }

  srand48(time(NULL));

  Molecule *molecules[2];
  molecules[0] = molecule_new();
  molecules[1] = molecule_new();

  read_molecule(molecules[0], filename);
  read_molecule(molecules[1], second_filename);

  const time_t t = time(NULL);
  struct tm *td = localtime(&t);
  char time_string[100];
  strftime(time_string, sizeof time_string, "%Y-%m-%d %H:%M:%S", td);

  char hostname[100];
  if(gethostname(hostname, 100) != 0)
  {
    strcpy(hostname, "unknown host");
  }

  fprintf(stderr, "Starting simulation at %s on %s:\n"
               "  step_count\t= %u\n"
               "  drop_count\t= %u\n"
               "  max_dist\t= %f Å\n"
               "  max_angle\t= %f rad\n"
               "  RT ratio\t= %f\n"
               "  temperature\t= %f K\n",
               time_string,
               hostname,
               step_count,
               drop_count,
               max_dist,
               max_angle,
               rotation_translation_ratio,
               temperature);

  run_simulation(molecules, step_count, drop_count,
                 max_dist, max_angle, rotation_translation_ratio,
                 temperature, output_intermediate);

  molecule_free(molecules[0]);
  molecule_free(molecules[1]);
  return EXIT_SUCCESS;
}
