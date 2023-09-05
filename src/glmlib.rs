#![allow(dead_code)]
#[forbid(private_in_public)]


#[repr(C)]
pub struct Vec2i {
	pub x: i32,
	pub y: i32,
}
#[repr(C)]
pub struct Vec2f {
	pub x: f32,
	pub y: f32,
}
#[repr(C)]
pub struct Vec2d {
	pub x: f64,
	pub y: f64,
}


#[repr(C)]
pub struct Vec3i {
	pub x: i32,
	pub y: i32,
	pub z: i32,
}
#[repr(C)]
pub struct Vec3f {
	pub x: f32,
	pub y: f32,
	pub z: f32,
}
#[repr(C)]
pub struct Vec3d {
	pub x: f64,
	pub y: f64,
	pub z: f64,
}

#[repr(C)]
pub struct Col8 {
	pub r: u8,
	pub g: u8,
	pub b: u8
}
#[repr(C)]
pub struct Col8a {
	pub r: u8,
	pub g: u8,
	pub b: u8,
	pub a: u8
}
#[repr(C)]
pub struct Col5a {
	pub rgba: u16
}

#[repr(C)]
pub struct Mat4 {
	pub data: [f32; 16],
}


extern {
	// Declare the Fortran subroutine and its arguments
	#[link_name = "glmath_mp_set_vec3f_from_floats"]
	pub fn set_vec3f_from_floats(
		vec: *mut Vec3f, x: *const f32, y:
		*const f32, z: *const f32
	);

	#[link_name = "glmath_mp_set_vec3f_from_vec3f"]
	pub fn set_vec3f_from_vec3f(vec: *mut Vec3f, src: *const Vec3f);

	#[link_name = "glmath_mp_mul_mat4"]
	pub fn mul_mat4(mat: *mut Mat4, a: *const Mat4, b: *const Mat4);

	#[link_name = "glmath_mp_perspective"]
	pub fn perspective(
		mat: *mut Mat4, fovy: *const f32, aspect: *const f32,
		z_near: *const f32, z_far: *const f32
	);

	#[link_name = "glmath_mp_add_vec3f"]
	pub fn add_vec3f(vec: *mut Vec3f, a: *const Vec3f, b: *const Vec3f);

	#[link_name = "glmath_mp_sub_vec3f"]
	pub fn sub_vec3f(vec: *mut Vec3f, a: *const Vec3f, b: *const Vec3f);

	#[link_name = "glmath_mp_normalize_vec3f"]
	pub fn normalize_vec3f(vec: *mut Vec3f);

	#[link_name = "glmath_mp_cross"]
	pub fn cross(vec: *mut Vec3f, a: *const Vec3f, b: *const Vec3f);

	#[link_name = "glmath_mp_dot"]
	pub fn dot(vec: *mut Vec3f, a: *const Vec3f, b: *const Vec3f);

	#[link_name = "glmath_mp_lookat"]
	pub fn lookat(
		mat: *mut Mat4, pos_camera: *const Vec3f,
		look_at: *const Vec3f, i_up: *const Vec3f
	);

	#[link_name = "glmath_mp_ortho"]
	pub fn ortho(mat: *mut Mat4, left: *const f32,
		right: *const f32, bottom: *const f32, top: *const f32,
		z_near: *const f32, z_far: *const f32
	);
}

extern {
	#[link_name = "glhelp_mp_prt_mat4"]
	pub fn prt_mat4(mat: *const Mat4);
}
