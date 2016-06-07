#ifndef CURSES_RENDER
#define CURSES_RENDER

#define _XOPEN_SOURCE_EXTENDED

#include <stddef.h>

#ifdef WIN32
#define PDC_WIDE
#include <curses.h>
#endif

#ifdef __CYGWIN__
#include <curses.h>
#endif


#ifdef __GNUC__
#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/curses.h>
/* #include <curses.h> */
#endif

#include "ui.h"


void curses_init_screen (unsigned int group_size);
void curses_draw_board (unsigned int group_size);
void curses_draw_screen ();
void curses_terminate_board ();
void curses_draw_cell (int x, int y, int value);
void curses_move_cursor (int x, int y);
enum input_code curses_get_keypress ();
void curses_hide_cursor ();
void curses_show_cursor ();

#endif
