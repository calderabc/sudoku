#ifndef SUDOKU_GRID
#define SUDOKU_GRID

#include "common.h"
#include "cell.h"

extern unsigned int group_size;

/* Have to pass in by reference because since I'm using malloc to allocate
   arrays dynamically if I don't pass by reference the reference address that is assigned will not make it back to the
   calling grid.  If I had passed by value it would have put the new
   address into the local variable (new_grid) without changing the
   address contained in the calling variable (grid).
   If I had not added pass by reference this function would create a new
   grid then the memory allocated would be marooned (allocated but no
   longer accessable) when the function terminates.*/
enum Return_Code
create_grid (struct cell ****new_grid);

enum Return_Code
associate_groups (struct cell ****groups,
                  struct cell ***grid);

enum Return_Code
associate_cells (struct cell ***groups,
                 struct cell ***grid,
                 unsigned int max_groups_per_cell);

void
add_cell_to_group (struct cell ***group,
                   unsigned int group_pointer_index,
                   unsigned int cell_pointer_index,
                   struct cell *** grid,
                   unsigned int x,
                   unsigned int y);

enum Return_Code
delete_grid (struct cell ****terminal_grid,
             struct cell ****terminal_groups,
             unsigned int max_groups_per_cell);

#endif
