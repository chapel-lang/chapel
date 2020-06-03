module desirable_syntax {

  // Proposal for syntax to allow reindexing a formal array argument,
  // preserving shape.

  // This is desirable to permit Fortran and C programmers to mimic their
  // existing styles, and to allow simpler code for array operands whose
  // formal index ranges are dissimilar across dimensions

  config const n = 4, row_base = -4, col_base = 17;

  proc main {

    var A : [row_base .. #n, col_base .. #n] 2*int;

    for i in A.domain do
      A (i) = i;

    print_A (A);

    ref A_reindexed = A.reindex(0..#n, 0..#n);

    print_A (A_reindexed);

    print_A_reindexed (A);

  }

  proc print_A (A) {
    for i in A.domain.dim(0) do
      writeln (i, A (i, ..) );
    writeln ();
  }

  proc print_A_reindexed (A : [0.., 0..]) {
    for i in A.domain.dim(0) do
      writeln (i, A (i, ..) );
  }
  
}
      