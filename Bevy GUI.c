#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
//#undef main

void func_to_show_mem_leak()
{	int* ptr = malloc(sizeof(int)); } // Found by cppcheck and intel inspector


int main()
{	printf("Hi!\n");
	func_to_show_mem_leak();
	int *ptr = malloc(sizeof(int));
	int **dangling = &ptr;
	free(*dangling);
	free(ptr);  // Found by intellisense and intel inspector
	free(ptr); // Found by cppcheck and intel inspector
}
