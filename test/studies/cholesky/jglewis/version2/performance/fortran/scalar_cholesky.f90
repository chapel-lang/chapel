program test_scalar_cholesky

  integer, parameter :: r64 = selected_real_kind (14),                         &
                        r32 = selected_real_kind (6)
  integer :: n
  
  integer :: i, j, k

  logical :: positive_definite

  real (r64), allocatable :: A (:,:), B (:,:), L (:,:)

  real start_time, stop_time

  read (*, *) n

  allocate ( A (1:n, 1:n), B (1:n, 1:n), L (1:n, 1:n) )

  ! -------------------------------------------------------------
  ! create a positive definite matrix A by setting A equal to the
  ! matrix-matrix product B B^T.  This normal equations matrix is 
  ! positive-definite as long as B is full rank.
  ! -------------------------------------------------------------

    write (*,*) "Cholesky Factorization Tests"
    write (*,*) "   Matrix Dimension: ", n
    write (*,*) "   Block Size      : ", 1
    write (*,*) "   Indexing Base   : ", 1

   call random_number ( harvest = B )

  do i = 1, n
     do j = 1, n
        A (i,j) = 0.0
        do k = 1, n
           A (i,j) = A (i,j) + B (i,k) * B (j,k)
        end do
     end do
  end do

  ! factorization algorithms overwrite a copy of A, leaving
  ! the factor L in its place

  L = A

  call cpu_time ( start_time )
  call scalar_outer_product_cholesky ( n, L, positive_definite )
  call cpu_time ( stop_time )

  elapsed_time = stop_time - start_time
  write (*,*)  "  Cholesky Factorization time:   ", elapsed_time
  write (*,*)  "  Collective speed in megaflops: ",                            &
               ( (n**3) / 3.0 )  / ( 1.0E6 * elapsed_time )
  
  if ( positive_definite ) then
     call check_factorization ( n, A, L )
  else
     write (*,*) "factorization failed for non-positive semi-definite matrix"
  end if

end program test_scalar_cholesky


subroutine scalar_outer_product_cholesky ( n, A, positive_definite )

  integer, parameter :: r64 = selected_real_kind (14),                         &
                        r32 = selected_real_kind (6)
  integer    :: n
  real (r64) :: A (1:n, 1:n)
  logical    :: positive_definite

  integer    :: j, k

  positive_definite = .true.

  do j = 1, n

     if ( A (j,j) > 0.0 ) then

        A (j,j) = sqrt ( A (j,j) )

        A (j+1:n, j) = A (j+1:n, j) / A (j,j)

        do k = j+1, n
           A (k:n, k) = A (k:n, k) - A(k:n, j) * A (k, j)
        end do

     else

        positive_definite = .false.
        exit

     end if

  end do

end subroutine scalar_outer_product_cholesky

subroutine check_factorization ( n, A, L )

  integer, parameter :: r64 = selected_real_kind (14),                         &
                        r32 = selected_real_kind (6)
  integer    :: n

  real (r64) :: A (1:n, 1:n), L (1:n, 1:n)

  ! -----------------------------------------------------------------------
  ! Check the factorization by forming L L^T and comparing the result to A.
  ! The factorization is successful if the results are within the
  ! point-wise perturbation bounds of Demmel, given as Theorem 10.5 in
  ! Higham's "Accuracy and Stability of Numerical Algorithms, 2nd Ed."
  ! -----------------------------------------------------------------------
  
  real (r64) :: unit_roundoff, gamma_n1
  
  real (r64) :: d (1:n)
  
  real (r64) :: resid, max_ratio
  
  unit_roundoff = 2.0 ** (-53) ! IEEE 64 bit floating point,
  !  also epsilon (unit_roundoff) in Fortran
  
  gamma_n1      = (n * unit_roundoff) / (1.0 - n * unit_roundoff)
  
  max_ratio = 0.0
  
  do i = 1, n
     d (i) = sqrt ( A (i,i) )
  end do
  
  do i = 1, n
     do j = i+1, n
        
        resid = A (i,j) 
        do k = 1, min (i,j)
           resid = resid - L (i,k) * L (j,k)
        end do
        resid = abs (resid)
        
        max_ratio = max ( max_ratio,                                           &
                          resid * (1 - gamma_n1) /                             &
                          ( gamma_n1 * d (i) * d (j) ) )
        
     end do
  end do
  
  if ( max_ratio <= 1.0 ) then
     write (*,*)   "factorization successful"
  else
     write (*,*) "factorization error exceeds bound by ratio:", max_ratio
  end if
  
end subroutine check_factorization

