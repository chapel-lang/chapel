use grid_base_defs;
use grid_solution_defs;



//|""""""""""""""""""""\
//|===> GridBC class ===>
//|____________________/
class GridBC {

  const grid: Grid;

  //==== Dummy routines to be provided in derived classes ====
  def fillGhostCells(q: GridArray, t: real){}
  def fillGhostCellsHomogeneous(q: GridArray){}
  
}
// /""""""""""""""""""""|
//<=== GridBC Class <===|
// \____________________|







//===> PeriodicGridBC class ===>
//=============================>
class PeriodicGridBC: GridBC {


  //===> ghostFill method ===>
  //=========================>
  def fillGhostCells(q: GridArray, t: real) {
    //==== Periodic BCs are homogeneous ====
    fillGhostCellsHomogeneous(q);
  }
  //<=== ghostFill method <===
  //<=========================



  //===> homogeneousGhostFill method ===>
  //====================================>
  def fillGhostCellsHomogeneous(q: GridArray) {

    for loc in ghost_locations {
      var shift: dimension*int;

      for d in dimensions {
	if loc(d) == loc1d.low  then 
	  shift(d) = 2*grid.n_cells(d);
	else if loc(d) == loc1d.high then 
	  shift(d) = -2*grid.n_cells(d);
      }

      forall cell in grid.ghost_cells(loc) {
	q.value(cell) = q.value(cell+shift);
      }

    }

  }
  //<=== homogeneousGhostFill method <===
  //<====================================


}
//<=============================
//<=== PeriodicGridBC class <===