use std::alloc::{alloc, dealloc, Layout};
use std::ptr;
use std::ops::{Index, IndexMut};


// This(CVec) is for reuses. It can't resize very fast like FlxVec.
// But it write and read faster than FlxVec because it doesn't stall like FlxVec
// FlxVec is made for frequently resized arrays and numbers.
// It has a size of 32 KiB array, so it will resize very fast.
// But it will stall on every 32 KiB boundary
#[derive(Debug)]
pub(crate) struct CVec<'a, T> {
	pub ptr: *mut T, // You need it to free the array
	pub data: &'a mut [T], // You need it to access the array
	pub len: usize, // You need it to get the size in constant time O(1)
}

impl<T> CVec<'_, T> {
	pub fn new(size: usize) -> Result<Self, &'static str> {
		// Calculate the layout for the elements of the slice
		let element_layout = Layout::new::<T>();

		// Calculate the layout for the slice
		let layout = Layout::from_size_align(
			size * element_layout.size(),
			element_layout.align()
		)
		.unwrap();

		// Allocate memory for the slice
		let ptr = unsafe { alloc(layout) as *mut T };

		if ptr.is_null() { return Err("Memory allocation failed."); }

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(ptr, 0, size); }

		let array = unsafe { std::slice::from_raw_parts_mut(ptr, size) };

		Ok(CVec {
			ptr: ptr,
			len: size,
			data: array,
		})
	}
}

impl<T> Index<usize> for CVec<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output { &self.data[index] }
}

impl<T> IndexMut<usize> for CVec<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output { &mut self.data[index] }
}

// C++ Destructor equivalent
impl<T> Drop for CVec<'_, T> {
	fn drop(&mut self) {
		unsafe {
			// Deallocate the memory in the drop method
		dealloc(
			self.ptr as *mut u8,
			Layout::from_size_align_unchecked(self.len * std::mem::size_of::<T>(), std::mem::align_of::<T>()),
		);
		}
	}
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
	pub outer_ptrs: *mut *mut T,
	pub outer_slices: *mut &'a mut [T],
	pub outer_ptrs_slice: &'a mut [*mut T],
	pub outer_slices_slice: &'a mut [&'a mut [T]],
	pub outer_len: usize,
	pub inner_len: usize,
	pub size: usize
}

impl<T: IsNumber> FlxVec<'_, T> {
	pub fn new(size: usize) -> Result<Self, &'static str> {
		let generic_layout = Layout::new::<T>();
		let inner_len = 32768 / generic_layout.size();
		let outer_len = size / inner_len + 1;

		// ----- Outer ptrs ----- //
		let outer_ptrs_layout = Layout::array::<*mut T>(outer_len).unwrap();
		let outer_ptrs = unsafe { alloc(outer_ptrs_layout) as *mut *mut T };
		if outer_ptrs.is_null() { return Err("Memory allocation for row pointers failed.") }

		let outer_ptrs_slice = unsafe { std::slice::from_raw_parts_mut(
			outer_ptrs, outer_len
		) };
		// ----- Outer slices ----- //
		let outer_slices = unsafe { alloc(outer_ptrs_layout) as *mut &mut [T] };
		if outer_slices.is_null() { return Err("Memory allocation for row pointers failed.") }

		let outer_slices_slice = unsafe { std::slice::from_raw_parts_mut(
			outer_slices, outer_len
		) };


		// ----- Inner slices ----- //
		let inner_layout = Layout::array::<T>(inner_len).unwrap();
		for i in 0..outer_len - 1 {
			let inner_ptr = unsafe { alloc(inner_layout) as *mut T };
			if inner_ptr.is_null() { return Err("Memory allocation for inner pointers failed.") }

			let inner_slice = unsafe { std::slice::from_raw_parts_mut(
				inner_ptr, inner_len
			) };
			unsafe { ptr::write_bytes(inner_slice.as_mut_ptr(), 0, inner_len); }

			outer_ptrs_slice[i] = inner_ptr;
			outer_slices_slice[i] = inner_slice;
		}


		// ----- Last inner slice ----- //
		let inner_ptr = unsafe { alloc(inner_layout) as *mut T };
		if inner_ptr.is_null() { return Err("Memory allocation for inner pointers failed.") }

		let inner_slice = unsafe { std::slice::from_raw_parts_mut(
			inner_ptr, size & (inner_len - 1)
		) };
		unsafe { ptr::write_bytes(inner_slice.as_mut_ptr(), 0, inner_len); }

		outer_ptrs_slice[outer_len - 1] = inner_ptr;
		outer_slices_slice[outer_len - 1] = inner_slice;

		Ok(FlxVec {
			outer_ptrs,
			outer_slices,
			outer_ptrs_slice,
			outer_slices_slice,
			outer_len,
			inner_len,
			size
		})
	}

	pub fn resize(&mut self, new_size: usize) -> Result<(), &'static str> {
		let generic_layout = Layout::new::<T>();
		let new_inner_len = 32768 / generic_layout.size();
		let new_outer_len = new_size / new_inner_len + 1;

		let old_inner_len = self.inner_len;
		let old_outer_len = self.outer_len;

		Ok(())
	}
}

impl<T: IsNumber> Index<usize> for FlxVec<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output {
		&self.outer_slices_slice[index >> self.inner_len.trailing_zeros()][index & (self.inner_len - 1)]
	}
}

impl<T: IsNumber> IndexMut<usize> for FlxVec<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output {
		&mut self.outer_slices_slice[index >> self.inner_len.trailing_zeros()][index & (self.inner_len - 1)]
	}
}
