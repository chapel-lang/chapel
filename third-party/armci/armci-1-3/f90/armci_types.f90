module armci_types

  type armci_slice
    integer :: lo(7)
    integer :: hi(7)
    integer :: stride(7)
  end type armci_slice

end module armci_types
