module blocked_elemental_schur_complement {

  // ==============================================
  // Symmetric Elemental Outer Product_Modification
  // ==============================================

  // elementary unblocked version for the moment.  Note that the
  // eventual block size for this operation does not need to be the
  // same as the block size for the outer algorithm partitioning.
  // Note that the same code is used for block cyclic distributions
  // and cyclic distributions.  The domains for the arrays are unstrided 
  // in the standard block cyclic case and strided in the cyclic case.

  use cholesky_execution_config_consts;
 
  proc blocked_elemental_schur_complement ( A : [] , L12 : [], L21 : [] )

    where ( A.domain.rank   == 2 && 
	    L12.domain.rank == 2 && 
	    L21.domain.rank == 2) {

    const schur_complement_rows = A.domain.dim (0);
    const schur_complement_cols = A.domain.dim (1);
    const Lx1_cols              = L21.domain.dim (1);
 
    //    const my_locale = Lx1_cols.locale;
    //    forall i in L12 do
    //      assert ( i.locale == my_locale);
    //    forall i in L21 do
    //      assert ( i.locale == my_locale);
    //    forall i in A do
    //      assert (i.locale == my_locale);

    // The low rank modification to Schur Complement must be computed
    // block column by block column because we only want to touch
    // the lower triangle of the symmetric block matrix. 

    // outer loops should be forall once we have parallel iterators

    for ( AII_row_indices, AIPI_row_indices ) in 
      symmetric_strided_2_by_2_block_partition
                 (schur_complement_rows, schur_complement_block_size) do {

      const AxI_cols = 
	schur_complement_cols (AII_row_indices.low .. AII_row_indices.high );

       // the diagonal block itself is symmetric, so we cannot use
       // a standard matrix-matrix product

      for (i, j, k) in { AII_row_indices, AxI_cols, Lx1_cols } do
	if j <= i then
	  A (i,j) -= + L21 (i,k) * L12 (j,k);
    
      for AJI_row_indices in 
	  strided_vector_block_partition ( AIPI_row_indices, 
					   schur_complement_block_size )  do
	for (i, j, k) in {AJI_row_indices, AxI_cols, Lx1_cols} do
	  A (i,j) -= + L21 (i,k) * L12 (j,k);
    }
  }
}
