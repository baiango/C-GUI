#pragma once
#include <vector>
using std::vector;
#include <variant>
using std::variant;
//using std::get;
#include <string>
using std::string;
using std::to_string;
#include <array>
using std::array;


typedef unsigned char u8; typedef unsigned short u16; typedef unsigned u32; typedef unsigned long long u64;
typedef char i8; typedef short i16; typedef int i32; typedef long long i64;
typedef float f32; typedef double f64; typedef long double f80;


typedef struct { f32 x, y; } Vec2f;
typedef struct { i32 x, y; } Vec2i;
typedef struct { f32 x, y, z; } Vec3f;
typedef struct { i32 x, y, z; } Vec3i;
/// (Vec2i){ i % row, i / row }
Vec2i GUI_Expand1DTo2D(i32 i, i32 row) { return { i % row, i / row }; }
/// (Vec3i){ i % row, i / row % col, i / row / col }
Vec3i GUI_Expand1DTo3D(i32 i, i32 row, i32 col) { return { i % row, i / row % col, i / row / col }; }
/// x + (y * row)
i32 GUI_Flat2DTo1D(Vec2i vec, i32 row) { return vec.x + (vec.y * row); }
/// x + (y * row) + (z * row * col)
i32 GUI_Flat3DTo1D(Vec3i vec, i32 row, i32 col) { return vec.x + (vec.y * row) + (vec.z * row * col); }


typedef struct {
	SDL_Color text_color
;	SDL_Texture *text_texture
;	SDL_Rect text_stretch
; } GUI_TxtTexture;


/// return nullptr;
string NO_PATH = "NO_PATH";
/*Example:
	GUI_TxtTexture text
;{	SDL_Color col = { 0xff, 0xff, 0xff }
;	Vec2i ofst = { 10, 10 }
;	text = GUI_StrToTexture(renderer, "WWW! my grass!", ofst, 24, col, NO_PATH)
;}
	It will cause memory leak if you don't destroy the texture. */
GUI_TxtTexture GUI_StrToTexture(
		SDL_Renderer *renderer, string str, Vec2i offset, int font_size,
		SDL_Color text_color, string font_path
)	{
	GUI_TxtTexture ret
;	TTF_Font *font
;	if (NO_PATH == font_path) {
		font = TTF_OpenFont("pixel-clear-condensed.ttf", font_size)
	;}
	else {
		font = TTF_OpenFont(font_path.c_str(), font_size)
	;}

	SDL_Surface *text_surface = TTF_RenderText_Solid(font, str.c_str(), text_color)
;	TTF_CloseFont(font)
;	ret.text_texture = SDL_CreateTextureFromSurface(renderer, text_surface)
;	SDL_FreeSurface(text_surface)
;
	i32 txtw, txth
;	SDL_QueryTexture(ret.text_texture, nullptr, nullptr, &txtw, &txth)
;	ret.text_stretch = { offset.x, offset.y, txtw, txth }
;
	return ret
; }


/*For Reactive Programming:
{	i32 a = 10
;	array<GUI_Var, 2> b = {&a, 1}
;	a = 20
;	cout << *get<i32*>(b[0]) + get<i32>(b[1]) << "\n" // 21
//;	cout << get<i32>(b[0]) + get<i32>(b[1]) // Panic!
;}
*/
typedef variant<
	u8*, u16*, u32*, u64*
,	i8*, i16*, i32*, i64*
,	f32*, f64*
,	u8, u16, u32, u64
,	i8, i16, i32, i64
,	f32, f64
  > GUI_Var;

/*Example:
	string str = GUI_Join(", ", { "a1", "bb2", "ccc3" })
;	cout << str // a1, bb2, ccc3
;
*/
string GUI_Join(string separator, vector<string> strs) {
	string ret;
	for (auto i = 0; i < strs.size() - 1; i++) {
		ret += strs[i]
	;	ret += separator
	;}
	ret += strs[strs.size() - 1]
;	return ret
; }
