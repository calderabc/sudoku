#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "file_io.h"
#include "ui.h"
#include "cell.h"
#include "ui.h"
#include "grid.h"

/**
 * A simple Sudoku puzzle generator and solver.
 * TODO: implement generator.
 */

int
main (int argc, char *argv[])
{
  struct cell ***grid = NULL;
  struct cell ***grid_other = NULL;
  struct cell ***groups = NULL;
  struct cell ***groups_other = NULL;
  enum Return_Code return_code;
  unsigned int max_groups_per_cell;



  /* This is the only place this variable should ever be set.  Trying to
     set directly will cause compiler error.  */
  group_size = extract_group_size(argc, argv);
  max_groups_per_cell = 3;

  return_code = create_grid (&grid);
  if (return_code != SUCCESS)
    {
      puts ("Error caught in create_grid function: Program terminated.");
      exit (EXIT_FAILURE);
    }

  associate_groups (&groups, grid);

  associate_cells (groups, grid, max_groups_per_cell);


  init_screen (NCURSES, group_size);

  print_grid (grid);

  read_grid_from_file (&grid, "file.sdk", group_size);
  print_grid (grid);
  input_grid (&grid, group_size);

  /*grid_other = clone_grid (grid, group_size);

  /* Make cursor invisible.  */
  /*hide_cursor (); */

  solve_brute_force_recursive (grid, 0, 0);
  /* TODO: Finish making this work.
  /* solve_elimination (grid, groups); */

  show_cursor();

  /*
  print_grid (grid_other);

  associate_groups (&groups_other, grid);

  associate_cells (groups_other, grid, max_groups_per_cell);

  input_grid (&grid_other);
  */


  print_grid (grid);

  while (get_keypress() != QUIT) {}

  terminate_board ();

  return_code = delete_grid(&grid, &groups, max_groups_per_cell);
  if (return_code != SUCCESS)
    {
      printf ("Error caught in delete_grid function: Program terminated.\n");
      exit (EXIT_FAILURE);
    }

  exit (EXIT_SUCCESS);
}

static unsigned int
extract_group_size (int argc, char *argv[])
{
  unsigned input_value;
  unsigned block_width = 3; /* Default  */

  if (argc >= 2)
    {
      input_value = atoi(argv[1]);
      if (input_value >= 2 && input_value <= 5)
        {
          block_width = input_value;
        }
    }

  return block_width * block_width;
}


static void
print_grid (struct cell ***output_grid)
{
  unsigned int i, j;

  for (i = 0; i < group_size; i++)
    {
      for (j = 0; j < group_size; j++)
        {
          draw_cell (i, j, get_cell_value (output_grid[i][j]));
        }
    }
  move_cursor (0, 0);
  draw_board (group_size);
}




static enum Return_Code
input_grid (struct cell ****write_grid, unsigned int group_size)
{
  unsigned int x, y, cell_value;
  enum input_code keypress;

  char *keypress_string = NULL;

  if (*write_grid == NULL)
    return POINTER_NOT_NULL;

  x = y = 0;

  do
    {
      move_cursor (x, y);
      draw_screen ();

      do
        {
          keypress = get_keypress ();
        }
      while (keypress == NONE);


      /* The if statements within the movement case statements check to see
       * if curser has left the boundaries of the puzzle and if so moves the
       * cursor to the first position on the opposite side.
       * Cursor wrapping.  */
      switch (keypress)
        {
        case DOWN:
          y++;
          if (y >= group_size)
            y = 0;
          break;
        case UP:
          y--;
          if (y < 0)
            y = group_size - 1;
          break;
        case RIGHT:
          x++;
          if (x >= group_size)
            x = 0;
          break;
        case LEFT:
          x--;
          if (x < 0)
            x = group_size - 1;
          break;
        case QUIT:
          return SUCCESS;
        case UNKNOWN:
          break;
        default:
          /* TODO: Fix so I can input numbers greater than 9.  */
              cell_value = (int) keypress;
              draw_cell(x, y, cell_value);
              set_cell_value((*write_grid)[x][y], cell_value);
        }


    }
  while (keypress != QUIT);

  return SUCCESS;
}


void
move_cursor_draw_wait (int x, int y)
{
  move_cursor (x, y);
  draw_screen ();
  /* usleep (10000); */
}

static boolean
does_number_fit (struct cell *which_cell,
                 unsigned int test_number,
                 unsigned int group_size,
                 unsigned int max_groups_per_cell)
{
  unsigned int i, j;

  for (i = 0; i < max_groups_per_cell; i++)
  /*for (i = 0; which_cell->groups[i] != NULL; i++) */
    {
      for (j = 0; j < group_size; j++)
        {
          if (get_cell_value (which_cell->groups[i][j])
              == test_number)
            return FALSE;
        }
    }
  return TRUE;
}


