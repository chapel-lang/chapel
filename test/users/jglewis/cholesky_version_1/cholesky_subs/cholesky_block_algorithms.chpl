module cholesky_block_algorithms {

  // =========================================================================
   // The Cholesky factorization  A = L L^^T is defined by the following sets of
  // scalar equations derived by expanding the matrix-matrix product on the
  // right side of the matrix equation:
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
  // The outer product Cholesky factorization computes one block column of L at
  // each step. During each step the remaining columns of A are modified by a 
  // low rank outer product modication -- the reduce operations are accumulated 
  // one block step at a time for each entry in the yet unfactored part of the
  // matrix.  The computed entries of L will not otherwise need to be 
  // referenced again in the factorization.  
  // =========================================================================
    
  const empty_range = 1..0;

  proc block_outer_product_cholesky ( A : [], block_size : int )

    where ( A.domain.rank == 2 ) 

  {
    assert ( A.domain.dim (1) == A.domain.dim (2) && block_size > 0 );

    const col_indices = A.domain.dim (1);  // indices of either row or column
    var   pos_def : bool;

    writeln ( "Block Size: ", block_size );

    // compute L from A

    for (all_cols, active_cols, later_cols) in 
      iterated_block_column_partition ( col_indices, block_size ) do {

	// compute the Cholesky factor of the active diagonal block

	pos_def = scalar_outer_product_cholesky 
	                        ( A (active_cols, active_cols) );

	if pos_def && later_cols.size > 0 then {

	  // compute the remainder of the active block column of L by a
	  // block triangular solve realizing the equation
	  //      L (later_cols, active_cols) = 
	  //                              L (later_cols, active_cols) *
	  //                              L (active_cols, active_cols) ** (-T)
	  
	  transposed_block_triangular_solve ( A (active_cols, active_cols), 
		         		      A (later_cols, active_cols) );

	// make rank block_size (outerproduct) modification to the remaining 
	// block rows and columns of  A, which become the Schur complement

	  symmetric_block_schur_complement (  A (later_cols, later_cols),  
					      A (later_cols, active_cols),
					      block_size );

	}
	else

	  // error return if matrix is not positive definite

	  if !pos_def then return false;
    }

    // return success 

    return true;
  }

  // ======================
  // Block Triangular Solve
  // ======================

  proc transposed_block_triangular_solve ( L_diag    : [],
					  L_offdiag : [] ) {
    
    // ------------------------------------------------------
    // Solve the block equation
    //      L_offdiag = A_offdiag * L_diag^{-T}
    //           or
    //      L_offdiag^T = L_diag^{-1} A_offdiag^T
    // by triangular solve. 
    // This code is specialized to a factorization case where
    // L and A are submatrices of a common larger matrix.
    // ------------------------------------------------------

    const active_cols = L_diag.domain.dim(1);

    for (i,j) in L_offdiag.domain do {
      L_offdiag (i,j) -= 
	+reduce [k in active_cols (.. j-1)] L_offdiag (i,k) * L_diag (j,k);
      L_offdiag (i,j) = L_offdiag (i,j) / L_diag (j,j);
      }

  }
      
  // ==========================================
  // Symmetric Block Outer Product_Modification
  // ==========================================

  proc symmetric_block_schur_complement ( A : [] , L : [], block_size )

    where ( A.domain.rank == 2 && L.domain.rank == 2) 

    {
      for ( A_top_and_bottom_rows, A_top_rows, A_bottom_rows ) in 
	iterated_block_column_partition (L.domain.dim (1), block_size) do {

	// should be forall once iterator is made parallel

	symmetric_diagonal_low_rank_modification 
	             ( L (A_top_rows, ..), 
		       A (A_top_rows, A_top_rows) );

	if A_bottom_rows.size > 0 then
	  symmetric_offdiagonal_low_rank_modification 
	          ( L (A_top_and_bottom_rows, ..), 
		    A (A_bottom_rows, A_top_rows) );
      }
    }


  // ======================================================================
  // Symmetric Block Outer Product Modification for a single diagonal block
  // ======================================================================

  proc symmetric_diagonal_low_rank_modification ( L : [], A : [] ) {

    // -----------------------------------------------------------
    // form diagonal block A (K,K) = A (K,K) - L (K,J) L^T (J,K) 
    //                             = A (K,K) - L (K,J) L (K,J)^T
    // code is specialized to factorization case where L and A
    // are submatrices of a single larger matrix.
    // -----------------------------------------------------------

    assert ( A.domain.dim (1) == A.domain.dim (2) &&
	     A.domain.dim (1) == L.domain.dim (1) );

    const A_diag_rows   = A.domain.dim (1),
          L_active_cols = L.domain.dim (2);

    forall i in A_diag_rows do 
      forall j in A_diag_rows (..i) do
	A (i,j) -= + reduce [k in L_active_cols] L (i,k) * L (j,k);
    }
      

  // =========================================================================
  // Symmetric Block Outer Product Modification for a single offdiagonal block
  // =========================================================================

  proc symmetric_offdiagonal_low_rank_modification ( L : [], A : [] ) {

    // -------------------------------------------------------------
    // Form a single offdiagonal block 
    //       A (I,K) = A (I,K) - L (I,J) L^T (J,K) 
    //               = A (I,K) - L (I,J) L (J,K)^T
    // This code is specialized to the triangular factorization case 
    // where L and A are submatrices of a common larger matrix.
    // -------------------------------------------------------------

    const L_active_cols  = L.domain.dim (2);

    forall (i,j) in A.domain do 
	A (i,j) -= + reduce [k in L_active_cols] L (i,k) * L (j,k);
    }
      

  // =============================================
  // Iterator to Block Partition the Block Columns 
  // of a Block Lower Triangular Matrix
  // =============================================

  iter iterated_block_column_partition ( idx_range, block_size ) {

    // -----------------------------------------------------
    // Deliver as ranges the block partitioning of the block 
    // columns of a block triangular submatrix.  The output
    // ranges are (from the diagonal downward):
    //    the rows in the entire block column
    //    the rows in the top (diagonal) block
    //    the rows in the off-diagonal block
    // -----------------------------------------------------
    
    var n_block_steps = ( idx_range.size + block_size - 1 ) / block_size;
    var block_low      = idx_range.low;
    var next_block_low = block_low + block_size;

    // general case

    for block_step in 1 .. n_block_steps - 1 do {
      yield ( block_low      .. idx_range.high, 
	      block_low      .. #block_size, 
	      next_block_low .. idx_range.high );
      block_low       = next_block_low;
      next_block_low += block_size;
    }

    // final block is special

    yield  ( block_low .. idx_range.high, 
	     block_low .. idx_range.high, 
	     empty_range );

  }

  // ====================================
  // Iterator to Block Partition a Vector
  // (or the rows or columns of a Matrix)
  // ====================================

  iter block_partition ( idx_range, block_size )
  {
    var n_block_steps = ( idx_range.size  + block_size - 1 ) / block_size;

    var block_low = idx_range.low;

    // general case

    for block_step in 1 .. n_block_steps - 1 do {
      yield block_low .. #block_size;
      block_low += block_size;
    }

    // final block is special

    yield block_low .. idx_range.high;
  }
	

      
  // =========================================================================
  // The inner product Cholesky factorization also computes one column of L at
  // each step, in a manner reflecting the reduction form of the defining
  // equations.  During each step the previous columns of L are read to
  // implement the separate reduction operations for each entry of the current
  // column.  For efficiency, the reduction operations are combined into a 
  // matrix-vector product form.
  // =========================================================================
    
  proc block_inner_product_cholesky ( A : [] ) 

    where ( A.domain.rank == 2 ) 
    {
    assert ( A.domain.dim (1) == A.domain.dim (2) );

    const col_indices = A.domain.dim (1);  // indices of either row or column

    // compute L from A

    for j in col_indices do {

      // modify the jth column of A with the combined effects of the
      // elimination steps on all previous columns

      for k in col_indices (..j-1) do
	A (j.., j) -= A (j, k) * A (j.., k);

      if A (j, j) > 0.0 then {

	// compute the jth column of L

	A (j, j)      = sqrt ( A (j, j) );
	A (j+1.., j ) = A (j+1.., j) / A (j, j);
      }
      else 
	if A (j, j ) < 0.0 then
	    halt ( "Matrix is indefinite");

    }

  }


  // =========================================================================
  // The bordering Cholesky factorization computes one row of L at each step.
  // The leading entries in each row are computed first, in an operation that
  // can be viewed as a triangular system solve, using the leading rows of L 
  // as the coefficient matrix and the leading entries of the active row as 
  // the right-hand side vector.  Because solves are nasty to parallelize, 
  // this ordering of operations is rarely used.
  // =========================================================================
    
  proc block_bordering_cholesky ( A : [] )  

    where ( A.domain.rank == 2 ) {

    assert ( A.domain.dim (1) == A.domain.dim (2) );

    const row_indices = A.domain.dim (1);  // indices of either row or column

    // compute L from A

    for i in row_indices do {

      // compute the ith row of L.  incorporating the effects of the elimination
      // steps on the previous rows is identical to performing a forward solve
      // with a unit lower triangular matrix

      for k in row_indices (..i-1) do {
	A (i, k) -= + reduce [j in row_indices (..k-1)] A (i,j) * A (k,j) ;
	A (i, k) = A (i, k) / A (k, k);
      }

      // compute the ith diagonal entry of L

      A (i, i) -= + reduce [k in row_indices (..i-1)] A (i, k)**2;

      if A (i, i) > 0.0 then
	  A (i, i) = sqrt ( A (i, i) );
      else 
	if A (i, i ) < 0.0 then
	    halt ( "Matrix is indefinite");

    }

  }

}
	