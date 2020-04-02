module HPCC_PTRANS {

  //  ===============================================
  //  Chapel Implementation of HPCC PTRANS Benchmark
  //  Compute  C = beta C + A', where  A  and  C  are 
  //  large distributed dense matrices 
  //  ===============================================


  config const printPassFailOnly = false;

  //  default dimensions (overridable from command line)

  config const n_rows : int (64) = 800, n_cols : int (64) = 800, beta = 1.0;

  config const n_error_max = 1000;

  config const printStats = true;

  param zero = 0.0, one = 1.0, epsilon = 2.2E-16;

  use Norm, 
      Time,
      BlockDist;

  proc main () {

    // ===================================================================
    // Test Harness for simple Chapel distributed matrix transpose routine
    // ===================================================================

    // declare distribution rules for matrix and transpose

    const Matrix_Block_Dist 
      = new dmap(new Block(rank=2,idxType=int(64),boundingBox={1..n_rows, 1..n_cols}));

    const Transpose_Block_Dist 
      = new dmap(new Block(rank=2,idxType=int(64),boundingBox={1..n_cols, 1..n_rows}));

    // declare domains (index sets) for matrix and transpose

    const matrix_domain     : domain (2, int (64) ) dmapped 
                              Matrix_Block_Dist = { 1..n_rows, 1..n_cols },
          transpose_domain  : domain (2, int (64)) dmapped 
                              Transpose_Block_Dist = { 1..n_cols, 1..n_rows };

    var A                  : [matrix_domain   ] real, 
        C                  : [transpose_domain] real,
        C_plus_A_transpose : [transpose_domain] real;

    var PTRANS_time : Timer;

    var error, norm_A, norm_C : real;

    var error_tolerance, elapsed_time, GB_sec: real;

    //    var n_errors : int;

    // -------------------------------------------------------------------------

    writeln ( "" );
    writeln ( "Distributed Matrix Transpose" );
    writeln ( "" );
    writeln ( "  Problem Characteristics" );
    writeln ( "                       rows   : ", n_rows );
    writeln ( "                       columns: ", n_cols );
    writeln ( "                       beta   : ", beta );

   // -------------------------------------------------------------------------
    // Generate test matrices and explicit comparative result.  Real HPCC
    // benchmark uses a fragmented memory model random number, which is
    // difficult and expensive to replicate in the Chapel global memory model.
    // We substitute less expensive test matrices that are still likely to
     // detect any addressing errors.
    // -------------------------------------------------------------------------

    forall (i,j) in matrix_domain do {
      A [i,j] = erf (i) * cos (j);
      C [j,i] = sin (i) * cbrt (j);
    }

    forall (i,j) in transpose_domain do
      C_plus_A_transpose [i,j] = beta * sin (j) * cbrt (i) + erf(j) * cos (i);

    // norm_A = norm (A);
    // norm_C = norm (C);

    norm_C = sqrt ( + reduce [ (i,j) in transpose_domain ] ( C [i,j] ** 2 ) );
    norm_A = sqrt ( + reduce [ (i,j) in matrix_domain    ] ( A [i,j] ** 2 ) );

    //    norm_C$ = 0.0;
    //    forall (i,j) in transpose_domain do
    //      norm_C$ += C [i,j] ** 2;
    //    norm_C$ = sqrt ( norm_C$ );
    //
    //    norm_A$ = 0.0;
    //    forall (i,j) in matrix_domain do
    //      norm_A$ += A [i,j] ** 2;
    //    norm_A$ = sqrt ( norm_A$ );
    
    writeln ( "    norm of A                 : ", norm_A );
    writeln ( "    norm of C                 : ", norm_C );

    error_tolerance = ( norm_A  +  abs (beta) * norm_C ) * epsilon;
    writeln ( "    max acceptable discrepancy: ", error_tolerance);
    writeln ( );

    // ------------------------
    // Compute  C = beta C + A'
    // ------------------------

    PTRANS_time.clear ();
    PTRANS_time.start ();

    Chapel_PTRANS ( A, C, beta );

    PTRANS_time.stop ();

    // -------------------------------------------
    // Check results and compute timing statistics
    // -------------------------------------------

    // error = max reduce abs ( C - C_plus_A_transpose );

    error = max reduce forall (i,j) in transpose_domain do 
      abs ( C [i,j] - C_plus_A_transpose  [i,j] );

    writeln ( "  Results" );

    //    n_errors = 0;
    //
    //    for (i,j) in transpose_domain do {
    //      if ( ( i > 200 ) && ( abs ( C [i,j] - C_plus_A_transpose  [i,j] ) >
    //	     error_tolerance ) && ( n_errors < n_error_max ) ) then {
    //	writeln ("  error [", i, ",", j, "] =",
    //		 abs ( C [i,j] - C_plus_A_transpose  [i,j] ) );
    //	n_errors += 1;
    //      }
    //      else if ( n_errors >= n_error_max ) then
    //	break;
    //    }
      
    if printPassFailOnly then
      writeln ( if error > error_tolerance
                then "    *** FAILURE ***"
                else "    *** SUCCESS ***" );
    else {
      if ( error > error_tolerance ) then
        writeln ( "    *** FAILURE *** error     : ", error );
      else if ( error != zero ) then
        writeln ( "    *** SUCCESS ***  error    : ", error );
      else
        writeln ( "    *** SUCCESS ***  exact match" );
    }

    if (printStats) {
      elapsed_time = PTRANS_time.elapsed (TimeUnits.seconds);
    
      if ( elapsed_time  > zero ) then
        GB_sec = ( n_rows * n_cols * 8 ) / ( 1.0e9 * elapsed_time );
      else
        GB_sec = zero;

      writeln ( "    Elapsed time              : ", elapsed_time );
      writeln ( "    Gigabytes per second      : ", GB_sec );
    }
  }
    
  //  =====================================================
  //  | PTRANS:  compute  C = beta C + A',                |
  //  | where  A  and  C  are distributed dense matrices. |  
  //  =====================================================

  proc Chapel_PTRANS ( A : [?A_domain] real, 
		      C : [?C_domain] real, 
		      beta : real ) : bool
    where ( A.rank == 2 ) && ( C.rank == 2 )
    {

    //  ---------------------------------------------------------------------
    //  Array dimensions and distribution information is conveyed to the 
    //  transpose operation by the domain specification of each matrix.
    //  The extended transpose operation is realized as three separate cases.
    //  ---------------------------------------------------------------------

    if ( ( A_domain.dim(0) != C_domain.dim(1)) ||
	      ( A_domain.dim(1) != C_domain.dim(0))  ) then
      return false;
    else
      {
	if ( beta == 1.0 ) then

      	  forall (i,j) in C_domain do
	    C [i,j] += A [j,i];
    
	else if ( beta == 0.0 ) then
      
	  forall (i,j) in C_domain do
	    C [i,j] = A [j,i];
    
	else
      
	  forall (i,j) in C_domain do
	    C [i,j] = beta * C [i,j]  +  A [j,i];
	return true;
      }
  }

}
