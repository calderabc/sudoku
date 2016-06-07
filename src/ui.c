#include "ui.h"
#include "curses_ui.h"


void
init_screen (enum graphical_mode mode, unsigned int group_size)
{
  if (mode == NCURSES)
    {
      init_screen_ptr = &curses_init_screen;
      draw_board_ptr = &curses_draw_board;
      draw_screen_ptr = &curses_draw_screen;
      terminate_board_ptr = &curses_terminate_board;
      draw_cell_ptr = &curses_draw_cell;
      move_cursor_ptr = &curses_move_cursor;
      get_keypress_ptr = &curses_get_keypress;
      hide_cursor_ptr = &curses_hide_cursor;
      show_cursor_ptr = &curses_show_cursor;
    }

  (*init_screen_ptr) (group_size);
}
