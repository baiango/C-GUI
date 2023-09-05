use std::alloc::{alloc, dealloc, Layout};
use std::ptr;
use std::ops::{Index, IndexMut};


// This(CVec) is for general use. Like... storing the pointers
// Auto512Num and NumVec is made for auto-vectorization and numbers
// So it will compute, read, and write faster than CVec
#[derive(Debug)]
pub(crate) struct CVec<'a, T> {
	pub data_ptr: *mut T,
	pub len: usize,
	pub data: &'a mut [T],
}

impl<T> CVec<'_, T> {
	pub fn new(size: usize) -> Result<Self, &'static str> {
		// Calculate the layout for the elements of the slice
		let element_layout = Layout::new::<T>();

		// Calculate the layout for the slice
		let slice_layout = Layout::from_size_align(
			size * element_layout.size(),
			element_layout.align()
		)
		.unwrap();

		// Allocate memory for the slice
		let ptr = unsafe { alloc(slice_layout) as *mut T};

		if ptr.is_null() { return Err("Memory allocation failed."); }

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(ptr, 0, size); }

		let array = unsafe { std::slice::from_raw_parts_mut(ptr, size) };

		Ok(CVec {
			data_ptr: ptr,
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
			self.data_ptr as *mut u8,
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


// Auto512Num is not merged in NumVec,
// because the memory overhead O(n) is too high and not O(1)
#[derive(Debug)]
pub(crate) struct Auto512Num<'a, T: IsNumber> {
	pub data_ptr: *mut T,
	pub len: u8, // Need it to be smaller to used in NumVec
	pub data: &'a mut [T],
}
impl<T: IsNumber> Auto512Num<'_, T> {
	pub fn new() -> Result<Self, &'static str> {
		const BIT_SIZE: usize = 512;
		let element_layout = Layout::new::<T>();
		let data_size = BIT_SIZE / 8 / element_layout.size();

		// Calculate the layout for the slice
		let slice_layout = Layout::from_size_align(
			BIT_SIZE / 8,
			element_layout.align()
		)
		.unwrap();

		// Allocate memory for the slice
		let ptr = unsafe { alloc(slice_layout) as *mut T};
		if ptr.is_null() { return Err("Memory allocation failed."); }

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(ptr, 0, data_size); }

		let array = unsafe { std::slice::from_raw_parts_mut(ptr, data_size) };
		Ok(Auto512Num {
			data_ptr: ptr,
			len: data_size as u8,
			data: array,
		})
	}

	pub fn size(&self) -> usize { self.len as usize }
}

impl<T: IsNumber> Index<usize> for Auto512Num<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output { &self.data[index] }
}

impl<T: IsNumber> IndexMut<usize> for Auto512Num<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output { &mut self.data[index] }
}

impl<T: IsNumber> Drop for Auto512Num<'_, T> {
	fn drop(&mut self) {
		unsafe {
		dealloc(
			self.data_ptr as *mut u8,
			Layout::from_size_align_unchecked(512 / 8, std::mem::align_of::<T>()),
		);
		}
	}
}


// Work in progress
#[derive(Debug)]
pub(crate) struct NumVec<'a, T: IsNumber> {
	pub data_ptr: *mut T,
	pub alloc_len: usize,
	pub len: usize,
	pub data: &'a mut [T],
}

impl<T: IsNumber> NumVec<'_, T> {
	pub fn new(size: usize) -> Result<Self, &'static str> {
		// Calculate the layout for the elements of the slice
		let element_layout = Layout::new::<T>();

		const sizeof_avx512: usize = 64;
		let sizeof_generic = element_layout.size();

		let array_size = sizeof_avx512 / sizeof_generic;
		let ceiled_size = size + (16 - size % 16);

		// Calculate the layout for the slice
		let slice_layout = Layout::from_size_align(
			ceiled_size * sizeof_generic,
			element_layout.align()
		)
		.unwrap();

		// Allocate memory for the slice
		let ptr = unsafe { alloc(slice_layout) as *mut T};
		if ptr.is_null() { return Err("Memory allocation failed."); }

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(ptr, 0, size); }

		let array = unsafe { std::slice::from_raw_parts_mut(ptr, size) };

		Ok(NumVec {
			data_ptr: ptr,
			alloc_len: 0,
			len: size,
			data: array,
		})
	}

	pub fn size(&self) -> usize { self.len }
}

impl<T: IsNumber> Index<usize> for NumVec<'_, T> {
	type Output = T;
	fn index(&self, index: usize) -> &Self::Output { &self.data[index] }
}

impl<T: IsNumber> IndexMut<usize> for NumVec<'_, T> {
	fn index_mut(&mut self, index: usize) -> &mut Self::Output { &mut self.data[index] }
}

impl<T: IsNumber> Drop for NumVec<'_, T> {
	fn drop(&mut self) {
		unsafe {
		dealloc(
			self.data_ptr as *mut u8,
			Layout::from_size_align_unchecked(self.len * std::mem::size_of::<T>(), std::mem::align_of::<T>()),
		);
		}
	}
}
