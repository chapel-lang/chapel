module scalar_outer_product_cholesky {

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
  // The outer product Cholesky factorization computes one column of L at each
  // step. During each step the remaining columns of A are modified by a single 
  // outer product operation -- the reduce operations are accumulated one step
  // at a time for each entry in the yet unfactored part of the matrix.  The
  // computed entries of L will not otherwise need to be referenced again in
  // the factorization.  
  // =========================================================================
    
  proc scalar_outer_product_cholesky ( A : [] )

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

    const A_rc_indices = A.domain.dim (0);  // row and column indices of A

    // compute L from A

    for j in A_rc_indices do {

      if A (j, j) > 0.0 then {

	// compute the jth column of L

	A (j, j)       = sqrt ( A (j, j) );
	A (j+1.., j ) /= A (j, j);

	// make rank 1 (outerproduct) modification to the remaining rows
	// and columns of  A, which become the Schur complement

	forall k in A_rc_indices (j+1..) do
	  A (k.., k) -= A(k.., j) * A (k, j);
      }
      else

	// error return if matrix is not positive sdefinite
	return false;
    }
    return true;
  }
}