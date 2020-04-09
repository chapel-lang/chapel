module elemental_schur_complement {

  // ==============================================
  // Symmetric Elemental Outer Product_Modification
  // ==============================================

  // elementary unblocked version for the moment.  Note that the
  // eventual block size for this operation does not need to be the
  // same as the block size for the outer algorithm partitioning.
  // Note that the same code is used for block cyclic distributions
  // and cyclic distributions.  The domains for the arrays are unstrided 
  // in the standard block cyclic case and strided in the cyclic case.
 
  proc elemental_schur_complement ( A : [] , L12 : [], L21 : [] )

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

    for (i, j, k) in 
      {schur_complement_rows, schur_complement_cols, Lx1_cols} do
	if j <= i then
	  A (i,j) -= + L21 (i,k) * L12 (j,k);
    }
}
