module elemental_cholesky_symmetric_index_ranges {

  // =========================================================================
  // CHOLESKY FACTORIZATION IN THE STYLE OF THE UNIV. OF TEXAS "ELEMENTAL"
  // LINEAR ALGEBRA PACKAGE (Jack Poulson, Bryan Marker, Robert van de Geijn,
  // FLAME working note #44)
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
  //
  // These equations can be promoted to block equations by treating:
  //    scalar/ multiplication involving only non-diagonal blocks as ordinary 
  //       matrix-matrix multiplication;
  //    scalar/ multiplication involving diagonal blocks as ordinary triangular
  //       or symmetric matrix-matrix multiplication;
  //    taking the Cholesky factor of a block as its square root.
  //
  // Conventionally only one array argument, A, is used in factorization
  // routines, and only the lower triangle is used.  On output the entries of 
  // L overwrite the entries of A.  The partial sums of the reductions are 
  // accumulated during the course of the algorithm also in the space occupied
  // by the input matrix  A.  Conventionally, the entries in the upper
  // triangle of A are left untouched. 
  // =========================================================================
  // The outer product Cholesky factorization computes one block column of L at
  // each step. During each step the remaining columns of A are modified by a 
  // low rank outer product modication -- the reduce operations are accumulated 
  // one block step at a time for each entry in the yet unfactored part of the
  // matrix.  The computed entries of L will not otherwise need to be 
  // referenced again in the factorization.  
  // =========================================================================
  // The Cholesky factorization in the "Elemental" package uses an outer 
  // product factorization scheme in which the matrix is distributed globally 
  // cyclically in 2 dimensions.  The factorization uses a limited amount of
  // redundant computation and some additional communication to improve
  // load balance over the standard block Cholesky factorization for block
  // cyclic distributions.  The additional communication is offset by
  // sharing more work across all processors.
  // =========================================================================
  // This Chapel implementation emulates the standard SPMD programming style
  // via its native task parallelism constructs.  The lack of a standard way
  // to replicate data and computation across tasks requires this emulation of
  // the SPMD style.  We expect that later versions will fit the native Chapel
  // execution model more closely.  Barriers are implemented in the Barriers
  // standard module for synchronization beyond the specific functionality of
  // coforall and sync statements.
  // =========================================================================
  // The input array should be a two-dimensional array, indexed by the
  // same set of indices for rows as for columns.
  // =========================================================================
  // Certain important sections of the algorithm will be entirely
  // local to each task when the data is distributed cyclically.
  // Chapel "local" clauses should provide higher efficiency for these 
  // sections, but for the moment they are commented-out" because of bugs in
  // the implementation of such local declarations.
  // =========================================================================

  use CyclicDist, Barriers;

  use elemental_schur_complement, 
      locality_info, 
      local_reduced_matrix_cyclic_partition,
      scalar_inner_product_cholesky,
      transposed_block_triangular_solve;

  proc elemental_cholesky_symmetric_index_ranges ( A : [] )

