// It's more memory efficient than "Some libary here" one.
// And gives us more control without changing the "Some libary here" binding soruce code.
// These are designed to fit in L1 cache if storing in RAM is unavoidable.
package ODIN_GUI

import "core:fmt"
import "core:math"

Vec2i :: struct { x, y: i32 }
Vec2u :: struct { x, y: u32 }
Vec2f :: struct { x, y: f32 }
Vec3i :: struct { x, y, z: i32 }
Vec3u :: struct { x, y, z: u32 }
Vec3f :: struct { x, y, z: f32 }


Col8 :: struct { r, g, b: u8 }
BLACK_Col8 :: Col8 { 0, 0, 0 }
LIGHT_GRAY_Col8 :: Col8 { 0xe1, 0xe1, 0xe1 }
MED_GRAY_Col8 :: Col8 { 0x99, 0x99, 0x99 }
DARK_GRAY_Col8 :: Col8 { 0x20, 0x20, 0x20 }
// Use one of these color palette only to color a button.
DARK_BLUE_Col8 :: Col8 { 0x02, 0x3e, 0x8a }
LIGHT_BLUE_Col8 :: Col8 { 0xca, 0xf0, 0xf8 }

DARK_GREEN_Col8 :: Col8 { 0x3a, 0x5a, 0x40 }
LIGHT_GREEN_Col8 :: Col8 { 0xda, 0xd7, 0xcd }

Col5 :: struct { rgb: u16 }
set_col5 :: proc(r, g, b: u8) -> Col5 {
	return Col5{
		u16(min(31, r)) | u16(min(31, g)) << 5 | u16(min(31, b)) << 10
	}
}
raw_col5_to_8 :: proc(using col5: Col5) -> Col8 {
	return {
		u8(0b11111 & rgb)
		u8(0b11111 & (rgb / 32))
		u8(0b11111 & (rgb / 1024))
	}
}
col5_to_8 :: proc(using col5: Col5) -> Col8 {
	return {
		u8(math.floor_f32(min(31.0, f32(rgb))) * (255.0 / 31.0))
		u8(math.floor_f32(min(31.0, f32(rgb) / 32.0)) * (255.0 / 31.0))
		u8(math.floor_f32(min(31.0, f32(rgb) / 1024.0)) * (255.0 / 31.0))
	}
}

WdoRect :: struct { x, y, w, h: i16 }
mov_rect :: proc (mov: Vec2i, using pos: WdoRect) -> WdoRect {
	return {x + i16(mov.x), y + i16(mov.y), w, h }
}
RdRect :: struct { window_rect: WdoRect, roundness: f32 }
SHARP_RECT :: RdRect { {0, 0, 180, 48}, 0.0 }
RD4_RECT :: RdRect { {0, 0, 180, 48}, 0.08 }
RD8_RECT :: RdRect { {0, 0, 180, 48}, 0.16 }
RD16_RECT :: RdRect { {0, 0, 180, 48}, 0.32 }
PILL_RECT :: RdRect { {0, 0, 180, 48}, 1.0 }


TxtBox :: struct {
	box: RdRect
	button: Button
}
Button :: struct {
	txt: string
	Icon: Tex2D
}
Tex2D :: struct {
	pxs: []u8 // Variable sized array(Not Vector)
}

// SDL2 specific
import "core:c"
import SDL "vendor:sdl2"
import TTF "sdl2_ttf"

col8_to_SDL :: proc(using color: Col8) -> SDL.Color {
	return { r, g, b, 0xff }
}
col5_to_SDL :: proc(color: Col5) -> SDL.Color {
	return col8_to_SDL(col5_to_8(color))
}

get_txt_tex :: proc(
	txt: cstring = "Hello!"
	fnt_pt: cstring = "res/pixel-clear-condensed.ttf"
	pos: Vec2i = {32, 32}
	col: Col8 = {0xff, 0xff, 0xff}
	pen_sz: c.int = 24
	// Boilerplate
	r_er: ^SDL.Renderer
)	-> (^SDL.Texture, SDL.Rect) {
	ttf := TTF.OpenFont(fnt_pt, pen_sz)
	surface := TTF.RenderText_Solid(ttf, txt, col8_to_SDL(col))
	texture := SDL.CreateTextureFromSurface(r_er, surface)
	stretch := SDL.Rect{pos.x, pos.y, 0, 0}
	SDL.QueryTexture(texture, nil, nil, &stretch.w, &stretch.h)
	return texture, stretch
}

draw_tex :: proc(
	txt: cstring = "Hello!"
	fnt_pt: cstring = "res/pixel-clear-condensed.ttf"
	pos: Vec2i = {32, 32}
	col: Col8 = {0xff, 0xff, 0xff}
	pen_sz: c.int = 24
	// Boilerplate
	r_er: ^SDL.Renderer
)	{
	texture, stretch := get_txt_tex(txt, fnt_pt, pos, col, pen_sz, r_er)
	SDL.RenderCopy(r_er, texture, nil, &stretch)
}

place_middle :: proc(stretch, box: SDL.Rect) -> SDL.Rect {
	return {
		box.x - (stretch.w / 2) + (box.w / 2)
		// Need to move down 1 px to make it look "middle".
		box.y - (stretch.h / 2) + (box.h / 2) + 1
		stretch.w, stretch.h
	}
}

draw_circle :: proc(r_er: ^SDL.Renderer) {
	canva: SDL.Surface
	SDL.LockSurface(&canva)
	SDL.UnlockSurface(&canva)
}
