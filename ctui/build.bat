cd C:\Users\%username%\source\repos\C-TUI\ctui
del main.o
del glad.o
del main.exe
tcc -c main.c external/source/*.c src/gllib/*.c -Iexternal/include
tcc main.o glad.o glmath.o gltypes.o -Lexternal/lib -lglfw3
main
pause