static enum Return_Code
solve_brute_force_recursive (struct cell ***grid,
                             unsigned int x,
                             unsigned int y)
{
  unsigned int test_value;
  enum Return_Code return_code;
  struct cell *cell_pointer;

  if (get_keypress() == QUIT) return TERMINATE;




  while (TRUE)
    {
      if (x >= group_size)
        {
          /* Past the end of row so move to leftmost cell of the row below.  */
          x = 0;

          if (++y >= group_size)
            /* End case for recursion.  Past the bottommost row.  */
            return SUCCESS;
        }

      /* Continue until a cell which has not already been set is reached.  */
      if (is_cell_set (grid[x][y]) == FALSE)
        break;

      x++;
    }

  /* Unsure if this is necessary.  Compiler may already do something like
   * this, but makes the following code simpler, less error prone.  */
  cell_pointer = grid[x][y];

  for (test_value = 1; test_value <= group_size; test_value++)
    {
      if (does_number_fit (cell_pointer, test_value, group_size, 3) == TRUE)
        {
          /* Set temporary test value.  */
          set_test_value (cell_pointer, test_value);

          draw_cell (x, y, test_value);
          /* move_cursor_draw_wait (x, y); */
          draw_screen ();

          /* Test the next cell in this row.  */
          return_code = solve_brute_force_recursive (grid,
                                                     x + 1,
                                                     y);
          if (return_code == SUCCESS)
            {
              /* All the cells tested after this one check out.
                 Set permanent value.  */
              set_cell_value (cell_pointer, test_value);
              return SUCCESS;
            }
          else if (return_code == TERMINATE)
            return TERMINATE;

          unset_cell_value (cell_pointer);

          draw_cell (x, y, 0);
          /* move_cursor_draw_wait (x, y); */
          draw_screen ();
        }
    }
    return UNSOLVABLE;
}

static void
eliminate_in_cells_groups(struct cell *which, unsigned int value)
{
  unsigned int i, j;
  struct cell **test_group;

  for (i = 0; i < which->group_count; i++)
    {
      test_group = which->groups[i];

      for (j = 0; j < group_size; j++)
        {
          eliminate_possibility(test_group[j], value);
        }
    }
}

enum Return_Code
solve_elimination (struct cell ***grid,
                   struct cell ***groups)
{
  boolean is_solved;
  struct cell *possible_cell;
  struct cell **test_group;
  unsigned int x, y, i;
  unsigned int test_value;
  int match_count;
  unsigned int cell_index;
  struct cell *test_cell;

  do
    {
      is_solved = TRUE;

      for (x = 0; x < group_size; x++)
        {
          for (y = 0; y < group_size; y++)
            {
              if (get_keypress() == QUIT) return TERMINATE;

              test_cell = grid[x][y];

              /* TODO: Inefficient.  Fix to only output solved cell when it
               * is first solved.  No need to print it out every traversal
               * through the grid.  Also eliminate possibilities only the
               * first time.*/
              /* print_possibilities (test_cell, x, y); */
              if (is_cell_set (test_cell))
                {
                  draw_cell (x, y, get_cell_value(test_cell));
                  draw_screen ();

                  eliminate_in_cells_groups(test_cell, get_cell_value (test_cell));

                } /* if (is_cell_set (test_cell))  */
              else
                {
                  is_solved = FALSE;
                }
            } /* for (x = 0; x < group_size; x++)  */
        } /* for (y = 0; y < group_size; y++)  */


      /* TODO: Fix so uses the actual size of groups array.  */
      for (i = 0; i < group_size * 3; i++)
        {
          test_group = groups[i];

          /* Check to see if number can only be in one place
           * in the group. */
          for (test_value = 1; test_value <= group_size; test_value++)
            {
              possible_cell = NULL;
              match_count = 0;
              for (cell_index = 0;
                   cell_index < group_size;
                   cell_index++)
                {
                  test_cell = test_group[cell_index];

                  if (is_possible (test_cell, test_value))
                    {
                      possible_cell = test_cell;
                      match_count++;

                      if (match_count > 1)
                        break;
                    }
                }

              if (match_count == 1)
                {
                  set_cell_value(possible_cell, test_value);
                  eliminate_in_cells_groups(possible_cell, test_value);

                  break;
                }

            }
        }
    }
  while (is_solved == FALSE);

  return SUCCESS;
}
