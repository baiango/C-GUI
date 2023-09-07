use std::alloc::{alloc, dealloc, Layout};
use std::cmp::max;
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

	pub fn size(&self) -> usize { self.len }
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
	pub oned_ptrs: &'a mut [*mut T],
	pub oned_slices: &'a mut [&'a mut [T]],
	pub twod_ptr: *mut *mut T,
	pub twod_slice: *mut &'a mut [T],
	pub oned_len: usize,
	pub oned_log2_len: u8,
	pub twod_len: usize,
	pub len: usize,
}

impl<T: IsNumber> FlxVec<'_, T> {
	pub fn new(size: usize) -> Result<Self, &'static str> {
		// Calculate the layout for the elements of the slice
		const ONED_BYTE_SIZE: usize = 32 * 1024;
		let element_layout = Layout::new::<T>();

		let oned_size = ONED_BYTE_SIZE / element_layout.size();
		let twod_size = size / oned_size + 1;

		// ----- 2D pointers ----- //
		// Calculate the layout for the slice
		let twod_layout_ptr = Layout::from_size_align(
			twod_size * std::mem::size_of::<*mut *mut T>(),
			std::mem::align_of::<*mut *mut T>()
		)
			.unwrap();

		// Allocate memory for the pointers
		let twod_alloc_ptrs = unsafe { alloc(twod_layout_ptr) as *mut *mut T };
		if twod_alloc_ptrs.is_null() { return Err("Memory allocation failed."); }

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(twod_alloc_ptrs, 0, twod_size); }

		let twod_array_ptrs = unsafe { std::slice::from_raw_parts_mut(twod_alloc_ptrs, twod_size) };

		// ----- 2D layout ----- //
		let twod_layout_data = Layout::from_size_align(
			twod_size * std::mem::size_of::<*mut &mut [T]>(),
			std::mem::align_of::<*mut &mut [T]>()
		)
			.unwrap();

		let twod_alloc_slice = unsafe { alloc(twod_layout_data) as *mut &mut [T] };
		if twod_alloc_slice.is_null() { return Err("Memory allocation failed."); }

		// Invalid memory access????
		unsafe { ptr::write_bytes(twod_alloc_slice, 0, twod_size); }

		let twod_array_slices = unsafe { std::slice::from_raw_parts_mut(twod_alloc_slice, twod_size) };

		// ----- 1D ----- //
		let oned_layout = Layout::from_size_align(
			ONED_BYTE_SIZE,
			std::mem::align_of::<T>()
		)
			.unwrap();

		for i in 0..twod_size {
			let oned_alloc_ptr = unsafe { alloc(oned_layout) as *mut T };
			if oned_alloc_ptr.is_null() { return Err("Memory allocation failed."); }

			unsafe { ptr::write_bytes(oned_alloc_ptr, 0, oned_size); }

			let oned_array = unsafe { std::slice::from_raw_parts_mut(oned_alloc_ptr, oned_size) };

			twod_array_ptrs[i] = oned_alloc_ptr;
			// Invalid memory access????
			twod_array_slices[i] = oned_array;
		}
		// ----- 1D last index ----- //
		let oned_alloc_ptr = unsafe { alloc(oned_layout) as *mut T };
		if oned_alloc_ptr.is_null() { return Err("Memory allocation failed."); }

		unsafe { ptr::write_bytes(oned_alloc_ptr, 0, size & (oned_size - 1)); }

		let oned_array = unsafe { std::slice::from_raw_parts_mut(oned_alloc_ptr, size & (oned_size - 1)) };

		twod_array_ptrs[twod_size - 1] = oned_alloc_ptr;
		// Invalid memory access????
		twod_array_slices[twod_size - 1] = oned_array;

		Ok(FlxVec {
			oned_ptrs: twod_array_ptrs,
			oned_slices: twod_array_slices,
			twod_ptr: twod_alloc_ptrs,
			twod_slice: twod_alloc_slice,
			oned_len: oned_size,
			oned_log2_len: oned_size.trailing_zeros() as u8,
			twod_len: twod_size,
			len: size,
		})
	}

	pub fn size(&self) -> usize { self.len }

	pub fn resize(&mut self, size: usize) {
		let old_len = self.twod_len;
		self.twod_len = size / self.oned_len + 1;
		println!("{} {}", size, self.twod_len);

		unsafe {
		// ----- 1D ----- //
		for i in max(0, self.twod_len - 1)..old_len - 1 {
			dealloc(
				self.oned_ptrs[i] as *mut u8,
				Layout::from_size_align_unchecked(self.oned_len * std::mem::size_of::<T>(), std::mem::align_of::<T>()),
			);
		}
		dealloc(
			self.oned_ptrs[old_len - 1] as *mut u8,
			Layout::from_size_align_unchecked((self.len & (self.oned_len - 1)) * std::mem::size_of::<T>(), std::mem::align_of::<T>()),
		);
		}

		self.len = size;
	}
}

impl<T: IsNumber> Index<usize> for FlxVec<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output {
		// Invalid memory access????
		&self.oned_slices[index >> self.oned_log2_len][index & (self.oned_len - 1)]
	}
}

impl<T: IsNumber> IndexMut<usize> for FlxVec<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output {
		// Invalid memory access????
		&mut self.oned_slices[index >> self.oned_log2_len][index & (self.oned_len - 1)]
	}
}

impl<T: IsNumber> Drop for FlxVec<'_, T> {
	fn drop(&mut self) {
		unsafe {
		// ----- 1D ----- //
		for i in 0..self.twod_len - 1 {
			dealloc(
				self.oned_ptrs[i] as *mut u8,
				Layout::from_size_align_unchecked(self.oned_len * std::mem::size_of::<T>(), std::mem::align_of::<T>()),
			);
		}
		dealloc(
			self.oned_ptrs[self.twod_len - 1] as *mut u8,
			Layout::from_size_align_unchecked((self.len & (self.oned_len - 1)) * std::mem::size_of::<T>(), std::mem::align_of::<T>()),
		);
		// ----- 2D ----- //
		dealloc(
			self.twod_ptr as *mut u8,
			Layout::from_size_align_unchecked(self.twod_len * std::mem::size_of::<*mut *mut T>(), std::mem::align_of::<T>()),
		);
		dealloc(
			self.twod_slice as *mut u8,
			Layout::from_size_align_unchecked(self.twod_len * std::mem::size_of::<*mut &mut [T]>(), std::mem::align_of::<T>()),
		);
		}
	}
}
