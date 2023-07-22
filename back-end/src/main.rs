use std::time::Duration;
use sdl2::{event::Event, pixels::Color, keyboard::Keycode};


fn main()
{	println!("Hi!\n")
;	let sdl_context = sdl2::init().unwrap()
;	let video_subsystem = sdl_context.video().unwrap()
;
	let window = video_subsystem.window("rust-sdl2 demo", 800, 600)
		.build()
		.unwrap()
;
	let mut canvas = window.into_canvas().build().unwrap()
;
	let mut event_pump = sdl_context.event_pump().unwrap()
;	let mut i = 0
;	'running: loop
	{	i = (i + 1) % 255
	;	canvas.set_draw_color(Color::RGB(i, 64, 255 - i))
	;	canvas.clear()
	;	for event in event_pump.poll_iter()
		{	match event
			{Event::Quit {..} |
			Event::KeyDown { keycode: Some(Keycode::Escape), .. } => { break 'running },
			_ => {}}}

		canvas.present()
;		std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60))
; }}
