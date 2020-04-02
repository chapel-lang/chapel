module symmetric_blas {


  // ======================
  // Block Triangular Solve 
  // ======================

  proc transposed_block_triangular_solve ( L_diag    : [],
					  L_offdiag : [] ) {
    
    // ------------------------------------------------------
    // Solve the block equation
    //      L_offdiag = A_offdiag * L_diag^{-T}
    //           or
    //      L_offdiag^T = L_diag^{-1} A_offdiag^T
    // by triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.
    // ------------------------------------------------------

    const diag_block_cols = L_diag.domain.dim(1);

    // next loop is embarassingly parallel when we get a parallel iterator

    for (i,j) in L_offdiag.domain do {
      L_offdiag (i,j) -= 
	+reduce [k in diag_block_cols (.. j-1)] L_offdiag (i,k) * L_diag (j,k);
      L_offdiag (i,j) /= L_diag (j,j);
    }

  }
      
  // =========================
  // 2D Block Triangular Solve
  // =========================

  proc transposed_2D_block_triangular_solve ( L_diag    : [],
					     L_offdiag : [] ) {
    
    // ------------------------------------------------------
    // Solve the block equation
    //      L_offdiag = A_offdiag * L_diag^{-T}
    //           or
    //      L_offdiag^T = L_diag^{-1} A_offdiag^T
    // by triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.
    // ------------------------------------------------------

    const diag_block_cols = L_diag.domain.dim(1);

    // next loop is embarassingly parallel when we get a parallel iterator

    for block_rows in vector_block_partition ( L_offdiag.domain.dim (1) ) do

      for (i,j) in {block_rows, diag_block_cols} do {
	L_offdiag (i,j) -= 
	  +reduce [k in diag_block_cols (.. j-1)] 
	  L_offdiag (i,k) * L_diag (j,k);
	L_offdiag (i,j) /= L_diag (j,j);
      }
    
  }


  proc block_transposed_block_triangular_solve ( L : [], A : [] )
    where ( A.domain.rank == 2  && L.domain.rank == 2 ) {

    // -----------------------------------------------------------
    // I  denotes the index set for a block row of a block matrix.
    // Solve the block equation
    //      L(I,..I-1) = A(I,..I-1) * L(..I-1,..I-1)^{-T}
    //           or
    //      L(I,..I-1)^T = L(..I-1,..I-1)^{-1} A(I,..I-1)^T
    // by block triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.
    // ----------------------------------------------------------

    const active_rows = A.domain.dim (1),
          prev_rows   = A.domain.dim(2);

    // The block solve proceeds blockwise over the block triangular
    // coefficent matrix.  We use a right-looking (outer-product) form of 
    // the block solve step that defines the next block row of the factor

    for (ignore, active_cols, trailing_cols) 
      in symmetric_reduced_matrix_2_by_2_block_partition (prev_rows) 
      do {

	// take the final solve step to create one square block of the
	// block row of L being computed

	transposed_block_triangular_solve 
	  ( L (active_cols, active_cols), 
	    A (active_rows, active_cols) );

	// apply outer product modification to the remainder of the 
	// active block row, omitting the final diagonal block

	forall (i,j,k) in {active_rows, trailing_cols, active_cols} do
	  A (i,j) -= A (i,k) * L (j,k);
      }
  }


  proc block_2D_transposed_block_triangular_solve ( L : [], A : [] )
    where ( A.domain.rank == 2  && L.domain.rank == 2 ) {

    // -----------------------------------------------------------
    // I  denotes the index set for a block row of a block matrix.
    // Solve the block equation
    //      L(I,..I-1) = A(I,..I-1) * L(..I-1,..I-1)^{-T}
    //           or
    //      L(I,..I-1)^T = L(..I-1,..I-1)^{-1} A(I,..I-1)^T
    // by block triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.
    // ----------------------------------------------------------

    const active_rows = A.domain.dim (1),
          prev_rows   = A.domain.dim(2);

    // The block solve proceeds blockwise over the block triangular
    // coefficent matrix.  We use a right-looking (outer-product) form of 
    // the block solve step that defines the next block row of the factor

    for (ignore, active_cols, trailing_cols) 
      in symmetric_reduced_matrix_2_by_2_block_partition (prev_rows) 
      do {

	// take the final solve step to create one square block of the
	// block row of L being computed

	transposed_block_triangular_solve 
	  ( L (active_cols, active_cols), 
	    A (active_rows, active_cols) );

	// apply outer product modification to the remainder of the 
	// active block row, omitting the final diagonal block

	for later_block_col in vector_block_partition (trailing_cols) do
	  forall (i,j,k) in {active_rows, later_block_col, active_cols} do
	    A (i,j) -= A (i,k) * L (j,k);
      }
  }


      
  // ==========================================
  // Symmetric Block Outer Product_Modification
  // ==========================================

  proc symmetric_block_schur_complement ( A : [] , L : [] )

    where ( A.domain.rank == 2 && L.domain.rank == 2) 

    {
      for ( reduced_mtx_rows, leading_rows, trailing_rows ) in 
	symmetric_reduced_matrix_2_by_2_block_partition (L.domain.dim (1)) do {

	// should be forall once iterator is made parallel

	symmetric_diagonal_low_rank_modification 
	  ( L (leading_rows, ..), 
	    A (leading_rows, leading_rows) );

	if trailing_rows.size > 0 then
	  symmetric_offdiagonal_low_rank_modification 
	    ( L (reduced_mtx_rows, ..), 
	      A (trailing_rows, leading_rows) );
      }
    }


  // =============================================
  // Symmetric 2D Block Outer Product_Modification
  // =============================================

  proc symmetric_2D_block_schur_complement ( A : [] , L : [] )

    where ( A.domain.rank == 2 && L.domain.rank == 2) 

    {
      for ( reduced_mtx_rows, leading_rows, trailing_rows ) in 
	symmetric_reduced_matrix_2_by_2_block_partition (L.domain.dim (1)) do {

	// should be forall once iterator is made parallel

	symmetric_diagonal_low_rank_modification 
	  ( L (leading_rows, ..), 
	    A (leading_rows, leading_rows) );

	for block_rows in vector_block_partition ( trailing_rows ) do
	  symmetric_offdiagonal_low_rank_modification 
	    ( L (reduced_mtx_rows, ..), 
	      A (block_rows, leading_rows) );
      }
    }


  // ======================================================================
  // Symmetric Block Outer Product Modification for a single diagonal block
  // ======================================================================

  proc symmetric_diagonal_low_rank_modification ( L : [], A : [] ) {

    // -----------------------------------------------------------
    // form diagonal block A (K,K) = A (K,K) - L (K,J) L^T (J,K) 
    //                             = A (K,K) - L (K,J) L (K,J)^T
    // code is specialized to factorization case where L and A
    // are submatrices of a single larger matrix.
    // -----------------------------------------------------------

    assert ( A.domain.dim (1) == A.domain.dim (2) &&
	     A.domain.dim (1) == L.domain.dim (1) );

    const A_diag_rows   = A.domain.dim (1),
      L_active_cols = L.domain.dim (2);

    forall i in A_diag_rows do 
      forall j in A_diag_rows (..i) do
	A (i,j) -= + reduce [k in L_active_cols] L (i,k) * L (j,k);
  }
      

  // =========================================================================
  // Symmetric Block Outer Product Modification for a single offdiagonal block
  // =========================================================================

  proc symmetric_offdiagonal_low_rank_modification ( L : [], A : [] ) {

    // -------------------------------------------------------------
    // Form a single offdiagonal block 
    //       A (I,K) = A (I,K) - L (I,J) L^T (J,K) 
    //               = A (I,K) - L (I,J) L (J,K)^T
    // This code is specialized to the triangular factorization case 
    // where L and A are submatrices of a common larger matrix.
    // -------------------------------------------------------------

    const L_active_cols  = L.domain.dim (2);

    forall (i,j) in A.domain do 
      A (i,j) -= + reduce [k in L_active_cols] L (i,k) * L (j,k);
  }

  // =======================================================
  // Symmetric Block Inner Product for a Single Block Column
  // =======================================================

  proc block_inner_product ( L : [] , A : [] )

    where ( A.domain.rank == 2 && L.domain.rank == 2) {


    // -----------------------------------------------------------
    // form block column of reduced matrix
    //     A (..,J) = A (..,J) - L (.., .. J-1) L^T (..,J)
    //              = A (..,J) - L (.., .. J-1) L (J,..) )
    // code is specialized to factorization case where L and A
    // are submatrices of a single larger matrix.
    // -----------------------------------------------------------

    assert ( A.domain.dim (1) == L.domain.dim (1) );

    const row_range = A.domain.dim (1);

    const diag_block_cols        = A.domain.dim (2),
          subdiagonal_block_rows = row_range (diag_block_cols.high + 1 ..),
          L_prev_cols            = L.domain.dim (2);

    //  Symmetric modification to diagonal block

    forall i in diag_block_cols do 
      forall j in diag_block_cols (..i) do
	A (i,j) -= + reduce [k in L_prev_cols] L (i,k) * L (j,k);

    //  General modification to off-diagonal block

    forall (i,j) in {subdiagonal_block_rows, diag_block_cols} do 
      A (i,j) -= + reduce [k in L_prev_cols] L (i,k) * L (j,k);
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
    // are submatrices of a single larger matrix.
    // -----------------------------------------------------------

    assert ( A.domain.dim (1) == L.domain.dim (1) );

    const row_range = A.domain.dim (1);

    const diag_block_cols        = A.domain.dim (2),
          subdiagonal_block_rows = row_range (diag_block_cols.high + 1 ..),
          L_prev_cols            = L.domain.dim (2);

    //  Symmetric modification to diagonal block

    forall i in diag_block_cols do 
      forall j in diag_block_cols (..i) do
	A (i,j) -= + reduce [k in L_prev_cols] L (i,k) * L (j,k);

    //  General modification to off-diagonal block

    for block_rows in vector_block_partition ( subdiagonal_block_rows ) do
      for block_cols in vector_block_partition ( L_prev_cols ) do
	forall (i,j, k) in {block_rows, diag_block_cols, block_cols} do 
	  A (i,j) -= L (i,k) * L (j,k);
  }
      



}
