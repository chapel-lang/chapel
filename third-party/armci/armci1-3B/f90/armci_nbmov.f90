module ARMCI_NbMov

use armci_types
interface ARMCI_NbPut_fa
  module procedure ARMCI_NbPut_1di4
  module procedure ARMCI_NbPut_2di4
  module procedure ARMCI_NbPut_3di4
  module procedure ARMCI_NbPut_4di4
  module procedure ARMCI_NbPut_5di4
  module procedure ARMCI_NbPut_6di4
  module procedure ARMCI_NbPut_7di4
  module procedure ARMCI_NbPut_1di8
  module procedure ARMCI_NbPut_2di8
  module procedure ARMCI_NbPut_3di8
  module procedure ARMCI_NbPut_4di8
  module procedure ARMCI_NbPut_5di8
  module procedure ARMCI_NbPut_6di8
  module procedure ARMCI_NbPut_7di8
  module procedure ARMCI_NbPut_1dr4
  module procedure ARMCI_NbPut_2dr4
  module procedure ARMCI_NbPut_3dr4
  module procedure ARMCI_NbPut_4dr4
  module procedure ARMCI_NbPut_5dr4
  module procedure ARMCI_NbPut_6dr4
  module procedure ARMCI_NbPut_7dr4
  module procedure ARMCI_NbPut_1dr8
  module procedure ARMCI_NbPut_2dr8
  module procedure ARMCI_NbPut_3dr8
  module procedure ARMCI_NbPut_4dr8
  module procedure ARMCI_NbPut_5dr8
  module procedure ARMCI_NbPut_6dr8
  module procedure ARMCI_NbPut_7dr8
  module procedure ARMCI_NbPut_1dc4
  module procedure ARMCI_NbPut_2dc4
  module procedure ARMCI_NbPut_3dc4
  module procedure ARMCI_NbPut_4dc4
  module procedure ARMCI_NbPut_5dc4
  module procedure ARMCI_NbPut_6dc4
  module procedure ARMCI_NbPut_7dc4
  module procedure ARMCI_NbPut_1dc8
  module procedure ARMCI_NbPut_2dc8
  module procedure ARMCI_NbPut_3dc8
  module procedure ARMCI_NbPut_4dc8
  module procedure ARMCI_NbPut_5dc8
  module procedure ARMCI_NbPut_6dc8
  module procedure ARMCI_NbPut_7dc8
end interface

interface ARMCI_NbGet_fa
  module procedure ARMCI_NbGet_1di4
  module procedure ARMCI_NbGet_2di4
  module procedure ARMCI_NbGet_3di4
  module procedure ARMCI_NbGet_4di4
  module procedure ARMCI_NbGet_5di4
  module procedure ARMCI_NbGet_6di4
  module procedure ARMCI_NbGet_7di4
  module procedure ARMCI_NbGet_1di8
  module procedure ARMCI_NbGet_2di8
  module procedure ARMCI_NbGet_3di8
  module procedure ARMCI_NbGet_4di8
  module procedure ARMCI_NbGet_5di8
  module procedure ARMCI_NbGet_6di8
  module procedure ARMCI_NbGet_7di8
  module procedure ARMCI_NbGet_1dr4
  module procedure ARMCI_NbGet_2dr4
  module procedure ARMCI_NbGet_3dr4
  module procedure ARMCI_NbGet_4dr4
  module procedure ARMCI_NbGet_5dr4
  module procedure ARMCI_NbGet_6dr4
  module procedure ARMCI_NbGet_7dr4
  module procedure ARMCI_NbGet_1dr8
  module procedure ARMCI_NbGet_2dr8
  module procedure ARMCI_NbGet_3dr8
  module procedure ARMCI_NbGet_4dr8
  module procedure ARMCI_NbGet_5dr8
  module procedure ARMCI_NbGet_6dr8
  module procedure ARMCI_NbGet_7dr8
  module procedure ARMCI_NbGet_1dc4
  module procedure ARMCI_NbGet_2dc4
  module procedure ARMCI_NbGet_3dc4
  module procedure ARMCI_NbGet_4dc4
  module procedure ARMCI_NbGet_5dc4
  module procedure ARMCI_NbGet_6dc4
  module procedure ARMCI_NbGet_7dc4
  module procedure ARMCI_NbGet_1dc8
  module procedure ARMCI_NbGet_2dc8
  module procedure ARMCI_NbGet_3dc8
  module procedure ARMCI_NbGet_4dc8
  module procedure ARMCI_NbGet_5dc8
  module procedure ARMCI_NbGet_6dc8
  module procedure ARMCI_NbGet_7dc8
end interface

contains 

