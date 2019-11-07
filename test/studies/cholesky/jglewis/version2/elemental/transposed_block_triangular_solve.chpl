module transposed_block_triangular_solve {


  // ============================================
  // Special Block Triangular Solve used in Block 
  // Inner and Outer Product Cholesky codes
  // ============================================

  proc transposed_block_triangular_solve ( L_diag    : [],
					  L_offdiag : [] ) {
    
    // ------------------------------------------------------
    // Solve the block equation
    //      L_offdiag = A_offdiag * L_diag^{-T}
    //           or
    //      L_offdiag^T = L_diag^{-1} A_offdiag^T
    // by triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.  L
    // overwrites the values of A.
    // ------------------------------------------------------

    const diag_block_cols    = L_diag.domain.dim(1),
          offdiag_block_rows = L_offdiag.domain.dim(0);

    //    const x : int;
    //    const my_locale = diag_block_cols.locale;
    //    forall i in L_diag do
    //      assert ( i.locale == my_locale);
    //    forall i in L_offdiag do
    //      assert ( i.locale == my_locale);

    // next loop nest is embarassingly parallel over rows of the off-diagonal
    // block and not parallel within each row, due to the triangular solve.
    // (each row is the result of a triangular solve.)

    forall i in offdiag_block_rows do
      for j in diag_block_cols do {
	L_offdiag (i,j) -= 
	  +reduce [k in diag_block_cols (.. j-1)] 
	           L_offdiag (i,k) * L_diag (j,k);
	L_offdiag (i,j) /= L_diag (j,j);
      }
  }
}
