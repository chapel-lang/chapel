module ChapelInterface
  use ISO_C_BINDING

  interface
    subroutine chpl_library_init_ftn() bind(C, name="chpl_library_init_ftn")
    end subroutine chpl_library_init_ftn

    subroutine chpl_library_finalize() bind(C, name="chpl_library_finalize")
    end subroutine chpl_library_finalize

    subroutine setint(i) bind(C, name="setint")
      import c_int64_t
      integer(kind=c_int64_t), value :: i
    end subroutine setint

    subroutine setreal(r) bind(C, name="setreal")
      import c_double
      real(kind=c_double), value :: r
    end subroutine setreal

    function getint() bind(C, name="getint")
      import c_int64_t
      integer(kind=c_int64_t) :: getint
    end function getint

    function getreal() bind(C, name="getreal")
      import c_double
      real(kind=c_double) :: getreal
    end function getreal
  end interface
end module ChapelInterface

program callChapel
  use ChapelInterface
  implicit none

  integer(8) :: i, j
  real(8) :: r, s
  i = 1
  r = 2.718281828
  call chpl_library_init_ftn()
  call setReal(r)
  call setInt(i)

  s = getReal()
  j = getInt()

  print *, s
  print *, j
  call chpl_library_finalize()
end program callChapel
