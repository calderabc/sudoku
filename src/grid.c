#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "grid.h"
#include "cell.h"

struct cell ***clone_grid (struct cell ***grid)
{
  unsigned int x, y;
  struct cell ***other;
  struct cell *current;

  create_grid (&other);
  for (x = 0; x < group_size; x++)
    {
      for (y = 0; y < group_size; y++)
        {
          current = grid[x][y];
          if (is_cell_set (current))
            {
              set_cell_value (other[x][y], get_cell_value (current) );
            }
        }
    }

  return other;
}



enum Return_Code
create_grid (struct cell ****new_grid)
{
  unsigned int i, j; /* Increment variables.  */

  /* Require that the referenced pointer is NULL to avoid marooned memory.  */
  if (*new_grid != NULL)
    return POINTER_NOT_NULL;

  *new_grid = (struct cell***) malloc (group_size * sizeof (struct cell**));

  if (*new_grid == NULL) /* Exit on failed malloc.  */
    return FAIL_MALLOC;

  for (i = 0; i < group_size; i++)
    {
      (*new_grid)[i] = (struct cell**) malloc (group_size
                                               * sizeof (struct cell*));

      if ((*new_grid)[i] == NULL)
        return FAIL_MALLOC;

      for (j = 0; j < group_size; j++)
        {
          (*new_grid)[i][j] = (struct cell*) malloc (sizeof (struct cell));

          if ((*new_grid)[i][j] == NULL)
            return FAIL_MALLOC;

          init_cell ((*new_grid)[i][j], group_size);
        }
    }

  return SUCCESS;
}

enum Return_Code
associate_groups (struct cell ****groups,
                  struct cell ***grid)
{
  /* TODO add associations for more complex sudoku-ish games. */

  /* TODO replace usleep() delays with delays that check with system clock to
   * normalize the time spent on each cell.  */
  unsigned int i, j, k, l;

  unsigned int width;
  int group_index;

  /* If grid pointer is NULL the grid hasn't been initialized yet. Exit.  */
  if (*grid == NULL)
    return POINTER_NULL;

  *groups = (struct cell***) malloc (3 * group_size * sizeof (struct cell**));

  if (*groups == NULL)
    return FAIL_MALLOC;

  /* columns  */
  for (i = 0; i < group_size; i++)
    {
      (*groups)[i] = (struct cell**) malloc (group_size
                                             * sizeof (struct cell*));

      if ((*groups)[i] == NULL)
        return FAIL_MALLOC;

      for (j = 0; j < group_size; j++)
        {
          add_cell_to_group ((*groups), i, j, grid, i, j);
        }
    }


  /* rows  */
  for (i = group_size; i < 2 * group_size; i++)
    {
      (*groups)[i] = (struct cell**) malloc (group_size
                                             * sizeof (struct cell*));
      if ((*groups)[i] == NULL)
        return FAIL_MALLOC;

      for (j = 0; j < group_size; j++)
        {
          add_cell_to_group (*groups, i, j, grid, j, i - group_size);
        }
    }


  /* box sections  */
  for (i = 0; i < group_size; i++)
    {
      group_index = i + 2 * group_size;

      (*groups)[group_index] =
        (struct cell**) malloc (group_size * sizeof (struct cell*));

      if ((*groups)[group_index] == NULL)
        return FAIL_MALLOC;

      width = (unsigned int) sqrt((double) group_size); /* TODO: Fix, really slow */

      j = 0;
      for (k = 0; k < width; k++)
        {
          for (l = 0; l < width; l++)
            {
              add_cell_to_group (*groups, group_index, j, grid,
                                 (i % width) * width + l,
                                 (i / width) * width + k);
              j++;
            }
        }
    }
  return SUCCESS;
}


void
add_cell_to_group (struct cell ***group,
                   unsigned int group_pointer_index,
                   unsigned int cell_pointer_index,
                   struct cell *** grid,
                   unsigned int x,
                   unsigned int y)
{
  struct cell *cell_pointer = grid[x][y];

  group[group_pointer_index][cell_pointer_index] = cell_pointer;

  cell_pointer->group_count++;

  /* Interesting effect */
  /* move_cursor_draw_wait (x, y); */
}


enum Return_Code
associate_cells (struct cell ***groups,
                 struct cell ***grid,
                 unsigned int max_groups_per_cell)
{
  unsigned int i, j;

  for (i = 0; i < group_size; i++)
    {
      for (j = 0; j < group_size; j++)
        {
          grid[i][j]->groups =
            (struct cell ***) calloc (grid[i][j]->group_count + 1,
                                      sizeof (struct cell **));

          if (groups[i][j]->groups == NULL)
            return FAIL_MALLOC;
        }
    }

  for (i = 0; i < group_size * max_groups_per_cell; i++)
    {
      for (j = 0; j < group_size; j++)
        {
          groups[i][j]->groups[i / group_size] = groups[i];
        }
    }

  return SUCCESS;
}


enum Return_Code
delete_grid (struct cell ****terminal_grid,
             struct cell ****terminal_groups,
             unsigned int max_groups_per_cell)
{
  struct cell *cell_pointer;
  unsigned int x, y, group_index;

  if (*terminal_grid == NULL)
    return POINTER_NULL;

  /* Deallocate grid.  */
  for (x = 0; x < group_size; x++)
    {
      for (y = 0; y < group_size; y++)
        {
          cell_pointer = (*terminal_grid)[x][y];

          free (cell_pointer->groups);
          cell_pointer->groups = NULL;


          terminate_cell(cell_pointer);
          free (cell_pointer);
          cell_pointer = NULL;
        }
      free ((*terminal_grid)[x]);
    }
  free (*terminal_grid);
  *terminal_grid = NULL;

  /* deallocate groups.  */
  for (group_index = 0; group_index < 3 * group_size; group_index++)
    {
      free ((*terminal_groups)[group_index]);
    }
  free (*terminal_groups);
  *terminal_groups = NULL;

  return SUCCESS;
}
