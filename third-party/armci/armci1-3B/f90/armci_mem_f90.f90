module armci_mem_f90

use armci_types
interface armci_malloc_fa
  module procedure armci_malloc_1di4
  module procedure armci_malloc_2di4
  module procedure armci_malloc_3di4
  module procedure armci_malloc_4di4
  module procedure armci_malloc_5di4
  module procedure armci_malloc_6di4
  module procedure armci_malloc_7di4
  module procedure armci_malloc_1di8
  module procedure armci_malloc_2di8
  module procedure armci_malloc_3di8
  module procedure armci_malloc_4di8
  module procedure armci_malloc_5di8
  module procedure armci_malloc_6di8
  module procedure armci_malloc_7di8
  module procedure armci_malloc_1dr4
  module procedure armci_malloc_2dr4
  module procedure armci_malloc_3dr4
  module procedure armci_malloc_4dr4
  module procedure armci_malloc_5dr4
  module procedure armci_malloc_6dr4
  module procedure armci_malloc_7dr4
  module procedure armci_malloc_1dr8
  module procedure armci_malloc_2dr8
  module procedure armci_malloc_3dr8
  module procedure armci_malloc_4dr8
  module procedure armci_malloc_5dr8
  module procedure armci_malloc_6dr8
  module procedure armci_malloc_7dr8
  module procedure armci_malloc_1dc4
  module procedure armci_malloc_2dc4
  module procedure armci_malloc_3dc4
  module procedure armci_malloc_4dc4
  module procedure armci_malloc_5dc4
  module procedure armci_malloc_6dc4
  module procedure armci_malloc_7dc4
  module procedure armci_malloc_1dc8
  module procedure armci_malloc_2dc8
  module procedure armci_malloc_3dc8
  module procedure armci_malloc_4dc8
  module procedure armci_malloc_5dc8
  module procedure armci_malloc_6dc8
  module procedure armci_malloc_7dc8
end interface

interface armci_free_fa
  module procedure armci_free_1di4
  module procedure armci_free_2di4
  module procedure armci_free_3di4
  module procedure armci_free_4di4
  module procedure armci_free_5di4
  module procedure armci_free_6di4
  module procedure armci_free_7di4
  module procedure armci_free_1di8
  module procedure armci_free_2di8
  module procedure armci_free_3di8
  module procedure armci_free_4di8
  module procedure armci_free_5di8
  module procedure armci_free_6di8
  module procedure armci_free_7di8
  module procedure armci_free_1dr4
  module procedure armci_free_2dr4
  module procedure armci_free_3dr4
  module procedure armci_free_4dr4
  module procedure armci_free_5dr4
  module procedure armci_free_6dr4
  module procedure armci_free_7dr4
  module procedure armci_free_1dr8
  module procedure armci_free_2dr8
  module procedure armci_free_3dr8
  module procedure armci_free_4dr8
  module procedure armci_free_5dr8
  module procedure armci_free_6dr8
  module procedure armci_free_7dr8
  module procedure armci_free_1dc4
  module procedure armci_free_2dc4
  module procedure armci_free_3dc4
  module procedure armci_free_4dc4
  module procedure armci_free_5dc4
  module procedure armci_free_6dc4
  module procedure armci_free_7dc4
  module procedure armci_free_1dc8
  module procedure armci_free_2dc8
  module procedure armci_free_3dc8
  module procedure armci_free_4dc8
  module procedure armci_free_5dc8
  module procedure armci_free_6dc8
  module procedure armci_free_7dc8
end interface

contains 

subroutine armci_malloc_1di4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i4), dimension(:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(1), ub(1)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i4), dimension(:), pointer :: a
  integer, dimension(1) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 1

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_1di4: error = ", rc

end subroutine armci_malloc_1di4

subroutine armci_malloc_2di4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i4), dimension(:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(2), ub(2)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i4), dimension(:,:), pointer :: a
  integer, dimension(2) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 2

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_2di4: error = ", rc

end subroutine armci_malloc_2di4

