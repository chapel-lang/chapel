module HPCC_PTRANS {

  //  ===============================================
  //  Chapel Implementation of HPCC PTRANS Benchmark
  //  Compute  C = beta C + A', where  A  and  C  are 
  //  large distributed dense matrices 
  //  ===============================================


  config param printTimings = true;
  config const printPassFailOnly = false;

  //  default dimensions (overridable from command line)

  config const n_rows = 100, n_cols = 100, beta = 1.0;

  config const n_error_max = 1000;

  config const row_block_size = 3, col_block_size = 5;

  param zero = 0.0, one = 1.0, epsilon = 2.2E-16;

  use LinearAlgebra, 
      Time,
      BlockDist;
 

  proc main () {

    // ===================================================================
    // Test Harness for simple Chapel distributed matrix transpose routine
    // ===================================================================

    // declare distribution rules for matrix and transpose

    const Matrix_Block_Dist 
      = new unmanaged Block ( boundingBox = { 1..n_rows, 1..n_cols } );

    const Transpose_Block_Dist 
      = new unmanaged Block ( boundingBox = { 1..n_cols, 1..n_rows } );

    // declare domains (index sets) for matrix and transpose

    const matrix_domain     : domain (2) dmapped new dmap (
                              Matrix_Block_Dist) = { 1..n_rows, 1..n_cols },
          transpose_domain  : domain (2) dmapped new dmap (
                              Transpose_Block_Dist) = { 1..n_cols, 1..n_rows };

    var A                  : [matrix_domain   ] real, 
        C                  : [transpose_domain] real,
        C_save             : [transpose_domain] real,
        C_plus_A_transpose : [transpose_domain] real;

    var PTRANS_time : Timer;

    var norm_A, norm_C : real;

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

    C_save = C;

    forall (i,j) in transpose_domain do
      C_plus_A_transpose [i,j] = beta * sin (j) * cbrt (i) + erf(j) * cos (i);

    // norm_A = norm (A);
    // norm_C = norm (C);

    norm_C = sqrt ( + reduce [ (i,j) in transpose_domain ] ( C [i,j] ** 2 ) );
    norm_A = sqrt ( + reduce [ (i,j) in matrix_domain    ] ( A [i,j] ** 2 ) );

    writeln ( "    norm of A                 : ", norm_A );
    writeln ( "    norm of C                 : ", norm_C );

    const error_tolerance = ( norm_A  +  abs (beta) * norm_C ) * epsilon;
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

    writeln ( "  Unblocked Results" );
    verify(C, C_plus_A_transpose, error_tolerance, PTRANS_time);

    // ------------------------
    // Compute  C = beta C + A'
    // ------------------------

    C = C_save;

    PTRANS_time.clear ();
    PTRANS_time.start ();

    Chapel_blocked_PTRANS_v1 ( A, C, beta );

    PTRANS_time.stop ();

    // -------------------------------------------
    // Check results and compute timing statistics
    // -------------------------------------------

    writeln ( " Blocked Results V1" );
    verify(C, C_plus_A_transpose, error_tolerance, PTRANS_time);

    // ------------------------
    // Compute  C = beta C + A'
    // ------------------------

    C = C_save;

    PTRANS_time.clear ();
    PTRANS_time.start ();

    Chapel_blocked_PTRANS_v2 ( A, C, beta );

    PTRANS_time.stop ();

    // -------------------------------------------
    // Check results and compute timing statistics
    // -------------------------------------------

    writeln ( " Blocked Results V2" );
    verify(C, C_plus_A_transpose, error_tolerance, PTRANS_time);
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

  //  =====================================================
  //  | PTRANS:  compute  C = beta C + A',                |
  //  | where  A  and  C  are distributed dense matrices. |  
  //  =====================================================

  proc Chapel_blocked_PTRANS_v1 ( A : [?A_domain] real, 
                                  C : [?C_domain] real, 
                                  beta : real           ) : bool
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

          for c_rows in block_partitioning (C_domain, 0) do
            for c_cols in block_partitioning (C_domain, 1) do
              forall i in c_rows do
                for j in c_cols do
                  C [i,j] += A [j,i];
    
        else if ( beta == 0.0 ) then
      
          for c_rows in block_partitioning (C_domain, 0) do
            for c_cols in block_partitioning (C_domain, 1) do
              forall i in c_rows do
                for j in c_cols do
                  C [i,j] = A [j,i];
    
        else
      
          for c_rows in block_partitioning (C_domain, 0) do
            for c_cols in block_partitioning (C_domain, 1) do 
              forall i in c_rows do
                for j in c_cols do
                  C [i,j] = beta * C [i,j]  +  A [j,i];
        return true;
}
  }

  //  =====================================================
  //  | PTRANS:  compute  C = beta C + A',                |
  //  | where  A  and  C  are distributed dense matrices. |  
  //  =====================================================

  proc Chapel_blocked_PTRANS_v2 ( A : [?A_domain] real, 
                                  C : [?C_domain] real, 
                                  beta : real           ) : bool
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
    else {
    
    // --------------------------------------------
    // Acquire the specifications of the underlying 
    // processor grid from A's distribution
    // --------------------------------------------

    const C_locale_grid = C.domain.dist.targetLocales(); // block version
    const C_grid_domain = C_locale_grid.domain,
          n_processors  = C_grid_domain.size;

    assert ( C_grid_domain.low(0) == 0 && C_grid_domain.low(1) == 0 );

    assert ( C (C.domain.low).locale.id == 0 );
             
    // ------------------------------------------------
    // SPMD -- launch a separate task on each processor
    // ------------------------------------------------

    coforall processor in C_grid_domain do {

      on C_locale_grid (processor) do {

        if ( beta == 1.0 ) then

          for c_rows in SPMD_block_partitioning (C_domain, processor,
                                                 C_grid_domain,0) do
            for c_cols in SPMD_block_partitioning (C_domain, processor,
                                                   C_grid_domain,1) do 
              forall i in c_rows do
                for j in c_cols do
                  C [i,j] += A [j,i];
    
        else if ( beta == 0.0 ) then
      
          for c_rows in SPMD_block_partitioning (C_domain, processor, 
                                                 C_grid_domain,0) do
            for c_cols in SPMD_block_partitioning (C_domain, processor, 
                                                   C_grid_domain,1) do 
              forall i in c_rows do
                for j in c_cols do
                  C [i,j] = A [j,i];
    
        else
      
          for c_rows in SPMD_block_partitioning (C_domain, processor,
                                                 C_grid_domain,0) do
            for c_cols in SPMD_block_partitioning (C_domain, processor,
                                                   C_grid_domain,1) do 
              forall i in c_cols do
                for j in c_cols do
                  C [i,j] = beta * C [i,j]  +  A [j,i];
      }
}
        return true;
  }
}


  // ====================================
  // Iterator to Block Partition a Matrix)
  // ====================================

  iter block_partitioning ( C_domain, dimen )
  {
    // -------------------------------------------------------------------
    // Deliver as ranges the block partitioning of a vector.
    // The block size to be used is delivered via a global constant
    // rather than as an argument to allow this code to emulate a code
    // in which the block size were obtained from a blocking distribution.
    // -------------------------------------------------------------------

    const block_size = if dimen == 0 then row_block_size else col_block_size;
    for block_low in C_domain.dim (dimen) by block_size do
      yield block_low .. min ( block_low + block_size - 1, 
      C_domain.dim(dimen).high );
  }

  // ====================================
  // Iterator to Block Partition a Matrix)
  // ====================================

  iter SPMD_block_partitioning ( C_domain, processor, grid, dimen )
  {
    // -------------------------------------------------------------------
    // Deliver as ranges the block partitioning of a vector.
    // The block size to be used is delivered via a global constant
    // rather than as an argument to allow this code to emulate a code
    // in which the block size were obtained from a blocking distribution.
    // -------------------------------------------------------------------

    const block_size = if dimen == 0 then row_block_size else col_block_size;
    for block_low in C_domain.dim (dimen) + block_size*processor (dimen) 
    by block_size*(grid.dim (dimen).high+1) do
      yield block_low .. min ( block_low + block_size - 1, 
                               C_domain.dim(dimen).high );
  }

  proc verify(C:[?transpose_domain], C_plus_A_transpose, 
              error_tolerance, PTRANS_time) {
    // -------------------------------------------
    // Check results and compute timing statistics
    // -------------------------------------------

    // error = max reduce abs ( C - C_plus_A_transpose );

    const error = max reduce forall (i,j) in transpose_domain do 
                               abs ( C [i,j] - C_plus_A_transpose  [i,j] );

    //    var n_errors : int;
    //
    //    n_errors = 0;
    //
    //    for (i,j) in transpose_domain do {
    //      if ( ( i > 200 ) && ( abs ( C [i,j] - C_plus_A_transpose  [i,j] ) >
    //       error_tolerance ) && ( n_errors < n_error_max ) ) then {
    //  writeln ("  error [", i, ",", j, "] =",
    //           abs ( C [i,j] - C_plus_A_transpose  [i,j] ) );
    //  n_errors += 1;
    //      }
    //      else if ( n_errors >= n_error_max ) then
    //  break;
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

    const elapsed_time = PTRANS_time.elapsed (TimeUnits.seconds);
    
    const GB_sec = if ( elapsed_time  > zero ) then
                     ( n_rows * n_cols * 8 ) / ( 1.0e9 * elapsed_time )
                   else
                     zero;

    if printTimings {
      writeln ( "    Elapsed time              : ", elapsed_time );
      writeln ( "    Gigabytes per second      : ", GB_sec );
    }
  }
}
