use sdl2::{ttf::Sdl2TtfContext, pixels::Color, render::{Texture, TextureQuery, TextureCreator}, rect::Rect, video::WindowContext};
use crate::engine::types::*;

pub const WHOLE: Option<Rect> = None;
///	# Example
/// ```
/// let TxtTex { tex: grass, strh } = TxtTex::new(
/// 	"00:00:00", 24,
/// 	Vec2i {x:10,y:10}, Color::GREEN,
/// 	NO_FONT_PATH, &ttf, &mktex
/// )
/// let _ = canvas.copy(&grass, WHOLE, strh)
/// ```
pub struct TxtTex<'a> {
	pub tex: Texture<'a>,
	pub strh: Rect
  }
impl TxtTex<'_> {
	pub fn new<'a>(
		str: &'a str, size: u16, offset: Vec2u, color: Color, font_path: &'a str,
		ttf: &'a Sdl2TtfContext, mktex: &'a TextureCreator<WindowContext>
		) -> TxtTex<'a>	{
			let px_font = ttf
				.load_font(font_path, size).unwrap()
		;	let px_surface = px_font
				.render(str)
				.blended(color).unwrap()
		;	let tex = mktex
				.create_texture_from_surface(&px_surface).unwrap()
		;	let TextureQuery { width, height, .. } = tex
				.query()
		;	let strh = Rect::new(offset.x as i32, offset.y as i32, width, height)
		;	return TxtTex{ tex, strh }
		;}

	pub fn new_default_font<'a>(
		str: &'a str, size: u16, offset: Vec2u, color: Color,
		ttf: &'a Sdl2TtfContext, mktex: &'a TextureCreator<WindowContext>
		) -> TxtTex<'a>	{
			let px_font = ttf
				.load_font("pixel-clear-condensed.ttf", size).unwrap()
		;	let px_surface = px_font
				.render(str)
				.blended(color).unwrap()
		;	let tex = mktex
				.create_texture_from_surface(&px_surface).unwrap()
		;	let TextureQuery { width, height, .. } = tex
				.query()
		;	let strh = Rect::new(offset.x as i32, offset.y as i32, width, height)
		;	return TxtTex{ tex, strh }
		;}
  }


pub fn middle_rect(rect: Rect) -> Rect { Rect::new(rect.x - (rect.w / 2), rect.y - (rect.h / 2), rect.w as u32, rect.h as u32) }
