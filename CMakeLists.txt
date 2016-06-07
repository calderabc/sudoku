cmake_minimum_required (VERSION 2.8.4)

project (sudoku C)

file (GLOB source_files src/*.c)

add_executable (sudoku ${source_files})

if (WIN32 AND NOT CYGWIN)
  include_directories (pdcurses)

  foreach (flag_variable
           CMAKE_C_FLAGS
           CMAKE_C_FLAGS_DEBUG
           CMAKE_C_FLAGS_RELEASE
           CMAKE_CXX_FLAGS
           CMAKE_CXX_FLAGS_DEBUG
           CMAKE_CXX_FLAGS_RELEASE
          )
    if (${flag_variable} MATCHES "/MD")
      string (REGEX REPLACE "/MD" "/MT"
            ${flag_variable} "${${flag_variable}}")
    endif (${flag_variable} MATCHES "/MD")
  endforeach (flag_variable)

  target_link_libraries (sudoku pdcurses/win32a/pdcurses)
else (WIN32 AND NOT CYGWIN)
  #  if (MINGW)
  #  message(STATUS ${INCLUDE})
  #
  #  set (CMAKE_C_COMPILER C:/MinGW/bin/gcc)
  #  include_directories (pdcurses)
  #  target_link_libraries (sudoku pdcurses/pdcurses)
  #else (MINGW)


    file (MAKE_DIRECTORY "build")
    file (COPY "file.sdk" DESTINATION "build")

    if (CYGWIN)
      set (CURSES_USE_NCURSES TRUE)
      set (CURSES_USE_WIDE TRUE)
      find_package (Curses REQUIRED)

      set (CURSES_INCLUDE_DIRS "/usr/include/ncursesw")
    endif (CYGWIN)


    message(STATUS ${CURSES_INCLUDE_DIRS})
    message(STATUS ${CURSES_LIBRARIES})

    set (EXECUTABLE_OUTPUT_PATH "build")
    include_directories (${CURSES_INCLUDE_DIRS})

    target_link_libraries(sudoku m)
    target_link_libraries(sudoku ncursesw)
    #endif (MINGW)
endif (WIN32 AND NOT CYGWIN)

#message(STATUS ${INCLUDE})

