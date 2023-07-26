// It's more memory efficient than "Some libary here" one.
// And gives us more control without changing the "Some libary here" binding soruce code.
package ODIN_GUI

import "core:fmt"

Vec2i :: struct { x, y: i32 }
Vec2u :: struct { x, y: u32 }
Vec2f :: struct { x, y: f32 }
Vec3i :: struct { x, y, z: i32 }
Vec3u :: struct { x, y, z: u32 }
Vec3f :: struct { x, y, z: f32 }


Col8 :: struct { r, g, b, a: u8 }
BLACK_Col8 :: Col8 { 0, 0, 0, 255 }
LIGHT_GRAY_Col8 :: Col8 { 0xe1, 0xe1, 0xe1, 0xff }
MED_GRAY_Col8 :: Col8 { 0x99, 0x99, 0x99, 0xff }
DARK_GRAY_Col8 :: Col8 { 0x20, 0x20, 0x20, 0xff }
// Use one of these color palette only to color a button.
DARK_BLUE_Col8 :: Col8 { 0x02, 0x3e, 0x8a, 0xff }
LIGHT_BLUE_Col8 :: Col8 { 0xca, 0xf0, 0xf8, 0xff }

DARK_GREEN_Col8 :: Col8 { 0x3a, 0x5a, 0x40, 0xff }
LIGHT_GREEN_Col8 :: Col8 { 0xda, 0xd7, 0xcd, 0xff }


WdoRect :: struct { x, y, w, h: i16 }
mov_rect :: proc (move: Vec2i, using window_rect: WdoRect) -> WdoRect {
	return {x + i16(move.x), y + i16(move.y), w, h }
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

col8_to_SDL :: proc(using color: Col8) -> SDL.Color { return { r, g, b, a } }

draw_txt :: proc(
	txt: cstring = "Hello!"
	fnt_pt: cstring = "pixel-clear-condensed.ttf"
	pos: Vec2i = {32, 32}
	col: Col8={0xff, 0xff, 0xff, 0xff}
	pen_sz: c.int =24
	// Boilerplate
	r_er: ^SDL.Renderer
)	{
	ttf := TTF.OpenFont(fnt_pt, pen_sz)
	surface := TTF.RenderText_Solid(ttf, txt, col8_to_SDL(col))
	texture := SDL.CreateTextureFromSurface(r_er, surface)
	using pos
	stretch := SDL.Rect{x, y, 0, 0}
	SDL.QueryTexture(texture, nil, nil, &stretch.w, &stretch.h)
	SDL.RenderCopy(r_er, texture, nil, &stretch)
}
