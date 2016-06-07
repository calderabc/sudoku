cd pdcurses
7za.exe x -y win32a.zip
cd win32a
nmake.exe -f vcwin32.mak WIDE=1
cd ..\..
cmake.exe .