subroutine ARMCI_NbPut_1di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use ARMCI_types
      integer(kind=I4), dimension(:), pointer :: src, dst
      type(ARMCI_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_1di4

subroutine ARMCI_NbPut_2di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_2di4

subroutine ARMCI_NbPut_3di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_3di4

subroutine ARMCI_NbPut_4di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_4di4

subroutine ARMCI_NbPut_5di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_5di4

subroutine ARMCI_NbPut_6di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_6di4

subroutine ARMCI_NbPut_7di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_7di4

subroutine ARMCI_NbPut_1di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_1di8

subroutine ARMCI_NbPut_2di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_2di8

subroutine ARMCI_NbPut_3di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_3di8

subroutine ARMCI_NbPut_4di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_4di8

subroutine ARMCI_NbPut_5di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_5di8

subroutine ARMCI_NbPut_6di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_6di8

subroutine ARMCI_NbPut_7di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_7di8

subroutine ARMCI_NbPut_1dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_1dr4

subroutine ARMCI_NbPut_2dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_2dr4

subroutine ARMCI_NbPut_3dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_3dr4

subroutine ARMCI_NbPut_4dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_4dr4

subroutine ARMCI_NbPut_5dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_5dr4

subroutine ARMCI_NbPut_6dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_6dr4

subroutine ARMCI_NbPut_7dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_7dr4

subroutine ARMCI_NbPut_1dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_1dr8

subroutine ARMCI_NbPut_2dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_2dr8

subroutine ARMCI_NbPut_3dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_3dr8

subroutine ARMCI_NbPut_4dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_4dr8

subroutine ARMCI_NbPut_5dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_5dr8

subroutine ARMCI_NbPut_6dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_6dr8

subroutine ARMCI_NbPut_7dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_7dr8

subroutine ARMCI_NbPut_1dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_1dc4

subroutine ARMCI_NbPut_2dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_2dc4

subroutine ARMCI_NbPut_3dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_3dc4

subroutine ARMCI_NbPut_4dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_4dc4

subroutine ARMCI_NbPut_5dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_5dc4

subroutine ARMCI_NbPut_6dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_6dc4

subroutine ARMCI_NbPut_7dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_7dc4

subroutine ARMCI_NbPut_1dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_1dc8

subroutine ARMCI_NbPut_2dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_2dc8

subroutine ARMCI_NbPut_3dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_3dc8

subroutine ARMCI_NbPut_4dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_4dc8

subroutine ARMCI_NbPut_5dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_5dc8

subroutine ARMCI_NbPut_6dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_6dc8

subroutine ARMCI_NbPut_7dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbPut_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbPut_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbPut_7dc8

!Get

subroutine ARMCI_NbGet_1di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_1di4

subroutine ARMCI_NbGet_2di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_2di4

subroutine ARMCI_NbGet_3di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_3di4

subroutine ARMCI_NbGet_4di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_4di4

subroutine ARMCI_NbGet_5di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_5di4

subroutine ARMCI_NbGet_6di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_6di4

subroutine ARMCI_NbGet_7di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_7di4

subroutine ARMCI_NbGet_1di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_1di8

subroutine ARMCI_NbGet_2di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_2di8

subroutine ARMCI_NbGet_3di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_3di8

subroutine ARMCI_NbGet_4di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_4di8

subroutine ARMCI_NbGet_5di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_5di8

subroutine ARMCI_NbGet_6di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_6di8

subroutine ARMCI_NbGet_7di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=I8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  integer(kind=I8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_7di8

subroutine ARMCI_NbGet_1dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_1dr4

subroutine ARMCI_NbGet_2dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_2dr4

subroutine ARMCI_NbGet_3dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_3dr4

subroutine ARMCI_NbGet_4dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_4dr4

subroutine ARMCI_NbGet_5dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_5dr4

subroutine ARMCI_NbGet_6dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_6dr4

subroutine ARMCI_NbGet_7dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_7dr4

subroutine ARMCI_NbGet_1dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_1dr8

subroutine ARMCI_NbGet_2dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_2dr8

subroutine ARMCI_NbGet_3dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_3dr8

subroutine ARMCI_NbGet_4dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_4dr8

subroutine ARMCI_NbGet_5dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_5dr8

subroutine ARMCI_NbGet_6dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_6dr8

subroutine ARMCI_NbGet_7dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=R8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  real(kind=R8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_7dr8

subroutine ARMCI_NbGet_1dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_1dc4

subroutine ARMCI_NbGet_2dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_2dc4

subroutine ARMCI_NbGet_3dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_3dc4

subroutine ARMCI_NbGet_4dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_4dc4

subroutine ARMCI_NbGet_5dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_5dc4

subroutine ARMCI_NbGet_6dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_6dc4

subroutine ARMCI_NbGet_7dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_7dc4

subroutine ARMCI_NbGet_1dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_1dc8

subroutine ARMCI_NbGet_2dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_2dc8

subroutine ARMCI_NbGet_3dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_3dc8

subroutine ARMCI_NbGet_4dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_4dc8

subroutine ARMCI_NbGet_5dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_5dc8

subroutine ARMCI_NbGet_6dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_6dc8

subroutine ARMCI_NbGet_7dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
!  implicit none
  interface
    subroutine ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=C8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine ARMCI_NbGet_farrays
  end interface
  complex(kind=C8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call ARMCI_NbGet_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine ARMCI_NbGet_7dc8

end module ARMCI_NbMov
