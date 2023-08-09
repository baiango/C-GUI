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
### ~~Place the semicolons at the scope to give a hint about the current indentions.~~  
It does not help with readability since the code is not Rust and the C++ code are written in mostly C way.
### If the line statement is important, limit the line length below 75 characters.
### Recommend to read it with 2 indentations. But up to 32 are supported.
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
Unless you are going to reuse it at least 3 times. Or if it is important.  

main.rs:
```
let TxtTex { tex, strh } = TxtTex::new(txt.as_str(), sz as u16, ofst, col, NO_PATH, &ttf, &mktex);
```
sdl2_types.rs:
```
pub const NO_PATH: &str = "NO_PATH";
if NO_PATH == font_path { font_path = "pixel-clear-condensed.ttf"; }
```
### Do not use `None` or `Option<T>` unless `insert some libary here` require you to.
Make constant global variable and use it to make it less vague or make the functions input types obvious.

`if NO_PATH == font_path { font_path = "pixel-clear-condensed.ttf"; }`
### Constants are first class.
Because constants are more important than variables. That is the first statement you read. And constants give you more informations than variables.  
Use Reversed conditions or "Yoda conditions" if you found conditions with constants.  

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
		str: &'a str, size: u16, offset: Vec2u, color: Color, font_path: &'a str,
		ttf: &'a Sdl2TtfContext, mktex: &'a TextureCreator<WindowContext>
	) -> TxtTex<'a>	{}
  }
```
### Reorder the arguments to most important to least.
str -> size -> offset -> color -> font_path -> ttf -> mktex
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

	pub fn new_df_fnt<'a>(
		str: &'a str, size: u16, offset: Vec2u, color: Color,
		ttf: &'a Sdl2TtfContext, mktex: &'a TextureCreator<WindowContext>
	) -> TxtTex<'a>	{
		Self::new(str, size, offset, color, "pixel-clear-condensed.ttf", ttf, mktex)
	 }

	pub fn new_mid_df_fnt<'a>(
		str: &'a str, size: u16, offset: Vec2u, color: Color,
		ttf: &'a Sdl2TtfContext, mktex: &'a TextureCreator<WindowContext>
	) -> TxtTex<'a>	{
		let ret = Self::new(str, size, offset, color, "pixel-clear-condensed.ttf", ttf, mktex)
	;	return TxtTex{ tex: ret.tex, strh: middle_rect(ret.strh) }
	;}
}
```
### Do not use flag arguments or if/else to abstract away the functions.
"if/else" just makes your code harder to read because of unpredictability. And sometimes slower to run too!  
**Flag Arguments are prohibited.**  
Make another function to abstract it instead.
### Name the functions in this order.
Verb -> Adjective -> Noun -> Data type  
Do: `fn middle_rect()`  
Not: `fn rect_middle()`  
`middle` is an adjective. `rect` is a noun.  
### Stick to Column database and Pure function.
**Pure function is...**
- It does not modify global variables.
- It always return same value with the same input.
- It does not have any `mut` keyword used on the function.
- Prefer to not modify local variables. That is impure function.  

**Column database is...**
- Let's say our type is `Rect`.
- It warps `Rect` into `vec<Rect.x, Rect.y, Rect.w, Rect.h>` and use all of them without skipping one variable.
- You'll be running into memory bandwidth limit more easily with this. So please keep the data small.
- It's perfect for physics engine. Since it calculate the object's transform 1000 times every frame.
- It might be bad for readability. If it is, use pure function to write the functions instead.  
If the readability is worth the performance cost.  

