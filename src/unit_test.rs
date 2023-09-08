use crate::{glmlib, vecalloc};
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
	let mut intvec = vecalloc::CVec::<i32>::new(10).unwrap();
	intvec[9] = 100;
	println!("{:?}", intvec);

	let mut strvec = vecalloc::CVec::<&str>::new(10).unwrap();
	strvec[5] = "123";
	println!("{:?}", strvec);

	// ----- fust_gui::calloc::FlxVec ----- //
	{
	// TODO: Test 0 size
	// let mut intvec = vecalloc::FlxVec::<u64>::new(20).unwrap();
	let mut intvec = vecalloc::FlxVec::<u64>::new(10000).unwrap();
	intvec[8193] = 123;
	intvec[9999] = 9999;
	for i in 0..4096 {
		intvec[i] = 4096 - i as u64;
	}
	// println!("{:?}", intvec);
	println!("{:?}", intvec[8193]);
	println!("{:?}", intvec[9999]);

	let intvec2 = intvec.resize(10).unwrap();
	println!("{:?}", intvec2);
	// Compiling error
	// let mut strvec = vecalloc::FlxVec::<&str>::new(10).unwrap();
	}
	println!("Done testing!");
}
