module fully_blocked_partition_iterators {

  use cholesky_execution_config_consts;

  // ===================================================================
  // Iterator to create the 2 x 2 block partitionings of the sequence of
  // reduced matrices that occur during a triangular factorization.
  // ===================================================================

  iter symmetric_strided_2_by_2_block_partition ( idx_range, block_size ) {

    // -------------------------------------------------------------------
    // Deliver as ranges the sequence of reduced matrices that arise in a
    // triangular factorization, partitioned into 2 by 2 blocks.
    // The output ranges are (from the diagonal downward):
    //    the rows (or columns) in the top (diagonal) block 
    //                          of the reduced matrix
    //    the rows (or columns) in the remainder of the reduced matrix
    // -------------------------------------------------------------------
    // The block size to be used is delivered via a global constant
    // rather than as an argument to allow this code to emulate a code
    // in which the block size were obtained from a blocking distribution.
    // -------------------------------------------------------------------

    // I would have liked to express this iterator using Chapel's "count"
    // operator, but that failed to handle the edge case where less than
    // a full block remained.

    var block_low = idx_range.low;
    for count in 0..idx_range.size-1 by block_size do {
      var next_block_low = block_low + block_size*idx_range.stride;
      var top_block = 
	idx_range ( block_low .. 
		    min ( next_block_low - idx_range.stride, 
			  idx_range.high ) ),
	  bottom_block = idx_range (next_block_low  .. );
      block_low = next_block_low;

      yield ( top_block, bottom_block );
    }
  }


  // ====================================
  // Iterator to Block Partition a Vector
  // (or the rows or columns of a Matrix)
  // ====================================

  iter strided_vector_block_partition ( idx_range, block_size  ) {

    // -------------------------------------------------------------------
    // Deliver as ranges the block partitioning of a vector.
    // The block size to be used is delivered via a global constant
    // rather than as an argument to allow this code to emulate a code
    // in which the block size were obtained from a blocking distribution.
    // -------------------------------------------------------------------

    // I would have liked to express this iterator using Chapel's "count"
    // operator, but that failed to handle the edge case where less than
    // a full block remained.

    var block_low = idx_range.low;
    for count in 0..idx_range.size-1 by block_size do {
      var next_block_low = block_low + block_size*idx_range.stride;
      yield ( idx_range ( block_low .. 
			  min ( next_block_low - idx_range.stride, 
				idx_range.high ) ) );
      block_low = next_block_low;
    }
  }
}