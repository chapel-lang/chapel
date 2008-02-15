module checkput
  use definekind
  use armci_types
  interface check_b
     module procedure check_b_i4
     module procedure check_b_i8
     module procedure check_b_r4
     module procedure check_b_r8
     module procedure check_b_c4
     module procedure check_b_c8
  end interface

contains
  subroutine check_b_i4(rank,b,dst_slice,src_slice,remote_proc,dlb,dub, &
 &                      slb,sub,joff,pass)
    implicit none
    integer rank
    integer(kind=i4), pointer :: b(:)
    type(armci_slice) :: dst_slice, src_slice
    integer remote_proc
    integer dlb(7),dub(7),slb(7),sub(7)
    integer joff
    integer pass
    integer me
    integer index
    integer dextent(7)
    integer sextent(7)
    integer dst_stride(7)
    integer src_stride(7)
    integer src_off(7),dst_off(7)
    integer src_lo(7),src_hi(7)
    integer dst_lo(7),dst_hi(7)
    integer i,m,i1,i2,i3,k4,i5,i6,i7
    integer j1s,j2s,j3s,j4s,j5s,j6s,j7s
    integer j1d,j2d,j3d,j4d,j5d,j6d,j7d
    integer factor
    integer myunit
    integer jbase
!    me = nodeid()
!    myunit = 10+me
!       write(myunit,*)me,'check_b_i4 rank = ',rank
!       write(myunit,*)me,'check_b_i4 dst_slice = ',dst_slice
!       write(myunit,*)me,'check_b_i4 src_slice = ',src_slice
!       write(myunit,*)me,'check_b_i4 remote_proc = ',remote_proc
!       write(myunit,*)me,'check_b_i4 dlb = ',dlb
!       write(myunit,*)me,'check_b_i4 dub = ',dub       
!       write(myunit,*)me,'check_b_i4 slb = ',slb
!       write(myunit,*)me,'check_b_i4 sub = ',sub       
!       write(myunit,*)me,'check_b_i4 joff = ',joff
    dst_lo(:) = 1
    dst_hi(:) = 1
    dst_stride(:) = 1
    do m = 1,rank
       dst_lo(m) = dst_slice%lo(m)
    enddo
    do m = 1,rank
       dst_hi(m) = dst_slice%hi(m)
    enddo
    do m = 1,rank
       dst_stride(m) = dst_slice%stride(m)
    enddo

    src_lo(:) = 1
    src_hi(:) = 1
    src_stride(:) = 1
    do m = 1,rank
       src_lo(m) = src_slice%lo(m)
    enddo
    do m = 1,rank
       src_hi(m) = src_slice%hi(m)
    enddo
    do m = 1,rank
       src_stride(m) = src_slice%stride(m)
    enddo

    do m = 1,rank
       sextent(m) = sub(m) - slb(m) + 1
    enddo

    do m = 1,rank
       dextent(m) = dub(m) - dlb(m) + 1
    enddo

    do m = rank+1,7
       dextent(m) = 1
    enddo
    do m = rank+1,7
       sextent(m) = 1
    enddo

    src_off(1) = 1
    do m = 2,rank
       src_off(m) = src_off(m-1)*sextent(m-1)
    enddo
    
    dst_off(1) = 1
    do m = 2,rank
       dst_off(m) = dst_off(m-1)*dextent(m-1)
    enddo

    do m = 1,rank
       src_off(m) = src_off(m) * src_slice%stride(m)
    enddo
       
    do m = 1,rank
       dst_off(m) = dst_off(m) * dst_slice%stride(m)
    enddo

    do m= rank+1,7
       dst_off(m) = 0
    enddo
    do m= rank+1,7
       src_off(m) = 0
    enddo
    j7d = 1
    factor = 1
    do m = 1,rank
       j7d = j7d + (dst_lo(m) - dlb(m))*factor
       factor = factor * dextent(1)
    enddo
    j7s = 1
    factor = 1
    do m = 1,rank
       j7s = j7s + (src_lo(m) - slb(m))*factor
       factor = factor * sextent(1)
    enddo
 !   write(myunit,*)me,'check_b_i4 j7d,j7s = ',j7d,j7s
    pass = 1
    index = 0
    jbase = joff -1
    do i7 = dst_lo(7),dst_hi(7),dst_stride(7)
       j6d = j7d
       j6s = j7s
       do i6 = dst_lo(6),dst_hi(6),dst_stride(6)
          j5d = j6d
          j5s = j6s
          do i5 = dst_lo(5),dst_hi(5),dst_stride(5)
             j4d = j5d
             j4s = j5s
             do k4 = dst_lo(4),dst_hi(4),dst_stride(4)
                j3d = j4d
                j3s = j4s
                do i3 = dst_lo(3),dst_hi(3),dst_stride(3)
                   j2d = j3d
                   j2s = j3s
                   do i2 = dst_lo(2),dst_hi(2),dst_stride(2)
                      j1d = j2d
                      j1s = j2s
                      do i1 = dst_lo(1),dst_hi(1),dst_stride(1)
                         if (b(j1d) .ne. jbase + j1s) then
