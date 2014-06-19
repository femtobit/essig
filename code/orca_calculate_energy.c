/**
 * This file is part of essig.
 *
 * Authors:
 *   Sebastian Fett <3fett@inf>
 */

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include "essig.h"
#include "vector.h"

#define FILE_BASENAME "orca-data"
#define ORCA_PATH     "/work/public/tcc/orca3.0/orca"
#define ORCA_KEYWORDS "PM3 MINIPRINT"

static void check_fopen(const FILE *file, const char *filename)
{
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file \"%s\"!\n", filename);
        exit(EXIT_FAILURE);
    }
}

double orca_calculate_energy(const Molecule *mol)
{
    FILE *file;
    unsigned long atom_number;
    int orca_return;
    char energy_string[60];
    double energy = 0.0;

    file = fopen(FILE_BASENAME ".in", "w+");
    check_fopen(file, FILE_BASENAME ".in");
    fprintf(file, "!" ORCA_KEYWORDS "\n\n*xyz %i 1\n", mol->charge);
    for (atom_number=0; atom_number<mol->atom_count; atom_number++)
        fprintf(file, "%s %20.14f %20.14f %20.14f\n",
            mol->atoms[atom_number].element_symbol,
            mol->atoms[atom_number].pos.x[0],
            mol->atoms[atom_number].pos.x[1],
            mol->atoms[atom_number].pos.x[2]);
    fprintf(file, "*\n");
    fclose(file);

    orca_return = system(ORCA_PATH " " FILE_BASENAME ".in"
        " | grep \"FINAL SINGLE POINT ENERGY\" > " FILE_BASENAME ".out");
    if (orca_return != 0)
    {
        fprintf(stderr, "Error: Orca call or Orca calculation unsuccessful!\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(FILE_BASENAME ".out", "r+");
    check_fopen(file, FILE_BASENAME ".out");
    while (fgets(energy_string, 60, file) != NULL)
        sscanf(energy_string, "%*[^-0-9]%lf", &energy);
    fclose(file);

    remove(FILE_BASENAME ".in");
    remove(FILE_BASENAME ".out");

    return energy*4.359744e-18;
}
