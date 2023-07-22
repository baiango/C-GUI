#pragma once
#include <stdlib.h>


typedef unsigned char u8; typedef unsigned short u16; typedef unsigned u32; typedef unsigned long long u64;
typedef char i8; typedef short i16; typedef int i32; typedef long long i64;
typedef float f32; typedef double f64; typedef long double f80;


typedef struct { f32 x, y; } Vec2f;
typedef struct { i32 x, y; } Vec2i;
typedef struct {
	SDL_Color text_color;
	SDL_Texture *text_texture;
	SDL_Rect text_stretch;
} TxtTexture;


const void *NO_PATH = NULL;
/* Example:
   TxtTexture text = str_to_texture(renderer, "WWW! my grass!", 24, (SDL_Color){ 0xff, 0xff, 0xff }, NO_PATH);
   SDL_DestroyTexture(text.text_texture);
   It will cause memory leak if you don't destroy the texture. */
TxtTexture str_to_texture(SDL_Renderer *renderer, char str[], int font_size, SDL_Color text_color, char font_path[])
{	TxtTexture ret
;{	TTF_Font *font
;	if (font_path == NULL)
	{	font = TTF_OpenFont("pixel-clear-condensed.ttf", font_size)
	;	if (font == NULL) { printf("Font not found! Panic!\n"); exit(-1); }
	}
	else
	{	font = TTF_OpenFont(font_path, font_size)
	;	if (font == NULL) printf("Font not found! Ignoring!\n");
	}

	SDL_Surface *text_surface = TTF_RenderText_Solid(font, str, text_color)
;	TTF_CloseFont(font)
;	ret.text_texture = SDL_CreateTextureFromSurface(renderer, text_surface)
;	SDL_FreeSurface(text_surface)
;
	Vec2i txtofst = { 10, 10 }
;	i32 txtw, txth
;	SDL_QueryTexture(ret.text_texture, NULL, NULL, &txtw, &txth)
;	ret.text_stretch = (SDL_Rect){ txtofst.x, txtofst.y, txtw, txth }
;}
	return ret
; }
