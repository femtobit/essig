#ifndef ESSIG_SIMULATION_H
#define ESSIG_SIMULATION_H

#include <stdbool.h>

#include "data-structures.h"

/**
 * Simulates the molecule using orca and outputs the calculated
 * energies to stdout.
 *
 * @param mol                        The molecule to be simulated.
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
void run_simulation(Molecule *mol,
                    unsigned int step_count,
                    unsigned int drop_count,
                    double max_dist,
                    double max_angle,
                    double rotation_translation_ratio,
                    double temperature,
                    bool output_intermediate);

#endif
