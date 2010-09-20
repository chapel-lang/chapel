use GridBC_def;



//===> ZeroInflowAdvectionGridBC class ===>
//========================================>
class ZeroInflowAdvectionGridBC: GridBC {
  
  
  def applyBoundaryCondition(q: GridArray, t: real) {
    //==== This type of BC is homogeneous ====
    applyBoundaryConditionHomogeneous(q);
  }
  
  
  def applyBoundaryConditionHomogeneous(q: GridArray) {

    for loc in ghost_locations {
      forall cell in grid.ghost_domain_set(loc) do
	      q.value(cell) = 0.0;
    }
    
  }
  
}
//<=== ZeroInflowAdvectionGridBC class <===
//<========================================