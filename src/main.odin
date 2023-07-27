package ODIN_GUI

import "core:fmt"
// import vk "vulkan"
import vk "vendor:vulkan"
import glfw "vendor:glfw"

main :: proc() {
	// sdl()
	vulkan()
}

vulkan :: proc() {
	// https://youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR
	// Tutorial 01
	glfw.Init()
	defer glfw.Terminate()
	// GLFW_NO_API means no OpenGL api. That leaves Vulkan api to you only.
	glfw.WindowHint(glfw.CLIENT_API, GLFW_NO_API)
	// Disable window resize
	glfw.WindowHint(GLFW_RESIZABLE, GLFW_FALSE)

	window := glfw.CreateWindow(800, 600, "Hello Vulkan!", nil, nil)
	defer glfw.DestroyWindow(window)

	for !glfw.WindowShouldClose(window) {
		glfw.PollEvents()
	}
}

read_file :: proc(path: string) {
	// todo
}
