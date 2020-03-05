module matrix_matrix_multiply_schur_complement {

  // ========================================================================
  // Given a symmetric matrix (section) partitioned as  A11 | A12 A13 A14 ..
  //                                                    --- + --------------
  //                                                    A21 | A22 A23 A24 ..
  //                                                    A31 | A32 A33 A34 ..
  //                                                    A41 | A42 A43 A44 ..
  //                                                    ... | ... ... 4 ..

  // and the corresponding partitioning of the factor L,
  // compute  AIJ = AIJ - LI1 L1J = AIJ - LI1 LJ1^T for I, J > 1.
  //
  // These versions respect symmetry by modifying only blocks in the lower 
  // triangular portion of A and modifying only the lower triangle of 
  // diagonal blocks.  In the block column (1D) format, the subdivision into
  // block rows is not present.
  //
  // We assume here that the index ranges for the rows and columns of both  A 
  // and L are all equal.
  // ========================================================================
  
  // ==========================================
  // Symmetric Block Outer Product_Modification
  // for Matrix in Block Colum Form (1D).
  // ==========================================

  proc symmetric_block_schur_complement ( A : [] , L : [] )

    where ( A.domain.rank == 2 && L.domain.rank == 2) {

    const schur_complement_rows = (L.domain.dim (1));

    // The low rank modification to Schur Complement must be computed
    // block column by block column because we only want to touch
    // the lower triangle of the symmetric block matrix. 

    // should be forall once we have a parallel iterator

     for ( AI_diag_row_indices, AI_offdiag_row_indices ) in 
      symmetric_2_by_2_block_partition (schur_complement_rows) do {

       // the diagonal block itself is symmetric, so we cannot use
       // a standard matrix-matrix product

       symmetric_diagonal_low_rank_modification 
	  ( L (AI_diag_row_indices, ..), 
	    A (AI_diag_row_indices, AI_diag_row_indices) );

       // the remainder of a block column is a dense rectangular matrix,
       // for which we can use a general matrix-matrix product

       if AI_offdiag_row_indices.size > 0 then 
	 symmetric_offdiagonal_low_rank_modification 
	    ( L,  A (AI_offdiag_row_indices, AI_diag_row_indices) );
      }
    }


  // =============================================
  // Symmetric 2D Block Outer Product_Modification
  // =============================================

  proc symmetric_2D_block_schur_complement ( A : [] , L : [] )

    where ( A.domain.rank == 2 && L.domain.rank == 2) {


    const schur_complement_rows = (L.domain.dim (1));

    // The low rank modification to Schur Complement must be computed
    // block column by block column because we only want to touch
    // the lower triangle of the symmetric block matrix. 

    // should be forall once we have a parallel iterator

    for ( AII_rc_indices, AIPI_row_indices ) in 
      symmetric_2_by_2_block_partition (schur_complement_rows) do {

       // the diagonal block itself is symmetric, so we cannot use
       // a standard matrix-matrix product

	symmetric_diagonal_low_rank_modification 
	  ( L (AII_rc_indices, ..), 
	    A (AII_rc_indices, AII_rc_indices) );

	for AJI_row_indices in vector_block_partition ( AIPI_row_indices ) do
	  symmetric_offdiagonal_low_rank_modification 
	    ( L, A (AJI_row_indices, AII_rc_indices) );
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

    const AKK_rc_indices = A.domain.dim (1),
          LJ_col_indices = L.domain.dim (2);

    forall i in AKK_rc_indices do 
      forall j in AKK_rc_indices (..i) do
	A (i,j) -= + reduce [k in LJ_col_indices] L (i,k) * L (j,k);
  }
      

  // =========================================================================
  // Symmetric Block Outer Product Modification for a single offdiagonal block
  // =========================================================================

  proc symmetric_offdiagonal_low_rank_modification ( L : [], A : [] ) {

    // -------------------------------------------------------------
    // Form a single offdiagonal block 
    //       A (I,K) = A (I,K) - L (I,J) L^T (J,K) 
    //               = A (I,K) - L (I,J) L (K,J)^T
    // This code is specialized to the triangular factorization case 
    // where L and A are submatrices of a common larger matrix.
    // -------------------------------------------------------------

    const AIK_row_indices = A.domain.dim(1),
          AIK_col_indices = A.domain.dim(2),
          LJ_col_indices  = L.domain.dim (2);

    forall (i,j, k) in { AIK_row_indices, AIK_col_indices, LJ_col_indices } do 
      A (i,j) -= L (i,k) * L (j,k);
  }
}
