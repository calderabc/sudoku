#ifndef SUDOKU_COMMON
#define SUDOKU_COMMON

#undef FALSE
#undef TRUE

typedef enum
{
  FALSE,
  TRUE
} boolean;



enum Return_Code
{
  SUCCESS,
  FAIL,
  FAIL_MALLOC,
  POINTER_NOT_NULL,
  POINTER_NULL,
  UNSOLVABLE,
  TERMINATE
};

#endif
