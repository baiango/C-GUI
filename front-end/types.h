#pragma once


typedef unsigned char u8; typedef unsigned short u16; typedef unsigned u32; typedef unsigned long long u64;
typedef char i8; typedef short i16; typedef int i32; typedef long long i64;
typedef float f32; typedef double f64; typedef long double f80;


typedef struct { f32 x, y; } Vec2f;
typedef struct { i32 x, y; } Vec2i;
typedef struct { f32 x, y, z; } Vec3f;
typedef struct { i32 x, y, z; } Vec3i;
/// (Vec2i){ i % row, i / row }
Vec2i GUI_Expand1DTo2D(i32 i, i32 row) { return (Vec2i){ i % row, i / row }; }
/// (Vec3i){ i % row, i / row % col, i / row / col }
Vec3i GUI_Expand1DTo3D(i32 i, i32 row, i32 col) { return (Vec3i){ i % row, i / row % col, i / row / col }; }
/// x + (y * row)
i32 GUI_Flat2DTo1D(Vec2i vec, i32 row) { return vec.x + (vec.y * row); }
/// x + (y * row) + (z * row * col)
i32 GUI_Flat3DTo1D(Vec3i vec, i32 row, i32 col) { return vec.x + (vec.y * row) + (vec.z * row * col); }


typedef struct {
	SDL_Color text_color
;	SDL_Texture *text_texture
;	SDL_Rect text_stretch
; } GUI_TxtTexture;


/// return NULL;
void *NO_PATH = NULL;
/*Example:
	{	GUI_TxtTexture text = GUI_StrToTexture(renderer, "WWW! my grass!", 24, (SDL_Color){ 0xff, 0xff, 0xff }, NO_PATH)
	;	SDL_DestroyTexture(text.text_texture)
	;}
	It will cause memory leak if you don't destroy the texture. */
GUI_TxtTexture GUI_StrToTexture(
		SDL_Renderer *renderer, char str[], Vec2i offset, int font_size,
		SDL_Color text_color, char font_path[]
)	{
	GUI_TxtTexture ret
;{	TTF_Font *font
;	if (NULL == font_path) {
		font = TTF_OpenFont("pixel-clear-condensed.ttf", font_size)
	;	if (NULL == font) { printf("Font not found! Panic!\n"); exit(-1)
	;}}
	else {
		font = TTF_OpenFont(font_path, font_size)
	;	if (NULL == font) { printf("Font not found! Ignoring!\n")
	;}}

	SDL_Surface *text_surface = TTF_RenderText_Solid(font, str, text_color)
;	TTF_CloseFont(font)
;	ret.text_texture = SDL_CreateTextureFromSurface(renderer, text_surface)
;	SDL_FreeSurface(text_surface)
;
;	i32 txtw, txth
;	SDL_QueryTexture(ret.text_texture, NULL, NULL, &txtw, &txth)
;	ret.text_stretch = (SDL_Rect){ offset.x, offset.y, txtw, txth }
;}
	return ret
; }


enum Type {
	u8_ptr, u16_ptr, u32_ptr, u64_ptr
,	i8_ptr, i16_ptr, i32_ptr, i64_ptr
,	f32_ptr, f64_ptr
,	u8_type, u16_type, u32_type, u64_type
,	i8_type, i16_type, i32_type, i64_type
,	f32_type, f64_type
,	};
/*For Reactive Programming:
{	i32 a = 10
	#pragma warning(push)
	#pragma warning(disable: 4047)
;	GUI_Var b[2] = {{&a, i32_ptr }, {1, i32_type}}
	#pragma warning(pop)
;	a = 20
;	printf("%i\n", *GUI_Geti32p(b[0]) + GUI_Geti32t(b[1])) // 21
//;	printf("%i\n", GUI_Geti32t(b[0]) + GUI_Geti32t(b[1])) // Panic!
;}
	It's almost the same as std::variant.
*/
typedef struct {
	union ptr
	{	void *get_void;
		u8 *u8p; u16 *u16p; u32 *u32p; u64 *u64p
	;	i8 *i8p; i16 *i16p; i32 *i32p; i64 *i64p
	;	f32 *f32p; f64 *f64p
	;	u8 u8t; u16 u16t; u32 u32t; u64 u64t
	;	i8 i8t; i16 i16t; i32 i32t; i64 i64t
	;	f32 f32t; f64 f64t
	;}
	;	u8 type
; } GUI_Var;


