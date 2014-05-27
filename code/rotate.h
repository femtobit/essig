/**
 * This file is part of essig.
 * Here functions for rotating parts of a molekule
 * 
 * Author(s):
 *   Michael Hufschmidt <michael@hufschmidt-web.de>
 */

#include "essig.h"
// Diese Funktion rotiert Teile eines Moleküls m
// um eine Achse, gegeben durch eine Bindung b
// (= Verbindungslinie r zweier Atome) dieses
// Moleküls um den Winkel φ (gegeben in Radian) 
// mögliche Werte (-2π ... +2π) und berechnet die
// neuen Koordinaten aller Atome dieses Moleküls
void molecule_rotate(Molecule *m, const Bond b, const double phi);

// Diese Funktion ruft molekule_rotate(...) auf und gibt ein entsprechend
// modifiziertes Molekül zurück.
Molecule molecule_rotated(const Molecule *m, const Bond b, const double phi);
