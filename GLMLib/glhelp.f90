module GLHelp
	implicit none

	contains

	subroutine prt_mat4(mat)
		use GLMath
		type(Mat4), intent(in) :: mat
		integer :: i

		do i = 1, 4
			print *, mat%data(i), mat%data(i + 1), mat%data(i + 2), mat%data(i + 3)
		end do
	end subroutine prt_mat4
end module GLHelp
