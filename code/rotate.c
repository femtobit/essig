/**
 * This file is part of essig.
 * Here functions for rotating parts of a molekule
 * 
 * Author(s):
 *   Michael Hufschmidt <michael@hufschmidt-web.de>
 */

#include <assert.h>
#include <math.h>
#include "essig.h"
#include "vector.h"
#include "rotate.h"


// Diese Funktion rotiert Teile eines Moleküls m
// um eine Achse, gegeben durch eine Bindung b
// (= Verbindungslinie r zweier Atome) dieses
// Moleküls um den Winkel φ (gegeben in Radian) 
// mögliche Werte (-2π ... +2π) und berechnet die
// neuen Koordinaten aller Atome dieses Moleküls
void molecule_rotate(Molecule *m, const Bond b, const double phi)
{
  int i = 0;
  double theta = 0;
  Vector ursprung;
  Matrix Rot, Rot_Inv;
  assert(m->atoms != NULL);
  assert(b.first != NULL);
  assert(fabs(phi) < 100.0);
  ursprung = b.first->pos;
  
  // 1. Verschiebe  den Koordinatenursprung fuer jedes Atom 
  // in das erste der zwei Bindungsatome
  for (i = 0; i < m->atom_count-1; i++)
    m->atoms[i].pos = vector_substract(m->atoms[i].pos, ursprung);
    
  // 2. Bestimme den Winkel, den die Verbindungslinie zwischen b.last
  // und b.first mit der z-Achse bildet:
  // TODO: Stimmt das so? Oder Betrag im Zaehler?
  theta = acos(b.second->pos.x[2] /
            sqrt(vector_scalar_product(b.second->pos, b.second->pos)));
            
  // 3. Berechne Rotatonsmatritzen
  Rot = euler_rotate(0, theta, 0);
  Rot_Inv = matrix_transpose(Rot);
  
  // 4. Berechne Kordinaten im gedrehten Koordinatensystem
  for (i = 0; i < m->atom_count-1; i++)
    m->atoms[i].pos = matrix_times_vector(Rot, m->atoms[i].pos);
    
  // 5. Drehe Atome oberhalb der Bindungslinie um die z-Achse um phi
  for (i = 0; i < b.right_count-1; i++)
    m->atoms[b.right[i]].pos = vector_rotate_z(
      m->atoms[b.right[i]].pos, phi);
  
  // 6. Drehe das Koordinatensystem zurueck
  for (i = 0; i < m->atom_count-1; i++)
    m->atoms[i].pos = matrix_times_vector(Rot_Inv, m->atoms[i].pos);

  // 7. Verschiebe  den Koordinatenursprung fuer jedes Atom 
  // wieder in die ursprüngliche Position
  for (i = 0; i < m->atom_count-1; i++)
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