subroutine armci_malloc_3di4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(3), ub(3)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i4), dimension(:,:,:), pointer :: a
  integer, dimension(3) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 3

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_3di4: error = ", rc

end subroutine armci_malloc_3di4

subroutine armci_malloc_4di4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(4), ub(4)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:), pointer :: a
  integer, dimension(4) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 4

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_4di4: error = ", rc

end subroutine armci_malloc_4di4

subroutine armci_malloc_5di4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(5), ub(5)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:,:), pointer :: a
  integer, dimension(5) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 5

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_5di4: error = ", rc

end subroutine armci_malloc_5di4

subroutine armci_malloc_6di4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(6), ub(6)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: a
  integer, dimension(6) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 6

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_6di4: error = ", rc

end subroutine armci_malloc_6di4

subroutine armci_malloc_7di4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(7), ub(7)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, dimension(7) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 7

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_7di4: error = ", rc

end subroutine armci_malloc_7di4

subroutine armci_free_1di4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i4), dimension(:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i4), dimension(:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 1

  call armci_free_farray(a, rank, rc)
  if (rc .ne. 0) print *, "error armci_free_1di4: error = ", rc

end subroutine armci_free_1di4

subroutine armci_free_2di4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i4), dimension(:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i4), dimension(:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 2
  call armci_free_farray(a, rank, rc)
  if (rc .ne. 0) print *, "error armci_free_2di4: error = ", rc

end subroutine armci_free_2di4

subroutine armci_free_3di4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i4), dimension(:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 3
  call armci_free_farray(a, rank, rc)
  if (rc .ne. 0) print *, "error armci_free_3di4: error = ", rc

end subroutine armci_free_3di4

subroutine armci_free_4di4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 4
  call armci_free_farray(a, rank, rc)
  if (rc .ne. 0) print *, "error armci_free_4di4: error = ", rc

end subroutine armci_free_4di4

subroutine armci_free_5di4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 5
  call armci_free_farray(a, rank, rc)
  if (rc .ne. 0) print *, "error armci_free_5di4: error = ", rc

end subroutine armci_free_5di4

subroutine armci_free_6di4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 6
  call armci_free_farray(a, rank, rc)
  if (rc .ne. 0) print *, "error armci_free_6di4: error = ", rc

end subroutine armci_free_6di4

subroutine armci_free_7di4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i4), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 7
  call armci_free_farray(a, rank, rc)
  if (rc .ne. 0) print *, "error armci_free_7di4: error = ", rc

end subroutine armci_free_7di4

subroutine armci_malloc_1di8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i8), dimension(:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(1), ub(1)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i8), dimension(:), pointer :: a
  integer, dimension(1) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 1

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_1di8: error = ", rc

end subroutine armci_malloc_1di8

subroutine armci_malloc_2di8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i8), dimension(:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(2), ub(2)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i8), dimension(:,:), pointer :: a
  integer, dimension(2) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 2

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_2di8: error = ", rc

end subroutine armci_malloc_2di8

subroutine armci_malloc_3di8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(3), ub(3)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i8), dimension(:,:,:), pointer :: a
  integer, dimension(3) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 3

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_3di8: error = ", rc

end subroutine armci_malloc_3di8

subroutine armci_malloc_4di8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(4), ub(4)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:), pointer :: a
  integer, dimension(4) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 4

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_4di8: error = ", rc

end subroutine armci_malloc_4di8

subroutine armci_malloc_5di8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(5), ub(5)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:,:), pointer :: a
  integer, dimension(5) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 5

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
  if (rc .ne. 0) print *, "error armci_malloc_5di8: error = ", rc

end subroutine armci_malloc_5di8

subroutine armci_malloc_6di8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(6), ub(6)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: a
  integer, dimension(6) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 6

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_6di8: error = ", rc

end subroutine armci_malloc_6di8

subroutine armci_malloc_7di8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(7), ub(7)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, dimension(7) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 7

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_7di8: error = ", rc

end subroutine armci_malloc_7di8

