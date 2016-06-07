#include <stdio.h>

#include "grid.h"

  /* TODO: Make more robust.  */

struct cell ***
read_grid_from_file (struct cell ****write_grid, const char *file_name, unsigned int group_size)
{
  FILE *input;
  char read;
  int x, y;

  if (*write_grid == NULL)
    create_grid(write_grid);

  /* TODO: Implement exception handling for file not found exception.  */
  input = fopen(file_name, "r");

  for (y = 0; y < group_size; y++)
    {
      for (x = 0; x < group_size; x++)
        {
          set_cell_value ((*write_grid)[x][y], fgetc(input) - '0');
        }

        read = fgetc(input);
    }

  fclose(input);
}
