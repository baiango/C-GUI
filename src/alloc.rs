use std::alloc::{alloc, dealloc, Layout};
use std::ptr;
use std::ops::{Index, IndexMut};


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

		if ptr.is_null() {
			return Err("Memory allocation failed.");
		}

		// Initialize all elements to 0 with write_bytes while ignoring the type
		unsafe { ptr::write_bytes(ptr, 0, size); }

		let array = unsafe { std::slice::from_raw_parts_mut(ptr, size) };

		Ok(CVec {
			data_ptr: ptr,
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
		// Deallocate the memory in the drop method
		unsafe {
			dealloc(
				self.data_ptr as *mut u8,
				Layout::from_size_align_unchecked(self.len * std::mem::size_of::<T>(), std::mem::align_of::<T>()),
			);
		}
	}
}
