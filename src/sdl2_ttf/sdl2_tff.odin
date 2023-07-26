package sdl2_tff

import "core:c"
import SDL "vendor:sdl2"

// https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2
// If it doesn't run,
// please check the if compiler can reach SDL2_ttf.lib
when ODIN_OS == .Windows {
	foreign import lib "SDL2_ttf.lib"
} else {
	foreign import lib "system:SDL2_ttf" // Not tested
}

Font :: struct {}

@(default_calling_convention="c", link_prefix="TTF_")
foreign lib {
	Init :: proc() ---
	OpenFont :: proc(path: cstring, pen_size: c.int) -> ^Font ---
	RenderText_Solid :: proc(font: ^Font, text: cstring, color: SDL.Color) -> ^SDL.Surface ---
}
