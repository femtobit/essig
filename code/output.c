/**
* This file is part of essig.
*
* Authors:
*   Niek Andresen <2andrese@inf>
*   Robert Hartmann <2hartman@inf>
*/

/* To think about: What is the maximum size of coordinates? (~line 25)*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "essig.h"

/**
 * Returns a list of the molecule's atoms in the orca/molden format.
 */
char *molecule_format_atom_list(const Molecule *mol)
{
  char *resultS; /* result string */
  char *temp;
  size_t i, j, atom_count;
  Atom *atom;

  atom_count = mol->atom_count;
  resultS = malloc((1 + atom_count * (3 * 14 + 1 * 2 + 1 * 1)) * sizeof *resultS);
  /* 1 end of string and 3 coordinates, 1 element symbol and 1 end of line per atom */
  resultS[0] = '\0';


  for(i = 0; i < atom_count; ++i)
  {
    atom = mol->atoms + i;
    resultS = strcat(resultS, atom->element_symbol);
    for(j = 0; j < 3; ++j)
    {
      temp = malloc(14 * sizeof *temp);
      /* 11 characters for one coordinate should be enough(?) */
      assert(temp != NULL);
      sprintf(temp, "   %*.5f", 11, atom->pos.x[j]);
      resultS = strcat(resultS, temp);
      free(temp);
    }
    resultS = strcat(resultS, "\n");
  }
  return resultS;
}

/**
 * Returns a file of the molecule atoms molden can work with.
 */
void molden_output_molecule(const Molecule *mol)
{
   char *list;
   FILE *dat;
   
   dat = fopen("molden_input.xyz", "w+");
   assert(dat != NULL);
   list = molecule_format_atom_list(mol);
   fprintf(dat,"%ld\n\n", mol->atom_count);
   fprintf(dat, "%s", list);   
   
   free(list);
   fclose(dat);
   
   system("/work/public/tcc/molden molden_input.xyz");
}

