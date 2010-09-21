use AMRBC_def;

class ZeroInflowBC: AMRBC {
  
  def applyBC(q: LevelArray, t: real) {
    applyBC_Homogeneous(q: LevelArray);
  }
  
  def applyBC_Homogeneous(q: LevelArray) {
    for grid in hierarchy.boundary(q.level).grids {
      for loc in ghost_locations do
        q(grid,grid.ghost_domain_set(loc)) = 0.0;
    }
  }
  
}