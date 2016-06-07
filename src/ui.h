#ifndef SUDOKU_UI
#define SUDOKU_UI

enum graphical_mode
{
  NCURSES,
  GTK
};


/* The digits have to stay in 0-9 order and come before any other values in
   the enum.  They are meant to correspond to the same integer value.  */
enum input_code
{
  ZERO = 0,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  LEFT,
  RIGHT,
  UP,
  DOWN,
  QUIT,
  UNKNOWN,
  NONE
};


#define draw_board(x) (*draw_board_ptr) (x)
#define draw_screen() (*draw_screen_ptr) ()
#define terminate_board() (*terminate_board_ptr) ()
#define draw_cell(x, y, value) (*draw_cell_ptr) (x, y, value)
#define move_cursor(x, y) (*move_cursor_ptr) (x, y)
#define get_keypress() (*get_keypress_ptr) ()
#define hide_cursor() (*hide_cursor_ptr) ()
#define show_cursor() (*show_cursor_ptr) ()

void
init_screen (enum graphical_mode mode,
             unsigned int group_size);

void (*init_screen_ptr) (unsigned int group_size);
void (*draw_board_ptr) (unsigned int group_size);
void (*draw_cell_ptr) (int x, int y, int value);
void (*move_cursor_ptr) (int x, int y);
void (*terminate_board_ptr) ();
void (*draw_screen_ptr) ();
enum input_code (*get_keypress_ptr) ();
void (*hide_cursor_ptr) ();
void (*show_cursor_ptr) ();
enum input_code (*get_keypress_ptr)();


#endif
