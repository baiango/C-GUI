cd C:\Users\%username%\source\repos\C-TUI\ctui
del main.o
del glad.o
del main.exe
tcc -c main.c external/src/*.c -Iexternal/include
tcc main.o glad.o -Lexternal/lib -lglfw3
main
pause
