use crate::{glmlib, vecalloc};
use crate::glmlib::Vec3f;
use std::time::{Instant, Duration};


pub fn test() {
	// ----- fust_gui::glmlib::cross ----- //
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

	// ----- fust_gui::glmlib::Mat4 ----- //
	let mut mymat = glmlib::Mat4{ data: [0.0; 16] };
	mymat.data[0] = 100.0;

	unsafe {
	glmlib::prt_mat4(&mymat);
	}

	// ----- fust_gui::calloc::CVec ----- //
	let mut intvec = vecalloc::CVec::<i32>::new(10);
	intvec[9] = 100;
	println!("{:?}", intvec);

	let mut strvec = vecalloc::CVec::<&str>::new(10);
	strvec[5] = "123";
	println!("{:?}", strvec);

	// ----- Resize speed test ----- //
	let run = | init_size: usize, resize_size: usize | {
		let start = Instant::now();

		let mut speedvec = vecalloc::CVec::<i32>::new(init_size);
		speedvec.resize(resize_size);

		let end = Instant::now();
		let elapsed = end - start;
		println!("CVec elapsed time: {:?}", elapsed);

		let start = Instant::now();

		let mut speedvec = Vec::<i32>::with_capacity(init_size);
		speedvec.truncate(resize_size);

		let end = Instant::now();
		let elapsed = end - start;
		println!("Vec elapsed time: {:?}", elapsed);
	};

	// ----- 3.8 MiB Normal usage ----- //
	// FlxVec elapsed time: 2.5547ms
	// CVec elapsed time: 1.3427ms
	// Vec elapsed time: 12.3µs
	run(1_000_000, 900_000);
	// ----- 3814 MiB Stress test ----- //
	// FlxVec elapsed time: 2.1915785s
	// CVec elapsed time: 11.661979s
	// Vec elapsed time: 53µs
	run(1_000_000_000, 900_000_000);

	println!("Done testing!");
}
