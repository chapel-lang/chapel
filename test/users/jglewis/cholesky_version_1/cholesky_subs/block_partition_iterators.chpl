module block_partition_iterators {

  use cholesky_execution_config_consts;

  // ===================================================================
  // Iterator to create the 2 x 2 block partitionings of the sequence of
  // reduced matrices that occur during a triangular factorization.
  // ===================================================================

  iter symmetric_reduced_matrix_2_by_2_block_partition ( idx_range ) {

    // -------------------------------------------------------------------
    // Deliver as ranges the sequence of reduced matrices that arise in a
    // triangular factorization, partitioned into 2 by 2 blocks.
    // The output ranges are (from the diagonal downward):
    //    the rows (or columns) in the entire reduced matrix
    //    the rows (or columns) in the top (diagonal) block 
    //                          of the reduced matrix
    //    the rows (or columns) in the remainder of the reduced matrix
    // -------------------------------------------------------------------
    // The block size to be used is delivered via a global constant
    // rather than as an argument to allow this code to emulate a code
    // in which the block size were obtained from a blocking distribution.
    // -------------------------------------------------------------------
    

    for block_low in idx_range by block_size do {
      var next_block_low = block_low + block_size;
      
      yield ( block_low      .. idx_range.high, 
	      block_low      .. min ( next_block_low - 1, idx_range.high ),
	      next_block_low .. idx_range.high );
    }
  }


  // ===================================================================
  // Iterator to create the sequence of 3 x 3 block partitionings of 
  // the original matrix that occur during a triangular factorization.
  // ===================================================================

  iter symmetric_matrix_3_by_3_block_partition ( idx_range ) {

    // -----------------------------------------------------
    // Deliver as ranges the block partitioning of the block 
    // columns of a block triangular submatrix.  The output
    // ranges are (from the diagonal downward):
    //    the rows and columns preceding the block column
    //    the rows in the entire block column
    //    the rows in the top (diagonal) block
    //    the rows in the off-diagonal block
    // -----------------------------------------------------
    
    //    var n_block_steps  = ( idx_range.size + block_size - 1 ) / block_size;
    //    var block_low      = idx_range.low;
    //    var next_block_low = block_low + block_size;
    //
    //    // general case
    //
    //    for block_step in 1 .. n_block_steps - 1 do {
    //      yield ( idx_range.low .. block_low-1,
    //	      block_low      .. idx_range.high, 
    //	      block_low      .. #block_size, 
    //	      next_block_low .. idx_range.high );
    //      block_low       = next_block_low;
    //      next_block_low += block_size;
    //    }
    //
    //    // final block is special
    //
    //    yield  ( idx_range.low .. block_low-1,
    //	     block_low .. idx_range.high, 
    //	     block_low .. idx_range.high, 
    //	     empty_range );

    
    for block_low in idx_range by block_size do {
      var next_block_low = block_low + block_size;
      
      yield ( idx_range.low .. block_low-1,
	      block_low      .. idx_range.high, 
	      block_low      .. min ( next_block_low - 1, idx_range.high ),
	      next_block_low .. idx_range.high );
    }
  }

  // ====================================
  // Iterator to Block Partition a Vector
  // (or the rows or columns of a Matrix)
  // ====================================

  iter vector_block_partition ( idx_range )
  {
    for block_low in idx_range by block_size do
      yield block_low .. min ( block_low + block_size - 1, idx_range.high );
  }

}