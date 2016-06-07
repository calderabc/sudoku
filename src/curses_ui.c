
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <ctype.h>

#include "curses_ui.h"
#include "ui.h"
#include "common.h"

static unsigned int cell_width = 4;
static unsigned int cell_height = 2;

static WINDOW *board;
static unsigned int max_x;
static unsigned int max_y;

static unsigned int screen_max_x;
static unsigned int screen_max_y;

static unsigned int group_size_sqrt;
static unsigned int inner_square_width;
static unsigned int inner_square_height;

static boolean is_unicode = TRUE;
static boolean is_large_board = TRUE;

void
curses_init_screen (unsigned int group_size)
{
  /* Set the character encoding.  */
  /* TODO: Make ncurses detect the encoding of the current terminal
     then change the output accordingly.  */
  setlocale(LC_ALL, "en_US.UTF-8");


  printf("%s\n", setlocale(LC_CTYPE, NULL));
  /* Initialize the ncurses screen.  */
  initscr ();
  /* Don't echo keyboard input on screen.  */
  noecho ();
  /* Take input as typed instead of waiting for return to enter line.  */
  cbreak();


  /* These values shouldn't be set anywhere else.
   * If they are set elsewhere it will probably cause a logic error..  */
  max_x = group_size * cell_width;
  max_y = group_size * cell_height;
  group_size_sqrt = (unsigned int) sqrt((double) group_size);
  inner_square_width = group_size_sqrt * cell_width;
  inner_square_height = group_size_sqrt * cell_height;
  board = newwin (max_y + 1, max_x + 1, 0, 0);

  getmaxyx(stdscr, screen_max_y, screen_max_x);

  if (screen_max_x < max_x || screen_max_y < max_y)
    is_large_board = FALSE;

  /* No-delay to TRUE so wgetch() won't stop and wait
     for keyboard input.  */
  nodelay(board, TRUE);

  /* keypad(board, TRUE);  */

}

void
curses_draw_cell (int x, int y, int value)
{
  curses_move_cursor (x, y);
  if (value == 0)
    wprintw (board, " ");
  else
    wprintw (board, "%i", value);
}

void
curses_move_cursor (int x, int y)
{
  wmove (board, y * cell_height + cell_height / 2,
         x * cell_width + cell_width / 2);
}


enum input_code
curses_get_keypress()
{
  int keypress;
  enum input_code input = UNKNOWN;

  keypress = wgetch (board);

  switch (keypress)
    {
    case 201:
    case 'k':
    case 'K':
      input = UP;
      break;
    case 'j':
    case 'J':
      input = DOWN;
      break;
    case 191:
    case 'h':
    case 'H':
      input = LEFT;
      break;
    case 171:
    case 'l':
    case 'L':
      input = RIGHT;
      break;
    case 'q':
    case 'Q':
      input = QUIT;
      break;
    case ERR:
      input = NONE;
    default:
      if (isdigit(keypress))
        {
          input = (enum input_code) (keypress - '0');
        }
    }

  return input;
}

/* Explicit values in case I add values I won't break other code.  */
enum line_type
{
  BLANK = 0,
  SMALL = 1,
  LARGE = 2,
  DOUBLE = 3
};

struct character
{
  enum line_type left;
  enum line_type right;
  enum line_type top;
  enum line_type bottom;

  char ascii;

  wchar_t *wide_char;
  cchar_t complex_char[2];
  boolean is_complex_set;
};

