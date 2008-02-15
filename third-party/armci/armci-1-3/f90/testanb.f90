program main
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_nbmov
  use testa_init
  use checkput
  type(ARMCI_slice) :: src_slice, dst_slice
!  integer(kind=I4), pointer :: a1(:)

  integer :: remote_proc
  integer :: lb(7), ub(7), rc, i, j,me, nproc
  integer :: myunit
  integer :: m
  integer :: extent(7)
  integer :: asize(7)

  call mpi_init(ierr)
  call mpi_comm_rank(mpi_comm_world, me, ierr)
  call mpi_comm_size(mpi_comm_world, nproc, ierr)

  remote_proc = nproc -1 -me
  myunit = 10+me

  call ARMCI_Arr_init()
  lb(:) = 1
  ub(:) = 7

  extent(:) = 1
  extent(:) = extent(:) + ub(:) - lb(:)
  asize(1) = extent(1)
  do m= 2,7
     asize(m) = asize(m-1)*extent(m)
  enddo

  call testanb_i4(nproc,me,remote_proc)
  call testanb_i8(nproc,me,remote_proc)
  call testanb_r4(nproc,me,remote_proc)
  call testanb_r8(nproc,me,remote_proc)
  call testanb_c4(nproc,me,remote_proc)
  call testanb_c8(nproc,me,remote_proc)

  call ARMCI_Arr_finalize()
  call mpi_finalize(ierr)

end program main
