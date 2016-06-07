#ifndef CURSES_FILE_IO
#define CURSES_FILE_IO

#include <stdio.h>

struct cell ***
read_grid_from_file (struct cell ****write_grid, 
                     const char *file_name, 
					 unsigned int group_size);

#endif
