module standard_cholesky_test {

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

  use Random, Time;

  use cholesky_execution_config_consts;

  use scalar_outer_product_cholesky, 
      scalar_inner_product_cholesky, 
      scalar_bordering_cholesky, 
      block_1D_outer_product_cholesky, 
      block_2D_outer_product_cholesky,
      block_1D_inner_product_cholesky,
      block_2D_inner_product_cholesky,
      block_1D_bordering_cholesky,
      block_2D_bordering_cholesky;

  proc main {

    var Rand = new RandomStream ( seed = 314159) ;

    const mat_dom : domain (2) = { index_base .. #n, index_base .. #n };

    var A : [mat_dom] real,
        B : [mat_dom] real,
        L : [mat_dom] real;

    var positive_definite : bool;

    writeln ("Cholesky Factorization Tests for Equal Unstrided Index Ranges");
    writeln ("   Matrix Dimension: ", n);
    writeln ("   Block Size      : ", block_size);
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
      A (i,j) = + reduce (  [k in mat_dom.dim (1) ] 
    			    B (i, k) * B (j, k) );

    // factorization algorithms overwrite a copy of A, leaving
    // the factor L in its place

    L = A;

    if print_matrix_details then {
      writeln ("test matrix");
      print_lower_triangle ( L );
    }

    writeln ("\n\n");
    writeln ("scalar outer product cholesky factorization ");

    var clock : Timer;
          
    clock.clear ();
    clock.start ();

    positive_definite = scalar_outer_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");

    L = A;

    writeln ("\n\n"); 
    writeln ("block 1D outer product cholesky factorization, block size: ",
	      block_size );

    clock.clear ();
    clock.start ();

    positive_definite = block_1D_outer_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");

    print_lower_triangle ( L );

    L = A;

    writeln ("\n\n"); 
    writeln ("block 2D outer product cholesky factorization, block size: ",
	      block_size );

    clock.clear ();
    clock.start ();

    positive_definite = block_2D_outer_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


    L = A;

    writeln ("\n\n");
    writeln ("scalar inner product cholesky factorization ");

    clock.clear ();
    clock.start ();

    positive_definite = scalar_inner_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


     
    L = A;

    writeln ("\n\n"); 
    writeln ("block 1D inner product cholesky factorization, block size: ",
	      block_size );

    clock.clear ();
    clock.start ();

    positive_definite = block_1D_inner_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


    L = A;

    writeln ("\n\n"); 
    writeln ("block 2D inner product cholesky factorization, block size: ",
	      block_size );

    clock.clear ();
    clock.start ();

    positive_definite = block_2D_inner_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


    L = A;

    writeln ("\n\n");
    writeln ("scalar bordering cholesky factorization ");

    clock.clear ();
    clock.start ();

    positive_definite = scalar_bordering_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");

    L = A;

    writeln ("\n\n"); 
    writeln ("block 1D bordering cholesky factorization, block size: ",
	      block_size );

    clock.clear ();
    clock.start ();

    positive_definite = block_1D_bordering_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


    L = A;

    writeln ("\n\n"); 
    writeln ("block 2D bordering cholesky factorization, block size: ",
	      block_size );

    clock.clear ();
    clock.start ();

    positive_definite = block_2D_bordering_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ", 
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");

    proc run_one_cholesky_algorithm ( header : string, cholesky_fun ) {
      
      // --------------------------------------------
      // Timer version assumes one process per locale
      // --------------------------------------------
      //
      // draft of a procedure to encapsulate the test process.  Unused at this
      // time because Chapel presently doesn't handle generic functions as
      // arguments. The cholesky codes all are written generically because they 
      // can be.
          
      var clock : Timer;
          
      writeln ("\n\n");
      writeln (header);
          
      clock.clear ();
      clock.start ();
          
      positive_definite = cholesky_fun ( L );
          
      clock.stop ();
      if !reproducible_output then {
	writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
	writeln ( " collective speed in megaflops: ", 
		  ( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
      }
          
      print_lower_triangle ( L );

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
          mat_rows = A.domain.dim(1),
          n        = A.domain.dim(1).length;

    const unit_roundoff = 2.0 ** (-53), // IEEE 64 bit floating point
          gamma_n1      = (n * unit_roundoff) / (1.0 - n * unit_roundoff);

    var   max_ratio = 0.0;

    var   d : [mat_rows] real;

    for i in mat_rows do
      d (i) = sqrt ( A (i,i) );
    
    forall (i,j) in mat_dom with (ref max_ratio) do { // race
      const resid: real =
               abs (A (i,j) - 
		    + reduce ( [k in mat_dom.dim(1) (..min (i,j))]
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


  proc print_lower_triangle ( L : [] ) {
   
    if print_matrix_details then
      for (i_row, i_col) in zip( L.domain.dim(1), L.domain.dim(2) ) do
	writeln (i_row, ":  ", L(i_row, ..i_col) );
  }
}
    
