#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "data-structures.h"
#include "simulation.h"

static const char *usage = "Usage: %s <input>\n";

static const unsigned int default_step_count = 1000000;
static const unsigned int default_drop_count = 1000;
static const double default_max_dist  = 0.05; // Å
static const double default_max_angle = 1; // rad
static const double default_rotation_translation_ratio = 1;
static const double default_temperature = 293.2; // K

static void exit_with_usage(char *programm_name)
{
  fprintf(stderr, usage, programm_name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  // TODO: Read command line options
  if(argc > 1)
  {
    exit_with_usage(argv[0]);
  }

  Molecule *molecule = molecule_new_from_file(NULL);
  run_simulation(molecule, default_step_count, default_drop_count,
                 default_max_dist, default_max_angle, default_rotation_translation_ratio,
                 default_temperature, true);

  molecule_free(molecule);
  return EXIT_SUCCESS;
}
