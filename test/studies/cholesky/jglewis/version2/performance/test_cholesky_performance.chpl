module performance_cholesky_test {

  // +------------------------------------------------------------------------+
  // |  TEST DRIVER FOR VARIATIONS ON DENSE SYMMETRIC CHOLESKY FACTORIZATION  |
  // +------------------------------------------------------------------------+
  // |  This driver tests swcwn different algorithms for computing the        |
  // |  L L^T factorization (Cholesky) of a symmetric positive definite       |
  // |  dense matrix.                                                         |
  // |                                                                        |
  // |  Five algorithms are variants of a simple point outer product          |
  // |  factorization, altered in some way to test the performance of the     |
  // |  cross-compiler and the linked C compilation.  The other two are the   |
  // |  same scalar inner product and bordering factorizations in the         |
  // |  standard test directory.                                              |
  // |                                                                        |
  // |  The major comparison is between an algorithm that might expect to     |
  // |  exploit a column major memory layout (like Fortran) and an algorithm  |
  // |  for a row major memory layout (like C and Chapel).  The standard      |
  // |  tests all used a column major programming style for ease of           |
  // |  comparison with standard Fortran.  Here, four codes compute the       |
  // |  upper triangle of the factor in what should be exploitable forms      |
  // |  for Chapel's row major memory layout.                                 |
  // |                                                                        |
  // |  Within these four codes there are coding variations to see what       |
  // |  may help the compiler produce better code.  The results as of         |
  // |  December 2010 were that the one change that made a large difference   |
  // |  was to use explicit loops instead of vector operations.  This         |
  // |  hits a major programmer productivity gain, so we hope that future     |
  // |  compilers do not require this for performance.  It should not be      |
  // |  a necessary trick!                                                    |
  // +------------------------------------------------------------------------+

  use Random, Time;

  use execution_config_consts;

  use cholesky_scalar_algorithms;

  proc main {

    var Rand = new owned RandomStream ( real, seed = 314159) ;

    const mat_dom : domain (2) = { index_base .. #n, index_base .. #n };

    var A : [mat_dom] real,
        B : [mat_dom] real,
        L : [mat_dom] real;

    var positive_definite : bool;

    writeln ("Cholesky Factorization Scalar Performance Tests");
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

    if print_matrix_details then {
      writeln ("test matrix");
      print_lower_triangle ( L );
    }

    writeln ("\n\n");
    writeln ("scalar column major outer product cholesky factorization ");

    var clock : Timer;

    clock.start ();

    positive_definite = scalar_column_major_outer_product_cholesky ( L );

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
    writeln ("scalar row major outer product cholesky factorization: " );

    clock.clear ();
    clock.start ();

    positive_definite = scalar_row_major_outer_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ",
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    forall j in mat_dom.dim (0) do
      forall i in j+1 .. mat_dom.dim(0).high do
	L (i,j) = L (j,i);
    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


    L = A;

    writeln ("\n\n");
    writeln ("scalar row major outer product cholesky factorization: " );
    writeln ( "  with explicit loops in place of vector operations: " );

    clock.clear ();
    clock.start ();

    positive_definite = scalar_row_major_outer_product_no_vector_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ",
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    forall j in mat_dom.dim (0) do
      forall i in j+1 .. mat_dom.dim(0).high do
	L (i,j) = L (j,i);
    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


    L = A;

    writeln ("\n\n");
    writeln ("scalar row major outer product cholesky factorization");
    writeln ( "  with explicit temporaries: " );

    clock.clear ();
    clock.start ();

    positive_definite = scalar_row_major_temp_outer_product_cholesky ( L );

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ",
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    forall j in mat_dom.dim (0) do
      forall i in j+1 .. mat_dom.dim(0).high do
	L (i,j) = L (j,i);
    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");

    L = A;

    writeln ("\n\n");
    writeln ("scalar row major outer product cholesky factorization");
    writeln ( "  with explicitly bounded iterations: " );

    clock.clear ();
    clock.start ();

    positive_definite = scalar_row_major_bounded_outer_product_cholesky (L);

    clock.stop ();
    if !reproducible_output then {
      writeln ( "Cholesky Factorization time:    ", clock.elapsed () );
      writeln ( " collective speed in megaflops: ",
		( (n**3) / 3.0 )  / (10.0**6 * clock.elapsed () ) );
    }

    forall j in mat_dom.dim (0) do
      forall i in j+1 .. mat_dom.dim(0).high do
	L (i,j) = L (j,i);

    print_lower_triangle ( L );

    if positive_definite then
      check_factorization ( A, L );
    else
      writeln ("factorization failed for non-positive semi-definite matrix");


    writeln ("\n\n");
    writeln ("scalar inner product cholesky factorization ");

    L = A;

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


    writeln ("\n\n");
    writeln ("scalar bordering cholesky factorization ");

    L = A;

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

    forall (i,j) in mat_dom with (ref max_ratio) do { // race
      const resid : real  = abs (A (i,j) -
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


  proc print_lower_triangle ( L : [] ) {

    if print_matrix_details then
      for (i_row, i_col) in zip( L.domain.dim(0), L.domain.dim(1) ) do
	writeln (i_row, ":  ", L(i_row, ..i_col) );
  }
}

