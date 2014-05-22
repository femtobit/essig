/**
 * Here the functions for rotating parts of a molekule
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
void molekule_rotate(Molecule *m, const Bond b, const double phi)
{
  assert(m->atoms != NULL);
  assert(b.first != NULL);
  assert(fabs(phi) < 100.0);
  // TODO noch keine Funktionalität
}

// Diese Funktion ruft molekule_rotate(...) auf und gibt ein 
// entsprechend modifiziertes Molekül zurück.
Molecule molekule_rotated(const Molecule *m, const Bond b, const double phi)
{
  Molecule *mr = molecule_new();
  molecule_deep_copy(mr, m);
  molekule_rotate(mr, b, phi);
  return *mr;
}
