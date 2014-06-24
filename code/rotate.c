/**
 * This file is part of essig.
 * Here functions for rotating parts of a molekule
 *
 * Author(s):
 *   Michael Hufschmidt <michael@hufschmidt-web.de>
 */

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "essig.h"
#include "rotate.h"
#include "util.h"
#include "vector.h"

// Choses a random bound from the molekule *m and rotate
// by an angle between 0 and max_angle. Actual work is done by
// calling molecule_rotate(...)
void transform_random_rotation(Molecule *mol, double max_angle)
{
  size_t bond_index;
  double phi = 0;
  do {
    bond_index = drand48() * mol->bond_count;
  } while(!mol->bonds[bond_index].rotatable);
  phi = drand48()*max_angle*2.0 - max_angle;
  molecule_rotate(mol, mol->bonds[bond_index], phi);

  DEBUG_PRINTF("Rotated bond #%zu (%s-%s) by %f rad\n",
    bond_index,
    mol->atoms[mol->bonds[bond_index].first].element_symbol,
    mol->atoms[mol->bonds[bond_index].second].element_symbol,
    phi);
}

// Diese Funktion rotiert Teile eines Moleküls m
// um eine Achse, gegeben durch eine Bindung b
// (= Verbindungslinie r zweier Atome) dieses
// Moleküls um den Winkel φ (gegeben in Radian)
// mögliche Werte (-2π ... +2π) und berechnet die
// neuen Koordinaten aller Atome dieses Moleküls
void molecule_rotate(Molecule *m, const Bond b, const double phi)
{
  unsigned long  i = 0;
  double theta = 0;
  Vector ursprung;
  Matrix Rot, Rot_Inv;
  assert(m->atoms != NULL);
  assert(fabs(phi) < 100.0);
  ursprung = m->atoms[b.first].pos;

  // 1. Verschiebe  den Koordinatenursprung fuer jedes Atom
  // in das erste der zwei Bindungsatome
  for (i = 0; i < m->atom_count; i++)
    m->atoms[i].pos = vector_substract(m->atoms[i].pos, ursprung);

  // 2. Bestimme den Winkel, den die Verbindungslinie zwischen b.second
  // und b.first mit der z-Achse bildet:
  theta = acos(m->atoms[b.second].pos.x[2] /
          sqrt(vector_scalar_product(m->atoms[b.second].pos,
                                       m->atoms[b.second].pos)));

  // 3. Berechne Rotatonsmatritzen
  Rot = euler_rotate(0, theta, 0);
  Rot_Inv = matrix_transpose(Rot);

  // 4. Berechne Kordinaten im gedrehten Koordinatensystem
  for (i = 0; i < m->atom_count; i++)
    m->atoms[i].pos = matrix_times_vector(Rot, m->atoms[i].pos);

  // 5. Drehe Atome oberhalb der Bindungslinie um die z-Achse um phi
  for (i = 0; i < b.right_count; i++)
    m->atoms[b.right[i]].pos = vector_rotate_z(m->atoms[b.right[i]].pos, phi);

  // 6. Drehe das Koordinatensystem zurueck
  for (i = 0; i < m->atom_count; i++)
    m->atoms[i].pos = matrix_times_vector(Rot_Inv, m->atoms[i].pos);

  // 7. Verschiebe  den Koordinatenursprung fuer jedes Atom
  // wieder in die ursprüngliche Position
  for (i = 0; i < m->atom_count; i++)
    m->atoms[i].pos = vector_add(m->atoms[i].pos, ursprung);
}

// Diese Funktion ruft molecule_rotate(...) auf und gibt ein
// entsprechend modifiziertes Molekül zurück.
Molecule molecule_rotated(const Molecule *m, const Bond b, const double phi)
{
  Molecule *mr = molecule_new();
  molecule_deep_copy(mr, m);
  molecule_rotate(mr, b, phi);
  return *mr;
}
