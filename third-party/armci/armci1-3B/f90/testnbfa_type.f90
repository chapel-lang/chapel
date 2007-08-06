subroutine testanb_i4(nproc,me,remote_proc)
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_nbmov
  use testa_init
  use checkput
  implicit none
  type(ARMCI_slice) :: src_slice, dst_slice
  integer :: nproc,me,remote_proc
  integer(kind=I4), pointer :: a7_i4(:,:,:,:,:,:,:), b7_i4(:,:,:,:,:,:,:)
  integer(kind=I4), pointer :: a6_i4(:,:,:,:,:,:),   b6_i4(:,:,:,:,:,:)
  integer(kind=I4), pointer :: a5_i4(:,:,:,:,:),     b5_i4(:,:,:,:,:)
  integer(kind=I4), pointer :: a4_i4(:,:,:,:),       b4_i4(:,:,:,:)
  integer(kind=I4), pointer :: a3_i4(:,:,:),         b3_i4(:,:,:)
  integer(kind=I4), pointer :: a2_i4(:,:),           b2_i4(:,:)
  integer(kind=I4), pointer :: a1_i4(:),             b1_i4(:)
  integer(kind=I4), pointer :: v_i4(:)

  integer :: lb(7), ub(7), rc, i, j
  integer :: pass
  integer :: myunit
  integer :: m
  integer :: rank
  integer :: joff
  integer :: vshape(1)
  integer :: extent(7)
  integer :: asize(7)
  integer :: score(7)
  integer :: vlb(1),vub(1)
  integer :: afirst,bfirst

  afirst = 1+me
  bfirst = afirst * afirst

  lb(:) = 1
  ub(:) = 7

  extent(:) = 1
  extent(:) = extent(:) + ub(:) - lb(:)
  asize(1) = extent(1)
  do m= 2,7
     asize(m) = asize(m-1)*extent(m)
  enddo