void
set_character(struct character *which)
{
  attr_t attributes;
  short pair;

  int test = (which->left << 12
              | which->right << 8
              | which->top << 4
              | which->bottom);

  /* Universal character names L"\x2502" are only valid in C99 and C++
     I changed to L"\x2502" form to get rid of C90 compiler warning.  */

  switch (test)
    {
    case 0x0011 :
      which->wide_char = L"\x2502";
      break;
    case 0x0012 :
      which->wide_char = L"\x257F";
      break;
    case 0x0021 :
      which->wide_char = L"\x257D";
      break;
    case 0x0022 :
      which->wide_char = L"\x2503";
      break;
    case 0x0033 :
      which->wide_char = L"\x2551";
      break;
    case 0x0133 :
    case 0x0233 :
      which->wide_char = L"\x255F";
      break;
    case 0x0303 :
      which->wide_char = L"\x2554";
      break;
    case 0x0330 :
      which->wide_char = L"\x255A";
      break;
    case 0x0333 :
      which->wide_char = L"\x2560";
      break;
    case 0x1033 :
    case 0x2033 :
      which->wide_char = L"\x2562";
      break;
    case 0x1100 :
      which->wide_char = L"\x2500";
      break;
    case 0x1122 :
      which->wide_char = L"\x2542";
      break;
    case 0x1111 :
      which->wide_char = L"\x253C";
      break;
    case 0x1133 :
      which->wide_char = L"\x256B";
      break;
    case 0x1200 :
      which->wide_char = L"\x257C";
      break;
    case 0x2100 :
      which->wide_char = L"\x257E";
      break;
    case 0x2200 :
      which->wide_char = L"\x2501";
      break;
    case 0x2211 :
      which->wide_char = L"\x253F";
      break;
    case 0x2222 :
      which->wide_char = L"\x254B";
      break;
    case 0x3003 :
      which->wide_char = L"\x2557";
      break;
    case 0x3030 :
      which->wide_char = L"\x255D";
      break;
    case 0x3033 :
      which->wide_char = L"\x2563";
      break;
    case 0x3300 :
      which->wide_char = L"\x2550";
      break;
    case 0x3301 :
    case 0x3302 :
      which->wide_char = L"\x2564";
      break;
    case 0x3303 :
      which->wide_char = L"\x2566";
      break;
    case 0x3310 :
    case 0x3320 :
      which->wide_char = L"\x2567";
      break;
    case 0x3311 :
      which->wide_char = L"\x256A";
      break;
    case 0x3330 :
      which->wide_char = L"\x2569";
      break;
    case 0x3333 :
      which->wide_char = L"\x256C";
      break;
    default :
      which->wide_char = NULL;
      which->is_complex_set = FALSE;
    }

  switch (test)
    {
    case 0x0011 :
    case 0x0012 :
    case 0x0021 :
      which->ascii = (char) 0xB3;
      /*which->ascii = ACS_ULCORNER; */
      /* waddch(board, which->ascii;
      getch();
      endwin ();
      exit(1); */
      break;
    case 0x0022 :
    case 0x0033 :
      which->ascii = (char) 0xBA;
      break;
    case 0x0133 :
      which->ascii = (char) 0xC7;
      break;
    case 0x0233 :
    case 0x0333 :
      which->ascii = (char) 0xCC;
      break;
    case 0x0303 :
      which->ascii = (char) 0xC9;
      break;
    case 0x0330 :
      which->ascii = (char) 0xC8;
      break;
    case 0x1033 :
      which->ascii = (char) 0xB6;
      break;
    case 0x1100 :
    case 0x1200 :
    case 0x2100 :
      which->ascii = (char) 0xC4;
      break;
    case 0x1122 :
    case 0x1133 :
      which->ascii = (char) 0xD7;
      break;
    case 0x1111 :
      which->ascii = (char) 0xC5;
      break;
    case 0x2033 :
    case 0x3033 :
      which->ascii = (char) 0xB9;
      break;
    case 0x2200 :
    case 0x3300 :
      which->ascii = (char) 0xCD;
      break;
    case 0x2211 :
    case 0x3311 :
      which->ascii = (char) 0xD8;
      break;
    case 0x2222 :
    case 0x3333 :
      which->ascii = (char) 0xCE;
      break;
    case 0x3003 :
      which->ascii = (char) 0xBB;
      break;
    case 0x3030 :
      which->ascii = (char) 0xBC;
      break;
    case 0x3301 :
      which->ascii = (char) 0xD1;
    case 0x3302 :
    case 0x3303 :
      which->ascii = (char) 0xCB;
      break;
    case 0x3310 :
      which->ascii = (char) 0xCF;
    case 0x3320 :
    case 0x3330 :
      which->ascii = (char) 0xCA;
      break;
    default :
      which->ascii = 0x0;
      which->is_complex_set = FALSE;
    }

  wattr_get(board, &attributes, &pair, NULL);

  if (which->wide_char != NULL)
    {
      setcchar(which->complex_char, which->wide_char, attributes, pair, NULL);

	  which->is_complex_set = TRUE;
    }
}


void
curses_draw_board (unsigned int group_size)
{
  unsigned int x, y;

  /* Variable length array not allowed in ISO C90
  struct character display_grid[max_x][max_y];
  */

  struct character *display_grid =
    (struct character *) calloc ((max_y + 1) * (max_x + 1), sizeof (struct character));


  struct character *character_pointer = display_grid;
  for (x = 0; x <= max_x; x++)
    {
      for (y = 0; y <= max_y; y++, character_pointer++)
        {



          if (x == 0 || x == max_x)
            {
              if (y != 0)
                character_pointer->top = DOUBLE;
              if (y != max_y)
                character_pointer->bottom = DOUBLE;
            }
          else if (x % inner_square_width == 0)
            {
              if (y != 0)
                  character_pointer->top = LARGE;

              if (y != max_y)
                  character_pointer->bottom = LARGE;

            }
          else if (x % cell_width == 0)
            {
              if (y != 0)
                character_pointer->top = SMALL;

              if (y != max_y)
                character_pointer->bottom = SMALL;
            }

          if (y == 0 || y == max_y)
            {
              if (x != 0)
                character_pointer->left = DOUBLE;
              if (x != max_x)
                character_pointer->right = DOUBLE;
            }
          else if (y % inner_square_height == 0)
            {
              if (x != 0)
                character_pointer->left = LARGE;

              if (x != max_x)
                character_pointer->right = LARGE;
            }
          else if (y % cell_height == 0)
            {
              if (x != 0)
                character_pointer->left = SMALL;

              if (x != max_x)
                character_pointer->right = SMALL;
            }

          set_character (character_pointer);

          if (character_pointer->is_complex_set == TRUE)
            {
              if (is_unicode == TRUE)
			    {
                  mvwadd_wch (board, y, x, character_pointer->complex_char);
			    }
			  else
			    {
                  mvwaddch (board, y, x, (char) 206);
                  waddch(board, ACS_ULCORNER);
			    }
            }
        }
    }

  free(display_grid);

  curses_draw_screen();
}


void
curses_draw_screen()
{
  refresh ();
  wrefresh (board);
}


void
curses_terminate_board ()
{
  endwin ();                       /* End curses mode.  */
}

void
curses_hide_cursor ()
{
  curs_set(0);
}

void
curses_show_cursor ()
{
  curs_set(1);
}

