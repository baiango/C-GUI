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
	pub fn new<'a>(size: usize) -> Result<Self, &'a str> {
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

impl<T: IsNumber + std::fmt::Debug> FlxVec<'_, T> {
	// Don't use self in here, because people won't realize it requires outer_len
	// without reading these huge chunks of code
	fn new_outer<'a>(outer_len: usize) -> Result<(
		*mut *mut T,
		*mut &'a mut [T],
		&'a mut [*mut T],
		&'a mut [&'a mut [T]],
		), &'a str> {
		// ----- Outer ptrs ----- //
		let outer_ptrs_layout = Layout::array::<*mut T>(outer_len).unwrap();
		let outer_ptrs = unsafe { alloc(outer_ptrs_layout) as *mut *mut T };
		if outer_ptrs.is_null() { return Err("Memory allocation for outer ptrs failed.") }

		let outer_ptrs_slice = unsafe { std::slice::from_raw_parts_mut(
			outer_ptrs, outer_len
		) };
		// ----- Outer slices ----- //
		// It doesn't matter if you use outer_ptrs_layout on outer_slices
		// as long as it's the same size.
		// It's still the same size because both are storing the same amount of pointer addresses
		let outer_slices = unsafe { alloc(outer_ptrs_layout) as *mut &mut [T] };
		if outer_slices.is_null() { return Err("Memory allocation for outer slices failed.") }

		let outer_slices_slice = unsafe { std::slice::from_raw_parts_mut(
			outer_slices, outer_len
		) };

		Ok((outer_ptrs, outer_slices, outer_ptrs_slice, outer_slices_slice))
	}


	pub fn new<'a>(size: usize) -> Result<Self, &'a str> {
		let generic_layout = Layout::new::<T>();
		let inner_len = 32768 / generic_layout.size();
		let outer_len = size / inner_len + 1;

		let (outer_ptrs,
			outer_slices,
			outer_ptrs_slice,
			outer_slices_slice) = Self::new_outer(outer_len).unwrap();

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


	pub fn resize<'a>(&mut self, new_size: usize) -> Result<(), &'a str> {
		let old_outer_len = self.outer_len;
		let new_outer_len = new_size / self.inner_len + 1;

		// ----- 1. Deallocate all of the excess arrays ----- //
		// Hint: The array of std::slice are immutable. You can only deallocate it
		// You can use Option<T> to get around that, but not for me
		// And you can't copy mutable references to the new array
		let (mut new_outer_ptrs,
			mut new_outer_slices,
			mut new_outer_ptrs_slice,
			mut new_outer_slices_slice) = Self::new_outer(new_outer_len).unwrap();
		println!("{:?}", new_outer_ptrs_slice);
		println!("{:?}", new_outer_slices_slice);

		// ----- 2. Move the inner array pointers to the new outer array ----- //
		// println!("{}", new_outer_len);
		// if new_outer_len > 1 {
		// 	for i in 0..new_outer_len {
		// 		new_outer_ptrs_slice[i] = self.outer_ptrs_slice[i];
		// 		let inner_slice = unsafe { std::slice::from_raw_parts_mut(
		// 			new_outer_ptrs_slice[i], self.inner_len
		// 		) };
		// 		new_outer_slices_slice[i] = inner_slice;
		// 	}
		// }
		// new_outer_slices_slice[0] = self.outer_slices_slice[0];

		// ----- 3. Deallocate all the old outer array ----- //
		// let old_outer_ptrs_layout = Layout::array::<*mut T>(old_outer_len).unwrap();
		// unsafe {
		// dealloc(self.outer_ptrs as *mut u8, old_outer_ptrs_layout);
		// dealloc(self.outer_slices as *mut u8, old_outer_ptrs_layout);
		// }

		// ----- 4. Allocate more inner array if the new size is bigger than the old size ----- //
		self.outer_ptrs = new_outer_ptrs;
		self.outer_slices = new_outer_slices;
		self.outer_ptrs_slice = new_outer_ptrs_slice;
		self.outer_slices_slice = new_outer_slices_slice;
		self.outer_len = new_outer_len;
		self.size = new_size;
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
