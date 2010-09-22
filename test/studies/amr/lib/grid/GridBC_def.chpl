use Grid_def;
use GridSolution_def;



//|""""""""""""""""""""\
//|===> GridBC class ===>
//|____________________/
class GridBC {

  const grid: Grid;

  //==== Dummy routines to be provided in derived classes ====
  def applyBoundaryCondition(q: GridArray, t: real) {}
  def applyBoundaryConditionHomogeneous(q: GridArray) {}
  
}
// /""""""""""""""""""""|
//<=== GridBC Class <===|
// \____________________|







//===> PeriodicGridBC class ===>
//=============================>
class PeriodicGridBC: GridBC {


  //===> ghostFill method ===>
  //=========================>
  def applyBoundaryCondition(q: GridArray, t: real) {
    //==== Periodic BCs are homogeneous ====
    applyBoundaryConditionHomogeneous(q);
  }
  //<=== ghostFill method <===
  //<=========================



  //===> homogeneousGhostFill method ===>
  //====================================>
  def applyBoundaryConditionHomogeneous(q: GridArray) {

    for ghost_domain in grid.ghost_domain_set {
      var loc = grid.locateDomain(ghost_domain);
      var shift: dimension*int;

      for d in dimensions {
	if loc(d) == loc1d.below then
	  shift(d) = 2*grid.n_cells(d);
	else if loc(d) == loc1d.above then
	  shift(d) = -2*grid.n_cells(d);
      }

      forall cell in ghost_domain do
	q.value(cell) = q.value(cell+shift);

    }

/*     for loc in ghost_locations { */
/*       var shift: dimension*int; */

/*       for d in dimensions { */
/* 	      if loc(d) == loc1d.low  then  */
/* 	      shift(d) = 2*grid.n_cells(d); */
/* 	      else if loc(d) == loc1d.high then  */
/* 	      shift(d) = -2*grid.n_cells(d); */
/*       } */

/*       forall cell in grid.ghost_domain_set(loc) { */
/* 	      q.value(cell) = q.value(cell+shift); */
/*       } */

/*     } */

  }
  //<=== homogeneousGhostFill method <===
  //<====================================


}
//<=============================
//<=== PeriodicGridBC class <===