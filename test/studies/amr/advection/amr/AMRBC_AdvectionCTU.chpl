use AMRBC_def;

class ZeroInflowBC: AMRBC {
  
  def apply(q: LevelArray, t: real) {
    apply_Homogeneous(q: LevelArray);
  }
  
  def apply_Homogeneous(q: LevelArray) {

    var level_boundary = hierarchy.boundary(q.level);

    for grid in level_boundary.grids {
/*       writeln(grid); */
/*       writeln(level_boundary.domain_sets(grid).domains); */
      for ghost_domain in level_boundary.domain_sets(grid) do
	q(grid, ghost_domain) = 0.0;
    }

/*     for grid in hierarchy.boundary(q.level).grids { */
/*       for loc in ghost_locations do */
/*         q(grid,grid.ghost_domain_set(loc)) = 0.0; */
/*     } */

  }
  
}