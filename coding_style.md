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
### If the line statement is important, limit the line length below 70 characters.
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
**If struct have methods(functions), call it class instead.**
- typedef pointer (Use `int *` instead of `int_ptr`.)
- Lambda (You can't even tell when the variables are values or functions with lambdas.)
- Try-catch (It reduce predictability just like if/else.)
- Throw exceptions (You never knew which code will [handle](https://250bpm.com/blog:4/) the exception or not. When you can just call the function to handle it. So the program will terminate if you don't handle the exception.)
- Constructors (It don't have return type and it force you to use exceptions. Use init function to initialize the classes instead.)
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
**Only use it when the arguments can fit in 2 or less lines, or functions that must use all the arguments you put in.**  
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
# C
### Don't hand unrolling the loop
Unrolling the loop will make the software bigger, and the code will less likely to fit in L1 cache.  
Let the compilers do it. Even the compilers overdo it sometimes. So that's why -O2 exist on GCC, and not to use -O3.  
It's very easy to overdo it and bloat the L1.  
```
#include <stdlib.h>

struct mat4
{	float data[16]; };

void cgui_mul_mat4(struct mat4* mat, struct mat4* a, struct mat4* b)
{	for (size_t i = 0; i < 4; i++)
	{	for (size_t j = 0; j < 4; j++)
		{	mat->data[4 * j + i] = 0.0f;

			for (size_t k = 0; k < 4; k++)
			{	mat->data[4 * j + i] += a->data[4 * k + i] * b->data[4 * j + k]; } } } }
```
[-O2 -mavx2:](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:___c,selection:(endColumn:1,endLineNumber:13,positionColumn:1,positionLineNumber:13,selectionStartColumn:1,selectionStartLineNumber:13,startColumn:1,startLineNumber:13),source:'%23include+%3Cstdlib.h%3E%0A%0Astruct+mat4%0A%7B%09float+data%5B16%5D%3B+%7D%3B%0A%0Avoid+cgui_mul_mat4(struct+mat4*+mat,+struct+mat4*+a,+struct+mat4*+b)%0A%7B%09for+(size_t+i+%3D+0%3B+i+%3C+4%3B+i%2B%2B)%0A%09%7B%09for+(size_t+j+%3D+0%3B+j+%3C+4%3B+j%2B%2B)%0A%09%09%7B%09mat-%3Edata%5B4+*+j+%2B+i%5D+%3D+0.0f%3B%0A%0A%09%09%09for+(size_t+k+%3D+0%3B+k+%3C+4%3B+k%2B%2B)%0A%09%09%09%7B%09mat-%3Edata%5B4+*+j+%2B+i%5D+%2B%3D+a-%3Edata%5B4+*+k+%2B+i%5D+*+b-%3Edata%5B4+*+j+%2B+k%5D%3B+%7D+%7D+%7D+%7D%0A'),l:'5',n:'0',o:'C+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:cg132,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:___c,libs:!(),options:'-O2+-mavx2',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+gcc+13.2+(Editor+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)
```
cgui_mul_mat4:
        xor     r11d, r11d
.L2:
        lea     r8, [rdi+r11*4]
        mov     r9, rdx
        xor     r10d, r10d
.L4:
        mov     DWORD PTR [r8], 0x00000000
        mov     rcx, rsi
        vxorps  xmm1, xmm1, xmm1
        xor     eax, eax
.L3:
        vmovss  xmm0, DWORD PTR [rcx]
        vmulss  xmm0, xmm0, DWORD PTR [r9+rax*4]
        add     rax, 1
        add     rcx, 16
        vaddss  xmm1, xmm1, xmm0
        vmovss  DWORD PTR [r8], xmm1
        cmp     rax, 4
        jne     .L3
        add     r10, 4
        add     r8, 16
        add     r9, 16
        cmp     r10, 16
        jne     .L4
        add     r11, 1
        add     rsi, 4
        cmp     r11, 4
        jne     .L2
        ret
```
[-O3 -mavx3:](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:___c,selection:(endColumn:1,endLineNumber:13,positionColumn:1,positionLineNumber:13,selectionStartColumn:1,selectionStartLineNumber:13,startColumn:1,startLineNumber:13),source:'%23include+%3Cstdlib.h%3E%0A%0Astruct+mat4%0A%7B%09float+data%5B16%5D%3B+%7D%3B%0A%0Avoid+cgui_mul_mat4(struct+mat4*+mat,+struct+mat4*+a,+struct+mat4*+b)%0A%7B%09for+(size_t+i+%3D+0%3B+i+%3C+4%3B+i%2B%2B)%0A%09%7B%09for+(size_t+j+%3D+0%3B+j+%3C+4%3B+j%2B%2B)%0A%09%09%7B%09mat-%3Edata%5B4+*+j+%2B+i%5D+%3D+0.0f%3B%0A%0A%09%09%09for+(size_t+k+%3D+0%3B+k+%3C+4%3B+k%2B%2B)%0A%09%09%09%7B%09mat-%3Edata%5B4+*+j+%2B+i%5D+%2B%3D+a-%3Edata%5B4+*+k+%2B+i%5D+*+b-%3Edata%5B4+*+j+%2B+k%5D%3B+%7D+%7D+%7D+%7D%0A'),l:'5',n:'0',o:'C+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:cg132,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:___c,libs:!(),options:'-O3+-mavx2',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+gcc+13.2+(Editor+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)
```
cgui_mul_mat4:
        mov     rcx, rsi
        mov     rax, rdx
        cmp     rdi, rdx
        je      .L2
        vmovss  xmm5, DWORD PTR [rdx]
        vxorps  xmm6, xmm6, xmm6
        vmovss  xmm8, DWORD PTR [rdx+8]
        vmovss  xmm7, DWORD PTR [rdx+4]
        mov     esi, DWORD PTR [rdx+12]
        vmovss  xmm15, DWORD PTR [rax+20]
        mov     edx, DWORD PTR [rdx+16]
        vshufps xmm5, xmm5, xmm5, 0
        vmovss  xmm14, DWORD PTR [rax+24]
        vmovss  xmm12, DWORD PTR [rax+28]
        vshufps xmm7, xmm7, xmm7, 0
        vmovss  xmm4, DWORD PTR [rax+32]
        vmovss  xmm3, DWORD PTR [rax+36]
        vshufps xmm15, xmm15, xmm15, 0
        vmovss  xmm1, DWORD PTR [rax+40]
        vmovss  xmm11, DWORD PTR [rax+44]
        vshufps xmm14, xmm14, xmm14, 0
        vshufps xmm12, xmm12, xmm12, 0
        vmovss  xmm0, DWORD PTR [rax+48]
        vmovss  xmm10, DWORD PTR [rax+52]
        vshufps xmm4, xmm4, xmm4, 0
        vshufps xmm3, xmm3, xmm3, 0
        vmovss  xmm2, DWORD PTR [rax+56]
        vmovss  xmm9, DWORD PTR [rax+60]
        vmovups XMMWORD PTR [rdi], xmm6
        vshufps xmm1, xmm1, xmm1, 0
        vmulps  xmm5, xmm5, XMMWORD PTR [rcx]
        vshufps xmm0, xmm0, xmm0, 0
        vaddps  xmm5, xmm5, xmm6
        vmovups XMMWORD PTR [rdi], xmm5
        vmulps  xmm7, xmm7, XMMWORD PTR [rcx+16]
        vaddps  xmm7, xmm7, xmm5
        vshufps xmm5, xmm8, xmm8, 0
        vmovups XMMWORD PTR [rdi], xmm7
        vmovups xmm13, XMMWORD PTR [rcx+32]
        vmulps  xmm5, xmm5, xmm13
        vmulps  xmm13, xmm14, xmm13
        vaddps  xmm5, xmm5, xmm7
        vmovd   xmm7, esi
        vshufps xmm7, xmm7, xmm7, 0
        vmovups XMMWORD PTR [rdi], xmm5
        vmovups xmm8, XMMWORD PTR [rcx+48]
        vmovups XMMWORD PTR [rdi+16], xmm6
        vmulps  xmm7, xmm7, xmm8
        vmulps  xmm12, xmm12, xmm8
        vaddps  xmm7, xmm7, xmm5
        vmovd   xmm5, edx
        vshufps xmm5, xmm5, xmm5, 0
        vmovups XMMWORD PTR [rdi], xmm7
        vmovups xmm7, XMMWORD PTR [rcx]
        vmulps  xmm5, xmm5, xmm7
        vmulps  xmm4, xmm4, xmm7
        vmulps  xmm0, xmm0, xmm7
        vaddps  xmm5, xmm5, xmm6
        vaddps  xmm4, xmm4, xmm6
        vaddps  xmm0, xmm0, xmm6
        vmovups XMMWORD PTR [rdi+16], xmm5
        vmulps  xmm15, xmm15, XMMWORD PTR [rcx+16]
        vmovups XMMWORD PTR [rdi+32], xmm4
        vaddps  xmm5, xmm15, xmm5
        vaddps  xmm5, xmm5, xmm13
        vaddps  xmm5, xmm5, xmm12
        vmovups XMMWORD PTR [rdi+16], xmm5
        vmovups xmm5, XMMWORD PTR [rcx+16]
        vmulps  xmm3, xmm3, xmm5
        vaddps  xmm3, xmm3, xmm4
        vmovups XMMWORD PTR [rdi+32], xmm3
        vmulps  xmm1, xmm1, XMMWORD PTR [rcx+32]
        vaddps  xmm1, xmm1, xmm3
        vshufps xmm3, xmm11, xmm11, 0
        vmulps  xmm3, xmm3, xmm8
        vaddps  xmm1, xmm1, xmm3
        vmovups XMMWORD PTR [rdi+32], xmm1
        vshufps xmm1, xmm10, xmm10, 0
        vmulps  xmm1, xmm1, xmm5
        vaddps  xmm0, xmm0, xmm1
        vshufps xmm1, xmm2, xmm2, 0
        vmovups XMMWORD PTR [rdi+48], xmm0
        vmulps  xmm1, xmm1, XMMWORD PTR [rcx+32]
        vaddps  xmm0, xmm1, xmm0
        vshufps xmm1, xmm9, xmm9, 0
        vmovups XMMWORD PTR [rdi+48], xmm0
        vmulps  xmm1, xmm1, XMMWORD PTR [rcx+48]
        vaddps  xmm0, xmm1, xmm0
        vmovups XMMWORD PTR [rdi+48], xmm0
        ret
.L2:
        mov     rsi, rdx
        vxorps  xmm3, xmm3, xmm3
        mov     rdx, rcx
        lea     rcx, [rax+16]
.L4:
        mov     DWORD PTR [rsi], 0x00000000
        vmovss  xmm1, DWORD PTR [rax]
        add     rsi, 4
        add     rdx, 4
        vmulss  xmm1, xmm1, DWORD PTR [rdx-4]
        vaddss  xmm1, xmm1, xmm3
        vmovss  DWORD PTR [rsi-4], xmm1
        vmovss  xmm0, DWORD PTR [rdx+12]
        vmulss  xmm0, xmm0, DWORD PTR [rax+4]
        vaddss  xmm0, xmm0, xmm1
        vmovss  DWORD PTR [rsi-4], xmm0
        vmovss  xmm1, DWORD PTR [rdx+28]
        vmulss  xmm2, xmm1, DWORD PTR [rax+8]
        vaddss  xmm0, xmm2, xmm0
        vmovss  DWORD PTR [rsi-4], xmm0
        vmovss  xmm2, DWORD PTR [rdx+44]
        vmulss  xmm4, xmm2, DWORD PTR [rax+12]
        mov     DWORD PTR [rsi+12], 0x00000000
        vaddss  xmm0, xmm4, xmm0
        vmovss  DWORD PTR [rsi-4], xmm0
        vmovss  xmm0, DWORD PTR [rdx-4]
        vmulss  xmm5, xmm0, DWORD PTR [rax+16]
        vaddss  xmm5, xmm5, xmm3
        vmovss  DWORD PTR [rsi+12], xmm5
        vmovss  xmm4, DWORD PTR [rdx+12]
        vmulss  xmm4, xmm4, DWORD PTR [rax+20]
        vaddss  xmm4, xmm4, xmm5
        vmovss  DWORD PTR [rsi+12], xmm4
        vmulss  xmm1, xmm1, DWORD PTR [rax+24]
        vaddss  xmm1, xmm1, xmm4
        vmovss  DWORD PTR [rsi+12], xmm1
        vmulss  xmm4, xmm2, DWORD PTR [rax+28]
        mov     DWORD PTR [rsi+28], 0x00000000
        vaddss  xmm1, xmm4, xmm1
        vmovss  DWORD PTR [rsi+12], xmm1
        vmulss  xmm4, xmm0, DWORD PTR [rax+32]
        vaddss  xmm4, xmm4, xmm3
        vmovss  DWORD PTR [rsi+28], xmm4
        vmovss  xmm1, DWORD PTR [rdx+12]
        vmulss  xmm5, xmm1, DWORD PTR [rax+36]
        vaddss  xmm5, xmm5, xmm4
        vmovss  DWORD PTR [rsi+28], xmm5
        vmovss  xmm4, DWORD PTR [rdx+28]
        vmulss  xmm4, xmm4, DWORD PTR [rax+40]
        vaddss  xmm4, xmm4, xmm5
        vmovss  DWORD PTR [rsi+28], xmm4
        vmulss  xmm2, xmm2, DWORD PTR [rax+44]
        mov     DWORD PTR [rsi+44], 0x00000000
        vaddss  xmm2, xmm2, xmm4
        vmovss  DWORD PTR [rsi+28], xmm2
        vmulss  xmm0, xmm0, DWORD PTR [rax+48]
        vaddss  xmm0, xmm0, xmm3
        vmovss  DWORD PTR [rsi+44], xmm0
        vmulss  xmm1, xmm1, DWORD PTR [rax+52]
        vaddss  xmm0, xmm1, xmm0
        vmovss  DWORD PTR [rsi+44], xmm0
        vmovss  xmm1, DWORD PTR [rdx+28]
        vmulss  xmm1, xmm1, DWORD PTR [rax+56]
        vaddss  xmm0, xmm1, xmm0
        vmovss  DWORD PTR [rsi+44], xmm0
        vmovss  xmm1, DWORD PTR [rdx+44]
        vmulss  xmm1, xmm1, DWORD PTR [rax+60]
        vaddss  xmm0, xmm1, xmm0
        vmovss  DWORD PTR [rsi+44], xmm0
        cmp     rcx, rsi
        jne     .L4
        ret
```
### Don't use inline
A function call overhead is better than DRAM reads.  
You only had 32 KiB of data cache and instruction cache in L1 to play around, and that's not much. So don't waste it.

### Prefer procedural than pure functions
Pure functions in C can lead overengineered, buggy, and inconsistent code.  
And it force you to use malloc.
It's hard to return the arrays in C without debugging for hours.  
Another plus for not returning anything is, you can cut down a few lines of code.  
So I invented a style called ASM. Because it looks like ASM codes. You use the first argument as the return value. And the other arguments can do anything else.  
```
void cgui_mul_mat4(struct mat4* mat, struct mat4* a, struct mat4* b)
{	for (size_t i = 0; i < 4; i++)
	{	for (size_t j = 0; j < 4; j++)
		{	mat->data[4 * j + i] = 0.0f;

			for (size_t k = 0; k < 4; k++)
			{	mat->data[4 * j + i] += a->data[4 * k + i] * b->data[4 * j + k]; } } } }
```
`mat` is used to store the result of `a` and `b` multiplication. It's still feels like pure functions. But way more efficient and slightly less safe.  
This style will help you keep the variables on the stack, less debugging, and stopping you keep switching pure functions to procedural.

### Avoid heap allocations
Heap doesn't autofree. So don't use it for small or fixed size variables.  