Column database:
```
struct Nodes { vector<struct Vec3f> position, scale, rotation; };

struct Nodes nodes;

nodes.position = {0, 0, 0}, {1, 0, 0}, {0, 1, 1}, {0, 1, 0};
nodes.scale = {0, 0, 0}, {3, 3, 3}, {1, 1, 1}, {1, 1, 1};
nodes.rotation = {0, 0, 0}, {0, 0, 270}, {90, 0, 0}, {0, 0, 0};
```
Row database/Object-oriented programming:
```
struct Node { struct Vec3f position, scale, rotation; };

vector<struct Node> nodes;
// position / scale / rotation //
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 
{{1, 0, 0}, {3, 3, 3}, {0, 0, 270}},
{{0, 1, 1}, {1, 1, 1}, {90, 0, 0}},
{{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}
```
![cache locality](https://github.com/baiango/C-GUI/assets/105705580/e03b3a31-2b72-48e6-9d6a-6997f4a7a2a3)


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
`Window` is a struct that is nested 2 deep because `TxtTex` and `IconTex` are nested 2 level deep.  
Do not go over 3 or the code will be hard to track down or read.  

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
The `Player` is nested 3 level deep because `Mesh` is nested 3 level deep.  
At this point, try not to inherit the `Player` struct unless it's impossible. Or do it if it's much easier to track the inherents.

# For C++
### Boost is banned from this project.  
It's already really easy to write puzzling code with the STL. Let alone Boost, it has awfully nested deeply namespace. and the compiler don't even include Boost too! So you have to install it.  
The Boost functions are too generic to be fast. It would be run much faster if you roll your own algorithms with the STL instead. Or writing codes in C style on C++.  
Use functions from STL up to C++20 only.
### These C++ keywords are prohibited.
Unless it's coming from STL or elsewhere. Don't use it.
- Inlines (They just reduce cache locality by filling the L1 cache and increasing the binary size.)
- Function overloading (Most people can't tell when the functions are overloaded or not.)
- typedef struct (You must prefix every struct/class declaration with struct/class keyword from this software. Except struct/class from the libraries)
- typedef pointer (Use `int *` instead of `int_ptr`.)
- Lambda (You can't even tell when the variables are values or functions with lambdas.)
- Try-catch (It reduce predictability just like if/else.)
### Avoid these C++ keywords.
- #define (Use constants instead for type safety.)
- #if, #ifdef (Conditional statements are not preferred. Avoid them.)
- auto (Unless you casted the assigning value.)
- Generics (It's an explicit version of function overloading.)
- Global variables (Global namespace are for constants, not variables. They are good as gotos. Pass the variables with struct or class instead.)
- #include that get parent directory such as "#include ../scene0.hpp"  
(It's vague and it doesn't tell the name of parent directory. The scope is really big too. So it's much easier to create header dependency hell with it.)
- [Templates](https://github.com/godotengine/godot-cpp/blob/3162be28e594bf5b17889117670fc6f2d75f2f0c/include/godot_cpp/templates/cowdata.hpp#L60) (Another code smell that hide Generics/Classes inside itself. Use it on containers only.)
### Functions should keep it as long as possible, but still do one thing only.
**Don't abstract functions that won't be used more than 2 times.**  

I don't want to abstracting functions that doesn't need to be abstracted.  
When you abstracting the functions, you should be not taking away the control. Only take away the boilerplate.  

### Use Builder pattern over JavaBean pattern.
[In fact it's used in the first examples you see in Raylib Rust.](https://docs.rs/raylib/latest/raylib/#examples)
```
class Mesh pyramid;
pyramid
	.set_vertices(8, pyramid_vertices, sizeof pyramid_vertices)
	.set_indices(pyramid_indices, sizeof pyramid_indices)
	.cook_vertices()
	.add_attribute(3) // Color
	.add_attribute(2); // UV
```
Instead of JavaBean pattern:
```
class Mesh pyramid;
pyramid.set_vertices(8, pyramid_vertices, sizeof pyramid_vertices)
pyramid.set_indices(pyramid_indices, sizeof pyramid_indices)
pyramid.cook_vertices()
pyramid.add_attribute(3) // Color
pyramid.add_attribute(2); // UV
```
Or Telescoping constructor pattern: (They are the worst pattern on earth.)  
**Only use it when the arguments are less than 4, or functions that must use all the arguments you put in.**  
```
class Mesh pyramid;
pyramid.new(
	8, pyramid_vertices, sizeof pyramid_vertices,
	pyramid_indices, sizeof pyramid_indices
);
pyramid.cook_vertices();
pyramid.add_attribute(3) // Color
pyramid.add_attribute(2); // UV
```
