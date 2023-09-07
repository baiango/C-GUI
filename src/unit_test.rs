use crate::{glmlib, calloc};
use crate::glmlib::Vec3f;


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
	let mut intvec = calloc::CVec::<i32>::new(10).unwrap();
	intvec[9] = 100;
	println!("{:?}", intvec);

	let mut strvec = calloc::CVec::<&str>::new(10).unwrap();
	strvec[5] = "123";
	println!("{:?}", strvec);

	// ----- fust_gui::calloc::FlxVec ----- //
	let mut intvec = calloc::FlxVec::<u64>::new(10000).unwrap();
	intvec[8193] = 123;
	intvec[9999] = 9999;
	println!("{:?}", intvec);
	println!("{:?}", intvec[8193]);
	println!("{:?}", intvec[9999]);

	intvec.resize(9);
	// println!("{:?}", intvec);

	// Compiling error
	//let mut strvec = calloc::FlxVec::<&str>::new(10).unwrap();
}
