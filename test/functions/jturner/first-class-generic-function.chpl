module cholesky_execution_config_consts
{
  // +======================================================+
  // |  Execution time constants control                    |
  // |                                                      |
  // |    1.  Problem Size                                  |
  // |    2.  Block Size                                    |
  // |    3.  Indexing Base                                 |
  // |                                                      |
  // |   All can be set from the command line.              |
  // +======================================================+
 
  // -------------
  // Problem Size:
  // -------------
 
  config const n = 59;
 
  config const block_size = 7;
 
  config const index_base = -3;
 
}
module cholesky_scalar_algorithms {

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

    assert ( A.domain.dim (1) == A.domain.dim (2) );

    const col_indices = A.domain.dim (1);  // indices of either row or column

    // compute L from A

    for j in col_indices do {

      if A (j, j) > 0.0 then {

	// compute the jth column of L

	A (j, j)       = sqrt ( A (j, j) );
	A (j+1.., j ) /= A (j, j);

	// make rank 1 (outerproduct) modification to the remaining rows
	// and columns of  A, which become the Schur complement

	for k in col_indices (j+1..) do
	  A (k.., k) -= A(k.., j) * A (k, j);
      }
      else

	// error return if matrix is not positive sdefinite
	return false;
    }
    return true;

  }

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

    assert ( A.domain.dim (1) == A.domain.dim (2) );

    const col_indices = A.domain.dim (1);  // indices of either row or column

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

    assert ( A.domain.dim (1) == A.domain.dim (2) );

    const row_indices = A.domain.dim (1);  // indices of either row or column

    // compute L from A

    for i in row_indices do {

      // compute the ith row of L.  incorporating the effects of the elimination
      // steps on the previous rows is identical to performing a forward solve
      // with a unit lower triangular matrix

      for k in row_indices (..i-1) do {
	A (i, k) -= + reduce [j in row_indices (..k-1)] A (i,j) * A (k,j) ;
	A (i, k) = A (i, k) / A (k, k);
      }

      // compute the ith diagonal entry of L

      A (i, i) -= + reduce [k in row_indices (..i-1)] A (i, k)**2;

      if A (i, i) > 0.0 then
	  A (i, i) = sqrt ( A (i, i) );
      else

	  // error return if matrix is not positive semi-definite
	  return false;
    }
    return true;
  }

}
	module cholesky_test {

  // +------------------------------------------------------------------------+
  // |  TEST DRIVER FOR VARIATIONS ON DENSE SYMMETRIC CHOLESKY FACTORIZATION  |
  // +------------------------------------------------------------------------+
  // |  Version 1 -- June 25, 2010                                            |
  // |                                                                        |
  // |  This driver tests nine different algorithms for computing the         |
  // |  L L^T factorization (Cholesky) of a symmetric positive definite       |
  // |  dense matrix.                                                         |
  // |                                                                        |
  // |  There are three sets of variations in each of two dimensions:         |
  // |  o  three different standard orderings of the linear algebra           |
  // |     operations in the factorization                                    |
  // |  o  three different levels of blocking.                                |
  // |                                                                        |
  // |  The Cholesky factorization  A = L L^^T is defined by the following    |
  // |  set of n^2 scalar equations derived by expanding the matrix-matrix    |
  // |  product on the right side of the matrix equation:                     |
  // |                                                                        |
  // |                A(i,j) = + reduce ( L (i, ..) L (j, ..)                 |
  // |                                                                        |
  // |  As written, these equations do not recognize the symmetry of A and    |
  // |  the triangular structure of L.  Recognizing those two facts allows    |
  // |  us to turn these equations into an algorithm for computing the        |
  // |  decomposition.                                                        |
  // |                                                                        |
  // |  Main diagonal:                                                        |
  // |     L(j,j) = sqrt ( A(j,j) - (+ reduce [k in ..j-1] L(j,k)**2 ) )      |
  // |  Below main diagonal:                                                  |
  // |     L(i,j) = ( A(i,j) - (+ reduce [k in ..j-1]                         |
  // |                                     L(i,k) * L(j,k) ) ) / L(j,j)       |
  // |                                                                        |
  // |  These equations can be promoted to block equations by treating:       |
  // |     scalar/ multiplication involving only non-diagonal blocks as       |
  // |        ordinary matrix-matrix multiplication;                          |
  // |     scalar/ multiplication involving diagonal blocks as ordinary       |
  // |        triangularor symmetric matrix-matrix multiplication;            |
  // |     taking the square root of a block to be its Cholesky factor.       |
  // |                                                                        |
  // |  These equations can be solved in many different orders, provided      |
  // |  that the following data dependencies are observed:                    |
  // |  o  The set of subdiagonal entries in a given row collectively         |
  // |     require that all entries to their left and above are known.        |
  // |  o  Each diagonal entry requires that all entries to its left in the   |
  // |     same row of L are known                                            |
  // |                                                                        |
  // |  The factorization process necessarily begins by computing L11 as the  |
  // |  square root of A11.  Thereafter, the individual scalar operations     |
  // |  can be performed in many different orders.  This package includes     |
  // |  three standard ways to perform the factorization:                     |
  // |    1. The outer product (right-looking) method                         |
  // |    2. The inner product (left-looking) method                          |
  // |    3. The bordering (up-looking) method.                               |
  // |  These are all vector methods that perform operations on entire        |
  // |  rows or columns at once.  There are many other, non-vector, methods.  |
  // |                                                                        |
  // |  Each of the three methods above appears in three levels of blocking:  |
  // |    1. No blocking -- all operations are on scalar entries              |
  // |    2. Each step considered as a 2x2 or 3x3 partitioning of the matrix  |
  // |       (similar to FLAME or MATLAB)                                     |
  // |    3. The entire matrix partitioned into k x k square subblocks (as    |
  // |       in a block 2D cyclic partitioning).                              |
  // |                                                                        |
  // |  The formulation in Chapel uses iterators extensively to avoid,        |
  // |  wherever possible, references to loop limits. As a result, the codes  |
  // |  are independent of the base indexing used and perform operations on   |
  // |  submatrices without index translation (as long as the row and column  |
  // |  index domains are identical).                                         |
  // |                                                                        |
  // |  The block size used in the block partitioning is visible only in the  |
  // |  iterators.  This is intended to emulate a later development where     |
  // |  the factorization codes will automatically adopt the block size of    |
  // |  the input argument's distribution.                                    |
  // |                                                                        |
  // +------------------------------------------------------------------------+

  use Random;

  use CyclicDist, BlockDist, Time;

  use cholesky_execution_config_consts;

  use cholesky_scalar_algorithms; 
  //elemental_cholesky_symmetric_index_ranges1,
  // elemental_cholesky_symmetric_index_ranges2;

  proc main {

    var Rand = new owned RandomStream ( real, seed = 314159) ;

    const MatIdx = { index_base .. #n, index_base .. #n };

    const mat_dom : domain (2) dmapped Cyclic ( startIdx = MatIdx.low )
      = MatIdx; // CYCLIC VERSION
    const distribution_type = "cyclic";

    // block_only const mat_dom : domain (2) dmapped Block ( boundingBox = MatIdx )
    // block_only   = MatIdx;   // BLOCK Version
    // block_only const distribution_type = "block";

    var A : [mat_dom] real,
        B : [mat_dom] real,
        L : [mat_dom] real;

    var positive_definite : bool;

    writeln ("Cholesky Factorization Tests");
    writeln ("   Matrix Dimension  : ", n);
    writeln ("   Block Size        : ", block_size);
    writeln ("   Indexing Base     : ", index_base);
    writeln ("   Array distribution: ", distribution_type );
    writeln ("");
    writeln ("Parallel Environment");
    writeln ("   Number of Locales         : ", numLocales );
    writeln ("   Number of cores per locale: ", Locales.numPUs());
    writeln ("   Max tasking parallelism   : ", Locales.maxTaskPar );
 

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
      A (i,j) = + reduce (  [k in mat_dom.dim (1) ] 
    			    B (i, k) * B (j, k) );

    // factorization algorithms overwrite a copy of A, leaving
    // the factor L in its place

    L = A;

    // writeln ("test matrix");

    print_L ( L );

    run_one_cholesky_algorithm ("scalar outer product cholesky factorization ",
				scalar_outer_product_cholesky );

    //    L = A;
    //
    //    run_one_cholesky_algorithm ("elemental cholesky factorization\n " +
    //				"iterators version 1", 
    //				elemental_cholesky_v1 );
    //    L = A;
    //
    //    run_one_cholesky_algorithm ("elemental cholesky factorization\n " +
    //				"iterators version 2", 
    //				elemental_cholesky_v2 );

    proc run_one_cholesky_algorithm ( header : string, cholesky_fun ) {

      // --------------------------------------------
      // Timer version assumes one process per locale
      // --------------------------------------------
      
      var clock : Timer;
      
      writeln ();
      writeln (header, "");
      
      clock.clear ();
      clock.start ();
      
      positive_definite = cholesky_fun ( L );
      
      clock.stop ();
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
      
      print_L ( L );
      
      if positive_definite then
	check_factorization ( A, L );
      else
	writeln ("factorization failed for non-positive semi-definite matrix");
    }
  }

  proc check_factorization ( A : [], L : [] )

    // -----------------------------------------------------------------------
    // Check the factorization by forming L L^T and comparing the result to A.
    // The factorization is successful if the results are within the
    // point-wise perturbation bounds of Demmel, given as Theorem 10.5 in
    // Higham's "Accuracy and Stability of Numerical Algorithms, 2nd Ed."
    // -----------------------------------------------------------------------
    
    where ( A.domain.rank == 2 ) {
    
    assert ( A.domain.dim (1) == A.domain.dim (2)  &&
	     L.domain.dim (1) == A.domain.dim (1)  &&
	     L.domain.dim (2) == A.domain.dim (2) 
	     );
    
    const mat_dom  = A.domain,
          mat_rows = A.domain.dim(0),
           n        = A.domain.dim(0).size;
    
    const unit_roundoff = 2.0 ** (-53), // IEEE 64 bit floating point
          gamma_n1      = (n * unit_roundoff) / (1.0 - n * unit_roundoff);
    
    var   resid : real,
           max_ratio = 0.0;
    
    var   d : [mat_rows] real;

    for i in mat_rows do
      d (i) = sqrt ( A (i,i) );
    
    forall (i,j) in mat_dom do {
      resid  = abs (A (i,j) - 
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


  proc print_L ( L : [] ) {
   
    const rows = L.domain.dim (1);

    // for i in rows do
    //   writeln (i, ":  ", L(i, ..i) );
  }

}
    