! Test I4 flavor.
  vlb(1) = 1
  vub(1) = asize(7)
  call ARMCI_Malloc_fa(v_i4,vlb,vub,rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for v_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a7_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a7_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b7_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b7_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a6_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a6_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b6_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b6_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a5_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a5_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b5_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b5_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a4_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a4_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b4_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b4_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a3_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a3_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b3_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b3_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a2_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a2_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b2_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b2_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a1_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a1_i4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b1_i4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b1_i4 failed rc = ',rc
     stop
  endif

! Let all processors get allocated.

  call ARMCI_Sync()

! I4 allocations done, now loop over slices.
! We will test three flavors of slices,
! all unit stride, unit stride in the first dimension only,
! all non-unit stride (2).
  score(:) = 0
  joff = remote_proc + 1
!
! Test Put.
!
  do m = 1,3
!    Initialize arrays.
!
     call init_7d(a7_i4,b7_i4,lb,ub,lb,ub,afirst,bfirst)
     a6_i4(:,:,:,:,:,:) = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5_i4(:,:,:,:,:) = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4_i4(:,:,:,:) = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3_i4(:,:,:) = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2_i4(:,:) = a7_i4(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1_i4(:) = a7_i4(lb(1):ub(1),1,1,1,1,1,1)
     b6_i4(:,:,:,:,:,:) = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5_i4(:,:,:,:,:) = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4_i4(:,:,:,:) = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3_i4(:,:,:) = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2_i4(:,:) = b7_i4(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1_i4(:) = b7_i4(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the put.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2

! Rank 1.     
     rank = 1
     call ARMCI_NbPut_fa(a1_i4, src_slice, b1_i4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()
     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 1D I4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b1 is the piece of the sent a1.
!
     call check_b(rank,b1_i4,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 1D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 2.

     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbPut_fa(a2_i4, src_slice, b2_i4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 2D I4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b2 is the piece of the sent a2.
!
!     call check_b(rank,reshape(b2_i4,vshape),dst_slice,src_slice,remote_proc, &
!     call check_b(rank,b2_i4,dst_slice,src_slice,remote_proc, &
     v_i4(1:asize(2)) = reshape(b2_i4,vshape)

     call check_b(rank,v_i4,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 2D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 3.

     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbPut_fa(a3_i4, src_slice, b3_i4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()
     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 3D I4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b3 is the piece of the sent a3.
!
     v_i4(1:asize(3)) = reshape(b3_i4,vshape)
     call check_b(rank,v_i4,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 3D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 4.

     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbPut_fa(a4_i4, src_slice, b4_i4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()
     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 4D I4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b4 is the piece of the sent a4.
!
     v_i4(1:asize(4)) = reshape(b4_i4,vshape)
     call check_b(rank,v_i4,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 5.

     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbPut_fa(a5_i4, src_slice, b5_i4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()
     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 5D I4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b5 is the piece of the sent a5.
!
     v_i4(1:asize(5)) = reshape(b5_i4,vshape)
     call check_b(rank,v_i4,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 6.

     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbPut_fa(a6_i4, src_slice, b6_i4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 6D I4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b6 is the piece of the sent a6
!
     v_i4(1:asize(6)) = reshape(b6_i4,vshape)
     call check_b(rank,v_i4,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 6D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 7.

     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbPut_fa(a7_i4, src_slice, b7_i4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 7D I4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b7 is the piece of the sent a7
!
     v_i4(1:asize(7)) = reshape(b7_i4,vshape)
     call check_b(rank,v_i4,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 7D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbPut_fa for I4, ',m,'D passed'
        endif
     enddo
  endif
!
! Test Get.
!
  score(:) = 0
  joff = (remote_proc + 1)*(remote_proc+1)
  do m = 1,3
!
!    Initialize arrays.
!
     call init_7d(a7_i4,b7_i4,lb,ub,lb,ub,afirst,bfirst)
     a6_i4 = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5_i4 = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4_i4 = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3_i4 = a7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2_i4 = a7_i4(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1_i4 = a7_i4(lb(1):ub(1),1,1,1,1,1,1)
     b6_i4 = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5_i4 = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4_i4 = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3_i4 = b7_i4(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2_i4 = b7_i4(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1_i4 = b7_i4(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the get.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2
!
! Rank 1
!
     rank = 1
     call ARMCI_NbGet_fa(b1_i4, dst_slice, a1_i4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     call check_b(rank,a1_i4,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 1D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 2
!
     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbGet_fa(b2_i4, dst_slice, a2_i4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i4(1:asize(2)) = reshape(a2_i4,vshape)
     call check_b(rank,v_i4,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 2D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 3
!
     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbGet_fa(b3_i4, dst_slice, a3_i4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i4(1:asize(3)) = reshape(a3_i4,vshape)
     call check_b(rank,v_i4,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 3D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 4
!
     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbGet_fa(b4_i4, dst_slice, a4_i4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i4(1:asize(4)) = reshape(a4_i4,vshape)
     call check_b(rank,v_i4,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 4D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 5
!
     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbGet_fa(b5_i4, dst_slice, a5_i4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i4(1:asize(5)) = reshape(a5_i4,vshape)
     call check_b(rank,v_i4,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 5D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 6
!
     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbGet_fa(b6_i4, dst_slice, a6_i4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i4(1:asize(6)) = reshape(a6_i4,vshape)
     call check_b(rank,v_i4,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 6D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 7
!
     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbGet_fa(b7_i4, dst_slice, a7_i4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i4(1:asize(7)) = reshape(a7_i4,vshape)
     call check_b(rank,v_i4,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 7D I4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbGet_fa for I4, ',m,'D passed'
        endif
     enddo
  endif
!
! Free v, a and b arrays.
!
  call ARMCI_Sync()
  call ARMCI_Free_fa(v_i4, rc)
  call ARMCI_Free_fa(a1_i4, rc)
  call ARMCI_Free_fa(b1_i4, rc)
  call ARMCI_Free_fa(a2_i4, rc)
  call ARMCI_Free_fa(b2_i4, rc)
  call ARMCI_Free_fa(a3_i4, rc)
  call ARMCI_Free_fa(b3_i4, rc)
  call ARMCI_Free_fa(a4_i4, rc)
  call ARMCI_Free_fa(b4_i4, rc)
  call ARMCI_Free_fa(a5_i4, rc)
  call ARMCI_Free_fa(b5_i4, rc)
  call ARMCI_Free_fa(a6_i4, rc)
  call ARMCI_Free_fa(b6_i4, rc)
  call ARMCI_Free_fa(a7_i4, rc)
  call ARMCI_Free_fa(b7_i4, rc)
  call ARMCI_Sync()
  return
endsubroutine

subroutine testanb_i8(nproc,me,remote_proc)
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_nbmov
  use testa_init
  use checkput
  implicit none
  type(ARMCI_slice) :: src_slice, dst_slice
  integer :: nproc,me,remote_proc
  integer(kind=I8), pointer :: a7_i8(:,:,:,:,:,:,:), b7_i8(:,:,:,:,:,:,:)
  integer(kind=I8), pointer :: a6_i8(:,:,:,:,:,:),   b6_i8(:,:,:,:,:,:)
  integer(kind=I8), pointer :: a5_i8(:,:,:,:,:),     b5_i8(:,:,:,:,:)
  integer(kind=I8), pointer :: a4_i8(:,:,:,:),       b4_i8(:,:,:,:)
  integer(kind=I8), pointer :: a3_i8(:,:,:),         b3_i8(:,:,:)
  integer(kind=I8), pointer :: a2_i8(:,:),           b2_i8(:,:)
  integer(kind=I8), pointer :: a1_i8(:),             b1_i8(:)
  integer(kind=I8), pointer :: v_i8(:)

  integer :: lb(7), ub(7), rc, i, j
  integer :: pass
  integer :: myunit
  integer :: m
  integer :: rank
  integer :: joff
  integer :: vshape(1)
  integer :: extent(7)
  integer :: asize(7)
  integer :: score(7)
  integer :: vlb(1),vub(1)
  integer :: afirst,bfirst
  
  lb(:) = 1
  ub(:) = 7

  extent(:) = 1
  extent(:) = extent(:) + ub(:) - lb(:)
  asize(1) = extent(1)
  do m= 2,7
     asize(m) = asize(m-1)*extent(m)
  enddo
  afirst = (1+me)
  bfirst = afirst * afirst 

! Test I8 flavor.
  vlb(1) = 1
  vub(1) = asize(7)
  call ARMCI_Malloc_fa(v_i8,vlb,vub,rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for v_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a7_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a7_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b7_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b7_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a6_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a6_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b6_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b6_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a5_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a5_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b5_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b5_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a4_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a4_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b4_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b4_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a3_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a3_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b3_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b3_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a2_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a2_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b2_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b2_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a1_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a1_i8 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b1_i8, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b1_i8 failed rc = ',rc
     stop
  endif

! Let all processors get alloccated.

  call ARMCI_Sync()

! I8 allocations done, now loop over slices.
! We will test three flavors of slices,
! all unit stride, unit stride in the first dimension only,
! all non-unit stride (2).
  score(:) = 0
  joff = remote_proc + 1
!
! Test Put.
!
  do m = 1,3
!    Initialize arrays.
!
     call init_7d(a7_i8,b7_i8,lb,ub,lb,ub,afirst,bfirst)
     a6_i8(:,:,:,:,:,:) = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5_i8(:,:,:,:,:) = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4_i8(:,:,:,:) = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3_i8(:,:,:) = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2_i8(:,:) = a7_i8(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1_i8(:) = a7_i8(lb(1):ub(1),1,1,1,1,1,1)
     b6_i8(:,:,:,:,:,:) = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5_i8(:,:,:,:,:) = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4_i8(:,:,:,:) = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3_i8(:,:,:) = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2_i8(:,:) = b7_i8(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1_i8(:) = b7_i8(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the put.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2

! Rank 1.     
     rank = 1
     call ARMCI_NbPut_fa(a1_i8, src_slice, b1_i8, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 1D I8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b1 is the piece of the sent a1.
!
     call check_b(rank,b1_i8,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 1D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 2.

     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbPut_fa(a2_i8, src_slice, b2_i8, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 2D I8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b2 is the piece of the sent a2.
!
!     call check_b(rank,reshape(b2_i8,vshape),dst_slice,src_slice,remote_proc, &
!     call check_b(rank,b2_i8,dst_slice,src_slice,remote_proc, &
     v_i8(1:asize(2)) = reshape(b2_i8,vshape)

     call check_b(rank,v_i8,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 2D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 3.

     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbPut_fa(a3_i8, src_slice, b3_i8, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 3D I8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b3 is the piece of the sent a3.
!
     v_i8(1:asize(3)) = reshape(b3_i8,vshape)
     call check_b(rank,v_i8,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 3D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 4.

     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbPut_fa(a4_i8, src_slice, b4_i8, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 4D I8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b4 is the piece of the sent a4.
!
     v_i8(1:asize(4)) = reshape(b4_i8,vshape)
     call check_b(rank,v_i8,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 5.

     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbPut_fa(a5_i8, src_slice, b5_i8, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 5D I8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b5 is the piece of the sent a5.
!
     v_i8(1:asize(5)) = reshape(b5_i8,vshape)
     call check_b(rank,v_i8,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 6.

     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbPut_fa(a6_i8, src_slice, b6_i8, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 6D I8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b6 is the piece of the sent a6
!
     v_i8(1:asize(6)) = reshape(b6_i8,vshape)
     call check_b(rank,v_i8,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 6D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 7.

     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbPut_fa(a7_i8, src_slice, b7_i8, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 7D I8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b7 is the piece of the sent a7
!
     v_i8(1:asize(7)) = reshape(b7_i8,vshape)
     call check_b(rank,v_i8,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 7D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbPut_fa for I8, ',m,'D passed'
        endif
     enddo
  endif
!
! Test Get.
!
  score(:) = 0
  joff = (remote_proc + 1)*(remote_proc+1)
  do m = 1,3
!
!    Initialize arrays.
!
     call init_7d(a7_i8,b7_i8,lb,ub,lb,ub,afirst,bfirst)
     a6_i8 = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5_i8 = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4_i8 = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3_i8 = a7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2_i8 = a7_i8(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1_i8 = a7_i8(lb(1):ub(1),1,1,1,1,1,1)
     b6_i8 = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5_i8 = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4_i8 = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3_i8 = b7_i8(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2_i8 = b7_i8(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1_i8 = b7_i8(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the get.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2
!
! Rank 1
!
     rank = 1
     call ARMCI_NbGet_fa(b1_i8, dst_slice, a1_i8, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     call check_b(rank,a1_i8,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 1D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 2
!
     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbGet_fa(b2_i8, dst_slice, a2_i8, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i8(1:asize(2)) = reshape(a2_i8,vshape)
     call check_b(rank,v_i8,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 2D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 3
!
     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbGet_fa(b3_i8, dst_slice, a3_i8, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i8(1:asize(3)) = reshape(a3_i8,vshape)
     call check_b(rank,v_i8,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 3D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 4
!
     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbGet_fa(b4_i8, dst_slice, a4_i8, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i8(1:asize(4)) = reshape(a4_i8,vshape)
     call check_b(rank,v_i8,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 4D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 5
!
     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbGet_fa(b5_i8, dst_slice, a5_i8, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i8(1:asize(5)) = reshape(a5_i8,vshape)
     call check_b(rank,v_i8,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 5D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 6
!
     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbGet_fa(b6_i8, dst_slice, a6_i8, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i8(1:asize(6)) = reshape(a6_i8,vshape)
     call check_b(rank,v_i8,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 6D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 7
!
     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbGet_fa(b7_i8, dst_slice, a7_i8, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v_i8(1:asize(7)) = reshape(a7_i8,vshape)
     call check_b(rank,v_i8,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 7D I8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbGet_fa for I8, ',m,'D passed'
        endif
     enddo
  endif
!
! Free v, a and b arrays.
!
  call ARMCI_Sync()
  call ARMCI_Free_fa(v_i8, rc)
  call ARMCI_Free_fa(a1_i8, rc)
  call ARMCI_Free_fa(b1_i8, rc)
  call ARMCI_Free_fa(a2_i8, rc)
  call ARMCI_Free_fa(b2_i8, rc)
  call ARMCI_Free_fa(a3_i8, rc)
  call ARMCI_Free_fa(b3_i8, rc)
  call ARMCI_Free_fa(a4_i8, rc)
  call ARMCI_Free_fa(b4_i8, rc)
  call ARMCI_Free_fa(a5_i8, rc)
  call ARMCI_Free_fa(b5_i8, rc)
  call ARMCI_Free_fa(a6_i8, rc)
  call ARMCI_Free_fa(b6_i8, rc)
  call ARMCI_Free_fa(a7_i8, rc)
  call ARMCI_Free_fa(b7_i8, rc)
  call ARMCI_Sync()
  return
endsubroutine
subroutine testanb_r4(nproc,me,remote_proc)
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_nbmov
  use testa_init
  use checkput
  implicit none
  type(ARMCI_slice) :: src_slice, dst_slice
  integer :: nproc,me,remote_proc
  real(kind=R4), pointer :: a7(:,:,:,:,:,:,:), b7(:,:,:,:,:,:,:)
  real(kind=R4), pointer :: a6(:,:,:,:,:,:),   b6(:,:,:,:,:,:)
  real(kind=R4), pointer :: a5(:,:,:,:,:),     b5(:,:,:,:,:)
  real(kind=R4), pointer :: a4(:,:,:,:),       b4(:,:,:,:)
  real(kind=R4), pointer :: a3(:,:,:),         b3(:,:,:)
  real(kind=R4), pointer :: a2(:,:),           b2(:,:)
  real(kind=R4), pointer :: a1(:),             b1(:)
  real(kind=R4), pointer :: v(:)

  integer :: lb(7), ub(7), rc, i, j
  integer :: pass
  integer :: myunit
  integer :: m
  integer :: rank
  integer :: joff
  integer :: vshape(1)
  integer :: extent(7)
  integer :: asize(7)
  integer :: score(7)
  integer :: vlb(1),vub(1)
  integer :: afirst,bfirst

  afirst = 1+me
  bfirst = afirst*afirst

  lb(:) = 1
  ub(:) = 7

  extent(:) = 1
  extent(:) = extent(:) + ub(:) - lb(:)
  asize(1) = extent(1)
  do m= 2,7
     asize(m) = asize(m-1)*extent(m)
  enddo

! Test R4 flavor.
  vlb(1) = 1
  vub(1) = asize(7)
  call ARMCI_Malloc_fa(v,vlb,vub,rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for v failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a1 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b1 failed rc = ',rc
     stop
  endif

! Let all processors get alloccated.

  call ARMCI_Sync()

! R4 allocations done, now loop over slices.
! We will test three flavors of slices,
! all unit stride, unit stride in the first dimension only,
! all non-unit stride (2).
  score(:) = 0
  joff = remote_proc + 1
!
! Test Put.
!
  do m = 1,3
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6(:,:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5(:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4(:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3(:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2(:,:) = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1(:) = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6(:,:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5(:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4(:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3(:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2(:,:) = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1(:) = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the put.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2

! Rank 1.     
     rank = 1
     call ARMCI_NbPut_fa(a1, src_slice, b1, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 1D R4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b1 is the piece of the sent a1.
!
     call check_b(rank,b1,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 1D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 2.

     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbPut_fa(a2, src_slice, b2, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 2D R4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b2 is the piece of the sent a2.
!
!     call check_b(rank,reshape(b2,vshape),dst_slice,src_slice,remote_proc, &
!     call check_b(rank,b2,dst_slice,src_slice,remote_proc, &
     v(1:asize(2)) = reshape(b2,vshape)

     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 2D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 3.

     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbPut_fa(a3, src_slice, b3, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 3D R4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b3 is the piece of the sent a3.
!
     v(1:asize(3)) = reshape(b3,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 3D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 4.

     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbPut_fa(a4, src_slice, b4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 4D R4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b4 is the piece of the sent a4.
!
     v(1:asize(4)) = reshape(b4,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 5.

     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbPut_fa(a5, src_slice, b5, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 5D R4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b5 is the piece of the sent a5.
!
     v(1:asize(5)) = reshape(b5,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 6.

     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbPut_fa(a6, src_slice, b6, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 6D R4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b6 is the piece of the sent a6
!
     v(1:asize(6)) = reshape(b6,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 6D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 7.

     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbPut_fa(a7, src_slice, b7, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 7D R4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b7 is the piece of the sent a7
!
     v(1:asize(7)) = reshape(b7,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 7D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbPut_fa for R4, ',m,'D passed'
        endif
     enddo
  endif
!
! Test Get.
!
  score(:) = 0
  joff = (remote_proc + 1)*(remote_proc+1)
  do m = 1,3
!
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2 = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1 = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2 = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1 = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the get.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2
!
! Rank 1
!
     rank = 1
     call ARMCI_NbGet_fa(b1, dst_slice, a1, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     call check_b(rank,a1,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 1D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 2
!
     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbGet_fa(b2, dst_slice, a2, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(2)) = reshape(a2,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 2D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 3
!
     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbGet_fa(b3, dst_slice, a3, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(3)) = reshape(a3,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 3D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 4
!
     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbGet_fa(b4, dst_slice, a4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(4)) = reshape(a4,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 4D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 5
!
     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbGet_fa(b5, dst_slice, a5, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(5)) = reshape(a5,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 5D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 6
!
     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbGet_fa(b6, dst_slice, a6, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(6)) = reshape(a6,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 6D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 7
!
     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbGet_fa(b7, dst_slice, a7, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(7)) = reshape(a7,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 7D R4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbGet_fa for R4, ',m,'D passed'
        endif
     enddo
  endif
!
! Free v, a and b arrays.
!
  call ARMCI_Sync()
  call ARMCI_Free_fa(v, rc)
  call ARMCI_Free_fa(a1, rc)
  call ARMCI_Free_fa(b1, rc)
  call ARMCI_Free_fa(a2, rc)
  call ARMCI_Free_fa(b2, rc)
  call ARMCI_Free_fa(a3, rc)
  call ARMCI_Free_fa(b3, rc)
  call ARMCI_Free_fa(a4, rc)
  call ARMCI_Free_fa(b4, rc)
  call ARMCI_Free_fa(a5, rc)
  call ARMCI_Free_fa(b5, rc)
  call ARMCI_Free_fa(a6, rc)
  call ARMCI_Free_fa(b6, rc)
  call ARMCI_Free_fa(a7, rc)
  call ARMCI_Free_fa(b7, rc)
  call ARMCI_Sync()
  return
endsubroutine
subroutine testanb_r8(nproc,me,remote_proc)
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_nbmov
  use testa_init
  use checkput
  implicit none
  type(ARMCI_slice) :: src_slice, dst_slice
  integer :: nproc,me,remote_proc
  real(kind=R8), pointer :: a7(:,:,:,:,:,:,:), b7(:,:,:,:,:,:,:)
  real(kind=R8), pointer :: a6(:,:,:,:,:,:),   b6(:,:,:,:,:,:)
  real(kind=R8), pointer :: a5(:,:,:,:,:),     b5(:,:,:,:,:)
  real(kind=R8), pointer :: a4(:,:,:,:),       b4(:,:,:,:)
  real(kind=R8), pointer :: a3(:,:,:),         b3(:,:,:)
  real(kind=R8), pointer :: a2(:,:),           b2(:,:)
  real(kind=R8), pointer :: a1(:),             b1(:)
  real(kind=R8), pointer :: v(:)

  integer :: lb(7), ub(7), rc, i, j
  integer :: pass
  integer :: myunit
  integer :: m
  integer :: rank
  integer :: joff
  integer :: vshape(1)
  integer :: extent(7)
  integer :: asize(7)
  integer :: score(7)
  integer :: vlb(1),vub(1)
  integer :: afirst,bfirst

  afirst = 1+me
  bfirst = afirst*afirst

  lb(:) = 1
  ub(:) = 7

  extent(:) = 1
  extent(:) = extent(:) + ub(:) - lb(:)
  asize(1) = extent(1)
  do m= 2,7
     asize(m) = asize(m-1)*extent(m)
  enddo

! Test R8 flavor.
  vlb(1) = 1
  vub(1) = asize(7)
  call ARMCI_Malloc_fa(v,vlb,vub,rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for v failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a1 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b1 failed rc = ',rc
     stop
  endif

! Let all processors get alloccated.

  call ARMCI_Sync()

! R8 allocations done, now loop over slices.
! We will test three flavors of slices,
! all unit stride, unit stride in the first dimension only,
! all non-unit stride (2).
  score(:) = 0
  joff = remote_proc + 1
!
! Test Put.
!
  do m = 1,3
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6(:,:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5(:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4(:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3(:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2(:,:) = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1(:) = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6(:,:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5(:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4(:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3(:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2(:,:) = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1(:) = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the put.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2

! Rank 1.     
     rank = 1
     call ARMCI_NbPut_fa(a1, src_slice, b1, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 1D R8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b1 is the piece of the sent a1.
!
     call check_b(rank,b1,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 1D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 2.

     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbPut_fa(a2, src_slice, b2, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 2D R8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b2 is the piece of the sent a2.
!
!     call check_b(rank,reshape(b2,vshape),dst_slice,src_slice,remote_proc, &
!     call check_b(rank,b2,dst_slice,src_slice,remote_proc, &
     v(1:asize(2)) = reshape(b2,vshape)

     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 2D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 3.

     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbPut_fa(a3, src_slice, b3, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 3D R8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b3 is the piece of the sent a3.
!
     v(1:asize(3)) = reshape(b3,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 3D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 4.

     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbPut_fa(a4, src_slice, b4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 4D R8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b4 is the piece of the sent a4.
!
     v(1:asize(4)) = reshape(b4,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 5.

     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbPut_fa(a5, src_slice, b5, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 5D R8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b5 is the piece of the sent a5.
!
     v(1:asize(5)) = reshape(b5,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 6.

     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbPut_fa(a6, src_slice, b6, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 6D R8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b6 is the piece of the sent a6
!
     v(1:asize(6)) = reshape(b6,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 6D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 7.

     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbPut_fa(a7, src_slice, b7, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 7D R8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b7 is the piece of the sent a7
!
     v(1:asize(7)) = reshape(b7,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 7D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbPut_fa for R8, ',m,'D passed'
        endif
     enddo
  endif
!
! Test Get.
!
  score(:) = 0
  joff = (remote_proc + 1)*(remote_proc+1)
  do m = 1,3
!
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2 = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1 = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2 = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1 = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the get.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2
!
! Rank 1
!
     rank = 1
     call ARMCI_NbGet_fa(b1, dst_slice, a1, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     call check_b(rank,a1,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 1D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 2
!
     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbGet_fa(b2, dst_slice, a2, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(2)) = reshape(a2,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 2D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 3
!
     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbGet_fa(b3, dst_slice, a3, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(3)) = reshape(a3,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 3D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 4
!
     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbGet_fa(b4, dst_slice, a4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(4)) = reshape(a4,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 4D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 5
!
     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbGet_fa(b5, dst_slice, a5, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(5)) = reshape(a5,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 5D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 6
!
     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbGet_fa(b6, dst_slice, a6, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(6)) = reshape(a6,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 6D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 7
!
     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbGet_fa(b7, dst_slice, a7, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(7)) = reshape(a7,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 7D R8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbGet_fa for R8, ',m,'D passed'
        endif
     enddo
  endif
!
! Free v, a and b arrays.
!
  call ARMCI_Sync()
  call ARMCI_Free_fa(v, rc)
  call ARMCI_Free_fa(a1, rc)
  call ARMCI_Free_fa(b1, rc)
  call ARMCI_Free_fa(a2, rc)
  call ARMCI_Free_fa(b2, rc)
  call ARMCI_Free_fa(a3, rc)
  call ARMCI_Free_fa(b3, rc)
  call ARMCI_Free_fa(a4, rc)
  call ARMCI_Free_fa(b4, rc)
  call ARMCI_Free_fa(a5, rc)
  call ARMCI_Free_fa(b5, rc)
  call ARMCI_Free_fa(a6, rc)
  call ARMCI_Free_fa(b6, rc)
  call ARMCI_Free_fa(a7, rc)
  call ARMCI_Free_fa(b7, rc)
  call ARMCI_Sync()
  return
endsubroutine

subroutine testanb_c4(nproc,me,remote_proc)
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_nbmov
  use testa_init
  use checkput
  implicit none
  type(ARMCI_slice) :: src_slice, dst_slice
  integer :: nproc,me,remote_proc
  complex(kind=C4), pointer :: a7(:,:,:,:,:,:,:), b7(:,:,:,:,:,:,:)
  complex(kind=C4), pointer :: a6(:,:,:,:,:,:),   b6(:,:,:,:,:,:)
  complex(kind=C4), pointer :: a5(:,:,:,:,:),     b5(:,:,:,:,:)
  complex(kind=C4), pointer :: a4(:,:,:,:),       b4(:,:,:,:)
  complex(kind=C4), pointer :: a3(:,:,:),         b3(:,:,:)
  complex(kind=C4), pointer :: a2(:,:),           b2(:,:)
  complex(kind=C4), pointer :: a1(:),             b1(:)
  complex(kind=C4), pointer :: v(:)

  integer :: lb(7), ub(7), rc, i, j
  integer :: pass
  integer :: myunit
  integer :: m
  integer :: rank
  integer :: joff
  integer :: vshape(1)
  integer :: extent(7)
  integer :: asize(7)
  integer :: score(7)
  integer :: vlb(1),vub(1)
  integer :: afirst,bfirst

  afirst = 1+me
  bfirst = afirst*afirst

  lb(:) = 1
  ub(:) = 7

  extent(:) = 1
  extent(:) = extent(:) + ub(:) - lb(:)
  asize(1) = extent(1)
  do m= 2,7
     asize(m) = asize(m-1)*extent(m)
  enddo

! Test C4 flavor.
  vlb(1) = 1
  vub(1) = asize(7)
  call ARMCI_Malloc_fa(v,vlb,vub,rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for v failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a1 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b1 failed rc = ',rc
     stop
  endif

! Let all processors get alloccated.

  call ARMCI_Sync()

! C4 allocations done, now loop over slices.
! We will test three flavors of slices,
! all unit stride, unit stride in the first dimension only,
! all non-unit stride (2).
  score(:) = 0
  joff = remote_proc + 1
!
! Test Put.
!
  do m = 1,3
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6(:,:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5(:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4(:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3(:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2(:,:) = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1(:) = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6(:,:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5(:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4(:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3(:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2(:,:) = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1(:) = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the put.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2

! Rank 1.     
     rank = 1
     call ARMCI_NbPut_fa(a1, src_slice, b1, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 1D C4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b1 is the piece of the sent a1.
!
     call check_b(rank,b1,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 1D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 2.

     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbPut_fa(a2, src_slice, b2, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 2D C4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b2 is the piece of the sent a2.
!
!     call check_b(rank,reshape(b2,vshape),dst_slice,src_slice,remote_proc, &
!     call check_b(rank,b2,dst_slice,src_slice,remote_proc, &
     v(1:asize(2)) = reshape(b2,vshape)

     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 2D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 3.

     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbPut_fa(a3, src_slice, b3, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 3D C4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b3 is the piece of the sent a3.
!
     v(1:asize(3)) = reshape(b3,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 3D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 4.

     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbPut_fa(a4, src_slice, b4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 4D C4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b4 is the piece of the sent a4.
!
     v(1:asize(4)) = reshape(b4,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 5.

     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbPut_fa(a5, src_slice, b5, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 5D C4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b5 is the piece of the sent a5.
!
     v(1:asize(5)) = reshape(b5,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 6.

     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbPut_fa(a6, src_slice, b6, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 6D C4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b6 is the piece of the sent a6
!
     v(1:asize(6)) = reshape(b6,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 6D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 7.

     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbPut_fa(a7, src_slice, b7, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 7D C4, m = ',m,' failed, rc = ',rc
     endif

! Check that received b7 is the piece of the sent a7
!
     v(1:asize(7)) = reshape(b7,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 7D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbPut_fa for C4, ',m,'D passed'
        endif
     enddo
  endif
!
! Test Get.
!
  score(:) = 0
  joff = (remote_proc + 1)*(remote_proc+1)
  do m = 1,3
!
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2 = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1 = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2 = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1 = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the get.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2
!
! Rank 1
!
     rank = 1
     call ARMCI_NbGet_fa(b1, dst_slice, a1, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     call check_b(rank,a1,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 1D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 2
!
     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbGet_fa(b2, dst_slice, a2, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(2)) = reshape(a2,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 2D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 3
!
     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbGet_fa(b3, dst_slice, a3, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(3)) = reshape(a3,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 3D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 4
!
     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbGet_fa(b4, dst_slice, a4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(4)) = reshape(a4,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 4D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 5
!
     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbGet_fa(b5, dst_slice, a5, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(5)) = reshape(a5,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 5D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 6
!
     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbGet_fa(b6, dst_slice, a6, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(6)) = reshape(a6,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 6D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 7
!
     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbGet_fa(b7, dst_slice, a7, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(7)) = reshape(a7,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 7D C4, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbGet_fa for C4, ',m,'D passed'
        endif
     enddo
  endif
!
! Free v, a and b arrays.
!
  call ARMCI_Sync()
  call ARMCI_Free_fa(v, rc)
  call ARMCI_Free_fa(a1, rc)
  call ARMCI_Free_fa(b1, rc)
  call ARMCI_Free_fa(a2, rc)
  call ARMCI_Free_fa(b2, rc)
  call ARMCI_Free_fa(a3, rc)
  call ARMCI_Free_fa(b3, rc)
  call ARMCI_Free_fa(a4, rc)
  call ARMCI_Free_fa(b4, rc)
  call ARMCI_Free_fa(a5, rc)
  call ARMCI_Free_fa(b5, rc)
  call ARMCI_Free_fa(a6, rc)
  call ARMCI_Free_fa(b6, rc)
  call ARMCI_Free_fa(a7, rc)
  call ARMCI_Free_fa(b7, rc)
  call ARMCI_Sync()
  return
endsubroutine

subroutine testanb_c8(nproc,me,remote_proc)
  use definekind
  use armci_types
  use armci_mem_f90
  use armci_nbmov
  use testa_init
  use checkput
  implicit none
  type(ARMCI_slice) :: src_slice, dst_slice
  integer :: nproc,me,remote_proc
  complex(kind=C8), pointer :: a7(:,:,:,:,:,:,:), b7(:,:,:,:,:,:,:)
  complex(kind=C8), pointer :: a6(:,:,:,:,:,:),   b6(:,:,:,:,:,:)
  complex(kind=C8), pointer :: a5(:,:,:,:,:),     b5(:,:,:,:,:)
  complex(kind=C8), pointer :: a4(:,:,:,:),       b4(:,:,:,:)
  complex(kind=C8), pointer :: a3(:,:,:),         b3(:,:,:)
  complex(kind=C8), pointer :: a2(:,:),           b2(:,:)
  complex(kind=C8), pointer :: a1(:),             b1(:)
  complex(kind=C8), pointer :: v(:)

  integer :: lb(7), ub(7), rc, i, j
  integer :: pass
  integer :: myunit
  integer :: m
  integer :: rank
  integer :: joff
  integer :: vshape(1)
  integer :: extent(7)
  integer :: asize(7)
  integer :: score(7)
  integer :: vlb(1),vub(1)
  integer :: afirst,bfirst

  afirst = 1+me
  bfirst = afirst*afirst

  lb(:) = 1
  ub(:) = 7
  ub(7) = 3

  extent(:) = 1
  extent(:) = extent(:) + ub(:) - lb(:)
  asize(1) = extent(1)
  do m= 2,7
     asize(m) = asize(m-1)*extent(m)
  enddo

! Test C8 flavor.
  vlb(1) = 1
  vub(1) = asize(7)
  call ARMCI_Malloc_fa(v,vlb,vub,rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for v failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b7, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b7 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b6, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b6 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b5, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b5 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b4, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b4 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b3, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b3 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b2, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b2 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(a1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for a1 failed rc = ',rc
     stop
  endif

  call ARMCI_Malloc_fa(b1, lb, ub, rc)
  if (rc .ne. 0) then
     print *,' ARMCI_Malloc_fa for b1 failed rc = ',rc
     stop
  endif

! Let all processors get alloccated.

  call ARMCI_Sync()

! C8 allocations done, now loop over slices.
! We will test three flavors of slices,
! all unit stride, unit stride in the first dimension only,
! all non-unit stride (2).
  score(:) = 0
  joff = remote_proc + 1
!
! Test Put.
!
  do m = 1,3
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6(:,:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5(:,:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4(:,:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3(:,:,:) = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2(:,:) = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1(:) = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6(:,:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5(:,:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4(:,:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3(:,:,:) = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2(:,:) = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1(:) = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the put.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%lo(7) = 1
     src_slice%hi(7) = 2
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%lo(7) = 2
     dst_slice%hi(7) = 3
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2

! Rank 1.     
     rank = 1
     call ARMCI_NbPut_fa(a1, src_slice, b1, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 1D C8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b1 is the piece of the sent a1.
!
     call check_b(rank,b1,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 1D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 2.

     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbPut_fa(a2, src_slice, b2, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 2D C8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b2 is the piece of the sent a2.
!
!     call check_b(rank,reshape(b2,vshape),dst_slice,src_slice,remote_proc, &
!     call check_b(rank,b2,dst_slice,src_slice,remote_proc, &
     v(1:asize(2)) = reshape(b2,vshape)

     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 2D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 3.

     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbPut_fa(a3, src_slice, b3, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 3D C8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b3 is the piece of the sent a3.
!
     v(1:asize(3)) = reshape(b3,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 3D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 4.

     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbPut_fa(a4, src_slice, b4, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 4D C8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b4 is the piece of the sent a4.
!
     v(1:asize(4)) = reshape(b4,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 5.

     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbPut_fa(a5, src_slice, b5, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 5D C8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b5 is the piece of the sent a5.
!
     v(1:asize(5)) = reshape(b5,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 4D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 6.

     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbPut_fa(a6, src_slice, b6, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 6D C8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b6 is the piece of the sent a6
!
     v(1:asize(6)) = reshape(b6,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 6D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

! Rank 7.

     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbPut_fa(a7, src_slice, b7, dst_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
     if (rc .ne. 0) then
        print *,me,': ARMCI_NbPut_fa for 7D C8, m = ',m,' failed, rc = ',rc
     endif

! Check that received b7 is the piece of the sent a7
!
     v(1:asize(7)) = reshape(b7,vshape)
     call check_b(rank,v,dst_slice,src_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbPut_fa verify for 7D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbPut_fa for C8, ',m,'D passed'
        endif
     enddo
  endif
!
! Test Get.
!
  score(:) = 0
  joff = (remote_proc + 1)*(remote_proc+1)
  do m = 1,3
!
!    Initialize arrays.
!
     call init_7d(a7,b7,lb,ub,lb,ub,afirst,bfirst)
     a6 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     a5 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     a4 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     a3 = a7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     a2 = a7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     a1 = a7(lb(1):ub(1),1,1,1,1,1,1)
     b6 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),lb(6):ub(6),1)
     b5 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),lb(5):ub(5),1,1)
     b4 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),lb(4):ub(4),1,1,1)
     b3 = b7(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3),1,1,1,1)
     b2 = b7(lb(1):ub(1),lb(2):ub(2),1,1,1,1,1)
     b1 = b7(lb(1):ub(1),1,1,1,1,1,1)

! Let all processors get initialized.

     call ARMCI_Sync()

! Set up slice info for the get.
!
! For m = 1, the slice has all unit strides.
! For m = 2, the first dimension has a unit stride and all other
!            dimensions have a stride of 2.
! For m = 3, all dimensions have a stride of 2.
!  
     src_slice%lo(:) = 2
     src_slice%hi(:) = 6
     src_slice%lo(7) = 1
     src_slice%hi(7) = 2
     src_slice%stride(:) = (m+2)/2
     src_slice%stride(1) = (m+1)/2

     dst_slice%lo(:) = 3
     dst_slice%hi(:) = 7
     dst_slice%lo(7) = 2
     dst_slice%hi(7) = 3
     dst_slice%stride(:) = (m+2)/2
     dst_slice%stride(1) = (m+1)/2
!
! Rank 1
!
     rank = 1
     call ARMCI_NbGet_fa(b1, dst_slice, a1, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     call check_b(rank,a1,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(1) = score(1) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 1D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 2
!
     rank = 2
     vshape(1) = asize(2)
     call ARMCI_NbGet_fa(b2, dst_slice, a2, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(2)) = reshape(a2,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(2) = score(2) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 2D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 3
!
     rank = 3
     vshape(1) = asize(3)
     call ARMCI_NbGet_fa(b3, dst_slice, a3, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(3)) = reshape(a3,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(3) = score(3) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 3D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 4
!
     rank = 4
     vshape(1) = asize(4)
     call ARMCI_NbGet_fa(b4, dst_slice, a4, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(4)) = reshape(a4,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(4) = score(4) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 4D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 5
!
     rank = 5
     vshape(1) = asize(5)
     call ARMCI_NbGet_fa(b5, dst_slice, a5, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(5)) = reshape(a5,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(5) = score(5) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 5D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 6
!
     rank = 6
     vshape(1) = asize(6)
     call ARMCI_NbGet_fa(b6, dst_slice, a6, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(6)) = reshape(a6,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(6) = score(6) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 6D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()
     
!
! Rank 7
!
     rank = 7
     vshape(1) = asize(7)
     call ARMCI_NbGet_fa(b7, dst_slice, a7, src_slice, remote_proc, rc)
     call ARMCI_Waitall_fa()

     call ARMCI_Sync()
!
! Check that recieved a is the piece of b that was sent.

     v(1:asize(7)) = reshape(a7,vshape)
     call check_b(rank,v,src_slice,dst_slice,remote_proc, &
    &             lb,ub,lb,ub,joff,pass)
     score(7) = score(7) + pass
     if (pass .ne. 1) then
        print *,me,': ARMCI_NbGet_fa verify for 7D C8, m = ',m,' failed'
     endif
     call ARMCI_Sync()

  enddo
  if (me .eq. 0) then
     do m = 1,7
        if (score(m) .eq. 3) then
           print *,' ARMCI_NbGet_fa for C8, ',m,'D passed'
        endif
     enddo
  endif
!
! Free v, a and b arrays.
!
  call ARMCI_Sync()
  call ARMCI_Free_fa(v, rc)
  call ARMCI_Free_fa(a1, rc)
  call ARMCI_Free_fa(b1, rc)
  call ARMCI_Free_fa(a2, rc)
  call ARMCI_Free_fa(b2, rc)
  call ARMCI_Free_fa(a3, rc)
  call ARMCI_Free_fa(b3, rc)
  call ARMCI_Free_fa(a4, rc)
  call ARMCI_Free_fa(b4, rc)
  call ARMCI_Free_fa(a5, rc)
  call ARMCI_Free_fa(b5, rc)
  call ARMCI_Free_fa(a6, rc)
  call ARMCI_Free_fa(b6, rc)
  call ARMCI_Free_fa(a7, rc)
  call ARMCI_Free_fa(b7, rc)
  call ARMCI_Sync()
  return
endsubroutine
