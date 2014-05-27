/**
 * This file is part of essig.
 *
 * Authors:
 *  Florian Jochheim<florian.jochheim@gmx.de>
 *  Jan Fabian Schmid <2schmid@inf>
 */
#include<stdlib.h>
#include"vector.h"
#include"essig.h"
#include<time.h>
#include <math.h>


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
  atom_index = drand48()*(mol->atom_count-1);
  mol->atoms[atom_index].pos = vector_add(mol->atoms[atom_index].pos, direction);  
}

void transform_reset_origin(Molecule *mol)
{
  unsigned long i;
  Vector dist = mol->atoms[0].pos;
  for(i = 0; i < mol->atom_count; i++)
    mol->atoms[i].pos = vector_substract(mol->atoms[i].pos, dist);
}
