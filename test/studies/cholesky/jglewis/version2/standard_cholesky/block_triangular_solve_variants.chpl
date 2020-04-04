module block_triangular_solve_variants {

  // ============================================
  // Special Block Triangular Solve used in Block 
  // Inner and Outer Product Cholesky codes
  // ============================================

  proc transposed_block_triangular_solve ( L11 : [], L21 : [] ) {
    
    // ------------------------------------------------------
    // Solve the block equation
    //      L21 = A21 * L11^{-T}
    //           or
    //      L21^T = L11^{-1} A21^T
    // by triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.  L
    // overwrites the values of A.
    // ------------------------------------------------------

    const L11_cols = L11.domain.dim(1),
          L21_rows = L21.domain.dim(0);

    // next loop nest is embarassingly parallel over rows of the off-diagonal
    // block and not parallel within each row, due to the triangular solve.
    // (each row is the result of a triangular solve.)

    forall i in L21_rows do
      for j in L11_cols do {
	L21 (i,j) -=  +reduce [k in L11_cols (.. j-1)] L21 (i,k) * L11 (j,k);
	L21 (i,j) /= L11 (j,j);
      }

  }
      
  // ===============================================
  // Special 2D Block Triangular Solve used in Block 
  // Inner and Outer Product Cholesky codes
  // ===============================================

  proc transposed_2D_block_triangular_solve ( L11 : [],
					     L21 : [] ) {
    
    // ------------------------------------------------------
    // Solve the block equation
    //      L21 = A21 * L11^{-T}
    //           or
    //      L21^T = L11^{-1} A21^T
    // by triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.  L
    // overwrites the values of A.
    // ------------------------------------------------------

    const L11_cols = L11.domain.dim(0),
          L21_rows = L21.domain.dim(0);

    // next loop is embarassingly parallel when we get a parallel iterator

    for block_of_L21_rows in vector_block_partition ( L21_rows ) do

    // next loop nest is embarassingly parallel over rows of the off-diagonal
    // block and not parallel within each row, due to the triangular solve.
    // (each row is the result of a triangular solve.)

      for i in block_of_L21_rows do 
	for j in L11_cols do {
	  L21 (i,j) -= 
	    +reduce [k in L11_cols (.. j-1)] 
	            L21 (i,k) * L11 (j,k);
	  L21 (i,j) /= L11 (j,j);
	}
    
  }



  // ===================================
  // Special Block Triangular Solve used 
  // in Block Bordering Cholesky codes
  // ===================================

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
    // L and A are submatrices of a common larger matrix.  L
    // overwrites the values of A.
    // ----------------------------------------------------------

    const A11_rc_indices = A.domain.dim (0),
          A00_rc_indices = A.domain.dim(1);

    // The block solve proceeds blockwise over the block triangular
    // coefficent matrix.  We use a right-looking (outer-product) form of 
    // the block solve step that defines the next block row of the factor

    for (active_cols, trailing_cols) 
      in symmetric_2_by_2_block_partition (A00_rc_indices) do {

	// take the final solve step to create one square block of the
	// block row of L being computed

	transposed_block_triangular_solve 
	  ( L (active_cols, active_cols), 
	    A (A11_rc_indices, active_cols) );

	// apply outer product modification to the remainder of the 
	// active block row, omitting the final diagonal block

	forall (i,j,k) in {A11_rc_indices, trailing_cols, active_cols} do
	  A (i,j) -= A (i,k) * L (j,k);
      }
  }



  // ======================================
  // Special 2D Block Triangular Solve used 
  // in Block Bordering Cholesky codes
  // ======================================

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
    // L and A are submatrices of a common larger matrix.  L
    // overwrites the values of A.
    // ----------------------------------------------------------

    const A11_rc_indices = A.domain.dim (0),
          A00_rc_indices   = A.domain.dim(1);

    // The block solve proceeds blockwise over the block triangular
    // coefficent matrix.  We use a right-looking (outer-product) form of 
    // the block solve step that defines the next block row of the factor

    for (active_cols, trailing_cols) 
      in symmetric_2_by_2_block_partition (A00_rc_indices) do {

	// take the final solve step to create one square block of the
	// block row of L being computed

	transposed_block_triangular_solve 
	  ( L (active_cols, active_cols), 
	    A (A11_rc_indices, active_cols) );

	// apply outer product modification to the remainder of the 
	// active block row, omitting the final diagonal block

	for later_block_col in vector_block_partition (trailing_cols) do
	  forall (i,j,k) in {A11_rc_indices, later_block_col, active_cols} do
	    A (i,j) -= A (i,k) * L (j,k);
      }
  }
}
