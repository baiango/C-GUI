cd C:\Users\%username%\source\repos\C-TUI\ctui
del main.o
del main.exe
tcc -c main.c -Iexternal/include -o main.o
tcc main.o -Lexternal/lib -lglfw3
main
pause
