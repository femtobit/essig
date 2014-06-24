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
  size_t i;
  if(mol)
  {
    for(i = 0; i < mol->bond_count;i++)
    {      
      free(mol->bonds[i].right);
    }
    free(mol->bonds);
    free(mol->atoms);
    free(mol);
  }
}

void molecule_deep_copy(Molecule *dest, const Molecule *src)
{
  assert(src != NULL);
  assert(dest != NULL);

  size_t size, i;
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
    if(dest->bond_count == 0)
      memset(dest->bonds, 0, size);
    CHECK_ALLOC(dest->bonds);
    for(i = 0; i < src->bond_count;i++) //deep copy every bond
    {
      if(src->bonds[i].right == NULL)
      {
        dest->bonds[i].right = NULL;
        dest->bonds[i].right_count = 0;
      }
      else
      {
        size = src->bonds[i].right_count*sizeof(*src->bonds[i].right);
        dest->bonds[i].right = realloc(dest->bonds[i].right, size);
        CHECK_ALLOC(dest->bonds[i].right);
        size = sizeof(*(src->bonds[i].right))*src->bonds[i].right_count;
        memcpy(dest->bonds[i].right, src->bonds[i].right, size);
        dest->bonds[i].right_count = src->bonds[i].right_count;
      }
      dest->bonds[i].rotatable = src->bonds[i].rotatable;
      dest->bonds[i].first = src->bonds[i].first;
      dest->bonds[i].second = src->bonds[i].second;
    }
    
  }
  dest->atom_count = src->atom_count;
  dest->bond_count = src->bond_count;
  dest->charge = src->charge;
}
/*
int main()
{
  FILE *in = fopen("essig_protoniert.mol", "r");
  Molecule *mol = molecule_new(), *mol2 = molecule_new();
  molecule_read_from_file(mol, in);
  molecule_deep_copy(mol2, mol);
  molecule_free(mol);
  molecule_free(mol2);
  fclose(in);
  return 0;
  
}*/
