extern crate glfw;
mod glmlib;
mod alloc;

use glfw::{Action, Context, Key};
use glmlib::Vec3f;


fn main() {
	//----- fust_gui::glmlib::cross -----//
	// Initialize Vec3f instances
	let mut result = Vec3f { x: 0.0, y: 0.0, z: 0.0 };
	let a = Vec3f { x: 1.0, y: 2.0, z: 3.0 };
	let b = Vec3f { x: 4.0, y: 5.0, z: 6.0 };

	// Call the Fortran subroutine from Rust
	unsafe {
		glmlib::cross(&mut result, &a, &b);
	}

	// Print the result
	println!("Result: x={}, y={}, z={}", result.x, result.y, result.z);

	//----- fust_gui::glmlib::Mat4 -----//
	let mut mymat = glmlib::Mat4{ data: [0.0; 16] };
	mymat.data[0] = 100.0;

	unsafe {
		glmlib::prt_mat4(&mymat);
	}

	//----- fust_gui::alloc::CVec -----//
	let mut intvec = alloc::CVec::<i32>::new(10).unwrap();
	intvec[9] = 100;
	println!("{:?}", intvec);

	let mut strvec = alloc::CVec::<&str>::new(10).unwrap();
	strvec[5] = "123";
	println!("{:?}", strvec);

	//----- glfw -----//
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

		// End
		window.swap_buffers();
		}
	}
}
