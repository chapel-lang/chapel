module test_elemental_explicitly_strided_cholesky {

  // +------------------------------------------------------------------------+
  // |  TEST DRIVER FOR VARIATIONS ON DENSE SYMMETRIC CHOLESKY FACTORIZATION  |
  // +------------------------------------------------------------------------+
  // |  Special Driver to test changes needed to standard elemental Cholesky  |
  // |  factorization code to explicitly code for striding in the input       |
  // |  matrix.                                                               |
  // |                                                                        |
  // |  This version expects A to be a matrix with identical, but strided,    |
  // |  row and column indices.                                               |
  // |                                                                        |
  // |  (Allowing unequal strides creates complications similar to allowing   |
  // |   different row and column index sets.)                                |
  // +------------------------------------------------------------------------+

  use Random;

  use CyclicDist, Time;

  config const n = 4;

  config const index_base = -3;

  config const row_offset = 13;

  config const col_offset = 8;

  config const stride = 5;

  config const print_matrix_details = true;

  proc main {

    var Rand = new RandomStream ( seed = 314159) ;

    const unstrided_MatIdx = { index_base .. #n, index_base .. #n };

    const strided_MatIdx   = { index_base .. by stride #n , 
		               index_base .. by stride #n };

    const strided_mat_dom : domain (2, stridable = true) 
          dmapped Cyclic ( startIdx = strided_MatIdx.low )
      =   strided_MatIdx;

    const unstrided_mat_dom : domain (2, stridable = false) 
          dmapped Cyclic ( startIdx = unstrided_MatIdx.low )
      =   unstrided_MatIdx;

    const distribution_type = "cyclic";

    var A : [unstrided_mat_dom] real,
        B : [unstrided_mat_dom] real,
        L : [strided_mat_dom] real;

    var positive_definite : bool;

    writeln ("Cholesky Factorization Tests for Strided Index Ranges");
    writeln ("   Matrix Dimension  : ", n);
    writeln ("   Indexing Base     : ", index_base);
    writeln ("   Array distribution: ", distribution_type );
    writeln ("");
    writeln ("Parallel Environment");
    writeln ("   Number of Locales         : ", numLocales );
    writeln ("   Number of cores per locale: ", Locales.numPUs() );
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

    forall (i,j) in unstrided_mat_dom do
      A (i,j) = + reduce (  [k in unstrided_mat_dom.dim (1) ] 
    			    B (i, k) * B (j, k) );

    // factorization algorithms overwrite a copy of A, leaving
    // the factor L in its place

    L = A;

    print_lower_triangle ( L );

    check_procedural_alias ( A, L );
  }

  proc check_procedural_alias ( A : [], L_formal : [] )

    where ( A.domain.rank == 2 ) {

    assert ( A.domain.dim (1)               == A.domain.dim (2)         &&
	     L_formal.domain.dim (1).size == A.domain.dim (1).size  &&
	     L_formal.domain.dim (2).size == A.domain.dim (2).size 
	     );
    
    const mat_dom  = A.domain,
          mat_rows = A.domain.dim(1),
          n        = A.domain.dim(1).size;
    
    //    ref L = L_formal.reindex(mat_rows, mat_row);
    ref L = L_formal.reindex(mat_dom);
    
    writeln (" unaliased array" );
    print_lower_triangle ( L_formal );

    writeln (" aliased array" );
    print_lower_triangle ( L );

  }


  proc print_lower_triangle ( L : [] ) {
   
    if print_matrix_details then
      for (i_row, i_col) in ( L.domain.dim(1), L.domain.dim(2) ) do
	writeln (i_row, ":  ", L(i_row, L.domain.dim(2)(..i_col)) );
  }

}
    
