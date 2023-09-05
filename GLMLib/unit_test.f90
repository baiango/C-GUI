program unit_test
	use GLMath
	use GLHelp

	! Variables
	type(Mat4) :: a

	call prt_mat4(a)
	print *, ''
	do i = 1, 4
		print *, a%data(i), a%data(i + 1), a%data(i + 2), a%data(i + 3)
	end do

	! Body of unit_test
	print *, 'Hello World'

	! Pause the program
	print *, 'Press Enter to continue...'
	read(*, *)
end program unit_test
