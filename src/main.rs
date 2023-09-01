extern {
	// Declare the Fortran subroutine and its arguments
	fn CGUI_CROSS(vec: *mut Vec3f, a: *const Vec3f, b: *const Vec3f);
}

// Define the Vec3f type for Rust
#[repr(C)]
pub struct Vec3f {
	pub x: f32,
	pub y: f32,
	pub z: f32,
}

extern crate glfw;

use glfw::{Action, Context, Key};

fn main() {
	// Initialize Vec3f instances
	let mut result = Vec3f { x: 0.0, y: 0.0, z: 0.0 };
	let a = Vec3f { x: 1.0, y: 2.0, z: 3.0 };
	let b = Vec3f { x: 4.0, y: 5.0, z: 6.0 };

	// Call the Fortran subroutine from Rust
	unsafe {
		CGUI_CROSS(&mut result, &a, &b);
	}

	// Print the result
	println!("Result: x={}, y={}, z={}", result.x, result.y, result.z);

	let mut glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();

	let (mut window, events) = glfw.create_window(300, 300, "Hello this is window", glfw::WindowMode::Windowed)
		.expect("Failed to create GLFW window.");
	window.make_current();
	window.set_key_polling(true);

	while !window.should_close() {
		// Input
		glfw.poll_events();
		for (_, event) in glfw::flush_messages(&events) {
			println!("{:?}", event);
			match event {
				glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => {
					window.set_should_close(true)
				},
				_ => {},
			}

			// window.

		// End
		window.swap_buffers();
		}
	}
}
