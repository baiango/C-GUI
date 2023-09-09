use std::alloc::{alloc, dealloc, Layout};
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
	fn get_layout(size: usize) -> Layout {
		// Calculate the layout for the elements of the slice
		let generic = Layout::new::<T>();

		Layout::from_size_align(
			size * generic.size(),
			generic.align()
		)
			.unwrap()
	}

	pub fn new<'a>(size: usize) -> Self {
		// Calculate the layout for the slice
		let layout = Self::get_layout(size);

		// Allocate memory for the slice
		let ptr = unsafe { alloc(layout) as *mut T };
		if ptr.is_null() { panic!("Memory allocation failed."); }

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(ptr, 0, size); }

		unsafe { CVec { data: slice::from_raw_parts_mut(ptr, size) } }
	}

	pub fn len(&self) -> usize { self.data.len() }

	pub fn resize<'a>(&mut self, size: usize) {
		let new_layout = Self::get_layout(size);

		// Allocate memory for the slice
		let new_ptr = unsafe { alloc(new_layout) as *mut T };
		if new_ptr.is_null() { panic!("Memory allocation failed."); }

		let copy_len = std::cmp::min(self.data.len(), size);
		let new_slice;
		unsafe {
		ptr::copy_nonoverlapping(self.data.as_ptr(), new_ptr, copy_len);
		new_slice = slice::from_raw_parts_mut(new_ptr, size);
		}

		self.data = new_slice;
	}
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
pub(crate) struct FlxVec<'a, T> {
	pub outer_slices_slice: &'a mut [CVec<'a, T>],
	pub inner_len: usize,
	pub outer_len: usize,
	pub size: usize
}

impl<T: Copy> FlxVec<'_, T> {
	// Don't use self in here, because people won't realize it requires outer_len
	// without reading these huge chunks of code
	fn new_outer<'a>(outer_len: usize) -> &'a mut [CVec<'a, T>] {
		// ----- Outer ptrs ----- //
		let outer_ptrs_layout = Layout::array::<CVec<T>>(outer_len).unwrap();
		let outer_ptrs = unsafe { alloc(outer_ptrs_layout) as *mut CVec<T> };
		if outer_ptrs.is_null() { panic!("Memory allocation for outer ptrs failed.") }

		// ----- Outer slices ----- //
		// The first index of the slice is the pointer of the outer pointer
		unsafe { slice::from_raw_parts_mut(outer_ptrs, outer_len) }
	}

	pub fn new<'a>(size: usize) -> Self {
		let generic_layout = Layout::new::<T>();
		let inner_len = 32768 / generic_layout.size();
		let outer_len = size / inner_len + 1;

		// ----- Outer slices ----- //
		let outer_slices_slice = Self::new_outer(outer_len);

		// ----- Inner slices ----- //
		for i in 0..outer_len{
			let mut inner_size = inner_len;
			if i == outer_len - 1 { inner_size = size & (inner_len - 1); }

			outer_slices_slice[i] = CVec::<T>::new(inner_size);
		}

		FlxVec {
			outer_slices_slice,
			inner_len,
			outer_len,
			size
		}
	}

	pub fn resize<'a>(&mut self, size: usize) {
		let outer_len = size / self.inner_len + 1;

		let new_outer_slices_slice = Self::new_outer(outer_len);

		// ----- if size is smaller ----- //
		// ----- Fill CVec ----- //
		for i in 0..outer_len {
			let mut inner_size = self.inner_len;
			if i == outer_len - 1 { inner_size = size & (self.inner_len - 1); }

			new_outer_slices_slice[i] = CVec::<T>::new(inner_size);
		}
		if self.size > size {
			// ----- Copy ----- //
			for i in 0..outer_len - 1 {
				for j in 0..self.inner_len {
					new_outer_slices_slice[i][j] = self.outer_slices_slice[i][j];
				}
			}
			// ----- Copy last ----- //
			for i in 0..size & (self.inner_len - 1) {
				new_outer_slices_slice[outer_len - 1][i] = self.outer_slices_slice[outer_len - 1][i];
			}
		}

		self.outer_slices_slice = new_outer_slices_slice;
		self.outer_len = outer_len;
		self.size = size;
	}
}

impl<T> Index<usize> for FlxVec<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output {
		&self.outer_slices_slice[index >> self.inner_len.trailing_zeros()][index & (self.inner_len - 1)]
	}
}

impl<T> IndexMut<usize> for FlxVec<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output {
		&mut self.outer_slices_slice[index >> self.inner_len.trailing_zeros()][index & (self.inner_len - 1)]
	}
}
