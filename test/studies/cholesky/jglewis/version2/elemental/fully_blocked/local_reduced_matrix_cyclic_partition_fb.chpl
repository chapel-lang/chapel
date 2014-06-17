module local_reduced_matrix_cyclic_partition_fb {

  use cholesky_execution_config_consts;

  iter local_reduced_matrix_cyclic_partition_fb ( idx_range ) {

    // ----------------------------------------------------------------------
    // Deliver as ranges the sequence of reduced matrices that arise in a
    // triangular factorization, partitioned into 2 by 2 blocks.  It is
    // required that the row and column domains of the matrix are identical.
    // The seven output ranges are
    //    1. the rows (or columns) in the top (diagonal) block 
    //       of the reduced matrix
    //    2. the rows (or columns) in the remainder of the reduced matrix
    // ----------------------------------------------------------------------
    // The block size to be used is delivered via a global constant
    // rather than as an argument to allow this code to emulate a code
    // in which the block size were obtained from a blocking distribution.
    // ----------------------------------------------------------------------
    
    for block_low in idx_range by factor_block_size do {

      var next_block_low = block_low + factor_block_size;

      const A11_cols = block_low .. 
	               min ( next_block_low - 1, idx_range.high ),
	    A11_rows = A11_cols;
	
      const A22_cols = next_block_low .. idx_range.high;

      yield ( A11_cols, A22_cols );
    }
  }
}
