/**
 * This file is part of essig.
 *
 * Authors:
 *  Damian Hofmann <2hofmann@inf>
 */
#include "essig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


Molecule *molecule_new(void)
{
  Molecule *mol = calloc(1, sizeof *mol);
  CHECK_ALLOC(mol);
  return mol;
}
/*
void molecule_read_from_file(Molecule *mol, FILE *fp)
{
  // TODO
  UNUSED(fp);
  Molecule *test = new_test_molecule();
  molecule_deep_copy(mol, test);
  molecule_free(test);
}*/

void molecule_free(Molecule *mol)
{
  if(mol)
  {
    free(mol->atoms);
    free(mol);
  }
}

void molecule_deep_copy(Molecule *dest, const Molecule *src)
{
  assert(src != NULL);
  assert(dest != NULL);

  size_t size;
  if(src->atom_count > 0)
  {
    size = src->atom_count * sizeof *src->atoms;
    dest->atoms = realloc(dest->atoms, size);
    memcpy(dest->atoms, src->atoms, size);
  }
  if(src->bond_count > 0)
  {
    size = src->bond_count * sizeof *src->bonds;
    dest->bonds = realloc(dest->bonds, size);
    memcpy(dest->bonds, src->bonds, size);
  }
  dest->atom_count = src->atom_count;
  dest->bond_count = src->bond_count;
  dest->charge = src->charge;
}
