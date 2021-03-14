module matrix_matrix_multiply_inner_product {
      
  // =======================================================
  // Symmetric Block Inner Product for a Single Block Column
  // =======================================================

  proc block_1D_inner_product ( L : [] , A : [] )

    where ( A.domain.rank == 2 && L.domain.rank == 2) {

    // -----------------------------------------------------------
    // form block column of reduced matrix
    //     A (..,J) = A (..,J) - L (.., .. J-1) L^T (..,J)
    //              = A (..,J) - L (.., .. J-1) L (J,..) )
    // code is specialized to factorization case where L and A
    // are submatrices of a single larger matrix.  L
    // overwrites the values of A.
    // -----------------------------------------------------------

    assert ( A.domain.dim (0) == L.domain.dim (0) );

    const row_range = A.domain.dim (0);

    const AJ_diag_rc_indices     = A.domain.dim (1),
          AJ_subdiag_row_indices = row_range (AJ_diag_rc_indices.high + 1 ..),
          L_prev_cols            = L.domain.dim (1);

    //  Symmetric modification to diagonal block, which is symmetric,
    //  so we cannot use a standard matrix-matrix product
    
    forall i in AJ_diag_rc_indices do 
      forall j in AJ_diag_rc_indices (..i) do
	A (i,j) -= + reduce [k in L_prev_cols] L (i,k) * L (j,k);

    //  General modification to off-diagonal block

    for LK_col_indices in vector_block_partition (L_prev_cols) do
      forall (i,j,k) in 
    {AJ_subdiag_row_indices, AJ_diag_rc_indices, LK_col_indices} do 
	A (i,j) -= L (i,k) * L (j,k);
  }



  // ================================================
  // Symmetric Block Inner Product for a Single Block
  // Column for a 2D Block Distributed Matrix
  // ================================================

  proc block_2D_inner_product ( L : [] , A : [] )

    where ( A.domain.rank == 2 && L.domain.rank == 2) {


    // -----------------------------------------------------------
    // form block column of reduced matrix
    //     A (..,J) = A (..,J) - L (.., .. J-1) L^T (..,J)
    //              = A (..,J) - L (.., .. J-1) L (J,..) )
    // code is specialized to factorization case where L and A
    // are submatrices of a single larger matrix.  L
    // overwrites the values of A.
    // -----------------------------------------------------------

    assert ( A.domain.dim (0) == L.domain.dim (0) );

    const row_range = A.domain.dim (0);

    const AJJ_rc_indices        = A.domain.dim (1),
          AJ_subdiag_row_indices = row_range (AJJ_rc_indices.high + 1 ..),
          L_prev_cols            = L.domain.dim (1);

    //  Symmetric modification to diagonal block

    forall i in AJJ_rc_indices do 
      forall j in AJJ_rc_indices (..i) do
	A (i,j) -= + reduce [k in L_prev_cols] L (i,k) * L (j,k);

    //  General modification to off-diagonal block.
    //  Outer two loops can be parallelized with leader-follower iterators.

    for AIJ_row_indices in vector_block_partition ( AJ_subdiag_row_indices ) do
      for LK_col_indices in vector_block_partition ( L_prev_cols )  do
	forall (i,j, k) in {AIJ_row_indices, AJJ_rc_indices, LK_col_indices} do 
	  A (i,j) -= L (i,k) * L (j,k);
  }
      



}
