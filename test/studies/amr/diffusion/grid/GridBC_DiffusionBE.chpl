use GridBC_def;


//===> ZeroFluxDiffusionGridBC derived class ===>
//==============================================>
class ZeroFluxDiffusionBC: GridBC {
  
  //===> applyBoundaryCondition method ===>
  //==============================>
  proc apply(q: GridVariable, t: real) {
    apply_Homogeneous(q);
  }
  //<=== applyBoundaryCondition method <===
  //<==============================


  //===> applyBoundaryConditionHomogeneous method ===>
  //=========================================>
  proc apply_Homogeneous(q: GridVariable) {

    for ghost_domain in grid.ghost_multidomain {
      var loc = grid.relativeLocation(ghost_domain);
      var shift = -1*loc;

      forall cell in ghost_domain do
        q.value(cell) = q.value(cell+shift);

    }
    
/*     for loc in ghost_locations { */
/*       var shift: dimension*int; */

/*       for d in dimensions { */
/* 	if loc(d) == loc1d.low then */
/* 	  shift(d) = 2; */
/* 	else if loc(d) == loc1d.high then */
/* 	  shift(d) = -2; */
/*       } */

/*       forall precell in grid.ghost_multidomain(loc) { */
/* 	var cell = tuplify(precell); */
/* 	q.value(cell) = q.value(cell+shift); */
/*       } */

/*     } */

  }
  //<=== applyBoundaryConditionHomogeneous method <===
  //<=========================================

  
}
//<=== ZeroFluxDiffusionGridBC derived class <===
//<==============================================