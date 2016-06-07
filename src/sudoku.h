#include "common.h"
#include "cell.h"


/* Assigned in main with pointer indirection.
I want the compiler to error if code attempts to change directly. */
unsigned int group_size;

static unsigned int
extract_group_size (int argc, char *argv[]);

static void
print_grid (struct cell ***);

static enum Return_Code
input_grid (struct cell ****write_grid, unsigned int group_size);

static void
move_cursor_draw_wait (int x, int y);

static enum Return_Code
solve_brute_force_recursive (struct cell ***grid,
                             unsigned int x,
                             unsigned int y);

static enum Return_Code
solve_elimination (struct cell ***grid,
                   struct cell ***groups);

