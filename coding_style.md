# Simplify Rust tips
```
{	let TxtTex { tex: grass, strh } = TxtTex::new(
		"00:00:00", 24,
		Vec2u {x: 100, y: 10}, Color::GREEN,
		NO_PATH, &ttf, &mktex
	)
;	let _ = canvas.copy(&grass, WHOLE, strh)
;}
```
### Place the semicolons at the scope to give a hint about the current indentions.  
### If the line statement is important, limit the line length below 75 characters.
### Recommend to read it with 2 indentations. But up to 8 are supported.
```
fn main() {
	let i = 3;
    // Comment 1
{	let borrow1 = &i;
	println!("borrow1: {}", borrow1)
	;} // Do not place comment or anything here.
    // Comment 2
{	let borrow2 = &i;
	println!("borrow2: {}", borrow2)
	;}	
}
```
### Wrap it around with curly brace to use lifetime to give hint about the indentation of the variables.
### Do not place comment or anything on the closing curly brace or right curly brace.
```
 	SDL_Event event
;	while (1) {
		SDL_Delay(15)
	;	// Poll
		SDL_PollEvent(&event)
	;	switch (event.type) {
		case SDL_QUIT: goto quit
		;}
	;}
```
### Do not indent switch statement.
```
	// Window
	let video = sdl.video().unwrap()
;	let window = video.window("rust-sdl2 demo", 800, 600).position_centered().build().unwrap();
;	let mut canvas = window.into_canvas().build().unwrap()
;	let mktex = canvas.texture_creator()
;
	// Time
	let time = sdl.timer().unwrap()
;
	// Ttf
	let ttf = ttf::init().unwrap()
;
```
### Use comment to mark sections instead of turning into new functions.
Unless you are going to reuse it at least 10 times. Or if it is important.
main.rs:
```
let TxtTex { tex, strh } = TxtTex::new(txt.as_str(), sz as u16, ofst, col, NO_PATH, &ttf, &mktex);
```
sdl2_types.rs:
```
pub const NO_PATH: &str = "NO_PATH";
if NO_PATH == font_path { font_path = "pixel-clear-condensed.ttf"; }
```
### Do not use `None` or `Option<T>` unless SDL2 require you to.
### Make constant global variable and use it to make it less vague or make the functions input types obvious .

`if NO_PATH == font_path { font_path = "pixel-clear-condensed.ttf"; }`
### Reversed conditions or "Yoda conditions" are first class.
Because constants are more important than variables. That is the first statement you read. And constants give you more informations than variables.
Using it:
```
let TxtTex { tex: grass, strh } = TxtTex::new(
		"00:00:00", 24,
		Vec2u {x: 100,y: 10}, Color::GREEN,
		NO_PATH, &ttf, &mktex
)
```

```
impl TxtTex<'_> {
	pub fn new<'a>(
		str: &'a str, size: u16, offset: Vec2u, color: Color, mut font_path: &'a str,
		ttf: &'a Sdl2TtfContext, mktex: &'a TextureCreator<WindowContext>
		) -> TxtTex<'a>	{}
  }
```
### Reorder the parameters to most important to least.
str -> size -> color -> font_path -> ttf -> mktex
```
fn main() {
	println!("Hello!\n");
  }
```
```
### 2 spaces at the end of the function instead of 1 tab.
fn main() {
	println!("Hello!\n")
; }
```
Or 1 semicolon and space if you have semicolon.
```
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
```
### Prefer new function over code reusability. When was the last time you heard people say...
> I am going to reuse my code.  

"Code reusability" just makes your code harder to read because of unpredictability. And sometimes slower to run too!
### Name the functions in this order.
Verb -> Noun  
Do: `fn middle_rect()`  
Not: `fn rect_middle()`  
### Stick to Data-oriented programming and Pure function only.
**Pure function is...**
- It does not modify global variables.
- It always return same value with the same input.
- It does not have any `mut` keyword used on the function.  

**Data-oriented programming is...**
- Let's say our type is `Rect`.
- It warps `Rect` into `vec<Rect>` and use all of them without skipping one variable.
- It make the vector looks like a database. Or it is a database.
- You'll be running into memory bandwidth limit more easily with this. So please keep the data small.
- It's perfect for physics engine. Since it calculate the object's transform 1000 times every frame.
- It might be bad for readability. If it is, use pure function to write the functions instead.

### Do not nest struct more than 4 times deep.  
**Example 1**
```
struct Window { // Level 3
	rect: Rect // L1
,	text: TxtTex // L2
,	icon: IconTex // L2
,	size: Vec2i // L1
}

struct TxtTex<'a> { tex: Texture<'a>, strh: Rect } // L2, it inherited Rect
struct IconTex { tex: Texture<'a>, strh: Rect } // L2, it inherited Rect
struct Texture<'a> { pixels_data: Vec<Color8> } // L2, it inherited Color8
struct Vec2i { x: i32, y: i32 } // L1, no inherit except from STL
struct Rect { x: i32, y: i32, w: i32, h: i32 } // L1, no inherit except from STL
```
`Window` is a struct that is nested 2 deep. Do not go over 3 or the code will be hard to track down or read.  
**Example 2**
```
struct Player { // Level 4
	Node: Node // L1
,	tran: Transform // L2
,	mesh: Mesh // L3
}

struct Mesh { tri: Vec<Triangle> } // L3
struct Transform { pos: Vec3f, rot: Vec3f,	sc: Vec3f } // L2
struct Triangle { p1: Vec3f, p2: Vec3f, p3: Vec3f } // L2
struct Node { index: Vec<u32> } // L1
struct Vec3f { x: f32, y: f32, z: f32 } // L1
impl Vec3f { fn new(x: f32, y: f32, z: f32) -> Vec3f { Vec3f{ x, y, z } } }
```
At this point, try not to inherit the `Player` struct unless it's impossible. or do it if it's much easier to track the inherents. The `Player` is nested 3 deep.
