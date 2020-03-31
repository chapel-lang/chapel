module scalar_gen_inner_product_cholesky {

  // =========================================================================
  // The inner product Cholesky factorization also computes one column of L at
  // each step, in a manner reflecting the reduction form of the defining
  // equations.  During each step the previous columns of L are read to
  // implement the separate reduction operations for each entry of the current
  // column.  For efficiency, the reduction operations are combined into a 
  // matrix-vector product form.
  // =========================================================================
    
  proc scalar_gen_inner_product_cholesky ( A : [] ) 

    where ( A.domain.rank == 2 ) 
    {
    // -----------------------------------------------------------------------
    // the input argument is a square symmetric matrix, whose entries will be 
    // overwritten by the entries of the Cholesky factor.  Only the entries in 
    // the lower triangule are referenced and modified.
    // The procedure additionally returns a success / failure flag, which is
    //   true if the matrix is numerically positive definite
    //   false if the matrix is not positive definite
    // Note that while a factorization is computable for positive 
    // semi-definite matrices, we do not compute it because this factorization
    // is designed for use in a context of solving a system of linear 
    // equations.
    // -----------------------------------------------------------------------

    assert ( A.domain.dim (0).size == A.domain.dim (1).size );

    const row_indices = A.domain.dim (0), col_indices = A.domain.dim (1);

    // compute L from A

    for (j_row, j_col) in zip(row_indices, col_indices) do {

      // modify the jth column of A with the combined effects of the
      // elimination steps on all previous columns

      for k in col_indices (..j_col-1) do
	A (j_row.., j_col) -= A (j_row, k) * A (j_row.., k);

      if A (j_row, j_col) > 0.0 then {

	// compute the jth column of L

	A (j_row    , j_col)  = sqrt ( A (j_row, j_col) );
	A (j_row+1.., j_col ) = A (j_row+1.., j_col) / A (j_row, j_col);
      }
      else 

	// error return if matrix is not positive definite
	return false;
    }
    return true;
  }
}
