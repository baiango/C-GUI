use string_join::Join;
use sdl2::{pixels::Color, event::Event, ttf, rect::Rect, render::TextureQuery};

mod ui {
	pub mod sdl2_types;
}
use crate::ui::sdl2_types::*;

mod engine {
	pub mod types;
}
use crate::engine::types::*;


fn main() {
	println!("Hi!\n")
;	let sdl = sdl2::init().unwrap()
;
	// Window
	let video = sdl.video().unwrap()
;	let window = video.window("rust-sdl2 demo", 1366, 768).position_centered().build().unwrap()
;	let mut canvas = window.into_canvas().build().unwrap()
;	let mktex = canvas.texture_creator()
;
	// Time
	let time = sdl.timer().unwrap()
;
	// Ttf
	let ttf = ttf::init().unwrap()
;
	let mut event = sdl.event_pump().unwrap()
; 'quit: loop {
		if let Some(e) = event.poll_event() {
			match e {
			Event::Quit {..} => { break 'quit },
			_ => {}
			}}
		// Visual Debug
	// {	show_debug_overlay("some.png")
	// ;}
		// Render
	{	let px_font = ttf.load_font("pixel-clear-condensed.ttf", 100).unwrap()
	;	let px_surface = px_font.render("WWW! my grass!").blended(Color::WHITE).unwrap()
	;	let px_texture = mktex.create_texture_from_surface(&px_surface).unwrap()
	; let TextureQuery { width: w, height: h, .. } = px_texture.query()
	;	let px_stretch = Rect::new(10, 10, w, h)
	;	let _ = canvas.copy(&px_texture, WHOLE, px_stretch)
	;}

	{	let (width, height) = canvas.output_size().unwrap()
	;	let sz = 96
	;	let ofst = Vec2u { x: width / 2, y: height / 2 }
	;	let col = Color::RGB(32, 32, 255)

	; let clock = Vec3u::expand_1d_to_3d(time.ticks(), 1000, 60)
	; let txt = ":".join([clock.z.to_string(), clock.y.to_string(), clock.x.to_string()])

	; let TxtTex { tex, strh } = TxtTex::new_mid_df_fnt(txt.as_str(), sz as u16, ofst, col, &ttf, &mktex)
	;	let _ = canvas.copy(&tex, WHOLE, strh)
	;}

	{	let TxtTex { tex: grass, strh } = TxtTex::new_df_fnt(
			"00:00:00", 24,
			Vec2u {x: 100,y: 10}, Color::GREEN,
			&ttf, &mktex
		)
	;	let _ = canvas.copy(&grass, WHOLE, strh)
	;}
	;	canvas.present()
	;	canvas.set_draw_color(Color::RGB(0x20, 0x20, 0x20))
	;	canvas.clear()
	;}
  }
