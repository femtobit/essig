/**
 * This file is part of essig.
 *
 * Authors:
 *  Damian Hofmann <2hofmann@inf>
 */
#include "essig.h"

#include <math.h>
#include <stdlib.h>

#include "util.h"

#define PROTONATED_BASE_ENERGY -32.592475724767
#define UNPROTONATED_BASE_ENERGY -32.057124772061

static void random_move(Molecule **mols, double max_dist, double max_angle,
                        double rotation_translation_ratio)
{
  bool mol_index = drand48() < 0.5;
  DEBUG_PRINTF("Moving molecule #%d\n", mol_index);
  double rand = drand48();
  if(rand < rotation_translation_ratio)
  {
    transform_random_displacement(mols[mol_index], max_dist);
  }
  else
  {
    transform_random_rotation(mols[mol_index], max_angle);
  }
}

static double calculate_energy(Molecule **mols, const double kT)
{
  double E0 = orca_calculate_energy(mols[0]);
  double E1 = orca_calculate_energy(mols[1]);

  double E = log(exp(E0 / kT) + exp(E1 / kT)) / kT;

  DEBUG_PRINTF("Calculating E = log(exp(%.6f) + exp(%.6f)) / kT = %.6f\n",
               E0 / kT, E1 / kT, E);
  return E;
}

void run_simulation(Molecule **mols,
                    unsigned int step_count,
                    unsigned int drop_count,
                    double max_dist,
                    double max_angle,
                    double rotation_translation_ratio,
                    double temperature,
                    bool output_intermediate)
{
  assert(mols != NULL);
  assert(step_count > 0);
  assert(max_dist > 0);
  assert(max_angle > 0);
  assert(rotation_translation_ratio >= 0 && rotation_translation_ratio <= 1);
  assert(temperature >= 0);

  const double kT = temperature * BOLTZMANN_CONSTANT;
  double energy;
  double last_energy = calculate_energy(mols, kT);

  Molecule **local_mols = malloc(2 * sizeof(*local_mols));
  CHECK_ALLOC(local_mols);
  local_mols[0] = molecule_new();
  local_mols[1] = molecule_new();

  bool accepted;
  for(unsigned int i = 0; i < step_count; i++)
  {
    accepted = false;
    molecule_deep_copy(local_mols[0], mols[0]);
    molecule_deep_copy(local_mols[1], mols[1]);

    random_move(local_mols, max_dist, max_angle, rotation_translation_ratio);

    energy = calculate_energy(local_mols, kT);
    double energy_delta = energy - last_energy;

    if(energy_delta < 0)
    {
      DEBUG_PRINTF("ΔE < 0, accepted\n")
      accepted = true;
    }
    else
    {
      double rand = drand48();
      if(rand <= exp(-energy_delta / kT))
      {
        DEBUG_PRINTF("ΔE >= 0, accepted anyway\n");
        accepted = true;
      }
    }

    // debug output calculated energy
    fprintf(stderr, "\n[%d] %s\t%f\t[ΔE=%f]\n",
           i,
           accepted ? "A" : "R",
           energy,
           energy_delta);
    if(output_intermediate)
    {
      fprintf(stderr, "    Molecule #0:");
      char *atom_list = molecule_format_atom_list(local_mols[0]);
      fprintf(stderr, "%s\n", atom_list);
      free(atom_list);
      fprintf(stderr, "    Molecule #1:");
      atom_list = molecule_format_atom_list(local_mols[1]);
      fprintf(stderr, "%s\n", atom_list);
      free(atom_list);
    }

    if(accepted)
    {
      molecule_deep_copy(mols[0], local_mols[0]);
      molecule_deep_copy(mols[1], local_mols[1]);
      last_energy = energy;
      if(i >= drop_count)
      {
        printf("%lf\n", energy);
      }
    }
  }
  molecule_free(local_mols[0]);
  molecule_free(local_mols[1]);
  free(local_mols);
}
