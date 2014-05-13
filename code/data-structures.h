#ifndef ESSIG_DATASTRUCTURES_H
#define ESSIG_DATASTRUCTURES_H

typedef struct
{
  Atom* first;
  Atom* second;
  
  /// Parts of the molecule separated by the bond.
  /// (for optimization)
  Atom** left;
  Atom** right;
} Bond;

typedef struct
{
  Atom* atoms;
  int   charge;
  Bond* bonds;
} Molecule;

typedef struct
{
  int    id;
  double x, y, z;
  char*  element_symbol;
} Atom;

#endif