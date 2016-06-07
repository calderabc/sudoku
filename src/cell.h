#ifndef SUDOKU_CELL
#define SUDOKU_CELL

#include "common.h"

struct possibility
{
  unsigned int value;
  struct possibility *next;
};

struct cell
{
  unsigned int value;
  boolean is_set;
  struct cell ***groups;
  unsigned int group_count;

  unsigned int possibility_count;
  struct possibility *possibilities;
};

void init_cell(struct cell *new_cell, unsigned int group_size);
void terminate_cell(struct cell *which);

boolean is_cell_set(struct cell *which);

void set_cell_value(struct cell *which, unsigned int value);

void set_test_value(struct cell *which, unsigned int value);

void unset_cell_value(struct cell *which);

int get_cell_value(struct cell *which);

boolean is_possible(struct cell *which, unsigned int value);

void eliminate_possibility(struct cell *which, unsigned int value);

void
print_possibilites (struct cell *which, int x, int y);

#endif
