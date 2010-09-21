use LevelBC_def;


//===> ZeroFluxDiffusionLevelBC derived class ===>
//===============================================>
class ZeroFluxDiffusionLevelBC: LevelBC {
  
  def applyBoundaryCondition(q: LevelArray, t: real) {
    applyBoundaryConditionHomogeneous(q);
  }

  
  def applyBoundaryConditionHomogeneous(q: LevelArray) {
    
    for grid in level.grids {
    
      for loc in ghost_locations {
	      var shift: dimension*int;

	      for d in dimensions {
	        if loc(d) == loc1d.low then shift(d) = 2;
	        else if loc(d) == loc1d.high then shift(d) = -2;
	      }

	      forall cell in grid.ghost_domain_set(loc) do
	        q(grid).value(cell) = q(grid).value(cell+shift);

      } // end for loc in ghost_locations

    } // end for grid in level.grids
  }

}
//<=== ZeroFluxDiffusionLevelBC derived class <===
//<===============================================