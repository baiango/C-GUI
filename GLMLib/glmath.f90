module GLModule
	implicit none
	type :: Vec2i
		integer :: x, y
	end type Vec2i
	! There are no unsigned in Fortran
	type :: Vec2f
		real :: x, y
	end type Vec2f
	type :: Vec2d
		double precision :: x, y
	end type Vec2d

	type :: Vec3i
		integer :: x, y, z
	end type Vec3i
	type :: Vec3f
		real :: x, y, z
	end type Vec3f
	type :: Vec3f64
		double precision :: x, y, z
	end type Vec3f64

	type :: Col8
		integer(1) :: r, g, b
	end type Col8
	type :: Col8a
		integer(1) :: r, g, b, a
	end type Col8a
	type :: Col5
		integer(2) :: rgba
	end type Col5

	! The reason for 1D is easier to expend with multiplications
	! 2D flattening will require divisions and modulo,
	! which is slower than multiplications
	type :: Mat4
		real :: data(16)
	end type Mat4
end module GLModule


subroutine set_vec3f_from_floats(vec, x, y, z)
	use GLModule
	type(Vec3f), intent(out) :: vec
	real, intent(in) :: x, y, z

	vec%x = x
	vec%y = y
	vec%z = z
end subroutine set_vec3f_from_floats


subroutine set_vec3f_from_vec3f(vec, src)
	use GLModule
	type(Vec3f), intent(out) :: vec
	type(Vec3f), intent(in) :: src

	vec%x = src%x
	vec%y = src%y
	vec%z = src%z
	end subroutine set_vec3f_from_vec3f


subroutine mul_mat4(mat, a, b)
	use GLModule
	type(Mat4), intent(out) :: mat
	type(Mat4), intent(in) :: a, b
	integer :: i, j, k

	! Not a good idea to do loop fusion on this
	do i = 1, 16
		mat%data(i) = 0.0
	end do

	do i = 1, 4
		do j = 1, 4
			do k = 1, 4
				mat%data(4 * j + i) = mat%data(4 * j + i) + (a%data(4 * k + i) * b%data(4 * j + k))
			end do
		end do
	end do
	end subroutine mul_mat4


! fovy: Vertical field of view angle in degrees.
! aspect: Aspect ratio of the viewport. It's calculated as width / height.
! z_near: Objects closer than this value will not be visible.
! z_far: Objects farther away than this value will not be visible.
subroutine perspective(mat, fovy, aspect, z_near, z_far)
	use GLModule
	type(Mat4), intent(out) :: mat
	real, intent(in) :: fovy, aspect, z_near, z_far
	integer :: i
	real :: tan_half_fovy

	tan_half_fovy = tan(fovy / 2.0);

	do i = 1, 16
		mat%data(i) = 0.0
	end do

	mat%data(1) = 1.0 / (aspect * tan_half_fovy)
	mat%data(4 * 1 + 2) = 1.0 / tan_half_fovy
	mat%data(4 * 2 + 3) = -(z_far + z_near) / (z_far - z_near)
	mat%data(4 * 2 + 4) = -1.0
	mat%data(4 * 3 + 3) = -(2.0 * z_far * z_near) / (z_far - z_near)
end subroutine perspective


subroutine add_vec3f(vec, a, b)
	use GLModule
	type(Vec3f), intent(out) :: vec
	type(Vec3f), intent(in) :: a, b

	vec%x = a%x + b%x
	vec%y = a%y + b%y
	vec%z = a%z + b%z
end subroutine add_vec3f


subroutine sub_vec3f(vec, a, b)
	use GLModule
	type(Vec3f), intent(out) :: vec
	type(Vec3f), intent(in) :: a, b

	vec%x = a%x - b%x
	vec%y = a%y - b%y
	vec%z = a%z - b%z
end subroutine sub_vec3f


subroutine normalize_vec3f(vec)
	use GLModule
	type(Vec3f), intent(out) :: vec
	real :: length, inv_length

	if (abs(vec%x + vec%y + vec%z) < 1.0e-6) then
		write(*, *) "normalize_vec3f: Zeroed vector found! Skipping..."
		return
	endif

	length = &
		sqrt(vec%x * vec%x &
			+ vec%y * vec%y &
			+ vec%z * vec%z)
	inv_length = 1.0 / length
	vec%x = vec%x * inv_length
	vec%y = vec%y * inv_length
	vec%z = vec%z * inv_length
