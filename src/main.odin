package ODIN_GUI

import "core:fmt"
import SDL "vendor:sdl2"
import TTF "sdl2_ttf"
import GLFW "vendor:vulkan"

main :: proc() {
	sdl()
}


sdl :: proc() {
	window_size := Vec2i{640, 480}
	// VIDEO
	check_err(SDL.Init(SDL.INIT_VIDEO))
	defer SDL.Quit()

	window := SDL.CreateWindow(
		"ODIN-GUI", SDL.WINDOWPOS_CENTERED, SDL.WINDOWPOS_CENTERED
		window_size.x, window_size.y
		SDL.WINDOW_RESIZABLE
	)
	check_err()
	defer SDL.DestroyWindow(window)

	renderer := SDL.CreateRenderer(window, -1, SDL.RENDERER_ACCELERATED)
	check_err()
	defer SDL.DestroyRenderer(renderer)

	// TFF
	TTF.Init()
	check_err()

	event: SDL.Event
	quit: for {
		if SDL.PollEvent(&event) {
			#partial switch event.type {
			case SDL.EventType.QUIT: break quit
			}
		} // if SDL.PollEvent(&event)
		// Render: Start
{		SDL.SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0)
		box := SDL.Rect{10, 10, 180, 48}
		SDL.RenderFillRect(renderer, &box)
		tex, stch := get_txt_tex(r_er=renderer)
		stch = place_middle(stch, box)
		SDL.RenderCopy(renderer, tex, nil, &stch)
		}

		draw_tex(pos={100, 200}, r_er=renderer)

		draw_tex(txt="Here's some text!"
			pos={32, window_size.y - 32 - 32}
			fnt_pt="res/JetBrainsMono-Light.ttf"
			pen_sz=32, r_er=renderer
		)

		draw_circle(renderer)
		// Render: End
		SDL.RenderPresent(renderer)
		SDL.SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0)
		SDL.RenderClear(renderer)
	}
	mycol := set_col5(32, 31, 30)
	fmt.println(col5_to_SDL(mycol))
	fmt.println(raw_col5_to_8(mycol))
}
