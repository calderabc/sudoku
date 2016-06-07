
#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "ui.h"

static unsigned int max_value;

static void
create_possibilities (struct cell *which)
{

  int i;
  /* p = possibility.  */
  struct possibility *new_p, *next_p;

  which->possibility_count = max_value;
  which->possibilities = NULL;

  next_p = NULL;
  for (i = max_value; i > 0; i--)
    {
      new_p = (struct possibility *) malloc (sizeof (struct possibility));
      new_p->value = i;
      new_p->next = next_p;
      next_p = new_p;
    }

  which->possibilities = next_p;
}

static void
remove_possibilities (struct cell *which)
{
  struct possibility *current;
  while (which->possibilities != NULL)
    {
      current = which->possibilities;
      which->possibilities = current->next;
      free (current);
    }
  which->possibility_count = 0;
}

void
init_cell(struct cell *new_cell, unsigned int group_size)
{
  /* Only place this should be assigned.  */
  max_value = group_size;

  new_cell->value = 0;
  new_cell->is_set = FALSE;
  new_cell->groups = NULL;
  new_cell->group_count = 0;

  create_possibilities (new_cell);

}

void
terminate_cell (struct cell *which)
{
  remove_possibilities(which);
}

boolean
is_cell_set(struct cell *which)
{
  return which->is_set;
}

void
set_cell_value(struct cell *which, unsigned int value)
{
  which->value = value;
  if (value == 0)
    {
      which->is_set = FALSE;
      create_possibilities(which);
    }
  else
    {
      which->is_set = TRUE;
      remove_possibilities(which);
    }
}

void
unset_cell_value(struct cell *which)
{
  which->value = 0;
  which->is_set = FALSE;
}

void
set_test_value(struct cell *which, unsigned int value)
{
  which->value = value;
}

int
get_cell_value(struct cell *which)
{
  /*if (!which->is_set) {
      return 0;
  }*/
  return which->value;
}

boolean
is_possible(struct cell *which, unsigned int value)
{
  struct possibility *test;



  for (test = which->possibilities; test != NULL; test = test->next)
    {
      if (test->value == value)
        return TRUE;
    }
  return FALSE;
}

void eliminate_possibility(struct cell *which, unsigned int value)
{
  struct possibility **current_next_ptr, *next;

  if (is_cell_set(which))
    return;

  current_next_ptr = &which->possibilities;
  next = *current_next_ptr;
  while (next != NULL && next->value <= value)
    {
      if (next->value == value)
        {
          *current_next_ptr = next->next;
          free (next);
          next = NULL;

          which->possibility_count--;

          break;
        }

      current_next_ptr = &next->next;
      next = *current_next_ptr;
    }
}

void
print_possibilities (struct cell *which, int x, int y)
{
  struct possibility *current;
  move_cursor(x, y);
  current = which->possibilities;
  while (current != NULL)
    {
      /* printw("%i", 1); */
      current = current->next;
    }
}


