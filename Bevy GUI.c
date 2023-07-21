#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>
#include <sdl2/SDL_ttf.h>
#include "types.h"


int main()
{	printf("Hi!\n");
	TTF_Init();
	SDL_Window *window;
	SDL_Renderer *renderer;
{	Vec2i window_size = { 800, 600 };
	// Memory leak. Don't call SDL_CreateWindowAndRenderer() more than 1 time.
	SDL_CreateWindowAndRenderer(window_size.x, window_size.y, SDL_WINDOW_OPENGL, &window, &renderer); }


	TxtTexture text;
{	SDL_Color col = { 0xff, 0xff, 0xff };
	text = str_to_texture(renderer, "WWW! my grass!", 24, col, NO_PATH); }


	SDL_Event event;
	while (1)
	{	// Poll
		SDL_PollEvent(&event);
		switch (event.type)
		{case SDL_QUIT: goto quit; }
		// Render: Clear
		SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xff);
		SDL_RenderClear(renderer);
		// Render
		SDL_RenderCopy(renderer, text.text_texture, NULL, &text.text_stretch);
		SDL_RenderPresent(renderer); }

quit:
	// Free
	SDL_DestroyTexture(text.text_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	printf("Quitting!\n"); }
