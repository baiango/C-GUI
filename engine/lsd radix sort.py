def radix_sort(arr):
	current_digit = 1
	# It stop when there's no more digits to sort
	while max(arr) // current_digit > 0:
		# Counting numbers in base 256
		prefix_sum = [0] * 256
		for n in arr:
			prefix_sum[n // current_digit & 255] += 1

		# Building the prefix sum list
		for i in range(1, 256):
			prefix_sum[i] += prefix_sum[i - 1]

		# Add the elements back to sort it
		swap = [0] * len(arr)
		for n in reversed(arr):
			prefix_sum[n // current_digit & 255] -= 1
			swap[prefix_sum[n // current_digit & 255]] = n

		# Swapping the array with the swap
		for i, n in enumerate(swap):
			arr[i] = n

		# Moving the algorithm to the next base 256 digit
		current_digit <<= 8

import time

arr = [170, 45, 75, 90, 802, 24, 2, 66]
arr = list(reversed(range(1_000_000)))

start_time = time.time()
radix_sort(arr)
print("--- %s seconds ---" % (time.time() - start_time))

if arr != [2, 24, 45, 66, 75, 90, 170, 802]: print("Error!")
print("Sorted array:", arr[-5:])
