module cholesky_test {

  // +------------------------------------------------------------------------+
  // |  TEST DRIVER FOR VARIATIONS ON DENSE SYMMETRIC CHOLESKY FACTORIZATION  |
  // +------------------------------------------------------------------------+

  config const n = 9;

  config const index_base = 1;

  use Random;

  use CyclicDist, BlockDist;

  proc main {

    var Rand = new owned RandomStream ( real, seed = 314159) ;

    const MatIdx = { index_base .. #n, index_base .. #n };

    const mat_dom : domain (2) dmapped Cyclic ( startIdx = MatIdx.low )
      = MatIdx; 

    //  const mat_dom : domain (2) dmapped Block ( boundingBox = MatIdx )
    //    = MatIdx; 

    var A : [mat_dom] real,
        B : [mat_dom] real,
        L : [mat_dom] real;

    var positive_definite : bool;

    writeln ("Cholesky Factorization Tests");
    writeln ("   Matrix Dimension: ", n);
    writeln ("   Indexing Base   : ", index_base);
    writeln ("");

    // ---------------------------------------------------------------
    // create a test problem, starting with a random general matrix B.
    // ---------------------------------------------------------------

    Rand.fillRandom (B);

    // -------------------------------------------------------------
    // create a positive definite matrix A by setting A equal to the
    // matrix-matrix product B B^T.  This normal equations matrix is 
    // positive-definite as long as B is full rank.
    // -------------------------------------------------------------

    A = 0.0;

    forall (i,j) in mat_dom do
      A (i,j) = + reduce (  [k in mat_dom.dim (0) ] 
    			    B (i, k) * B (j, k) );

    // factorization algorithms overwrite a copy of A, leaving
    // the factor L in its place

    L = A;

    writeln ("scalar outer product cholesky factorization ");

    positive_definite = scalar_outer_product_cholesky ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");
  }

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

    const col_indices = A.domain.dim (0);  // indices of either row or column

    // compute L from A

    for j in col_indices do {

      if A (j, j) > 0.0 then {

	// compute the jth column of L

	A (j, j)       = sqrt ( A (j, j) );
	A (j+1.., j ) /= A (j, j);

	// make rank 1 (outerproduct) modification to the remaining rows
	// and columns of  A, which become the Schur complement

	forall k in col_indices (j+1..) do
	  A (k.., k) -= A(k.., j) * A (k, j);
      }
      else

	// error return if matrix is not positive sdefinite
	return false;
    }
    return true;

  }

  proc check_factorization ( A : [], L : [] )

    // -----------------------------------------------------------------------
    // Check the factorization by forming L L^T and comparing the result to A.
    // The factorization is successful if the results are within the
    // point-wise perturbation bounds of Demmel, given as Theorem 10.5 in
    // Higham's "Accuracy and Stability of Numerical Algorithms, 2nd Ed."
    // -----------------------------------------------------------------------

    where ( A.domain.rank == 2 ) {

    assert ( A.domain.dim (0) == A.domain.dim (1)  &&
	     L.domain.dim (0) == A.domain.dim (0)  &&
	     L.domain.dim (1) == A.domain.dim (1) 
	     );
    
    const mat_dom  = A.domain,
	  mat_rows = A.domain.dim(0),
	  n        = A.domain.dim(0).size;

    const unit_roundoff = 2.0 ** (-53), // IEEE 64 bit floating point
	  gamma_n1      = (n * unit_roundoff) / (1.0 - n * unit_roundoff);

    var   max_ratio = 0.0;

    var   d : [mat_rows] real;

    for i in mat_rows do
      d (i) = sqrt ( A (i,i) );
    
    forall (i,j) in mat_dom with (ref max_ratio) do {  // race
      const resid : real =
               abs (A (i,j) - 
		    + reduce ( [k in mat_dom.dim(0) (..min (i,j))]
			       L (i,k) * L (j,k) ) ) ;
      max_ratio = max ( max_ratio,
			resid * (1 - gamma_n1) /
			( gamma_n1 * d (i) * d (j) ) );
    }

    if max_ratio <= 1.0 then
      writeln ("factorization successful");
    else
      writeln ("factorization error exceeds bound by ratio:", max_ratio);
  }
}
    