end subroutine normalize_vec3f


subroutine cross(vec, a, b)
	use GLModule
	type(Vec3f), intent(out) :: vec
	type(Vec3f), intent(in) :: a, b

	vec%x = a%y * b%z - a%z * b%y
	vec%y = a%z * b%x - a%x * b%z
	vec%z = a%x * b%y - a%y * b%x
end subroutine cross


subroutine dot(ret, a, b)
	use GLModule
	real, intent(out) :: ret
	type(Vec3f), intent(in) :: a, b

	ret = a%x * b%x &
		+ a%y * b%y &
		+ a%z * b%z
end subroutine dot


subroutine lookat(mat, pos_camera, look_at, i_up)
	use GLModule
	type(Mat4), intent(out) :: mat
	type(Vec3f), intent(in) :: pos_camera, look_at, i_up
	type(Vec3f) :: forward, right, up
	interface
		subroutine normalize_vec3f(vec)
			use GLModule
			type(Vec3f), intent(out) :: vec
		end subroutine normalize_vec3f

		subroutine cross(vec, a, b)
			use GLModule
			type(Vec3f), intent(out) :: vec
			type(Vec3f), intent(in) :: a, b
		end subroutine cross

		subroutine dot(ret, a, b)
			use GLModule
			real, intent(out) :: ret
			type(Vec3f), intent(in) :: a, b
		end subroutine dot
	end interface


	forward%x = look_at%x - pos_camera%x
	forward%y = look_at%y - pos_camera%y
	forward%z = look_at%z - pos_camera%z
	call normalize_vec3f(forward)

	call cross(right, forward, i_up)
	call normalize_vec3f(right)

	call cross(up, right, forward)

	mat%data(1) = right%x
	mat%data(2) = up%x
	mat%data(3) = -forward%x
	mat%data(4) = 0.0

	mat%data(4 * 1 + 1) = right%y
	mat%data(4 * 1 + 2) = up%y
	mat%data(4 * 1 + 3) = -forward%y
	mat%data(4 * 1 + 4) = 0.0

	mat%data(4 * 2 + 1) = right%z
	mat%data(4 * 2 + 2) = up%z
	mat%data(4 * 2 + 3) = -forward%z
	mat%data(4 * 2 + 4) = 0.0

	call dot(mat%data(4 * 3 + 1), right, pos_camera)
	call dot(mat%data(4 * 3 + 2), up, pos_camera)
	mat%data(4 * 3 + 1) = -mat%data(4 * 3 + 1)
	mat%data(4 * 3 + 2) = -mat%data(4 * 3 + 2)
	call dot(mat%data(4 * 3 + 3), forward, pos_camera)
	mat%data(4 * 3 + 4) = 0.0
end subroutine lookat


subroutine ortho(mat, left, right, bottom, top, z_near, z_far)
	use GLModule
	type(Mat4), intent(out) :: mat
	real, intent(in) :: left, right, bottom, top, z_near, z_far

	mat%data(1) = 2.0 / (right - left)
	mat%data(2) = 0.0
	mat%data(3) = 0.0
	mat%data(4) = 0.0

	mat%data(4 * 1 + 1) = 0.0
	mat%data(4 * 1 + 2) = 2.0 / (top - bottom)
	mat%data(4 * 1 + 3) = 0.0
	mat%data(4 * 1 + 4) = 0.0

	mat%data(4 * 2 + 1) = 0.0
	mat%data(4 * 2 + 2) = 0.0
	mat%data(4 * 2 + 3) = -2.0 / (z_far - z_near)
	mat%data(4 * 2 + 4) = 0.0

	mat%data(4 * 3 + 1) = -(right + left) / (right - left)
	mat%data(4 * 3 + 2) = -(top + bottom) / (top - bottom)
	mat%data(4 * 3 + 3) = -(z_far + z_near) / (z_far - z_near)
	mat%data(4 * 3 + 4) = 1.0
end subroutine ortho
