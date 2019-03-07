program callchapel
  use chapelprocs
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
