module scalar_bordering_cholesky {

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
  // The bordering Cholesky factorization computes one row of L at each step.
  // The leading entries in each row are computed first, in an operation that
  // can be viewed as a triangular system solve, using the leading rows of L 
  // as the coefficient matrix and the leading entries of the active row as 
  // the right-hand side vector.  Because solves are nasty to parallelize, 
  // this ordering of operations is rarely used.
  // The key equation driving this form of the factorization is the 
  // equation that defines most of the Ith block row of the factorization
  //    L(i,..i-1) = A(i,..i-1) * L (..i-1,..i-1)^T
  // This equation can be derived most simply by taking a block version of
  // the equations above, developed by taking a block partitioning of the 
  // matrix into rows and columns (..i-1), row and column i, and 
  // rows and columns (i+1..).
  // =========================================================================
    
  proc scalar_bordering_cholesky ( A : [] )  

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

    assert ( A.domain.dim (0) == A.domain.dim (1) );

    const A_rc_indices = A.domain.dim (0);  // row and column indices of A

    // compute L from A

    for i in A_rc_indices do {

      // compute the ith row of L.  incorporating the effects of the elimination
      // steps on the previous rows is identical to performing a forward solve
      // with a unit lower triangular matrix

      for k in A_rc_indices (..i-1) do {
	A (i, k) -= + reduce [j in A_rc_indices (..k-1)] A (i,j) * A (k,j) ;
	A (i, k) = A (i, k) / A (k, k);
      }

      // compute the ith diagonal entry of L

      A (i, i) -= + reduce [k in A_rc_indices (..i-1)] A (i, k)**2;

      if A (i, i) > 0.0 then
	  A (i, i) = sqrt ( A (i, i) );
      else

	  // error return if matrix is not positive semi-definite
	  return false;
    }
    return true;
  }

}
	