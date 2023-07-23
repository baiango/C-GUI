#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>
#include <sdl2/SDL_ttf.h>
#include "types.hpp"
#include <iostream>
using std::cout;


int main() {
	printf("Hi!\n")
;	TTF_Init()
;	SDL_Window *window
;	SDL_Renderer *renderer
;	Vec2i window_size = { 800, 600 }
;	SDL_CreateWindowAndRenderer(window_size.x, window_size.y,
			SDL_WINDOW_OPENGL, &window, &renderer)
;

	GUI_TxtTexture text
;{	SDL_Color col = { 0xff, 0xff, 0xff }
;	Vec2i ofst = { 10, 10 }
;	text = GUI_StrToTexture(renderer, "WWW! my grass!", ofst, 24, col, NO_PATH)
;}

{	i32 a = 10
;	array<GUI_Var, 2> b = {&a, 1}
;	a = 20
;	cout << *get<i32*>(b[0]) + get<i32>(b[1]) << "\n" // 21
//;	cout << get<i32>(b[0]) + get<i32>(b[1]) // Panic!
;}

;	SDL_Event event
;	while (1) {
		SDL_Delay(15)
	;	// Poll
		SDL_PollEvent(&event)
	;	switch (event.type) {
		case SDL_QUIT: goto quit
		;}
		// Process
		GUI_TxtTexture clock
	;{	SDL_Color col = { (u8)(255 - SDL_GetTicks() * 0.1), (u8)(SDL_GetTicks() / 3.0), 255 }
	;	i32 size = 96
	;	Vec2i ofst = { window_size.x / 2 - size / 2, window_size.y / 2 - size  / 2}
	;	Vec3i time = GUI_Expand1DTo3D(SDL_GetTicks(), 1000, 60)

	;	string time_ms = to_string(time.z), time_s = to_string(time.y), time_m = to_string(time.x)
	;	string txt = GUI_Join(":", { time_ms, time_s, time_m })
	;
		clock = GUI_StrToTexture(renderer, txt, ofst, size, col, NO_PATH)
	;}	// Render: Texture
		SDL_RenderCopy(renderer, text.text_texture, NULL, &text.text_stretch)
	;	SDL_RenderCopy(renderer, clock.text_texture, NULL, &clock.text_stretch)
	;	SDL_RenderPresent(renderer)
	;	// Render: Clear
		SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xff)
	;	SDL_RenderClear(renderer)
	;	SDL_DestroyTexture(clock.text_texture)
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
