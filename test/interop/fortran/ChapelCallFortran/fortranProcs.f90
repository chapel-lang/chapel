module TestProcs
  use iso_c_binding
  implicit none

  integer(8), bind(C, name='fortInt') :: fortInt = 0
  real(8), bind(C, name='fortReal') :: fortReal = 0.0

  contains

  function getInt() bind(C, name='getInt')
    integer(8) :: getInt
    getInt = fortInt
  end function getInt

  function getReal() bind(C, name='getReal')
    real(8) :: getReal
    getReal = fortReal
  end function getReal

  subroutine setInt(i) bind(C, name='setInt')
    integer(8) :: i
    fortInt = i
  end subroutine setInt

  subroutine setReal(r) bind(C, name='setReal')
    real(8) :: r
    fortReal = r
  end subroutine setReal

end module TestProcs

!program run
!  use TestProcs
!  call setInt(INT(42, 8))
!  print *, "Hello from Fortran: ", getInt()
!end program run
