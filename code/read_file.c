#include "essig.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void build_rights(Bond *bonds, size_t start_bond, size_t start_atom, size_t atom_count, size_t bond_count)
{
  size_t i, j, first, second, right_count = 0;
  bool *bools, changed = true;
  bools = calloc(atom_count, sizeof(*bools));
  assert(bools != NULL);
  bools[start_atom] = true;
  
  while ( changed == true)
  {
    changed = false;
    /*In everz run, every atom that is rechable from a currently selected atom
     * is now selected*/
    for (i = 0; i < bond_count; i++)
    {
      first = bonds[i].first;
      second = bonds[i].second;
      if ( i == start_bond)
        continue;        
      else if(bools[first] && !bools[second])
      {
        changed = true;
        bools[second] = true;
        right_count++;
      }
      else if(!bools[first] && bools[second])
      {
        changed = true;
        bools[first] = true;
        right_count++;
      }
    }
  }
  /*every atom that has been reached is saved in rights array of the bond
   * we are looking at*/
  bonds[start_bond].right = malloc(sizeof(size_t)*right_count);
  for(i=0, j= 0; i<bond_count;i++)
  {
    if(bools[i] && i != start_atom)
    {
      bonds[start_bond].right[j] = i;
      j++;
    }
  }
  bonds[start_bond].right_count = right_count;
  free(bools);
} 

void molecule_read_from_file(Molecule *mol, FILE *fp)
{
  Atom *atoms;
  Bond *bonds;
  char *buffer, *token;
  int charge;
  Vector pos;
  size_t atom_count= 0, bonds_count = 0, buffer_size, i, first, second;
  
  atoms = malloc(sizeof(*atoms));
  bonds = malloc(sizeof(*bonds));
  //bonds = malloc(sizeof(*bonds));
  fseek(fp, 0, SEEK_END);
  buffer_size = ftell(fp)+1;
  fseek(fp, 0, SEEK_SET);
  buffer = malloc(sizeof(*buffer)*buffer_size);
  fread(buffer, sizeof(char), buffer_size-1,fp);
  
  buffer[buffer_size-1] = '\0';
  
  token = strtok(buffer, "\n");
  assert(strcmp(token, "> Atoms") == 0);
  
  token = strtok(NULL, "\n");

  //Atome einlesen
  while(strcmp(token, "> Bonds") != 0)
  {
    atom_count++;
    if(atom_count != 1)
      atoms = realloc(atoms, sizeof(Atom)*atom_count);

    for(i=0;token[i] != '\t'; i++)
    {
      atoms[atom_count-1].element_symbol[i] = token[i];
    }
    atoms[atom_count-1].element_symbol[i] = '\0';

    if(3 != sscanf(&token[i+1],
                    "%lf\t%lf\t%lf",&(pos.x[0]),&(pos.x[1]), &(pos.x[2])))
    {
      fprintf(stderr, "Koordinates in wrong format for %lu\n", atom_count-1);
      exit(EXIT_FAILURE);
    }
    
    token = strtok(NULL, "\n");
    atoms[atom_count-1].pos = pos;

  }

  token = strtok(NULL, "\n");
  while(strcmp(token, "> Charge") != 0)
  {
    bonds_count++;
    if(bonds_count != 1)
      bonds = realloc(bonds, sizeof(*bonds)*bonds_count);
    
    if(2 != sscanf(token,"%lu-%lu", &first, &second))
    {
      fprintf(stderr, "Error reading file. Bonds not in correct format\n");
      exit(EXIT_FAILURE);
    }
    bonds[bonds_count-1].first = first-1;
    bonds[bonds_count-1].second = second-1;
    token = strtok(NULL, "\n");
    
  }
  token = strtok(NULL, "\n");
  if(1 != sscanf(token, "%d", &charge))
  {
    fprintf(stderr,"Charge missing or wrong format\n");
    exit(EXIT_FAILURE);
  }
  /*build right array for every bond that we have
   * */
  for ( i = 0; i < bonds_count; i++)
  {
    build_rights(bonds, i, bonds[i].second, atom_count, bonds_count); 
  }
  mol->atoms = atoms;
  mol->bonds = bonds;
  mol->atom_count = atom_count;
  mol->bond_count= bonds_count;
  mol->charge = charge;
  free(buffer);
}
void print_rights(Molecule *mol)
{
  size_t i,j;
  printf("%lu\n", mol->bond_count);
  for(i = 0; i < mol->bond_count; i++)
  {
    for(j = 0; j < mol->bonds[i].right_count;j++)
    {
      printf("In %lu -th bond :\nfirst is: %lu\n second is: %lu\n %lu -th right is: %lu\n", i,mol->bonds[i].first,mol->bonds[i].second,j, mol->bonds[i].right[j]);
    }
  }
  
}
int main(int argc, char *argv[])
{
  Molecule *mol = malloc(sizeof(Molecule));
  assert(argc == 2);
  char *filename = argv[1];
  FILE *file = fopen(filename, "r");
  assert(file != NULL);  
  molecule_read_from_file(mol, file);
  fclose(file);
  printf(molecule_format_atom_list(mol));
  print_rights(mol);
  free(mol->atoms);
  free(mol->bonds);
  free(mol);
  return EXIT_SUCCESS;
}