    where ( A.domain.rank == 2 ) {

    // argument checking -- would like to be able to check that A is
    // a cyclically distributed array

    const A_idx_range = A.domain.dim (0);  // indices of either row or column

    assert ( A_idx_range == A.domain.dim (1) );

    // --------------------------------------------
    // Acquire the specifications of the underlying 
    // processor grid from A's distribution
    // --------------------------------------------

    const A_locale_grid = A.targetLocales();
    const A_grid_domain = A_locale_grid.domain,
          n_processors  = A_grid_domain.size;

    assert ( A_grid_domain.low == (0,0) );

    assert ( A (A.domain.low).locale.id == 0 );
	     
    // initialize a tasking barrier
    var bar = new Barrier(n_processors);
      
    // ------------------------------------------------
    // SPMD -- launch a separate task on each processor
    // ------------------------------------------------

    coforall processor in A_grid_domain do {

      on A_locale_grid (processor) do {

	// --------------------------------------------------
	// Acquire locality information from A's distribution
	// --------------------------------------------------

	const ( my_rows, my_cols, my_L21_rows_to_compute ) = 
	  my_local_cyclic_data ( A.domain, A_grid_domain, processor );

	assert ( A (A.domain.low + processor ).locale ==
		 A_locale_grid (processor) );

	var save_tasks_to_finish : int;

	var pos_def : bool;

	// --------------------------------------------------------------------
	// Outer loop factors the matrix in blocks of columns.  The
	// iterator in the "for" loop returns for each instance of the loop
	// the indices of the four blocks in a 2 by 2 symmetric partition of 
	// the reduced matrix.  The ranges  A11_cols and  A22_cols  are the
	// global indices in the block partitioning.  The ranges beginning with
	// "my_" are the subsets of those indices for which this processor
	// holds some of the entries locally.
	// --------------------------------------------------------------------

	for ( A11_cols, A22_cols ) in
	  local_reduced_matrix_cyclic_partition ( A_idx_range ) 
	  do {

	    // ---------------------------------------------------------------
	    // Each task obtains and then factors a local copy of the leading 
	    // diagonal block.  The communication is an ALL-GATHER.  The 
	    // redundant computation allows each processor to participate in
	    // the second step (block solve).
	    // ---------------------------------------------------------------

	    // The locally owned subsets of rows and columns within each block
	    // are given as intersections of ranges, done neatly by Chapel

	    const my_A1x_rows        = my_rows [ A11_cols ];
	    const my_Ax1_cols        = my_cols [ A11_cols ];
	   
	    const my_A2x_rows        = my_rows [ A22_cols ];
	    const my_Ax2_cols        = my_cols [ A22_cols ];
	    const I_compute_L21_rows = my_L21_rows_to_compute [ A22_cols ];

	    const A11_indices = {A11_cols, A11_cols};

	    var   A11 : [A11_indices] real;

	    // Global ALL-GATHER

	    A11 (A11_indices) = A (A11_indices);

	    // Each task computes its own factorization of A11 locally

	    // local {
	    pos_def = scalar_inner_product_cholesky (A11);

	    // Each processor now puts entries of A11 that correspond to
	    // its local entries of A (A11_cols, A11_cols) back into
	    // the global array.  This requires no communication.

	    A (my_A1x_rows, my_Ax1_cols) = 
	      A11 (my_A1x_rows, my_Ax1_cols);
	    // }

	    if pos_def && A22_cols.size > 0 then {

	      // ---------------------------------------------------------------
	      // Compute the remainder of the active block column of L by a
	      // block triangular solve realizing the equation
	      //      L (A22_cols, A11_cols) = 
	      //                          L (A22_cols, A11_cols) *
	      //                          L (A11_cols, A11_cols) ** (-T)
	      // ALL processors participate in this operation, each taking its
	      // unique subset of rows.  The processors in each row of the 
	      // processor grid jointly hold all the data for all rows in which
	      // any hold entries.  This set of rows is split evenly among this
	      // set of processors.
	      // ---------------------------------------------------------------

	      const I_compute_L21_indices = {I_compute_L21_rows, A11_cols};

	      var   I_compute_L21 : [I_compute_L21_indices] real;

	      // the assignment following is an all-gather among processors in
	      // a single processor row.  The test on non-zero length should not
	      // be required, but is at present.

	      if  I_compute_L21_indices.size > 0 then {
		I_compute_L21 [I_compute_L21_indices] =
		  A [I_compute_L21_indices];
	
		// local {
		transposed_block_triangular_solve ( A11, I_compute_L21 );
		// }
	      
		// Everyone puts back their pieces of the global array, an 
		// ALL-SCATTER in a single processor row.
	      
		A [I_compute_L21_indices] =
		  I_compute_L21 [I_compute_L21_indices];
	      }

	      bar.barrier();

	      // Each processor acquire all entries in rows or columns of the
	      // off-diagonal block for which it owns any entries.  Data is
	      // replicated.  

	      // The assignment following is an ALL-GATHER among processors in
	      // a single processor row

	      const L21_Idx              = {my_A2x_rows, A11_cols};
	      const L21 : [L21_Idx] real = A [L21_Idx];

	      bar.barrier();

	      // The assignment following should be an ALL-GATHER among 
	      // processors in a single processor column, since those 
	      // processors together own copies of all rows in L21.
	      // However, implementing this requires that L21 be an
	      // array distributed cyclically by rows only and replicated
	      // across all processor columns.  Chapel replication is not
	      // ready for this yet.  (The barrier preceding this will be
	      // necessary when that replication is available.  It is 
	      // unnecessary in the current code.

	      const L12_Idx              = {my_Ax2_cols, A11_cols};
	      const L12 : [L12_Idx] real = A [L12_Idx];

	      // -------------------------------------------------------------
	      // make local rank block_size (outerproduct) modification to the
	      // remaining block rows and columns of A, which become the Schur 
	      // Complement
	      // -------------------------------------------------------------
	
	      // local {
	      elemental_schur_complement 
		( A (my_A2x_rows, my_Ax2_cols), L12, L21 );
	      // }
	
	      bar.barrier();
	    }
	    // else

	    // error return if matrix is not positive definite
	    // Chapel error-handling capabilities aren't up to handling
	    // this cleanly yet.
	
	    // if !pos_def then return false;
	  }
      }
    }

    // return success 

    return true;
  }
}
