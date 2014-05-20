#include "essig.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

static Molecule *new_test_molecule()
{
  Molecule *mol = malloc(sizeof *mol);
  CHECK_ALLOC(mol);

  Atom *atoms = malloc(8 * sizeof *atoms);
  CHECK_ALLOC(atoms);
  atoms[0] = (Atom) {0, 2.461,  -0.280,  1.067, "C"};
  atoms[1] = (Atom) {1, 1.630,  -0.964,  1.256, "H"};
  atoms[2] = (Atom) {2, 3.307,  -0.512,  1.718, "H"};
  atoms[3] = (Atom) {3, 2.779,  -0.396,  0.027, "H"};
  atoms[4] = (Atom) {4, 2.018,   1.128,  1.293, "C"};
  atoms[5] = (Atom) {5, 1.993,   2.023,  0.467, "O"};
  atoms[6] = (Atom) {6, 1.622,   1.344,  2.561, "O"};
  atoms[7] = (Atom) {7, 1.360,   2.288,  2.571, "H"};

  mol->atoms = atoms;
  mol->atom_count = 8;
  mol->bond_count = 0;
  mol->charge = 0;

  return mol;
}

Molecule *molecule_new_from_file(FILE *fp)
{
  return new_test_molecule();
}

void molecule_free(Molecule *mol)
{
  if(mol)
  {
    free(mol->atoms);
    free(mol);
  }
}
