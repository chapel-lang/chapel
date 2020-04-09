module local_reduced_matrix_cyclic_partition_alt {

  use cholesky_execution_config_consts;

  iter local_reduced_matrix_cyclic_partition_alt 
    ( idx_range, processor : 2*int, r, c ) {

    // -------------------------------------------------------------------
    // Deliver as ranges the sequence of reduced matrices that arise in a
    // triangular factorization, partitioned into 2 by 2 blocks.  It is
    // required that the row and column domains of the matrix are identical.
    // The seven output ranges are
    //    1. the rows (or columns) in the top (diagonal) block of the reduced 
    //       matrix
    //    2. the rows (or columns) in the remainder of the reduced matrix
    //    3. - 6. the subsets of each of these for which this processor
    //            stores some data locally.
    //    7. the subset of 5 for which this processor will actually compute
    //       entries of L outside the diagonal.
    // -------------------------------------------------------------------
    // The block size to be used is delivered via a global constant
    // rather than as an argument to allow this code to emulate a code
    // in which the block size were obtained from a blocking distribution.
    // -------------------------------------------------------------------
    
    const (my_processor_row, my_processor_col) = processor;

    // this processor owns data with indices in the tensor product
    // [ my_rows, my_cols ];

    const my_rows = idx_range.low + my_processor_row .. idx_range.high by r;
    const my_cols = idx_range.low + my_processor_col .. idx_range.high by c;
    const my_L21_rows_to_compute =
      idx_range.low + my_processor_row + r*my_processor_col .. idx_range.high 
      by  r*c;

    for block_low in idx_range by block_size do {

      var next_block_low = block_low + block_size;

      const A11_cols = block_low .. 
	                   min ( next_block_low - 1, idx_range.high ),
	    A11_rows = A11_cols;
	
      const A22_cols = next_block_low .. idx_range.high;

      // subsets are given as intersections of ranges, done neatly by Chapel

      const my_A1x_rows        = my_rows [ A11_rows ];
      const my_Ax1_cols        = my_cols ( A11_cols );

      const my_A2x_rows        = my_rows [ next_block_low .. ];
      const my_Ax2_cols        = my_cols [ next_block_low .. ];
      const I_compute_L21_rows = my_L21_rows_to_compute [ next_block_low .. ];

      yield ( A11_cols, A22_cols,
	      my_A1x_rows, my_Ax1_cols,
	      my_A2x_rows, my_Ax2_cols, I_compute_L21_rows );
    }
  }
}