subroutine armci_free_1di8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i8), dimension(:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i8), dimension(:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 1

  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_1di8: error = ", rc

end subroutine armci_free_1di8

subroutine armci_free_2di8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i8), dimension(:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i8), dimension(:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 2
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_2di8: error = ", rc

end subroutine armci_free_2di8

subroutine armci_free_3di8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i8), dimension(:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 3
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_3di8: error = ", rc

end subroutine armci_free_3di8

subroutine armci_free_4di8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 4
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_4di8: error = ", rc

end subroutine armci_free_4di8

subroutine armci_free_5di8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 5
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_5di8: error = ", rc

end subroutine armci_free_5di8

subroutine armci_free_6di8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 6
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_6di8: error = ", rc

end subroutine armci_free_6di8

subroutine armci_free_7di8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  integer(kind=i8), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 7
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_7di8: error = ", rc

end subroutine armci_free_7di8

subroutine armci_malloc_1dr4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r4), dimension(:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(1), ub(1)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r4), dimension(:), pointer :: a
  integer, dimension(1) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 1

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_1dr4: error = ", rc

end subroutine armci_malloc_1dr4

subroutine armci_malloc_2dr4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r4), dimension(:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(2), ub(2)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r4), dimension(:,:), pointer :: a
  integer, dimension(2) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 2

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_2dr4: error = ", rc

end subroutine armci_malloc_2dr4

subroutine armci_malloc_3dr4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r4), dimension(:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(3), ub(3)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r4), dimension(:,:,:), pointer :: a
  integer, dimension(3) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 3

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_3dr4: error = ", rc

end subroutine armci_malloc_3dr4

subroutine armci_malloc_4dr4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(4), ub(4)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r4), dimension(:,:,:,:), pointer :: a
  integer, dimension(4) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 4

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_4dr4: error = ", rc

end subroutine armci_malloc_4dr4

subroutine armci_malloc_5dr4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(5), ub(5)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r4), dimension(:,:,:,:,:), pointer :: a
  integer, dimension(5) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 5

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_5dr4: error = ", rc

end subroutine armci_malloc_5dr4

subroutine armci_malloc_6dr4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(6), ub(6)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:), pointer :: a
  integer, dimension(6) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 6

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_6dr4: error = ", rc

end subroutine armci_malloc_6dr4

subroutine armci_malloc_7dr4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(7), ub(7)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, dimension(7) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 4
  integer :: rank = 7

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_7dr4: error = ", rc

end subroutine armci_malloc_7dr4

subroutine armci_free_1dr4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r4), dimension(:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r4), dimension(:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 1

  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_1dr4: error = ", rc

end subroutine armci_free_1dr4

subroutine armci_free_2dr4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r4), dimension(:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r4), dimension(:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 2
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_2dr4: error = ", rc

end subroutine armci_free_2dr4

subroutine armci_free_3dr4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r4), dimension(:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r4), dimension(:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 3
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_3dr4: error = ", rc

end subroutine armci_free_3dr4

subroutine armci_free_4dr4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r4), dimension(:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 4
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_4dr4: error = ", rc

end subroutine armci_free_4dr4

subroutine armci_free_5dr4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r4), dimension(:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 5
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_5dr4: error = ", rc

end subroutine armci_free_5dr4

subroutine armci_free_6dr4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 6
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_6dr4: error = ", rc

end subroutine armci_free_6dr4

subroutine armci_free_7dr4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r4), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 7
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_7dr4: error = ", rc

end subroutine armci_free_7dr4

subroutine armci_malloc_1dr8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r8), dimension(:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(1), ub(1)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r8), dimension(:), pointer :: a
  integer, dimension(1) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 1

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_1dr8: error = ", rc

end subroutine armci_malloc_1dr8

subroutine armci_malloc_2dr8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r8), dimension(:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(2), ub(2)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r8), dimension(:,:), pointer :: a
  integer, dimension(2) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 2

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_2dr8: error = ", rc

end subroutine armci_malloc_2dr8

