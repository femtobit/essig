#include "essig.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void molecule_read_from_file(Molecule *mol, FILE *fp)
{
  Atom *atoms;
  Bond *bonds;
  char *buffer, *token, symbol[3], temp;
  Vector pos;
  size_t atom_count= 0, bonds_count = 0, buffer_size, i;
  atoms = malloc(sizeof(*atoms));
  //bonds = malloc(sizeof(*bonds));
  fseek(fp, 0, SEEK_END);
  buffer_size = ftell(fp)+1;
  fseek(fp, 0, SEEK_SET);
  buffer = malloc(sizeof(*buffer)*buffer_size);
  fread(buffer, sizeof(char), buffer_size-1,fp);
  buffer[buffer_size-1] = '\0';
  token = strtok(buffer, "\n");
  assert(strcmp(token, "> Atoms") == 0);
  token = strtok(buffer, "\n");
  while(strcmp(token, "> Bonds") != 0)
  {
    atom_count++;
    if(atom_count != 1)
      atoms = realloc(atoms, sizeof(Atom)*atom_count);
    i=0;
    while(token[i] != '\t')
    {
      printf("%lu\n", i);
      symbol[i] = token[i];
      i++;
    }
    symbol[2] = '\0';
    sscanf(token,"%lf\t%lf\t%lf",&(pos.x[0]),&(pos.x[1]), &(pos.x[2]));
    token = strtok(buffer, "\n");
    atoms[atom_count-1].pos = pos;
    atoms[atom_count-1].element_symbol = symbol;
    pritnf
  }
  
  /*
   * TODO: BONDS einlesen
   * */
  
  
}

int main(int argc, char *argv[])
{
  assert(argc == 2);
  char *filename = argv[1];
  FILE *file = fopen(filename, "r");
  assert(file != NULL);  
  molecule_read_from_file(NULL, file);
  
  return EXIT_SUCCESS;
}
