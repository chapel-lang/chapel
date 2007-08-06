module testa_init
  use definekind
  interface init_7d
     module procedure init_7d_i4
     module procedure init_7d_i8
     module procedure init_7d_r4
     module procedure init_7d_r8
     module procedure init_7d_c4
     module procedure init_7d_c8
  end interface

contains

      subroutine init_7d_i4(a,b,alb,aub,blb,bub,aoff,boff)
        integer(kind=i4), pointer :: a(:,:,:,:,:,:,:)
        integer(kind=i4), pointer :: b(:,:,:,:,:,:,:)
        integer alb(7),aub(7),blb(7),bub(7)
        integer aoff, boff 
        integer i1,i2,i3,k4,i5,i6,i7
        integer j
        j = aoff
        do i7 = alb(7),aub(7)
           do i6 = alb(6),aub(6)
              do i5 = alb(5),aub(5)
                 do k4 = alb(4),aub(4)
                    do i3 = alb(3),aub(3)
                       do i2 = alb(2),aub(2)
                          do i1 = alb(1),aub(1)
                             a(i1,i2,i3,k4,i5,i6,i7) = j
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        j = boff
        do i7 = blb(7),bub(7)
           do i6 = blb(6),bub(6)
              do i5 = blb(5),bub(5)
                 do k4 = blb(4),bub(4)
                    do i3 = blb(3),bub(3)
                       do i2 = blb(2),bub(2)
                          do i1 = blb(1),bub(1)
                             b(i1,i2,i3,k4,i5,i6,i7) = j
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        return
      end subroutine init_7d_i4

      subroutine init_7d_i8(a,b,alb,aub,blb,bub,aoff,boff)
        integer(kind=i8), pointer :: a(:,:,:,:,:,:,:)
        integer(kind=i8), pointer :: b(:,:,:,:,:,:,:)
        integer alb(7),aub(7),blb(7),bub(7)
        integer aoff, boff 
        integer i1,i2,i3,k4,i5,i6,i7
        integer(kind=i8) :: j
        j = aoff
        do i7 = alb(7),aub(7)
           do i6 = alb(6),aub(6)
              do i5 = alb(5),aub(5)
                 do k4 = alb(4),aub(4)
                    do i3 = alb(3),aub(3)
                       do i2 = alb(2),aub(2)
                          do i1 = alb(1),aub(1)
                             a(i1,i2,i3,k4,i5,i6,i7) = j
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        j = boff
        do i7 = blb(7),bub(7)
           do i6 = blb(6),bub(6)
              do i5 = blb(5),bub(5)
                 do k4 = blb(4),bub(4)
                    do i3 = blb(3),bub(3)
                       do i2 = blb(2),bub(2)
                          do i1 = blb(1),bub(1)
                             b(i1,i2,i3,k4,i5,i6,i7) = j
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        return
      end subroutine init_7d_i8

      subroutine init_7d_r4(a,b,alb,aub,blb,bub,aoff,boff)
        real(kind=r4), pointer :: a(:,:,:,:,:,:,:)
        real(kind=r4), pointer :: b(:,:,:,:,:,:,:)
        integer alb(7),aub(7),blb(7),bub(7)
        integer aoff, boff 
        integer i1,i2,i3,k4,i5,i6,i7
        integer(kind=i4) :: j
        j = aoff
        do i7 = alb(7),aub(7)
           do i6 = alb(6),aub(6)
              do i5 = alb(5),aub(5)
                 do k4 = alb(4),aub(4)
                    do i3 = alb(3),aub(3)
                       do i2 = alb(2),aub(2)
                          do i1 = alb(1),aub(1)
                             a(i1,i2,i3,k4,i5,i6,i7) = float(j)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        j = boff
        do i7 = blb(7),bub(7)
           do i6 = blb(6),bub(6)
              do i5 = blb(5),bub(5)
                 do k4 = blb(4),bub(4)
                    do i3 = blb(3),bub(3)
                       do i2 = blb(2),bub(2)
                          do i1 = blb(1),bub(1)
                             b(i1,i2,i3,k4,i5,i6,i7) = float(j)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        return
      end subroutine init_7d_r4

      subroutine init_7d_r8(a,b,alb,aub,blb,bub,aoff,boff)
        real(kind=r8), pointer :: a(:,:,:,:,:,:,:)
        real(kind=r8), pointer :: b(:,:,:,:,:,:,:)
        integer alb(7),aub(7),blb(7),bub(7)
        integer aoff, boff 
        integer i1,i2,i3,k4,i5,i6,i7
        integer(kind=i4) :: j
        j = aoff
        do i7 = alb(7),aub(7)
           do i6 = alb(6),aub(6)
              do i5 = alb(5),aub(5)
                 do k4 = alb(4),aub(4)
                    do i3 = alb(3),aub(3)
                       do i2 = alb(2),aub(2)
                          do i1 = alb(1),aub(1)
                             a(i1,i2,i3,k4,i5,i6,i7) = dble(j)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        j = boff
        do i7 = blb(7),bub(7)
           do i6 = blb(6),bub(6)
              do i5 = blb(5),bub(5)
                 do k4 = blb(4),bub(4)
                    do i3 = blb(3),bub(3)
                       do i2 = blb(2),bub(2)
                          do i1 = blb(1),bub(1)
                             b(i1,i2,i3,k4,i5,i6,i7) = dble(j)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        return
      end subroutine init_7d_r8

      subroutine init_7d_c4(a,b,alb,aub,blb,bub,aoff,boff)
        complex(kind=c4), pointer :: a(:,:,:,:,:,:,:)
        complex(kind=c4), pointer :: b(:,:,:,:,:,:,:)
        integer alb(7),aub(7),blb(7),bub(7)
        integer aoff, boff 
        integer i1,i2,i3,k4,i5,i6,i7
        integer(kind=i4) :: j
        j = aoff
        do i7 = alb(7),aub(7)
           do i6 = alb(6),aub(6)
              do i5 = alb(5),aub(5)
                 do k4 = alb(4),aub(4)
                    do i3 = alb(3),aub(3)
                       do i2 = alb(2),aub(2)
                          do i1 = alb(1),aub(1)
                             a(i1,i2,i3,k4,i5,i6,i7) = cmplx(j,j,c4)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        j = boff
        do i7 = blb(7),bub(7)
           do i6 = blb(6),bub(6)
              do i5 = blb(5),bub(5)
                 do k4 = blb(4),bub(4)
                    do i3 = blb(3),bub(3)
                       do i2 = blb(2),bub(2)
                          do i1 = blb(1),bub(1)
                             b(i1,i2,i3,k4,i5,i6,i7) = cmplx(j,j,c4)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        return
      end subroutine init_7d_c4

      subroutine init_7d_c8(a,b,alb,aub,blb,bub,aoff,boff)
        complex(kind=c8), pointer :: a(:,:,:,:,:,:,:)
        complex(kind=c8), pointer :: b(:,:,:,:,:,:,:)
        integer alb(7),aub(7),blb(7),bub(7)
        integer aoff,boff
        integer i1,i2,i3,k4,i5,i6,i7
        integer(kind=i4) :: j
        j = aoff
        do i7 = alb(7),aub(7)
           do i6 = alb(6),aub(6)
              do i5 = alb(5),aub(5)
                 do k4 = alb(4),aub(4)
                    do i3 = alb(3),aub(3)
                       do i2 = alb(2),aub(2)
                          do i1 = alb(1),aub(1)
                             a(i1,i2,i3,k4,i5,i6,i7) = cmplx(j,j,c8)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        j = boff
        do i7 = blb(7),bub(7)
           do i6 = blb(6),bub(6)
              do i5 = blb(5),bub(5)
                 do k4 = blb(4),bub(4)
                    do i3 = blb(3),bub(3)
                       do i2 = blb(2),bub(2)
                          do i1 = blb(1),bub(1)
                             b(i1,i2,i3,k4,i5,i6,i7) = cmplx(j,j,c8)
                             j = j + 1
                          enddo
                       enddo
                    enddo
                 enddo
              enddo
           enddo
        enddo
        return
      end subroutine init_7d_c8
    end module testa_init
