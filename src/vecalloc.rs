use std::alloc::{alloc, Layout};
use std::{ptr, slice};
use std::ops::{Index, IndexMut};


// This(CVec) is for reuses. It can't resize very fast like FlxVec.
// But it write and read faster than FlxVec because it doesn't stall like FlxVec
// FlxVec is made for frequently resized arrays and numbers.
// It has a size of 32 KiB array, so it will resize very fast.
// But it will stall on every 32 KiB boundary
#[derive(Debug)]
pub(crate) struct CVec<'a, T> {
	pub data: &'a mut [T], // You need it to access the array
}

impl<T> CVec<'_, T> {
	pub fn new<'a>(size: usize) -> Result<Self, &'a str> {
		// Calculate the layout for the elements of the slice
		let generic = Layout::new::<T>();

		// Calculate the layout for the slice
		let layout = Layout::from_size_align(
			size * generic.size(),
			generic.align()
		)
			.unwrap();

		// Allocate memory for the slice
		let ptr = unsafe { alloc(layout) as *mut T };
		if ptr.is_null() { return Err("Memory allocation failed."); }

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(ptr, 0, size); }

		let data = unsafe { slice::from_raw_parts_mut(ptr, size) };

		Ok(CVec { data })
	}

	pub fn len(&self) -> usize { self.data.len() }
}

impl<T> Index<usize> for CVec<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output { &self.data[index] }
}

impl<T> IndexMut<usize> for CVec<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output { &mut self.data[index] }
}


pub trait IsNumber {}
impl IsNumber for i8 {}
impl IsNumber for i16 {}
impl IsNumber for i32 {}
impl IsNumber for i64 {}
impl IsNumber for u8 {}
impl IsNumber for u16 {}
impl IsNumber for u32 {}
impl IsNumber for u64 {}
impl IsNumber for f32 {}
impl IsNumber for f64 {}


// Work in progress
#[derive(Debug)]
pub(crate) struct FlxVec<'a, T: IsNumber> {
	pub outer_slices_slice: Option<&'a mut [&'a mut [T]]>,
	pub inner_len: usize,
	pub outer_len: usize,
	pub size: usize
}

impl<T: IsNumber + std::fmt::Debug> FlxVec<'_, T> {
	// Don't use self in here, because people won't realize it requires outer_len
	// without reading these huge chunks of code
	fn new_outer<'a>(outer_len: usize) -> Result<Option<&'a mut [&'a mut [T]]>, &'a str> {
		// ----- Outer ptrs ----- //
		let outer_ptrs_layout = Layout::array::<&mut [T]>(outer_len).unwrap();
		let outer_ptrs = unsafe { alloc(outer_ptrs_layout) as *mut &mut [T] };
		if outer_ptrs.is_null() { return Err("Memory allocation for outer ptrs failed.") }

		// ----- Outer slices ----- //
		// The first index of the slice is the pointer of the outer pointer
		unsafe { Ok(Some(slice::from_raw_parts_mut(outer_ptrs, outer_len))) }
	}

	pub fn new<'a>(size: usize) -> Result<Self, &'a str> {
		let generic_layout = Layout::new::<T>();
		let inner_len = 32768 / generic_layout.size();
		let outer_len = size / inner_len + 1;

		// ----- Outer slices ----- //
		let mut outer_slices_slice = Self::new_outer(outer_len).unwrap();

		// ----- Inner slices ----- //
		for i in 0..outer_len{
			let mut inner_size = inner_len;
			if i == outer_len - 1 { inner_size = size & (inner_len - 1); }

			let inner_layout = Layout::array::<T>(inner_size).unwrap();
			let inner_ptr = unsafe { alloc(inner_layout) as *mut T };
			if inner_ptr.is_null() { return Err("Memory allocation for inner pointers failed.") }

			let inner_slice = unsafe { slice::from_raw_parts_mut(
				inner_ptr, inner_size
			) };

			unsafe { ptr::write_bytes(inner_slice.as_mut_ptr(), 0, inner_size); }

			let outer_slices_vec = outer_slices_slice.as_mut()
				.expect("outer_slices_vec should not be null");
			outer_slices_vec[i] = inner_slice;
		}

		Ok(FlxVec {
			outer_slices_slice,
			inner_len,
			outer_len,
			size
		})
	}

	pub fn resize<'a>(self, size: usize) -> Result<Self, &'a str> {
		let outer_len = size / self.inner_len + 1;

		// Create a new FlxVec
		let new_outer_slices_slice = Self::new_outer(outer_len).unwrap();

		Ok(FlxVec {
			outer_slices_slice: new_outer_slices_slice,
			inner_len: self.inner_len,
			outer_len,
			size,
		})
	}
}

impl<T: IsNumber> Index<usize> for FlxVec<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output {
		let outer_slices_vec = self.outer_slices_slice.as_ref()
			.expect("outer_slices_slice should not be null");

			&outer_slices_vec[index >> self.inner_len.trailing_zeros()][index & (self.inner_len - 1)]
	}
}

impl<T: IsNumber> IndexMut<usize> for FlxVec<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output {
		let outer_slices_vec = self.outer_slices_slice.as_mut()
			.expect("outer_slices_slice should not be null");

			&mut outer_slices_vec[index >> self.inner_len.trailing_zeros()][index & (self.inner_len - 1)]
	}
}
