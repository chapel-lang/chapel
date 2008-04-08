program main
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_mov_f90
  use armci_nbmov
  use testa_init
  type(armci_slice) :: src_slice, dst_slice
!  integer(kind=i4), pointer :: a1(:)

  integer :: remote_proc
  integer :: lb(7), ub(7), rc, i, j,me, nproc
  integer :: myunit
  integer :: m
  integer :: extent(7)
  integer :: asize(7)
  type(armci_slice) :: src_slice, dst_slice
  real(kind=8), dimension(:,:), pointer :: buff
  real(kind=8) :: time1,time2,time_array(28)
  integer chunk(28)
  data chunk /1,9,16,36,81,144,256,400,576,900,1600,2304,2704,4096,&
       6084,8281,10816,16384,20164,29241,44100,65536,90000,124609,&
       160000,193600,226576, 262144/ 
  integer tbuffid,oldtbuffid 

#ifdef MPI
  integer :: ierr
  include 'mpif.h' 
  call mpi_init(ierr)
  call mpi_comm_rank(mpi_comm_world,me,ierr)
  call mpi_comm_size(mpi_comm_world,nproc,ierr)
#else
  call pbeginf()
  nproc = nnodes()
  me = nodeid()
#endif


  remote_proc = nproc -1 -me

  call armci_arr_init()
  lb(:) = 1
  ub(:) = 550*550
  ub(2) = 2
  call armci_malloc_fa(buff,lb,ub,ierr)
  tbuffid = 2

  write (6,*) 'start'
! 1d
  if( me .eq. 0) then

    do m=1,28

      src_slice%lo(1) = 1
      src_slice%lo(2) = tbuffid
      src_slice%hi(1) = chunk(m)
      src_slice%hi(2) = tbuffid
      src_slice%stride(:)=2

      dst_slice%lo(1) = 1
      dst_slice%lo(2) = tbuffid
      dst_slice%hi(1) = chunk(m)
      dst_slice%hi(2) = tbuffid
      dst_slice%stride(:)=2
      call armci_put_fa(buff,src_slice,buff,dst_slice,1,ierr)

      time1 = mpi_wtime()      

      i = 50
!     if (m .gt. 13) i = 4

      do k=1,i

        if(tbuffid .eq. 1) then
          tbuffid=2
        else 
          tbuffid=1
        endif

        src_slice%lo(2) = tbuffid
        src_slice%hi(2) = tbuffid
        dst_slice%lo(2) = tbuffid
        dst_slice%hi(2) = tbuffid

        call armci_put_fa(buff,src_slice,buff,dst_slice,1,ierr)

      enddo 

      time2 = mpi_wtime()

      time_array(m) = (time2 - time1)/i
      write (6,*) chunk(m),' ',time_array(m)
    enddo
  endif
  call mpi_barrier(mpi_comm_world,ierr)
  call armci_arr_finalize()

#ifdef MPI
  call mpi_finalize(ierr)
#else
  call pend()
#endif

end program main