!                            write(myunit,*)'b(',j1d,') = ',b(j1d),' jbase + j1s = ',jbase+j1s
                            pass = 0
                            index = j1d
                         endif
                         j1d = j1d + dst_off(1) 
                         j1s = j1s + src_off(1)
                      enddo
                      j2d = j2d + dst_off(2)
                      j2s = j2s + src_off(2)
                   enddo
                   j3d = j3d + dst_off(3)
                   j3s = j3s + src_off(3)
                enddo
                j4d = j4d + dst_off(4)
                j4s = j4s + src_off(4)
             enddo
             j5d = j5d + dst_off(5)
             j5s = j5s + src_off(5)
          enddo
          j6d = j6d + dst_off(6)
          j6s = j6s + src_off(6)
       enddo
       j7d = j7d + dst_off(7)
       j7s = j7s + src_off(7)
    enddo
    return
  end subroutine check_b_i4

  subroutine check_b_i8(rank,b,dst_slice,src_slice,remote_proc,dlb,dub, &
 &                      slb,sub,joff,pass)
    implicit none
    integer rank
    integer(kind=i8), pointer :: b(:)
    type(armci_slice) :: dst_slice, src_slice
    integer remote_proc
    integer dlb(7),dub(7),slb(7),sub(7)
    integer joff
    integer pass
    integer me
    integer index
    integer dextent(7)
    integer sextent(7)
    integer dst_stride(7)
    integer src_stride(7)
    integer src_off(7),dst_off(7)
    integer src_lo(7),src_hi(7)
    integer dst_lo(7),dst_hi(7)
    integer i,m,i1,i2,i3,k4,i5,i6,i7
    integer(kind=i8) :: j1s,j2s,j3s,j4s,j5s,j6s,j7s
    integer(kind=i8) :: j1d,j2d,j3d,j4d,j5d,j6d,j7d
    integer factor
    integer myunit
    integer(kind=i8) :: jbase
    dst_lo(:) = 1
    dst_hi(:) = 1
    dst_stride(:) = 1
    do m = 1,rank
       dst_lo(m) = dst_slice%lo(m)
    enddo
    do m = 1,rank
       dst_hi(m) = dst_slice%hi(m)
    enddo
    do m = 1,rank
       dst_stride(m) = dst_slice%stride(m)
    enddo

    src_lo(:) = 1
    src_hi(:) = 1
    src_stride(:) = 1
    do m = 1,rank
       src_lo(m) = src_slice%lo(m)
    enddo
    do m = 1,rank
       src_hi(m) = src_slice%hi(m)
    enddo
    do m = 1,rank
       src_stride(m) = src_slice%stride(m)
    enddo

    do m = 1,rank
       sextent(m) = sub(m) - slb(m) + 1
    enddo

    do m = 1,rank
       dextent(m) = dub(m) - dlb(m) + 1
    enddo

    do m = rank+1,7
       dextent(m) = 1
    enddo
    do m = rank+1,7
       sextent(m) = 1
    enddo

    src_off(1) = 1
    do m = 2,rank
       src_off(m) = src_off(m-1)*sextent(m-1)
    enddo
    
    dst_off(1) = 1
    do m = 2,rank
       dst_off(m) = dst_off(m-1)*dextent(m-1)
    enddo

    do m = 1,rank
       src_off(m) = src_off(m) * src_slice%stride(m)
    enddo
       
    do m = 1,rank
       dst_off(m) = dst_off(m) * dst_slice%stride(m)
    enddo

    do m= rank+1,7
       dst_off(m) = 0
    enddo
    do m= rank+1,7
       src_off(m) = 0
    enddo
    j7d = 1
    factor = 1
    do m = 1,rank
       j7d = j7d + (dst_lo(m) - dlb(m))*factor
       factor = factor * dextent(1)
    enddo
    j7s = 1
    factor = 1
    do m = 1,rank
       j7s = j7s + (src_lo(m) - slb(m))*factor
       factor = factor * sextent(1)
    enddo
    pass = 1
    index = 0
    jbase = joff -1
    do i7 = dst_lo(7),dst_hi(7),dst_stride(7)
       j6d = j7d
       j6s = j7s
       do i6 = dst_lo(6),dst_hi(6),dst_stride(6)
          j5d = j6d
          j5s = j6s
          do i5 = dst_lo(5),dst_hi(5),dst_stride(5)
             j4d = j5d
             j4s = j5s
             do k4 = dst_lo(4),dst_hi(4),dst_stride(4)
                j3d = j4d
                j3s = j4s
                do i3 = dst_lo(3),dst_hi(3),dst_stride(3)
                   j2d = j3d
                   j2s = j3s
                   do i2 = dst_lo(2),dst_hi(2),dst_stride(2)
                      j1d = j2d
                      j1s = j2s
                      do i1 = dst_lo(1),dst_hi(1),dst_stride(1)
                         if (b(j1d) .ne. jbase + j1s) then
   print *,'checkb_i8 fail: j1d,b(j1d),jbase,j1s,jbase+j1s = ',&
                           &         j1d,b(j1d),jbase,j1s,jbase+j1s
                            stop
                            pass = 0
                            index = j1d
                         endif
                         j1d = j1d + dst_off(1) 
                         j1s = j1s + src_off(1)
                      enddo
                      j2d = j2d + dst_off(2)
                      j2s = j2s + src_off(2)
                   enddo
                   j3d = j3d + dst_off(3)
                   j3s = j3s + src_off(3)
                enddo
                j4d = j4d + dst_off(4)
                j4s = j4s + src_off(4)
             enddo
             j5d = j5d + dst_off(5)
             j5s = j5s + src_off(5)
          enddo
          j6d = j6d + dst_off(6)
          j6s = j6s + src_off(6)
       enddo
       j7d = j7d + dst_off(7)
       j7s = j7s + src_off(7)
    enddo
    return
  end subroutine check_b_i8

  subroutine check_b_r4(rank,b,dst_slice,src_slice,remote_proc,dlb,dub, &
 &                      slb,sub,joff,pass)
    implicit none
    integer rank
    real(kind=r4), pointer :: b(:)
    type(armci_slice) :: dst_slice, src_slice
    integer remote_proc
    integer dlb(7),dub(7),slb(7),sub(7)
    integer joff
    integer pass
    integer me
    integer index
    integer dextent(7)
    integer sextent(7)
    integer dst_stride(7)
    integer src_stride(7)
    integer src_off(7),dst_off(7)
    integer src_lo(7),src_hi(7)
    integer dst_lo(7),dst_hi(7)
    integer i,m,i1,i2,i3,k4,i5,i6,i7
    integer j1s,j2s,j3s,j4s,j5s,j6s,j7s
    integer j1d,j2d,j3d,j4d,j5d,j6d,j7d
    integer factor
    integer myunit
    integer jbase
    dst_lo(:) = 1
    dst_hi(:) = 1
    dst_stride(:) = 1
    do m = 1,rank
       dst_lo(m) = dst_slice%lo(m)
    enddo
    do m = 1,rank
       dst_hi(m) = dst_slice%hi(m)
    enddo
    do m = 1,rank
       dst_stride(m) = dst_slice%stride(m)
    enddo

    src_lo(:) = 1
    src_hi(:) = 1
    src_stride(:) = 1
    do m = 1,rank
       src_lo(m) = src_slice%lo(m)
    enddo
    do m = 1,rank
       src_hi(m) = src_slice%hi(m)
    enddo
    do m = 1,rank
       src_stride(m) = src_slice%stride(m)
    enddo

    do m = 1,rank
       sextent(m) = sub(m) - slb(m) + 1
    enddo

    do m = 1,rank
       dextent(m) = dub(m) - dlb(m) + 1
    enddo

    do m = rank+1,7
       dextent(m) = 1
    enddo
    do m = rank+1,7
       sextent(m) = 1
    enddo

    src_off(1) = 1
    do m = 2,rank
       src_off(m) = src_off(m-1)*sextent(m-1)
    enddo
    
    dst_off(1) = 1
    do m = 2,rank
       dst_off(m) = dst_off(m-1)*dextent(m-1)
    enddo

    do m = 1,rank
       src_off(m) = src_off(m) * src_slice%stride(m)
    enddo
       
    do m = 1,rank
       dst_off(m) = dst_off(m) * dst_slice%stride(m)
    enddo

    do m= rank+1,7
       dst_off(m) = 0
    enddo
    do m= rank+1,7
       src_off(m) = 0
    enddo
    j7d = 1
    factor = 1
    do m = 1,rank
       j7d = j7d + (dst_lo(m) - dlb(m))*factor
       factor = factor * dextent(1)
    enddo
    j7s = 1
    factor = 1
    do m = 1,rank
       j7s = j7s + (src_lo(m) - slb(m))*factor
       factor = factor * sextent(1)
    enddo
    pass = 1
    index = 0
    jbase = joff -1
    do i7 = dst_lo(7),dst_hi(7),dst_stride(7)
       j6d = j7d
       j6s = j7s
       do i6 = dst_lo(6),dst_hi(6),dst_stride(6)
          j5d = j6d
          j5s = j6s
          do i5 = dst_lo(5),dst_hi(5),dst_stride(5)
             j4d = j5d
             j4s = j5s
             do k4 = dst_lo(4),dst_hi(4),dst_stride(4)
                j3d = j4d
                j3s = j4s
                do i3 = dst_lo(3),dst_hi(3),dst_stride(3)
                   j2d = j3d
                   j2s = j3s
                   do i2 = dst_lo(2),dst_hi(2),dst_stride(2)
                      j1d = j2d
                      j1s = j2s
                      do i1 = dst_lo(1),dst_hi(1),dst_stride(1)
                         if (b(j1d) .ne. float(jbase + j1s)) then
                            pass = 0
                            index = j1d
                         endif
                         j1d = j1d + dst_off(1) 
                         j1s = j1s + src_off(1)
                      enddo
                      j2d = j2d + dst_off(2)
                      j2s = j2s + src_off(2)
                   enddo
                   j3d = j3d + dst_off(3)
                   j3s = j3s + src_off(3)
                enddo
                j4d = j4d + dst_off(4)
                j4s = j4s + src_off(4)
             enddo
             j5d = j5d + dst_off(5)
             j5s = j5s + src_off(5)
          enddo
          j6d = j6d + dst_off(6)
          j6s = j6s + src_off(6)
       enddo
       j7d = j7d + dst_off(7)
       j7s = j7s + src_off(7)
    enddo
    return
  end subroutine check_b_r4

  subroutine check_b_r8(rank,b,dst_slice,src_slice,remote_proc,dlb,dub, &
 &                      slb,sub,joff,pass)
    implicit none
    integer rank
    real(kind=r8), pointer :: b(:)
    type(armci_slice) :: dst_slice, src_slice
    integer remote_proc
    integer dlb(7),dub(7),slb(7),sub(7)
    integer joff
    integer pass
    integer me
    integer index
    integer dextent(7)
    integer sextent(7)
    integer dst_stride(7)
    integer src_stride(7)
    integer src_off(7),dst_off(7)
    integer src_lo(7),src_hi(7)
    integer dst_lo(7),dst_hi(7)
    integer i,m,i1,i2,i3,k4,i5,i6,i7
    integer j1s,j2s,j3s,j4s,j5s,j6s,j7s
    integer j1d,j2d,j3d,j4d,j5d,j6d,j7d
    integer factor
    integer myunit
    integer jbase
    dst_lo(:) = 1
    dst_hi(:) = 1
    dst_stride(:) = 1
    do m = 1,rank
       dst_lo(m) = dst_slice%lo(m)
    enddo
    do m = 1,rank
       dst_hi(m) = dst_slice%hi(m)
    enddo
    do m = 1,rank
       dst_stride(m) = dst_slice%stride(m)
    enddo

    src_lo(:) = 1
    src_hi(:) = 1
    src_stride(:) = 1
    do m = 1,rank
       src_lo(m) = src_slice%lo(m)
    enddo
    do m = 1,rank
       src_hi(m) = src_slice%hi(m)
    enddo
    do m = 1,rank
       src_stride(m) = src_slice%stride(m)
    enddo

    do m = 1,rank
       sextent(m) = sub(m) - slb(m) + 1
    enddo

    do m = 1,rank
       dextent(m) = dub(m) - dlb(m) + 1
    enddo

    do m = rank+1,7
       dextent(m) = 1
    enddo
    do m = rank+1,7
       sextent(m) = 1
    enddo

    src_off(1) = 1
    do m = 2,rank
       src_off(m) = src_off(m-1)*sextent(m-1)
    enddo
    
    dst_off(1) = 1
    do m = 2,rank
       dst_off(m) = dst_off(m-1)*dextent(m-1)
    enddo

    do m = 1,rank
       src_off(m) = src_off(m) * src_slice%stride(m)
    enddo
       
    do m = 1,rank
       dst_off(m) = dst_off(m) * dst_slice%stride(m)
    enddo

    do m= rank+1,7
       dst_off(m) = 0
    enddo
    do m= rank+1,7
       src_off(m) = 0
    enddo
    j7d = 1
    factor = 1
    do m = 1,rank
       j7d = j7d + (dst_lo(m) - dlb(m))*factor
       factor = factor * dextent(1)
    enddo
    j7s = 1
    factor = 1
    do m = 1,rank
       j7s = j7s + (src_lo(m) - slb(m))*factor
       factor = factor * sextent(1)
    enddo
    pass = 1
    index = 0
    jbase = joff -1
    do i7 = dst_lo(7),dst_hi(7),dst_stride(7)
       j6d = j7d
       j6s = j7s
       do i6 = dst_lo(6),dst_hi(6),dst_stride(6)
          j5d = j6d
          j5s = j6s
          do i5 = dst_lo(5),dst_hi(5),dst_stride(5)
             j4d = j5d
             j4s = j5s
             do k4 = dst_lo(4),dst_hi(4),dst_stride(4)
                j3d = j4d
                j3s = j4s
                do i3 = dst_lo(3),dst_hi(3),dst_stride(3)
                   j2d = j3d
                   j2s = j3s
                   do i2 = dst_lo(2),dst_hi(2),dst_stride(2)
                      j1d = j2d
                      j1s = j2s
                      do i1 = dst_lo(1),dst_hi(1),dst_stride(1)
                         if (b(j1d) .ne. dble(jbase + j1s)) then
                            pass = 0
                            index = j1d
                         endif
                         j1d = j1d + dst_off(1) 
                         j1s = j1s + src_off(1)
                      enddo
                      j2d = j2d + dst_off(2)
                      j2s = j2s + src_off(2)
                   enddo
                   j3d = j3d + dst_off(3)
                   j3s = j3s + src_off(3)
                enddo
                j4d = j4d + dst_off(4)
                j4s = j4s + src_off(4)
             enddo
             j5d = j5d + dst_off(5)
             j5s = j5s + src_off(5)
          enddo
          j6d = j6d + dst_off(6)
          j6s = j6s + src_off(6)
       enddo
       j7d = j7d + dst_off(7)
       j7s = j7s + src_off(7)
    enddo
    return
  end subroutine check_b_r8

  subroutine check_b_c4(rank,b,dst_slice,src_slice,remote_proc,dlb,dub, &
 &                      slb,sub,joff,pass)
    implicit none
    integer rank
    complex(kind=c4), pointer :: b(:)
    type(armci_slice) :: dst_slice, src_slice
    integer remote_proc
    integer dlb(7),dub(7),slb(7),sub(7)
    integer joff
    integer pass
    integer me
    integer index
    integer dextent(7)
    integer sextent(7)
    integer dst_stride(7)
    integer src_stride(7)
    integer src_off(7),dst_off(7)
    integer src_lo(7),src_hi(7)
    integer dst_lo(7),dst_hi(7)
    integer i,m,i1,i2,i3,k4,i5,i6,i7
    integer j1s,j2s,j3s,j4s,j5s,j6s,j7s
    integer j1d,j2d,j3d,j4d,j5d,j6d,j7d
    integer factor
    integer myunit
    integer jbase
    dst_lo(:) = 1
    dst_hi(:) = 1
    dst_stride(:) = 1
    do m = 1,rank
       dst_lo(m) = dst_slice%lo(m)
    enddo
    do m = 1,rank
       dst_hi(m) = dst_slice%hi(m)
    enddo
    do m = 1,rank
       dst_stride(m) = dst_slice%stride(m)
    enddo

    src_lo(:) = 1
    src_hi(:) = 1
    src_stride(:) = 1
    do m = 1,rank
       src_lo(m) = src_slice%lo(m)
    enddo
    do m = 1,rank
       src_hi(m) = src_slice%hi(m)
    enddo
    do m = 1,rank
       src_stride(m) = src_slice%stride(m)
    enddo

    do m = 1,rank
       sextent(m) = sub(m) - slb(m) + 1
    enddo

    do m = 1,rank
       dextent(m) = dub(m) - dlb(m) + 1
    enddo

    do m = rank+1,7
       dextent(m) = 1
    enddo
    do m = rank+1,7
       sextent(m) = 1
    enddo

    src_off(1) = 1
    do m = 2,rank
       src_off(m) = src_off(m-1)*sextent(m-1)
    enddo
    
    dst_off(1) = 1
    do m = 2,rank
       dst_off(m) = dst_off(m-1)*dextent(m-1)
    enddo

    do m = 1,rank
       src_off(m) = src_off(m) * src_slice%stride(m)
    enddo
       
    do m = 1,rank
       dst_off(m) = dst_off(m) * dst_slice%stride(m)
    enddo

    do m= rank+1,7
       dst_off(m) = 0
    enddo
    do m= rank+1,7
       src_off(m) = 0
    enddo
    j7d = 1
    factor = 1
    do m = 1,rank
       j7d = j7d + (dst_lo(m) - dlb(m))*factor
       factor = factor * dextent(1)
    enddo
    j7s = 1
    factor = 1
    do m = 1,rank
       j7s = j7s + (src_lo(m) - slb(m))*factor
       factor = factor * sextent(1)
    enddo
    pass = 1
    index = 0
    jbase = joff -1
    do i7 = dst_lo(7),dst_hi(7),dst_stride(7)
       j6d = j7d
       j6s = j7s
       do i6 = dst_lo(6),dst_hi(6),dst_stride(6)
          j5d = j6d
          j5s = j6s
          do i5 = dst_lo(5),dst_hi(5),dst_stride(5)
             j4d = j5d
             j4s = j5s
             do k4 = dst_lo(4),dst_hi(4),dst_stride(4)
                j3d = j4d
                j3s = j4s
                do i3 = dst_lo(3),dst_hi(3),dst_stride(3)
                   j2d = j3d
                   j2s = j3s
                   do i2 = dst_lo(2),dst_hi(2),dst_stride(2)
                      j1d = j2d
                      j1s = j2s
                      do i1 = dst_lo(1),dst_hi(1),dst_stride(1)
                         if (b(j1d) .ne. cmplx(jbase+j1s,jbase+j1s,c4)) then
                            pass = 0
                            index = j1d
                         endif
                         j1d = j1d + dst_off(1) 
                         j1s = j1s + src_off(1)
                      enddo
                      j2d = j2d + dst_off(2)
                      j2s = j2s + src_off(2)
                   enddo
                   j3d = j3d + dst_off(3)
                   j3s = j3s + src_off(3)
                enddo
                j4d = j4d + dst_off(4)
                j4s = j4s + src_off(4)
             enddo
             j5d = j5d + dst_off(5)
             j5s = j5s + src_off(5)
          enddo
          j6d = j6d + dst_off(6)
          j6s = j6s + src_off(6)
       enddo
       j7d = j7d + dst_off(7)
       j7s = j7s + src_off(7)
    enddo
    return
  end subroutine check_b_c4

  subroutine check_b_c8(rank,b,dst_slice,src_slice,remote_proc,dlb,dub, &
 &                      slb,sub,joff,pass)
    implicit none
    integer rank
    complex(kind=c8), pointer :: b(:)
    type(armci_slice) :: dst_slice, src_slice
    integer remote_proc
    integer dlb(7),dub(7),slb(7),sub(7)
    integer joff
    integer pass
    integer me
    integer index
    integer dextent(7)
    integer sextent(7)
    integer dst_stride(7)
    integer src_stride(7)
    integer src_off(7),dst_off(7)
    integer src_lo(7),src_hi(7)
    integer dst_lo(7),dst_hi(7)
    integer i,m,i1,i2,i3,k4,i5,i6,i7
    integer j1s,j2s,j3s,j4s,j5s,j6s,j7s
    integer j1d,j2d,j3d,j4d,j5d,j6d,j7d
    integer factor
    integer myunit
    integer jbase
    dst_lo(:) = 1
    dst_hi(:) = 1
    dst_stride(:) = 1
    do m = 1,rank
       dst_lo(m) = dst_slice%lo(m)
    enddo
    do m = 1,rank
       dst_hi(m) = dst_slice%hi(m)
    enddo
    do m = 1,rank
       dst_stride(m) = dst_slice%stride(m)
    enddo

    src_lo(:) = 1
    src_hi(:) = 1
    src_stride(:) = 1
    do m = 1,rank
       src_lo(m) = src_slice%lo(m)
    enddo
    do m = 1,rank
       src_hi(m) = src_slice%hi(m)
    enddo
    do m = 1,rank
       src_stride(m) = src_slice%stride(m)
    enddo

    do m = 1,rank
       sextent(m) = sub(m) - slb(m) + 1
    enddo

    do m = 1,rank
       dextent(m) = dub(m) - dlb(m) + 1
    enddo

    do m = rank+1,7
       dextent(m) = 1
    enddo
    do m = rank+1,7
       sextent(m) = 1
    enddo

    src_off(1) = 1
    do m = 2,rank
       src_off(m) = src_off(m-1)*sextent(m-1)
    enddo
    
    dst_off(1) = 1
    do m = 2,rank
       dst_off(m) = dst_off(m-1)*dextent(m-1)
    enddo

    do m = 1,rank
       src_off(m) = src_off(m) * src_slice%stride(m)
    enddo
       
    do m = 1,rank
       dst_off(m) = dst_off(m) * dst_slice%stride(m)
    enddo

    do m= rank+1,7
       dst_off(m) = 0
    enddo
    do m= rank+1,7
       src_off(m) = 0
    enddo
    j7d = 1
    factor = 1
    do m = 1,rank
       j7d = j7d + (dst_lo(m) - dlb(m))*factor
       factor = factor * dextent(1)
    enddo
    j7s = 1
    factor = 1
    do m = 1,rank
       j7s = j7s + (src_lo(m) - slb(m))*factor
       factor = factor * sextent(1)
    enddo
    pass = 1
    index = 0
    jbase = joff -1
    do i7 = dst_lo(7),dst_hi(7),dst_stride(7)
       j6d = j7d
       j6s = j7s
       do i6 = dst_lo(6),dst_hi(6),dst_stride(6)
          j5d = j6d
          j5s = j6s
          do i5 = dst_lo(5),dst_hi(5),dst_stride(5)
             j4d = j5d
             j4s = j5s
             do k4 = dst_lo(4),dst_hi(4),dst_stride(4)
                j3d = j4d
                j3s = j4s
                do i3 = dst_lo(3),dst_hi(3),dst_stride(3)
                   j2d = j3d
                   j2s = j3s
                   do i2 = dst_lo(2),dst_hi(2),dst_stride(2)
                      j1d = j2d
                      j1s = j2s
                      do i1 = dst_lo(1),dst_hi(1),dst_stride(1)
                         if (b(j1d) .ne. cmplx(jbase+j1s,jbase+j1s,c8)) then
                            pass = 0
                            index = j1d
                         endif
                         j1d = j1d + dst_off(1) 
                         j1s = j1s + src_off(1)
                      enddo
                      j2d = j2d + dst_off(2)
                      j2s = j2s + src_off(2)
                   enddo
                   j3d = j3d + dst_off(3)
                   j3s = j3s + src_off(3)
                enddo
                j4d = j4d + dst_off(4)
                j4s = j4s + src_off(4)
             enddo
             j5d = j5d + dst_off(5)
             j5s = j5s + src_off(5)
          enddo
          j6d = j6d + dst_off(6)
          j6s = j6s + src_off(6)
       enddo
       j7d = j7d + dst_off(7)
       j7s = j7s + src_off(7)
    enddo
    return
  end subroutine check_b_c8

end module checkput
