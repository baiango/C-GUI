package ODIN_GUI

import "core:fmt"
import "core:strings"
import SDL "vendor:sdl2"
import TTF "sdl2_ttf"

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

	my_text: cstring = "My text!"

	event: SDL.Event
	quit: for {
		if SDL.PollEvent(&event) {
			if event.type == SDL.EventType.QUIT do break quit
			else if event.type == SDL.EventType.KEYDOWN {
				#partial switch event.key.keysym.sym {
				case SDL.Keycode.UP: my_text = "My text!"
				case SDL.Keycode.DOWN: my_text = "Another text!"
				}
			}
		}
		// Render: Start
{		SDL.SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0)
		box := SDL.Rect{10, 10, 180, 48}
		SDL.RenderFillRect(renderer, &box)
		tex, stch := get_txt_tex(txt=my_text, r_er=renderer)
		middle_stch := place_middle(stch, box)
		SDL.RenderCopy(renderer, tex, nil, &middle_stch)

		// Reactive programming
		// It stores the pointer in a variable, then add it together.
		// Note this doesn't work yet.
		// b := 1
		// c := 2
		// a := &b + &c
		// b = 10
		// fmt.println(a) // Should be 12 instead of 3.
		concat_txt := " More text!"
		my_text2 := []^string{(^string)(&my_text), &concat_txt}
		my_text_ptr := &my_text
		tex2, stch2 := get_txt_tex_ptr(
			txt=(^cstring)(my_text2[0])
			r_er=renderer
		)
		mov_stch := place_middle(stch2, box)
		mov_stch = SDL_mov_rect({100, 100}, mov_stch)
		SDL.RenderCopy(renderer, tex2, nil, &mov_stch)

		// I don't know why this doesn't work.
		// aaa := []string{my_text2[0]^, concat_txt}
		aaa := []string{string(my_text), concat_txt}
		my_text3 := strings.concatenate(
			aaa
		)
		tex3, stch3 := get_txt_tex_ptr(
			txt=(^cstring)(&my_text3)
			r_er=renderer
		)
		mov_stch2 := SDL_mov_rect({100, 200}, stch3)
		SDL.RenderCopy(renderer, tex3, nil, &mov_stch2)
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
