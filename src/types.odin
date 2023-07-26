// It's more memory efficient than Raylib one.
// And gives us more control without changing the Raylib binding soruce code.
package ODIN_GUI

import "core:fmt"
import rl "raylib"

Vec2i :: struct { x, y: i32 }
Vec2u :: struct { x, y: u32 }
Vec2f :: struct { x, y: f32 }
Vec3i :: struct { x, y, z: i32 }
Vec3u :: struct { x, y, z: u32 }
Vec3f :: struct { x, y, z: f32 }


Col8 :: struct { r, g, b, a: u8,  }
BLACK_Col8 :: Col8 { 0, 0, 0, 255 }
LIGHT_GRAY_Col8 :: Col8 { 0xe1, 0xe1, 0xe1, 0xff }
MED_GRAY_Col8 :: Col8 { 0x99, 0x99, 0x99, 0xff }
DARK_GRAY_Col8 :: Col8 { 0x20, 0x20, 0x20, 0xff }
// Use one of these color palette only to color a button.
DARK_BLUE_Col8 :: Col8 { 0x02, 0x3e, 0x8a, 0xff }
LIGHT_BLUE_Col8 :: Col8 { 0xca, 0xf0, 0xf8, 0xff }

DARK_GREEN_Col8 :: Col8 { 0x3a, 0x5a, 0x40, 0xff }
LIGHT_GREEN_Col8 :: Col8 { 0xda, 0xd7, 0xcd, 0xff }
col8_to_color :: proc (using col: Col8) -> rl.Color {
	return {r, g, b, a}
}


Rect :: struct { x, y, w, h: i16 }
mov_rect :: proc (mov: Vec2i, using rect: Rect) -> Rect {
	return Rect{x + i16(mov.x), y + i16(mov.y), w, h }
}
rect_to_rlrect :: proc (using rect: Rect) -> rl.Rectangle {
	return { f32(x), f32(y), f32(w), f32(h) }
}
RdRect :: struct { rect: Rect, roundness: f32 }
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
	Txt: string
	Icon: Texture2D
}
Texture2D :: struct {
	pixels: []u8
}
DEFAULT_FONT := rl.LoadFontData("pixel-clear-condensed.ttf",)
draw_txt_box :: proc(using position: Vec2i) {
;	box := RD16_RECT
;	rl.DrawRectangleRounded(
	rect_to_rlrect(mov_rect(position, box.rect)),
	box.roundness, 0, col8_to_color(DARK_BLUE_Col8)
	)
	// Font
;	str: cstring = "New button 123"
;	sz: i32 = 24
	// This is very hard to use because it has bad abstractions.
// ;	ofst := rl.MeasureTextEx(DEFAULT_FONT, str, f32(sz), 1.0)
;	rl.DrawText(
			str,
			x + i32((box.rect.w) / 2), y,
			sz, col8_to_color(LIGHT_BLUE_Col8)
	)
}
// Trying to make builder pattern.
// Currently to chain a function look like this:
// d(c(b(a(input))))
// Builder pattern:
// a(input)
//   .b().c()
//   .d()
// Which is way easier to read.
col8_pipe :: proc() {
	
}
