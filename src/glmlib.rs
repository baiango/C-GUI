// Define the Vec3f type for Rust
#[repr(C)]
pub struct Vec3f {
	pub x: f32,
	pub y: f32,
	pub z: f32,
}

extern {
	// Declare the Fortran subroutine and its arguments
	pub fn cross(vec: *mut Vec3f, a: *const Vec3f, b: *const Vec3f);
}
