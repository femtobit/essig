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

static void random_move(Molecule *mol, double max_dist, double max_angle,
                        double rotation_translation_ratio)
{
  double rand = drand48();
  if(rand < rotation_translation_ratio)
  {
    transform_random_displacement(mol, max_dist);
  }
  else
  {
    transform_random_rotation(mol, max_angle);
  }
}

void run_simulation(Molecule *mol,
                    size_t molecule_count,
                    unsigned int step_count,
                    unsigned int drop_count,
                    double max_dist,
                    double max_angle,
                    double rotation_translation_ratio,
                    double temperature,
                    bool output_intermediate)
{
  UNUSED(molecule_count);
  assert(mol != NULL);
  assert(step_count > 0);
  assert(max_dist > 0);
  assert(max_angle > 0);
  assert(rotation_translation_ratio >= 0 && rotation_translation_ratio <= 1);
  assert(temperature >= 0);

  const double kT = temperature * BOLTZMANN_CONSTANT;
  double energy;
  double last_energy = orca_calculate_energy(mol);

  Molecule *local_mol = molecule_new();

  bool accepted;
  for(unsigned int i = 0; i < step_count; i++)
  {
    accepted = false;
    molecule_deep_copy(local_mol, mol);

    random_move(local_mol, max_dist, max_angle, rotation_translation_ratio);

    energy = orca_calculate_energy(local_mol);
    double energy_delta = energy - last_energy;

    if(energy_delta < 0)
    {
      accepted = true;
    }
    else
    {
      double rand = drand48();
      if(rand <= exp(-energy_delta / kT))
      {
        accepted = true;
      }
    }

    // debug output calculated energy
    fprintf(stderr, "%s\t%d\t%f\t[ΔE=%f]\n",
           accepted ? "A" : "R",
           i,
           energy,
           energy_delta);
    char *atom_list = molecule_format_atom_list(local_mol);
    fprintf(stderr, "%s\n", atom_list);
    free(atom_list);

    if(accepted)
    {
      molecule_deep_copy(mol, local_mol);
      if(i >= drop_count)
      {
        printf("%lf\n", energy);
      }

      if(output_intermediate)
      {
        molden_output_molecule(local_mol);
      }
    }
  }
  molecule_free(local_mol);
}