subroutine armci_malloc_3dr8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r8), dimension(:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(3), ub(3)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r8), dimension(:,:,:), pointer :: a
  integer, dimension(3) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 3

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_3dr8: error = ", rc

end subroutine armci_malloc_3dr8

subroutine armci_malloc_4dr8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(4), ub(4)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r8), dimension(:,:,:,:), pointer :: a
  integer, dimension(4) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 4

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_4dr8: error = ", rc

end subroutine armci_malloc_4dr8

subroutine armci_malloc_5dr8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(5), ub(5)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r8), dimension(:,:,:,:,:), pointer :: a
  integer, dimension(5) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 5

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_5dr8: error = ", rc

end subroutine armci_malloc_5dr8

subroutine armci_malloc_6dr8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(6), ub(6)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:), pointer :: a
  integer, dimension(6) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 6

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_6dr8: error = ", rc

end subroutine armci_malloc_6dr8

subroutine armci_malloc_7dr8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(7), ub(7)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, dimension(7) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 7

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_7dr8: error = ", rc

end subroutine armci_malloc_7dr8

subroutine armci_free_1dr8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r8), dimension(:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r8), dimension(:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 1

  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_1dr8: error = ", rc

end subroutine armci_free_1dr8

subroutine armci_free_2dr8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r8), dimension(:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r8), dimension(:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 2
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_2dr8: error = ", rc

end subroutine armci_free_2dr8

subroutine armci_free_3dr8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r8), dimension(:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r8), dimension(:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 3
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_3dr8: error = ", rc

end subroutine armci_free_3dr8

subroutine armci_free_4dr8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r8), dimension(:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 4
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_4dr8: error = ", rc

end subroutine armci_free_4dr8

subroutine armci_free_5dr8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r8), dimension(:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 5
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_5dr8: error = ", rc

end subroutine armci_free_5dr8

subroutine armci_free_6dr8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 6
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_6dr8: error = ", rc

end subroutine armci_free_6dr8

subroutine armci_free_7dr8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  real(kind=r8), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 7
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_7dr8: error = ", rc

end subroutine armci_free_7dr8

subroutine armci_malloc_1dc4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c4), dimension(:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(1), ub(1)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c4), dimension(:), pointer :: a
  integer, dimension(1) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 1

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_1dc4: error = ", rc

end subroutine armci_malloc_1dc4

subroutine armci_malloc_2dc4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c4), dimension(:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(2), ub(2)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c4), dimension(:,:), pointer :: a
  integer, dimension(2) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 2

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_2dc4: error = ", rc

end subroutine armci_malloc_2dc4

subroutine armci_malloc_3dc4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(3), ub(3)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c4), dimension(:,:,:), pointer :: a
  integer, dimension(3) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 3

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_3dc4: error = ", rc

end subroutine armci_malloc_3dc4

subroutine armci_malloc_4dc4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(4), ub(4)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:), pointer :: a
  integer, dimension(4) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 4

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_4dc4: error = ", rc

end subroutine armci_malloc_4dc4

subroutine armci_malloc_5dc4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(5), ub(5)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:,:), pointer :: a
  integer, dimension(5) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 5

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_5dc4: error = ", rc

end subroutine armci_malloc_5dc4

subroutine armci_malloc_6dc4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(6), ub(6)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: a
  integer, dimension(6) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 6

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_6dc4: error = ", rc

end subroutine armci_malloc_6dc4

