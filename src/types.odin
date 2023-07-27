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
		u8(0b11111 & (rgb >> 5))
		u8(0b11111 & (rgb >> 10))
	}
}
col5_to_8 :: proc(using col5: Col5) -> Col8 {
	return {
		// min(31, rgb) * (255.0 / 31)
		// min(31, rgb / 32) * (255.0 / 31)
		// min(31, rgb / 1024) * (255.0 / 31)
		u8(f32(0b11111 & rgb) * 8.225)
		u8(f32(0b11111 & (rgb >> 5)) * 8.225)
		u8(f32(0b11111 & (rgb >> 10)) * 8.225)
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

// Vulkan specific
GLFW_NO_API :: 0
GLFW_RESIZABLE :: 0x00020003
GLFW_FALSE :: 0
