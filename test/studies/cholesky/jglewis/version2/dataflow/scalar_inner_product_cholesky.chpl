module scalar_inner_product_cholesky {

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

  // Note that only entries in L to the left or in the main diagonal above
  // appear on the right hand side of either of these equations.  Thus, at the
  // outset, the leading diagonal element is computable, followed by any or all
  // other entries in the first column.  The order in which the entries overall
  // must be computed is specified only by the data dependencies expressed in
  // the two equations above.  There are many orderings of these operations that
  // satisfy the dependency constraints.  Three standard orderings of those 
  // operations are given in this file; these compute L one row or one column at
  // a time.  these do not begin to exhaust the possible orerings.  To show the
  // potential for much more general orderings , a data-flow version of the
  // algorithm will be implemented separately.

  // Conventionally only one array argument, A, is used in factorization
  // routines, and only the lower triangle is used.  On output the entries of 
  // L overwrite the entries of A.  The partial sums of the reductions are 
  // accumulated during the course of the algorithm also in the space occupied
  // by the input matrix  A.  Conventionally, the entries in the upper
  // triangle of A are left untouched. 
  // =========================================================================
 

  // =========================================================================
  // The inner product Cholesky factorization also computes one column of L at
  // each step, in a manner reflecting the reduction form of the defining
  // equations.  During each step the previous columns of L are read to
  // implement the separate reduction operations for each entry of the current
  // column.  For efficiency, the reduction operations are combined into a 
  // matrix-vector product form.
  // =========================================================================
    
  proc scalar_inner_product_cholesky ( A : [] ) 

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

    assert ( A.domain.dim (0) == A.domain.dim (1) );

    const col_indices = A.domain.dim (0);  // indices of either row or column

    // compute L from A

    for j in col_indices do {

      // modify the jth column of A with the combined effects of the
      // elimination steps on all previous columns

      for k in col_indices (..j-1) do
	A (j.., j) -= A (j, k) * A (j.., k);

      if A (j, j) > 0.0 then {

	// compute the jth column of L

	A (j, j)      = sqrt ( A (j, j) );
	A (j+1.., j ) = A (j+1.., j) / A (j, j);
      }
      else 

	// error return if matrix is not positive definite
	return false;
    }
    return true;

  }
}
