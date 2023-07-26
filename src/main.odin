package ODIN_GUI

import "core:c"
import "core:fmt"
import SDL "vendor:sdl2"
import TTF "sdl2_ttf"

main :: proc() {
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
			#partial switch event.type
			{case SDL.EventType.QUIT: break quit
			}
		} // if SDL.PollEvent(&event)
		// Render: Start
		draw_txt(pos={100, 200}, r_er=renderer)
		draw_txt(txt="Here's some text!"
			pos={32, window_size.y - 96 - 32}
			pen_sz=96, r_er=renderer
		)
		// Render: End
		SDL.RenderPresent(renderer)
		SDL.SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0)
		SDL.RenderClear(renderer)
	}
}
