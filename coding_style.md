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
### Place the semicolons at the scope to give a hint about the indentions.  
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
### Use comment to mark sections instead of turning into new functions. Unless you are going to reuse it at least 10 times. Or if it is important.
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
### Reversed conditions or "Yoda conditions" are first class. Because constants are more important than variables. That is the first statement you read.
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
### str -> size -> color -> font_path -> ttf -> mktex
### 2 spaces at the end of the function instead of 1 tab.
