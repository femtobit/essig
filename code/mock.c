/**
 * This file is part of essig.
 *
 * The functions in this files are here for testing purposes
 * until their real implementations are finished.
 *
 * Authors:
 * 	Damian Hofmann <2hofmann@inf>
 */
#include "essig.h"

#include <stdio.h>
#include <stdlib.h>

double orca_calculate_energy(const Molecule *mol)
{
  return 250 + 5*drand48();
}

void molden_output_molecule(const Molecule *mol)
{
}

void transform_reset_origin(Molecule *mol)
{
}

void transform_random_displacement(Molecule *mol, double max_dist)
{
}

void transform_random_rotation(Molecule *mol, double max_angle)
{
}
