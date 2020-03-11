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
  //      block in A and a single pair of blocks in L
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
    
  use block_partition_iterators,
      transposed_block_triangular_solve,
      scalar_inner_product_cholesky;

  proc dataflow_block_cholesky ( A : [] )

    where ( A.domain.rank == 2 ) {

    // This procedure expects as input a square matrix with the same ranges
    // (set of indices) for both rows and columns

    assert ( A.domain.dim (1) == A.domain.dim (2) );

    const A_rc_indices = A.domain.dim (1);  // row and column indices for A
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
    for block_col in vector_block_partition (A_rc_indices) do {
      const J = block_col.low;
      for block_row in vector_block_partition (A_rc_indices (J ..)) do {
        const I = block_row.low;
        if mods_necessary == 0 then
          all_schur_complement_mods_done$ (I, J) = true;
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

    for ( AII_rc_indices, AJKJK_row_indices ) in
      symmetric_2_by_2_block_partition ( A_rc_indices ) do {

      const I = AII_rc_indices.low;

      // wait until this diagonal block is ready to be factored

      all_schur_complement_mods_done$ (I, I) . readFF ();

      // factor this diagonal block

      pos_def = scalar_inner_product_cholesky
                                    ( A (AII_rc_indices, AII_rc_indices) );

      // this block of the factorization is complete

      block_computed$ (I, I) . writeEF (true);

      if pos_def && AJKJK_row_indices.size > 0 then {
        
        // Initiate separate threads to compute each subdiagonal block
        // in this block column of the matrix.  Each block is subjected to
        // a block triangular solve realizing the equation
        //      L (AJKJK_row_indices, AII_rc_indices) = 
        //                            L (AJKJK_row_indices, AII_rc_indices) *
        //                            L (AII_rc_indices, AII_rc_indices) ** (-T)
        // Each thread will block until all Schur complement modifications 
        // to its block have been completed.

	for (AJI_rows, A_later_K_rows) 
          in symmetric_2_by_2_block_partition (AJKJK_row_indices) 
	  do {
	    begin with (in AII_rc_indices,
                        in AII_rc_indices,
                        in AJI_rows,
                        in A_later_K_rows) {
	      compute_subdiagonal_block_launch_Schur_complement
		( AII_rc_indices, AII_rc_indices,
		  AJI_rows, AII_rc_indices,
		  A, A_later_K_rows );
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
      ( LII_rows, LII_cols, LJlaterKI_rows, LJlaterKI_cols, A : [], later_rows ) {

      // block indices for offdiagonal block 

      const I = LJlaterKI_rows.low,
            J = LJlaterKI_cols.low;

      const AJJ_cols     = LII_cols;

      assert ( AJJ_cols == LII_rows &&
               AJJ_cols == LJlaterKI_cols );

      // wait for all Schur complement modifications 
      // to be made to this subdiagonal block

      all_schur_complement_mods_done$ (I, J) . readFF ();
 
      // apply the inverse of the diagonal block to the subdiagonal block

      transposed_block_triangular_solve ( A(LII_rows, LII_cols), A(LJlaterKI_rows, LJlaterKI_cols) );

      // this block of the factorization is complete

      block_computed$ (I, J) . writeEF (true);

      // modify the related diagonal block of the Schur complement

      begin with (in LJlaterKI_rows, in LJlaterKI_cols) {
        modify_Schur_complement_diagonal_block ( LJlaterKI_rows,
                                                 LJlaterKI_cols,
                                                 A );
      }

      // spawn rest of the Schur complement tasks for the symmetric reduced 
      // matrix.  For each K > J, compute A (K,J) -= L(K,I) * L(J,I)^T

      for AKJ_rows in vector_block_partition (later_rows) do {

	begin with (in AKJ_rows,
                    in AJJ_cols,
                    in LJlaterKI_rows,
                    in LJlaterKI_cols) {
	  modify_Schur_complement_off_diagonal_block 
	      ( AKJ_rows, AJJ_cols,  LJlaterKI_rows, LJlaterKI_cols, A );
	}
      }
    }



    // ====================================================
    // Perform a single Schur complement block modification
    // to a diagonal subblock.
    // ====================================================
   
    proc modify_Schur_complement_diagonal_block ( LJJ_rows, LJJ_cols, A : [] ) {

      // block indices for offdiagonal block 

      const J      = LJJ_rows.low;

      // acquire lock for target block

      // read count of modifications left to be done
      // and lock access for other modifications

      var mods_left =  schur_complement_mods_to_be_done$ (J, J). readFE ();
      assert ( mods_left > 0 );

      // modify  A (J,J) -= L(J,J) * L(J,J)^T, using and preserving symmetry

      for i in LJJ_rows do
        for j in LJJ_rows (..i) do {
          A (i,j) -= + reduce [k in LJJ_cols] A (i,k) * A (j,k);
        }

      // check if this is the final modification to this block

      mods_left -= 1;
      if mods_left == 0 then 

	// yes -- mark as ready for next task

        all_schur_complement_mods_done$ ( J, J) . writeEF (true);

      else

	// no -- free lock for another modification

	schur_complement_mods_to_be_done$ (J, J) . writeEF (mods_left);
        
    }
    


    // ====================================================
    // Perform a single Schur complement block modification
    // to a off-diagonal subblock.
    // ====================================================

    proc modify_Schur_complement_off_diagonal_block 
                                             ( LKI_rows, LKI_cols, LJI_rows, LJI_cols, A : [] ) {

      // block indices for pair of offdiagonal blocks 

      const K = LKI_rows.low,
            I = LKI_cols.low,
            J = LJI_rows.low;

      assert ( I == LJI_cols.low );
 
      // wait for the second off-diagonal block to become ready

      block_computed$ ( K, I ) . readFF ();

       // read count of modifications left to be done
      // and lock access for other modifications

      var mods_left =  schur_complement_mods_to_be_done$ (K, J). readFE ();
      assert ( mods_left > 0 );

      // modify  A (K,J) -= LKI * LJI^T

      for i in LKI_rows do
        for j in LJI_rows do {
          A (i,j) -= + reduce [k in LKI_cols] A (i,k) * A (j,k);
        }

      // check if this is the final modification to this block

      mods_left -= 1;
      if mods_left == 0 then 

	// yes -- mark as ready for next task

        all_schur_complement_mods_done$ ( K, J) = true;
    
      else

	// no -- free lock for another modification

	schur_complement_mods_to_be_done$ (K, J) . writeEF (mods_left);
    }
  }
}
