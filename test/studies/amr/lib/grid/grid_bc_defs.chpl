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

    var shift: dimension*int;

    for g_loc in ghost_locations {
      for d in dimensions {
	if g_loc(d) == line_position.low then
	  shift(d) = n_cells(d);
      }

    }


    //==== Copy values into ghost cells from their periodic images ====
    for d in dimensions do {
      q.value(grid.ghost_cells.low(d))  = q.value(periodic_ghost_images.low(d));
      q.value(grid.ghost_cells.high(d)) = q.value(periodic_ghost_images.high(d));
    }
  }
  //<=== homogeneousGhostFill method <===
  //<====================================


}
//<=============================
//<=== PeriodicGridBC class <===