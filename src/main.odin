package ODIN_GUI

import "core:c"
// The binding is outdated. So I fork Raylib and changed it.
import rl "raylib"

main :: proc() {
// ;	flags: rl.ConfigFlags = 3
;	rl.SetConfigFlags(rl.ConfigFlag.WINDOW_RESIZABLE)
;	rl.InitWindow(1366, 768, "ODIN-GUI")
;
;	for !rl.WindowShouldClose() {
	;	rl.BeginDrawing()
	;	scene_1()
	;	draw_txt_box({500, 500})
	;	rl.EndDrawing()
	}
;	rl.CloseWindow()
}

scene_1 :: proc() {
	;	rl.ClearBackground(rl.RAYWHITE)
	;	aaa: cstring = "aaaaa!"
	;	rl.DrawText("Congrats! You created your first window!", 190, 200, 20, rl.LIGHTGRAY)
	;	rl.DrawText(aaa, 190, 240, 20, rl.LIGHTGRAY)
	;	col := DARK_BLUE_Col8
	;	rl.DrawRectangleRounded(
		rect_to_rlrect(mov_rect({300, 180}, {30, 30, 180, 100})),
		RD16_RECT.roundness, 0, col8_to_color(col)
	)
}
