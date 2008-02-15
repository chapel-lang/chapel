module armci_mov_f90

use armci_types
interface armci_put_fa
  module procedure armci_put_1di4
  module procedure armci_put_2di4
  module procedure armci_put_3di4
  module procedure armci_put_4di4
  module procedure armci_put_5di4
  module procedure armci_put_6di4
  module procedure armci_put_7di4
  module procedure armci_put_1di8
  module procedure armci_put_2di8
  module procedure armci_put_3di8
  module procedure armci_put_4di8
  module procedure armci_put_5di8
  module procedure armci_put_6di8
  module procedure armci_put_7di8
  module procedure armci_put_1dr4
  module procedure armci_put_2dr4
  module procedure armci_put_3dr4
  module procedure armci_put_4dr4
  module procedure armci_put_5dr4
  module procedure armci_put_6dr4
  module procedure armci_put_7dr4
  module procedure armci_put_1dr8
  module procedure armci_put_2dr8
  module procedure armci_put_3dr8
  module procedure armci_put_4dr8
  module procedure armci_put_5dr8
  module procedure armci_put_6dr8
  module procedure armci_put_7dr8
  module procedure armci_put_1dc4
  module procedure armci_put_2dc4
  module procedure armci_put_3dc4
  module procedure armci_put_4dc4
  module procedure armci_put_5dc4
  module procedure armci_put_6dc4
  module procedure armci_put_7dc4
  module procedure armci_put_1dc8
  module procedure armci_put_2dc8
  module procedure armci_put_3dc8
  module procedure armci_put_4dc8
  module procedure armci_put_5dc8
  module procedure armci_put_6dc8
  module procedure armci_put_7dc8
end interface

interface armci_get_fa
  module procedure armci_get_1di4
  module procedure armci_get_2di4
  module procedure armci_get_3di4
  module procedure armci_get_4di4
  module procedure armci_get_5di4
  module procedure armci_get_6di4
  module procedure armci_get_7di4
  module procedure armci_get_1di8
  module procedure armci_get_2di8
  module procedure armci_get_3di8
  module procedure armci_get_4di8
  module procedure armci_get_5di8
  module procedure armci_get_6di8
  module procedure armci_get_7di8
  module procedure armci_get_1dr4
  module procedure armci_get_2dr4
  module procedure armci_get_3dr4
  module procedure armci_get_4dr4
  module procedure armci_get_5dr4
  module procedure armci_get_6dr4
  module procedure armci_get_7dr4
  module procedure armci_get_1dr8
  module procedure armci_get_2dr8
  module procedure armci_get_3dr8
  module procedure armci_get_4dr8
  module procedure armci_get_5dr8
  module procedure armci_get_6dr8
  module procedure armci_get_7dr8
  module procedure armci_get_1dc4
  module procedure armci_get_2dc4
  module procedure armci_get_3dc4
  module procedure armci_get_4dc4
  module procedure armci_get_5dc4
  module procedure armci_get_6dc4
  module procedure armci_get_7dc4
  module procedure armci_get_1dc8
  module procedure armci_get_2dc8
  module procedure armci_get_3dc8
  module procedure armci_get_4dc8
  module procedure armci_get_5dc8
  module procedure armci_get_6dc8
  module procedure armci_get_7dc8
end interface

contains 

subroutine armci_put_1di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_1di4

subroutine armci_put_2di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_2di4

subroutine armci_put_3di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_3di4

subroutine armci_put_4di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_4di4

subroutine armci_put_5di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_5di4

subroutine armci_put_6di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_6di4

subroutine armci_put_7di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_7di4

subroutine armci_put_1di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_1di8

subroutine armci_put_2di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_2di8

subroutine armci_put_3di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_3di8

subroutine armci_put_4di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_4di8

subroutine armci_put_5di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_5di8

subroutine armci_put_6di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_6di8

subroutine armci_put_7di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_7di8

subroutine armci_put_1dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_1dr4

subroutine armci_put_2dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_2dr4

subroutine armci_put_3dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_3dr4

subroutine armci_put_4dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_4dr4

subroutine armci_put_5dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_5dr4