u8 *GUI_Getu8p(GUI_Var var) { if (u8_ptr != var.type) { printf("Not u8_ptr! Panic!\n"); exit(-1); } return var.u8p; }
u16 *GUI_Getu16p(GUI_Var var) { if (u16_ptr != var.type) { printf("Not u16_ptr! Panic!\n"); exit(-1); } return var.u16p; }
u32 *GUI_Getu32p(GUI_Var var) { if (u32_ptr != var.type) { printf("Not u32_ptr! Panic!\n"); exit(-1); } return var.u32p; }
u64 *GUI_Getu64p(GUI_Var var) { if (u64_ptr != var.type) { printf("Not u64_ptr! Panic!\n"); exit(-1); } return var.u64p; }
i8 *GUI_Geti8p(GUI_Var var) { if (i8_ptr != var.type) { printf("Not i8_ptr! Panic!\n"); exit(-1); } return var.i8p; }
i16 *GUI_Geti16p(GUI_Var var) { if (i16_ptr != var.type) { printf("Not i16_ptr! Panic!\n"); exit(-1); } return var.i16p; }
i32 *GUI_Geti32p(GUI_Var var) { if (i32_ptr != var.type) { printf("Not i32_ptr! Panic!\n"); exit(-1); } return var.i32p; }
i64 *GUI_Geti64p(GUI_Var var) { if (i64_ptr != var.type) { printf("Not i64_ptr! Panic!\n"); exit(-1); } return var.i64p; }
f32 *GUI_Getf32p(GUI_Var var) { if (f32_ptr != var.type) { printf("Not f32_ptr! Panic!\n"); exit(-1); } return var.f32p; }
f64 *GUI_Getf64p(GUI_Var var) { if (f64_ptr != var.type) { printf("Not f64_ptr! Panic!\n"); exit(-1); } return var.f64p; }
u8 GUI_Getu8t(GUI_Var var) { if (u8_type != var.type) { printf("Not u8_type! Panic!\n"); exit(-1); } return var.u8t; }
u16 GUI_Getu16t(GUI_Var var) { if (u16_type != var.type) { printf("Not u16_type! Panic!\n"); exit(-1); } return var.u16t; }
u32 GUI_Getu32t(GUI_Var var) { if (u32_type != var.type) { printf("Not u32_type! Panic!\n"); exit(-1); } return var.u32t; }
u64 GUI_Getu64t(GUI_Var var) { if (u64_type != var.type) { printf("Not u64_type! Panic!\n"); exit(-1); } return var.u64t; }
i8 GUI_Geti8t(GUI_Var var) { if (i8_type != var.type) { printf("Not i8_type! Panic!\n"); exit(-1); } return var.i8t; }
i16 GUI_Geti16t(GUI_Var var) { if (i16_type != var.type) { printf("Not i16_type! Panic!\n"); exit(-1); } return var.i16t; }
i32 GUI_Geti32t(GUI_Var var) { if (i32_type != var.type) { printf("Not i32_type! Panic!\n"); exit(-1); } return var.i32t; }
i64 GUI_Geti64t(GUI_Var var) { if (i64_type != var.type) { printf("Not i64_type! Panic!\n"); exit(-1); } return var.i64t; }
f32 GUI_Getf32t(GUI_Var var) { if (f32_type != var.type) { printf("Not f32_type! Panic!\n"); exit(-1); } return var.f32t; }
f64 GUI_Getf64t(GUI_Var var) { if (f64_type != var.type) { printf("Not f64_type! Panic!\n"); exit(-1); } return var.f64t; }

/*Example:
	char *num = GUI_u64ToStr(100)
;	printf("%s\n", num) // 100
;	free(num)
;
*/
char *GUI_u64ToStr(u64 num) {
	char buf[20]; // 18446744073709551615
;	if (NULL == buf) { printf("GUI_u64ToStr: Out of memory! Returning ""."); return ""; }
;	sprintf_s(buf, sizeof buf, "%llu", num)
;
	char *ret = malloc(strlen(buf) + 1)
;	if (NULL == ret) { return ""; }
;	strcpy_s(ret, strlen(buf) + 1, buf)
;	return ret;
; }

/*Example:
	char *str = GUI_Join(", ", 3, "a1", "bb2", "ccc3")
;	printf("%s\n", str) // a1, bb2, ccc3
;	free(str)
;
*/
char *GUI_Join(char *separator, i32 len, ...) {
	char buf[1024] = ""
;{	va_list list
;	va_start(list, len)
;	for (; len > 1; len--) {
		char *str = va_arg(list, char*)
	;	strcat_s(buf, 1024, str)
	;	strcat_s(buf, 1024, separator)
	;}
	strcat_s(buf, 1024, va_arg(list, char*))
;	va_end(list)
;}
	char *ret = malloc(strlen(buf) + 1)
;	if (NULL == ret) { return ""; }
;	strcpy_s(ret, strlen(buf) + 1, buf)
;	return ret
; }
