public use AMRBC_def;

class ZeroInflowBC: AMRBC {

  override proc apply(level_idx: int, q: unmanaged LevelVariable, t: real) {
    apply_Homogeneous(level_idx, q);
  }

  override proc apply_Homogeneous(level_idx: int, q: unmanaged LevelVariable)
  {

    //---- Safety check ----
    assert(hierarchy.levels(level_idx) == q.level);


    //---- Assign zeros to ghost cells on the physical boundary ----
    for (grid,multidomain) in hierarchy.physical_boundaries(level_idx) {
      for ghost_domain in multidomain! do
	      q(grid, ghost_domain) = 0.0;
    }
  }

}
