cd pdcurses
7za.exe x -y pdcurses.dll
cd ..
cmake.exe -G "MinGW Makefiles" .
make.exe
