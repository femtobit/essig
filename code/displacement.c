/**
 * This file is part of essig.
 *
 * Authors:
 *  Florian Jochheim<florian.jochheim@gmx.de>
 *  Jan Fabian Schmid <2schmid@inf>
 */
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "essig.h"
#include "util.h"
#include "vector.h"

Vector random_vector(double max_dist)
{
  int i;
  Vector direction = {{0,0,0}};
  double max_direction = max_dist/sqrt(3);
  for(i = 0;i<3;i++)
    direction.x[i] = drand48()*2*max_direction - max_direction;
        
  return direction;
}

void transform_random_displacement(Molecule *mol, double max_dist)
{
  size_t atom_index;
  Vector direction;

  direction = random_vector(max_dist);
  atom_index = drand48()*mol->atom_count;
  mol->atoms[atom_index].pos = vector_add(mol->atoms[atom_index].pos, direction);
  transform_reset_origin(mol);

  DEBUG_PRINTF("Displaced %s atom #%zu by (%f,%f,%f) Å\n",
    mol->atoms[atom_index].element_symbol,
    atom_index,
    direction.x[0], direction.x[1], direction.x[2]);
}

void transform_reset_origin(Molecule *mol)
{
  unsigned long i;
  Vector dist = mol->atoms[0].pos;
  for(i = 0; i < mol->atom_count; i++)
    mol->atoms[i].pos = vector_substract(mol->atoms[i].pos, dist);
}
