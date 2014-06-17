use AMRBC_def;

class ZeroInflowBC: AMRBC {
  
  proc apply(level_idx: int, q: LevelVariable, t: real) {
    apply_Homogeneous(level_idx, q);
  }
  
  proc apply_Homogeneous(level_idx: int, q: LevelVariable) 
  {
    
    //---- Safety check ----
    assert(hierarchy.levels(level_idx) == q.level);


    //---- Assign zeros to ghost cells on the physical boundary ----
    for (grid,multidomain) in hierarchy.physical_boundaries(level_idx) {
      for ghost_domain in multidomain do
	      q(grid, ghost_domain) = 0.0;
    }
  }
  
}