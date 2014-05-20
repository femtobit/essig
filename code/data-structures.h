#ifndef ESSIG_DATASTRUCTURES_H
#define ESSIG_DATASTRUCTURES_H

#include <stdio.h>

typedef struct
{
  int    id;
  double x, y, z;
  char  *element_symbol;
} Atom;

/**
 * Represents a chemical bond around which the molecule may be rotated.
 */
typedef struct
{
  /// Pointer to the first bound atom.
  Atom *first;
  /// Pointer to the second bound atom.
  Atom *second;
  /// Array of pointers to the atom behind 'first'.
  /// Stored here as an optimization.
  Atom **left;
  size_t left_count;
  /// Array of pointers to the atom behind 'second'.
  /// Stored here as an optimization.
  Atom **right;
  size_t right_count;
} Bond;

typedef struct
{
  /// Array of the atoms forming the molecule.
  Atom *atoms;
  /// Length of 'atoms'.
  size_t atom_count;
  /// Array of the rotatable chemical bonds of the molecule.
  Bond *bonds;
  /// Length of 'bonds'.
  size_t bond_count;
  int charge;
} Molecule;

/**
 * Loads molecule data from the file 'fp',
 * TODO: Currently only returns the data of a hard-coded
 *       CH3COOH molecule.
 */
Molecule *molecule_new_from_file(FILE *fp);

/**
 * Frees the memory allocated for the molecule data.
 */
void molecule_free(Molecule *mol);

/**
 * Run Orca to get the energy of the given molecule.
 */
double get_energy(Molecule *mol);

#endif
