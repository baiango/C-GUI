module GLMLib
    implicit none
    type :: Vec3f
        real :: x, y, z
    end type Vec3f

    public :: cgui_cross

    contains

    subroutine cgui_cross(vec, a, b)
        type(Vec3f), intent(out) :: vec
        type(Vec3f), intent(in) :: a, b

        vec%x = a%y * b%z - a%z * b%y
        vec%y = a%z * b%x - a%x * b%z
        vec%z = a%x * b%y - a%y * b%x
    end subroutine cgui_cross
end module GLMLib
