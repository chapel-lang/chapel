use LevelBC_def;

//===> ZeroInflowAdvectionLevelBC derived class ===>
//=================================================>
class ZeroInflowBC: LevelBC {


  def applyBoundaryCondition(q: LevelArray, t: real){
    applyBoundaryConditionHomogeneous(q);
  }


  def applyBoundaryConditionHomogeneous(q: LevelArray){

    for grid in level.grids {
      for loc in ghost_locations {
	      forall cell in grid.ghost_domain_set(loc) do
	      q(grid).value(cell) = 0.0;
      }
    }

  }
 
}
//<=== ZeroInflowAdvectionLevelBC derived class <===
//<=================================================