#include <iostream>

#ifdef _MSC_VER
#define C4267(x) \
	__pragma(warning(suppress : 4267)) x
#else
#define C4267(x) x
#endif


// Fast as C! But safer and with comes with autofree!
// But not as safe as std::vector.
// It's extremely hard to debug "new" since you can't see what's inside the "new" vector in the debugger.
// Please use std::vector instead and save your time.
// Or combine with std::vector to make it much easier to debug.
// Only use it if you are sure that it won't go out of bound.
// It's really easy to get "write access violation" crashes with it.
// std::vector won't slow things down too much.
// Performance issue is rarely caused by std::vector.
template <typename T>
class Cvector {
private:
	size_t list_size{};

public:
	// Variables, These are unsafe but fast.
	// The safe one will be suffixed '_s'.
	T *list{};
	// Auto free
	~Cvector() { delete[] list; }
	// Getters and setters
	size_t size() { return list_size; }

	void init(size_t size) {
		list_size = size;
		list = new T[size];
	}

	T get_s(size_t i) {
		if (i >= list_size) {
			std::cout << "Cvector: Out of index!\n";
			return T{};
		}
		return list[i];
	}

	void resize(size_t new_size) {
		T *swap = new T[new_size];
		for (size_t i = 0; i < new_size; i++) {
			if (i < list_size) {
				swap[i] = list[i];
			} else {
				swap[i] = T{};
			}
    }
		delete[] list;
		list = swap;
		list_size = new_size;
	}
	// Algorithms, not available yet.
	// Test
	void test_Cvector() {
		class Cvector<int> arr;
		arr.init(1'000'00);

		for (size_t i = 0; i < arr.size(); i++) {
			arr.list[i] = C4267(arr.size() - i);
		}
		arr.resize(5);

		for (size_t i = 0; i < 10; i++) { std::cout << arr.list[i] << " "; }
		std::cout << "\n";

		for (size_t i = 0; i < 10; i++) { std::cout << arr.get_s(i) << " "; }
		std::cout << "\n";
	}
};
