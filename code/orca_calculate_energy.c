/**
 * This file is part of essig.
 *
 * Authors:
 *   Sebastian Fett <3fett@inf>
 *   Damian Hofmann <2hofmann@inf>
 */
#include "essig.h"

#include <errno.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vector.h"

#define FILE_BASENAME "orca-data"
#define ORCA_PATH     "/work/tcc/orca3.0/orca"
#define ORCA_KEYWORDS "SP MINIPRINT"

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
    int pipefd[2];
    pid_t pid = 0;

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

    pipe(pipefd);
    pid = fork();
    if(pid == -1) // fork call failed
    {
        perror("Error while calling orca");
    }
    else if(pid == 0) // child process
    {
        close(pipefd[0]);
        // redirect orca output to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        // call orca
        if(execl(ORCA_PATH, ORCA_PATH, FILE_BASENAME ".in", (char *) NULL) == -1)
        {
            perror("Error while calling orca");
            exit(EXIT_FAILURE);
        }
    }
    else // parent process
    {
        close(pipefd[1]);
        file = fdopen(pipefd[0], "r");

        char *current_line = NULL;
        size_t buffer_size = 0;
        ssize_t chars_read = 0;
        char *energy_str_pos = NULL;
        double energy = 0.0;
        bool found = false;

        while ((chars_read = getline(&current_line, &buffer_size, file)) != -1)
        {
            if((energy_str_pos = strstr(current_line, "FINAL SINGLE POINT ENERGY")) != NULL)
            {
                if(sscanf(energy_str_pos, "%*[^-0-9]%lf", &energy) != 1)
                {
                    fprintf(stderr, "Error while calling orca: Could not read energy\n");
                    exit(EXIT_FAILURE);
                }
                found = true;
                break;
            }
        }
        fclose(file);

        // remove temporary files created by orca
        remove(FILE_BASENAME ".in");
        remove(FILE_BASENAME ".engrad");
        remove(FILE_BASENAME ".gbw");
        remove(FILE_BASENAME ".ges");
        remove(FILE_BASENAME ".opt");
        remove(FILE_BASENAME ".prop");
        remove(FILE_BASENAME ".trj");
        remove(FILE_BASENAME ".xyz");
        remove(FILE_BASENAME ".out");

        int status;
        waitpid(pid, &status, 0);
        if(status != 0)
        {
            fprintf(stderr, "Error: Orca returned with non-zero exit status %d\n", status);
            exit(EXIT_FAILURE);
        }

        if(!found)
        {
            fprintf(stderr, "Error: Orca did not return an energy value\n");
            exit(EXIT_FAILURE);
        }

        return energy;
    }
}
