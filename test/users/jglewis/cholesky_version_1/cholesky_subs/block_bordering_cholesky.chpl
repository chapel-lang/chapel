module block_bordering_cholesky {

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
 

  // =========================================================================
  // The outer product Cholesky factorization computes one block column of L at
  // each step. During each step the remaining columns of A are modified by a 
  // low rank outer product modication -- the reduce operations are accumulated 
  // one block step at a time for each entry in the yet unfactored part of the
  // matrix.  The computed entries of L will not otherwise need to be 
  // referenced again in the factorization.  
  // =========================================================================
    
  use block_partition_iterators, symmetric_blas;

  // =========================================================================
  // The bordering Cholesky factorization computes one row of L at each step.
  // The leading entries in each row are computed first, in an operation that
  // can be viewed as a triangular system solve, using the leading rows of L 
  // as the coefficient matrix and the leading entries of the active row as 
  // the right-hand side vector.  Because solves are nasty to parallelize, 
  // this ordering of operations is rarely used.
  // The key equation driving this form of the factorization is the 
  // equation that defines most of the Ith block row of the factorization
  //    L(I,..I-1) = A(I,..I-1) * L (..I-1,..I-1)^T
  // This equation can be derived most simply by taking a block version of
  // the equations above, developed by taking a block partitioning of the 
  // matrix into block rows and columns (..I-1), block row and column I, and 
  // block rows and columns (I+1..).
  // =========================================================================
    
  proc block_bordering_cholesky ( A : [] )  

    where ( A.domain.rank == 2 ) {

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

    assert ( A.domain.dim (1) == A.domain.dim (2) );

    const row_indices = A.domain.dim (1);  // indices of either row or column
    var   pos_def : bool;

    // compute L from A

    for ( prev_rows, reduced_mtx_rows, active_rows, trailing_rows ) 
      in symmetric_matrix_3_by_3_block_partition ( row_indices ) do {

      // modify the active block row of A with the combined effects of the
      // elimination steps on all previous block row.  This is identical to
      // performisng a block forward solve with a block lower triangular 
      // matrix where every diagonal block is a unit lower triangular matrix

      if prev_rows.size > 0 then {
	block_transposed_block_triangular_solve ( A (prev_rows, prev_rows), 
						  A (active_rows, prev_rows ) );
	
	// apply the outer product modifications to the symmetric 
	// diagonal block

	forall i in active_rows do
	  forall j in active_rows (..i) do
	    for k in prev_rows do
	      A (i,j) -= A (i,k) * A (j,k);
      }
      
      // compute the Cholesky factor of the active diagonal block
      
      pos_def = scalar_bordering_cholesky ( A (active_rows, active_rows) );
      
      if !pos_def then
	// error return if matrix is not positive semi-definite
	return false;
    }
    return true;
  }

}