module block_inner_product_cholesky  {

  // =========================================================================
  // The Cholesky factorization  A = L L^^T is defined by the following sets
  // of scalar equations derived by expanding the matrix-matrix product on
  // the right side of the matrix equation:
  //
  //               A(i,j) = + reduce ( L (i, ..) L (j, ..)
  //
  // As written, these equations do not recognize the symmetry of A and the 
  // triangular structure of L.  Recognizing those two facts allows us to turn
  // these equations into an algorithm for computing the decomposition.
  //
  // Main diagonal:  
  //    L(j,j) = sqrt ( A(j,j) - (+ reduce [k in ..j-1] L(j,k)**2 ) )
  // Below main diagonal:
  //    L(i,j) = ( A(i,j) - (+ reduce [k in ..j-1] L(i,k) * L(j,k) ) ) / L(j,j)
  //
  // These equations can be promoted to block equations by treating:
  //    scalar/ multiplication involving only non-diagonal blocks as ordinary 
  //       matrix-matrix multiplication;
  //    scalar/ multiplication involving diagonal blocks as ordinary triangular
  //       or symmetric matrix-matrix multiplication;
  //    taking the Cholesky factor of a block as its square root.
  //
  // Conventionally only one array argument, A, is used in factorization
  // routines, and only the lower triangle is used.  On output the entries of 
  // L overwrite the entries of A.  The partial sums of the reductions are 
  // accumulated during the course of the algorithm also in the space occupied
  // by the input matrix  A.  Conventionally, the entries in the upper
  // triangle of A are left untouched. 
  // =========================================================================
 

  // ==========================================================================
  // The inner product Cholesky factorization computes one block column of L at
  // each step, in a manner reflecting the reduction form of the defining
  // equations.  During each step the previous columns of L are read to
  // implement the separate reduction operations for each entry of the current
  // column.  For efficiency, the reduction operations are combined into a 
  // matrix-vector product form.
  // ==========================================================================
    
  use block_partition_iterators, symmetric_blas;

  proc block_inner_product_cholesky ( A : [] ) 

    where ( A.domain.rank == 2 ) 
    {
    assert ( A.domain.dim (0) == A.domain.dim (1) );

    const col_indices = A.domain.dim (0);  // indices of either row or column
    var   pos_def : bool;

    // compute L from A

    for ( prev_cols, reduced_mtx_cols, leading_cols, trailing_cols ) in 
      symmetric_matrix_3_by_3_block_partition ( col_indices ) do {

      // modify the active block column of A with the combined effects of the
      // elimination steps on all previous block columns

      if prev_cols.size > 0 then
	block_inner_product ( A ( reduced_mtx_cols, prev_cols), 
			      A ( reduced_mtx_cols, leading_cols ) );

      // compute the Cholesky factor of the active diagonal block

      pos_def = scalar_inner_product_cholesky ( A (leading_cols, leading_cols) );


      if pos_def && trailing_cols.size > 0 then {

	// compute the remainder of the active block column of L by a
	// block triangular solve realizing the equation
	//      L (trailing_cols, leading_cols) = 
	//                              L (trailing_cols, leading_cols) *
	//                              L (leading_cols, leading_cols) ** (-T)
	
	transposed_block_triangular_solve ( A (leading_cols, leading_cols), 
					    A (trailing_cols, leading_cols) );
	
      }
      else 

	// error return if matrix is not positive definite
	
	if !pos_def then return false;
    }

    // return success 

    return true;

    }      
}
	