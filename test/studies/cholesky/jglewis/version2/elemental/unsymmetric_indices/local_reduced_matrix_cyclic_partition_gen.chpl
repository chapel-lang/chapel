module local_reduced_matrix_cyclic_partition_gen {

  use cholesky_execution_config_consts;

  iter local_reduced_matrix_cyclic_partition_gen ( A_domain ) {

    // -------------------------------------------------------------------
    // Deliver as ranges the sequence of reduced matrices that arise in a
    // triangular factorization, partitioned into 2 by 2 blocks.  It is
    // required that the row and column domains of the matrix are identical.
    // The for output ranges are
    //    1. the rows in the top (diagonal) block of the reduced matrix
    //    2. the columns in the top (diagonal) block of the reduced matrix
    //    3. the rows in the remainder of the reduced matrix
    //    4. the columns in the remainder of the reduced matrix
    // -------------------------------------------------------------------
    // The block size to be used is delivered via an argument in this test
    // rather than as a  global constant because the block size used here
    // is independent of the distribution.  In a library implementation,
    // block size would probably be hard-coded in this routine or the top
    // level factorization routine.
    // -------------------------------------------------------------------
    
    for ( block_low_row, block_low_col ) in 
      zip( A_domain.dim (0) by block_size,  A_domain.dim (1) by block_size ) do {

      var next_block_low_row = block_low_row + block_size,
	  next_block_low_col = block_low_col + block_size;

      const A11_rows = block_low_row .. 
	               min ( next_block_low_row - 1, A_domain.high(0) ),
	    A11_cols = block_low_col .. 
	               min ( next_block_low_col - 1, A_domain.high(1) );
	
      const A22_rows = next_block_low_row .. A_domain.high (0),
	    A22_cols = next_block_low_col .. A_domain.high (1);

      yield ( A11_rows, A11_cols, A22_rows, A22_cols );
    }
  }
}