subroutine armci_malloc_7dc4(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(7), ub(7)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, dimension(7) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 8
  integer :: rank = 7

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_7dc4: error = ", rc

end subroutine armci_malloc_7dc4

subroutine armci_free_1dc4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c4), dimension(:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c4), dimension(:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 1

  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_1dc4: error = ", rc

end subroutine armci_free_1dc4

subroutine armci_free_2dc4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c4), dimension(:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c4), dimension(:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 2
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_2dc4: error = ", rc

end subroutine armci_free_2dc4

subroutine armci_free_3dc4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c4), dimension(:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 3
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_3dc4: error = ", rc

end subroutine armci_free_3dc4

subroutine armci_free_4dc4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 4
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_4dc4: error = ", rc

end subroutine armci_free_4dc4

subroutine armci_free_5dc4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 5
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_5dc4: error = ", rc

end subroutine armci_free_5dc4

subroutine armci_free_6dc4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 6
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_6dc4: error = ", rc

end subroutine armci_free_6dc4

subroutine armci_free_7dc4(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c4), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 7
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_7dc4: error = ", rc

end subroutine armci_free_7dc4


subroutine armci_malloc_1dc8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c8), dimension(:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(1), ub(1)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c8), dimension(:), pointer :: a
  integer, dimension(1) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 16
  integer :: rank = 1

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_1dc8: error = ", rc

end subroutine armci_malloc_1dc8

subroutine armci_malloc_2dc8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c8), dimension(:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(2), ub(2)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c8), dimension(:,:), pointer :: a
  integer, dimension(2) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 16
  integer :: rank = 2

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_2dc8: error = ", rc

end subroutine armci_malloc_2dc8

subroutine armci_malloc_3dc8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(3), ub(3)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c8), dimension(:,:,:), pointer :: a
  integer, dimension(3) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 16
  integer :: rank = 3

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_3dc8: error = ", rc

end subroutine armci_malloc_3dc8

subroutine armci_malloc_4dc8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(4), ub(4)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:), pointer :: a
  integer, dimension(4) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 16
  integer :: rank = 4

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_4dc8: error = ", rc

end subroutine armci_malloc_4dc8

subroutine armci_malloc_5dc8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(5), ub(5)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:,:), pointer :: a
  integer, dimension(5) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 16
  integer :: rank = 5

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_5dc8: error = ", rc

end subroutine armci_malloc_5dc8

subroutine armci_malloc_6dc8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(6), ub(6)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: a
  integer, dimension(6) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 16
  integer :: rank = 6

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_6dc8: error = ", rc

end subroutine armci_malloc_6dc8

subroutine armci_malloc_7dc8(a, lb, ub, rc)
  use definekind
  implicit none
  interface
    subroutine armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in) :: rank, elemsize, lb(7), ub(7)
      integer, intent(out) :: rc
    end subroutine armci_malloc_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, dimension(7) :: lb, ub
  integer, intent(out), optional :: rc
  integer :: elemsize = 16
  integer :: rank = 7

  call armci_malloc_farray(a, rank, elemsize, lb, ub, rc)
!  if (rc .ne. 0) print *, "error armci_malloc_7dc8: error = ", rc

end subroutine armci_malloc_7dc8

subroutine armci_free_1dc8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c8), dimension(:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c8), dimension(:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 1

  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_1dc8: error = ", rc

end subroutine armci_free_1dc8

subroutine armci_free_2dc8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c8), dimension(:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c8), dimension(:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 2
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_2dc8: error = ", rc

end subroutine armci_free_2dc8

subroutine armci_free_3dc8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c8), dimension(:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 3
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_3dc8: error = ", rc

end subroutine armci_free_3dc8

subroutine armci_free_4dc8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 4
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_4dc8: error = ", rc

end subroutine armci_free_4dc8

subroutine armci_free_5dc8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 5
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_5dc8: error = ", rc

end subroutine armci_free_5dc8

subroutine armci_free_6dc8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 6
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_6dc8: error = ", rc

end subroutine armci_free_6dc8

subroutine armci_free_7dc8(a, rc)
  use definekind
  implicit none
  interface
    subroutine armci_free_farray(a, rank, rc)
      use definekind
      complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: a
      integer, intent(in)  :: rank
      integer, intent(out) :: rc
    end subroutine armci_free_farray
  end interface
  complex(kind=c8), dimension(:,:,:,:,:,:,:), pointer :: a
  integer, intent(out), optional :: rc
  integer :: rank = 7
  call armci_free_farray(a, rank, rc)
!  if (rc .ne. 0) print *, "error armci_free_7dc8: error = ", rc

end subroutine armci_free_7dc8

end module armci_mem_f90
