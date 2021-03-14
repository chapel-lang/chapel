program PassArray
  use chapelProcs
  real(kind=8), dimension (3,3) :: arr

  call chpl_library_init_ftn()
  call takesArray(arr)

  print *, arr

end program PassArray