subroutine armci_put_6dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_6dr4

subroutine armci_put_7dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_7dr4

subroutine armci_put_1dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_1dr8

subroutine armci_put_2dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_2dr8

subroutine armci_put_3dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_3dr8

subroutine armci_put_4dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_4dr8

subroutine armci_put_5dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_5dr8

subroutine armci_put_6dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_6dr8

subroutine armci_put_7dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_7dr8

subroutine armci_put_1dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_1dc4

subroutine armci_put_2dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_2dc4

subroutine armci_put_3dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_3dc4

subroutine armci_put_4dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_4dc4

subroutine armci_put_5dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_5dc4

subroutine armci_put_6dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_6dc4

subroutine armci_put_7dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_7dc4

subroutine armci_put_1dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_1dc8

subroutine armci_put_2dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 2
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_2dc8

subroutine armci_put_3dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 3
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_3dc8

subroutine armci_put_4dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 4
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_4dc8

subroutine armci_put_5dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 5
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_5dc8

subroutine armci_put_6dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 6
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_6dc8

subroutine armci_put_7dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_put_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_put_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_put_7dc8

!get

subroutine armci_get_1di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_1di4

subroutine armci_get_2di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_2di4

subroutine armci_get_3di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_3di4

subroutine armci_get_4di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_4di4

subroutine armci_get_5di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_5di4

subroutine armci_get_6di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_6di4

subroutine armci_get_7di4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_7di4

subroutine armci_get_1di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_1di8

subroutine armci_get_2di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_2di8

subroutine armci_get_3di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_3di8

subroutine armci_get_4di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_4di8

subroutine armci_get_5di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_5di8

subroutine armci_get_6di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_6di8

subroutine armci_get_7di8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_7di8

subroutine armci_get_1dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_1dr4

subroutine armci_get_2dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_2dr4

subroutine armci_get_3dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_3dr4

subroutine armci_get_4dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_4dr4

subroutine armci_get_5dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_5dr4

subroutine armci_get_6dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_6dr4

subroutine armci_get_7dr4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_7dr4

subroutine armci_get_1dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_1dr8

subroutine armci_get_2dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_2dr8

subroutine armci_get_3dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_3dr8

subroutine armci_get_4dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_4dr8

subroutine armci_get_5dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_5dr8

subroutine armci_get_6dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_6dr8

subroutine armci_get_7dr8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_7dr8

subroutine armci_get_1dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c4), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_1dc4

subroutine armci_get_2dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c4), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_2dc4

subroutine armci_get_3dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c4), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_3dc4

subroutine armci_get_4dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_4dc4

subroutine armci_get_5dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_5dc4

subroutine armci_get_6dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_6dc4

subroutine armci_get_7dc4(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_7dc4

subroutine armci_get_1dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c8), dimension(:), pointer :: src, dst
  type(armci_slice), intent(in)           :: src_slc, dst_slc
  integer, intent(in)                     :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 1
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_1dc8

subroutine armci_get_2dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c8), dimension(:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 2
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_2dc8

subroutine armci_get_3dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c8), dimension(:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 3
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_3dc8

subroutine armci_get_4dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer :: rank

  rank = 4
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_4dc8

subroutine armci_get_5dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                    :: rc
  integer  :: rank

  rank = 5
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_5dc8

subroutine armci_get_6dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer  :: rank

  rank = 6
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_6dc8

subroutine armci_get_7dc8(src, src_slc, dst, dst_slc, proc, rc)
  use definekind
  implicit none
  interface
    subroutine armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)
      use definekind
      use armci_types
      complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
      type(armci_slice), intent(in) :: src_slc, dst_slc
      integer, intent(in) :: proc, rank
      integer, intent(out) :: rc
    end subroutine armci_get_farrays
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: src, dst
  type(armci_slice), intent(in)             :: src_slc, dst_slc
  integer, intent(in)                       :: proc
  integer, intent(out)                      :: rc
  integer :: rank

  rank = 7
  call armci_get_farrays(src, src_slc, dst, dst_slc, proc, rank, rc)

end subroutine armci_get_7dc8

end module armci_mov_f90
