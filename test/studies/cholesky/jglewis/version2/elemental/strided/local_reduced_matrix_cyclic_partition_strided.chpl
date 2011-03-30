module local_reduced_matrix_cyclic_partition_strided {

  use cholesky_execution_config_consts;

  iter local_reduced_matrix_cyclic_partition_strided ( idx_range ) {

    // -------------------------------------------------------------------
    // Deliver as ranges the sequence of reduced matrices that arise in a
    // triangular factorization, partitioned into 2 by 2 blocks.  It is
    // required that the row and column domains of the matrix are identical.
    // The two output ranges are
    //    1. the rows (or columns) in the top (diagonal) block of the reduced 
    //       matrix
    //    2. the rows (or columns) in the remainder of the reduced matrix
    // -------------------------------------------------------------------
    // The block size to be used is delivered via an argument in this test
    // rather than as a  global constant because the block size used here
    // is independent of the distribution.  In a library implementation,
    // block size would probably be hard-coded in this routine or the top
    // level factorization routine.
    // -------------------------------------------------------------------
    
    var stride = idx_range.stride;

    for block_low in idx_range by block_size do {

      var next_block_low = block_low + block_size * stride;

      const A11_cols = block_low .. 
	               min ( next_block_low - 1, idx_range.high ) 
	               by stride,
	    A11_rows = A11_cols;
	
      const A22_cols = next_block_low .. idx_range.high by stride;

      yield ( A11_cols, A22_cols );
    }
  }
}