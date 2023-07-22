#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>
#include <sdl2/SDL_ttf.h>
#include "types.h"


int main() {
	printf("Hi!\n")
;	TTF_Init()
;	SDL_Window *window
;	SDL_Renderer *renderer
;{Vec2i window_size = { 800, 600 }
;	// Memory leak. Don't call SDL_CreateWindowAndRenderer() more than 1 time.
	SDL_CreateWindowAndRenderer(window_size.x, window_size.y,
			SDL_WINDOW_OPENGL, &window, &renderer)
;}

	GUI_TxtTexture text
;{SDL_Color col = { 0xff, 0xff, 0xff }
;	text = GUI_StrToTexture(renderer, "WWW! my grass!", 24, col, (char *)NO_PATH)
;}

{	i32 a = 10
	#pragma warning(push)
	#pragma warning(disable: 4047)
;	GUI_Var b[2] = {{&a, i32_ptr }, {1, i32_type}}
	#pragma warning(pop)
;	a = 20
;	printf("%i\n", *GUI_Geti32p(b[0]) + GUI_Geti32t(b[1])) // 21
//;	printf("%i\n", GUI_Geti32t(b[0]) + GUI_Geti32t(b[1])) // Panic!
;}

;	SDL_Event event
;	while (1) {
		// Poll
		SDL_PollEvent(&event)
	;	switch (event.type) {
		case SDL_QUIT: goto quit
		;}
		// Render: Clear
		SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xff)
	;	SDL_RenderClear(renderer)
	;	// Render
		SDL_RenderCopy(renderer, text.text_texture, NULL, &text.text_stretch)
	;	SDL_RenderPresent(renderer)
	;}

quit:
	// Free
	SDL_DestroyTexture(text.text_texture)
;	SDL_DestroyRenderer(renderer)
;	SDL_DestroyWindow(window)
;	TTF_Quit()
;	SDL_Quit()
;	printf("Quitting!\n")
; }
