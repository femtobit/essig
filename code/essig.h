/**
 * This file is part of essig.
 *
 * Authors:
 *  Damian Hofmann <2hofmann@inf>
 */

#ifndef ESSIG_H
#define ESSIG_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "vector.h"

#define BOLTZMANN_CONSTANT 3.1668114e-6 // Eh/K

typedef struct
{
  int    id;
  Vector pos;
  char   element_symbol[4];
} Atom;

/**
 * Represents a chemical bond around which the molecule may be rotated.
 */
typedef struct
{
  /// Index of the first bound atom.
  size_t first;
  /// Index of the second bound atom.
  size_t second;
  /// Array of indices of the atoms behind 'second'.
  /// Stored here as an optimization.
  size_t *right;
  size_t right_count;
  bool rotatable;
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
 * Simulates the molecule using orca and outputs the calculated
 * energies to stdout.
 *
 * @param molecules                  An array of pointers to the
 *                                   molecules to be simulated.
 * @param step_count                 The number of performed simulation steps.
 * @param drop_count                 The number of results dropped at the start of the
 *                                   simulation.
 * @param max_dist                   The maximum distance in Angstrom an atom is displaced
 *                                   in one step.
 * @param max_angle                  The maximum angle in radians one bound is rotated
 *                                   in one step.
 * @param rotation_translation_ratio The ratio of random translations to random moves of
 *                                   single atoms.
 * @param temperature                The temperature in Kelvin.
 * @param output_intermediate        If this option is true, the programm will (in addition
 *                                   to the calculated energies) also output the intermediate
 *                                   molecule configurations generated in each step.
 */
void run_simulation(Molecule **molecules,
                    unsigned int step_count,
                    unsigned int drop_count,
                    double max_dist,
                    double max_angle,
                    double rotation_translation_ratio,
                    double temperature,
                    bool output_intermediate);

/**
 * Moves the molecule @p mol, so that its first atom
 * is exactly at the origin (0, 0, 0).
 */
void transform_reset_origin(Molecule *mol);

/**
 * Move a radomly selected atom of @p mol by a
 * random value between 0 and @p max_dist.
 */
void transform_random_displacement(Molecule *mol, double max_dist);

/**
 * Rotate the molecule @p mol around a radomly selected bond
 * by a random angle between 0 and @p max_angle.
 */
void transform_random_rotation(Molecule *mol, double max_angle);

/**
 * Returns a pointer to a newly allocated and zero initialized molecule.
 */
Molecule *molecule_new(void);

/**
 * Loads molecule data from the file 'fp' into @p mol.
 * TODO: Currently only returns the data of a hard-coded
 *       CH3COOH molecule.
 */
void molecule_read_from_file(Molecule *mol, FILE *fp);

/**
 * Performs a deep copy of the Molecule @p src to @p dest.
 * This operation requires dest->atoms and dest->bonds to be
 * already allocated (with dest->bonds having exactly the size
 * of @p src->bonds) or NULL.
 */
void molecule_deep_copy(Molecule *dest, const Molecule *src);

/**
 * Frees the memory allocated for the molecule data.
 */
void molecule_free(Molecule *mol);

/**
 * Returns a list of the molecule's atoms in the orca/molden format.
 */
char *molecule_format_atom_list(const Molecule *mol);

/**
 * Writes the molecule to a file in molden format.
 */
void molden_output_molecule(const Molecule *mol);

/**
 * Run Orca to get the energy of the given molecule.
 */
double orca_calculate_energy(const Molecule *mol);

#endif
