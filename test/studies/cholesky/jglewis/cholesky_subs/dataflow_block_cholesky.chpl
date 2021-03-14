module dataflow_block_cholesky {

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
 

  // =========================================================================
  // The Data Flow Block Cholesky factorization computes solutions to the
  // individual block equations as the input arguments become available.  It
  // treats each instance of the following operations as tasks:
  //   -- factoring a diagonal block into its Cholesky factors (the
  //      equations on the main diagonal)
  //   -- solving the block linear system associated with the "division" in
  //      the off-diagonal equations
  //   -- performing the Schur complement modification  A - L L^T for a single
  //      pair of blocks in L
  //  The diagonal factorizations are performed by the main thread of control,
  //  anticipating that locality will be used to perform these operations on
  //  the processor holding the block.  The other operations are each performed
  //  by independent Chapel threads.  The block solve threads for a given block
  //  column are launched as soon as the diagonal block factorization needed
  //  for the solve is completed.  Each Schur complement modification involves
  //  two blocks of L from the same block column.  Each modification operation
  //  is launched by the completion of the block solve step for the block with
  //  smaller row index.
  //
  //  Each operation stalls, waiting for an empty word of data to fill, until
  //  all of its arguments are ready. that is, until all of the needed Schur
  //  complement modifications have completed.  No attempt to prioritze the 
  //  scheduling of operations in this first, simple, version.
  // =========================================================================
    
  use block_partition_iterators, symmetric_blas;

  proc dataflow_block_cholesky ( A : [] )

    where ( A.domain.rank == 2 ) {

    assert ( A.domain.dim (0) == A.domain.dim (1) );

    const col_indices = A.domain.dim (0);  // indices of either row or column
    var   pos_def : bool;

    // --------------------------------------------------------------------
    // Synchronization variables:
    //   1.  Each block has a sync variable "schur_complement_mods_to_be_done$",
    //       which maintains the number of modifications remaining to be made.
    //       The normal state of this variable is full, but it is set empty
    //       while a modification to this block is under way.  In this way
    //       it also serves as a lock on the critical section so that only
    //       one Schur complement modification can have access to the location
    //       at a given time.
    //   2.  Each block has a single variable "all_schur_complement_mods_done$"
    //       that signals when the block is ready for its final operation.
    //       This variable is empty until the last Schur complement 
    //       modification has completed. 
    //   3.  Each block has a single variable "block_computed$" that signals
    //       that the final factorization values for this block have
    //       been stored.  In other words, the diagonal block factorization
    //       or the off-diagonal block solve has completed.
    //
    //   These arrays are allocated one per block, using the pair of low 
    //   row and column indices as a block index.  The storage overhead 
    //   then is three words per block.
    // --------------------------------------------------------------------

    var block_leading_index_domain : domain (2, stridable=true) 
      = block_leading_indices ( A.domain );

    var all_schur_complement_mods_done$  : [block_leading_index_domain] single 
                                                                        bool,
        block_computed$                  : [block_leading_index_domain] single 
                                                                        bool;

    var schur_complement_mods_to_be_done$ : [block_leading_index_domain] sync 
                                                                        int;


    // Each block is subject to one fewer modification than its block
    // column index.  Set counter for each block.

    var mods_necessary = 0;
    for col_block in vector_block_partition (col_indices) do {
      const J = col_block.low;
      for row_block in vector_block_partition (col_indices (J ..)) do {
        const I = row_block.low;
        if mods_necessary == 0 then
          all_schur_complement_mods_done$ (I, J).writeEF(true);
	else
	  schur_complement_mods_to_be_done$ (I, J) . writeEF (mods_necessary);
      }
      mods_necessary += 1;
    }

    // ------------------------------------------------------------
    // Main loop iterates over the block columns of the matrix,
    // computing the diagonal block factorization and launching the
    // subsidiary off-diagonal block solves.
    // ------------------------------------------------------------

    for ( reduced_mtx_cols, leading_cols, trailing_cols ) in
      symmetric_reduced_matrix_2_by_2_block_partition ( col_indices ) do {

      const I = leading_cols.low;

      // wait until this diagonal block is ready to be factored

      all_schur_complement_mods_done$ (I, I) . readFF ();

      // factor this diagonal block

      pos_def = scalar_outer_product_cholesky
                                    ( A (leading_cols, leading_cols) );

      // this block of the factorization is complete

      block_computed$ (I, I) . writeEF (true);

      if pos_def && trailing_cols.size > 0 then {
        
        // Initiate separate threads to compute each subdiagonal block
        // in this block column of the matrix.  Each block is subjected to
        // a block triangular solve realizing the equation
        //      L (trailing_cols, leading_cols) = 
        //                              L (trailing_cols, leading_cols) *
        //                              L (leading_cols, leading_cols) ** (-T)
        // Each thread will block until all Schur complement modifications 
        // to its block have been completed.

	for (reduced_mtx_rows, leading_offdiag_rows, trailing_offdiag_rows) 
          in symmetric_reduced_matrix_2_by_2_block_partition (trailing_cols) 
	  do {

	    begin with (in leading_cols,
                        in leading_offdiag_rows,
                        in trailing_offdiag_rows) {
	      compute_subdiagonal_block_launch_Schur_complement
		( leading_cols, leading_cols,
		  leading_offdiag_rows, leading_cols,
		  A, trailing_offdiag_rows );
	    }
	  }
	}
      else

        // error return if matrix is not positive definite

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // NEED TO TERMINATE OUTSTANDING THREADS
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
        if !pos_def then return false;
    }

    // return success 

    return true;



    // ==============================================================
    // Perform the block solve operation to finish computing a single 
    // off-diagonal block in the factor.  Launch the Schur complement
    // modifications involving this block and blocks in later block
    // rows of this block column.
    // ==============================================================


    proc compute_subdiagonal_block_launch_Schur_complement 
      ( L_diag_rows, L_diag_cols, L_offdiag_rows, L_offdiag_cols, A : [], trailing_rows ) {

      // block indices for offdiagonal block 

      const I = L_offdiag_rows.low,
            J = L_offdiag_cols.low;

      const diag_cols     = L_diag_cols;

      assert ( diag_cols == L_diag_rows &&
               diag_cols == L_offdiag_cols );

      // wait for all Schur complement modifications 
      // to be made to this subdiagonal block

      all_schur_complement_mods_done$ (I, J) . readFF ();
 
      // apply the inverse of the diagonal block to the subdiagonal block

      transposed_block_triangular_solve ( A(L_diag_rows, L_diag_cols), A( L_offdiag_rows, L_offdiag_cols) );

      // this block of the factorization is complete

      block_computed$ (I, J) . writeEF (true);

      // modify the related diagonal block of the Schur complement

      begin with (in L_offdiag_rows, in L_offdiag_cols) {
        modify_Schur_complement_diagonal_block ( L_offdiag_rows,
                                                 L_offdiag_cols,
                                                 A );
      }

      // spawn rest of the Schur complement tasks 
      // for the symmetric reduced matrix
      for
	lower_block_rows in vector_block_partition (trailing_rows) do {

	begin with (in lower_block_rows, in diag_cols,
                    in L_offdiag_rows, in L_offdiag_cols) {
	  modify_Schur_complement_off_diagonal_block 
	      ( lower_block_rows, diag_cols, L_offdiag_rows, L_offdiag_cols, A );
	}
      }
    }



    // ====================================================
    // Perform a single Schur complement block modification
    // to a diagonal subblock.
    // ====================================================
   
    proc modify_Schur_complement_diagonal_block ( L_rows, L_cols, A : [] ) {

      // block indices for offdiagonal block 

      const I      = L_rows.low;

      // acquire lock for target block

      const J = L_cols.low;

      // read count of modifications left to be done
      // and lock access for other modifications

      var mods_left =  schur_complement_mods_to_be_done$ (I, I). readFE ();
      assert ( mods_left > 0 );

      // modify  A (I,I) -= L * L^T, using and preserving symmetry

      for i in L_rows do
        for j in L_rows (..i) do {
          A (i,j) -= + reduce [k in L_cols] A (i,k) * A (j,k);
        }

      // check if this is the final modification to this block

      mods_left -= 1;
      if mods_left == 0 then 

	// yes -- mark as ready for next task

        all_schur_complement_mods_done$ ( I, I) . writeEF (true);

      else

	// no -- free lock for another modification

	schur_complement_mods_to_be_done$ (I, I) . writeEF (mods_left);
        
    }
    


    // ====================================================
    // Perform a single Schur complement block modification
    // to a off-diagonal subblock.
    // ====================================================

    proc modify_Schur_complement_off_diagonal_block 
                                             ( L1_rows, L1_cols, L2_rows, L2_cols, A : [] ) {

      // block indices for pair of offdiagonal blocks 

      const I = L1_rows.low,
            K = L1_cols.low,
            J = L2_rows.low;

      assert ( K == L2_cols.low );
 
      // wait for the second off-diagonal block to become ready

      block_computed$ ( I, K ) . readFF ();

       // read count of modifications left to be done
      // and lock access for other modifications

      var mods_left =  schur_complement_mods_to_be_done$ (I, J). readFE ();
      assert ( mods_left > 0 );

      // modify  A (I,J) -= L1 * L2^T

      for i in L1_rows do
        for j in L2_rows do {
          A (i,j) -= + reduce [k in L1_cols] A (i,k) * A (j,k);
        }

      // check if this is the final modification to this block

      mods_left -= 1;
      if mods_left == 0 then 

	// yes -- mark as ready for next task

        all_schur_complement_mods_done$ ( I, J).writeEF(true);
    
      else

	// no -- free lock for another modification

	schur_complement_mods_to_be_done$ (I, J) . writeEF (mods_left);
    }
  }
}
