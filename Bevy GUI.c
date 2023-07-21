#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>
#include <sdl2/SDL_ttf.h>
#include "types.h"


i32 main()
{	printf("Hi!\n");
	TTF_Init();
	SDL_Window *window;
	SDL_Renderer *renderer;
{	Vec2i window_size = { 800, 600 };
	SDL_CreateWindowAndRenderer(window_size.x, window_size.y, SDL_WINDOW_OPENGL, &window, &renderer); }


	SDL_Texture* text_texture;
	SDL_Rect text_scale;
{	SDL_Color text_color = { 0xff, 0xff, 0xff };
	TTF_Font *font = TTF_OpenFont("pixel-clear-condensed.ttf", 24);
	if (font == NULL) { printf("Font not found!\n"); goto error; }

	SDL_Surface *text_surface = TTF_RenderText_Solid(font, "WWW! my grass!", text_color);
	TTF_CloseFont(font);

	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);

	Vec2i txtofst = { 10, 10 };
	i32 txtw, txth;
	SDL_QueryTexture(text_texture, NULL, NULL, &txtw, &txth);
	text_scale = (SDL_Rect){ txtofst.x, txtofst.y, txtw, txth }; }

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
		SDL_RenderCopy(renderer, text_texture, NULL, &text_scale);
		SDL_RenderPresent(renderer); }

	// Free
	SDL_DestroyTexture(text_texture);

error:
	return -1;
quit:
	printf("Quitting!\n"); }
