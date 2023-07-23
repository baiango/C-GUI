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
		SDL_Delay(15)
	;	// Poll
		SDL_PollEvent(&event)
	;	switch (event.type) {
		case SDL_QUIT: goto quit
		;}
		// Process
		GUI_TxtTexture clock
	;{	SDL_Color col = { 255 - (u8)(SDL_GetTicks() * 0.1), (u8)(SDL_GetTicks() / 3.0), 255 }
	;	i32 size = 96
	;	Vec2i ofst = { window_size.x / 2 - size / 2, window_size.y / 2 - size  / 2}
	;	Vec3i time = GUI_Expand1DTo3D(SDL_GetTicks(), 1000, 60)

	;	char *time_ms = GUI_u64ToStr(time.z), *time_s = GUI_u64ToStr(time.y), *time_m = GUI_u64ToStr(time.x)
	;	char *txt = GUI_Join(":", 3, time_ms, time_s, time_m)
	;	free(time_ms); free(time_s); free(time_m)

	;	clock = GUI_StrToTexture(renderer, txt, ofst, size, col, NO_PATH)
	;	free(txt)
	;}// Render: Texture
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
