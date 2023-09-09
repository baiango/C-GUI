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
