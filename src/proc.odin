package ODIN_GUI

import "core:fmt"
import SDL "vendor:sdl2"

check_err :: proc(_: ..any) {
	err := SDL.GetErrorString()
	if "" != err do fmt.println(err)
}
