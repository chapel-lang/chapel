use grid_base_defs;
use grid_solution_defs;



//===> GridBC class ===>
//=====================>
class GridBC {

  var grid: BaseGrid;

  //==== Dummy routines; to be provided in derived classes ====
  def ghostFill(q: [grid.ext_cells] real, t: real){}
  def homogeneousGhostFill(q: [grid.ext_cells] real){}
  
}
//<=== GridBC Class <===
//<=====================




//===> PeriodicGridBC class ===>
//=============================>
class PeriodicGridBC: GridBC {


  var periodic_ghost_images: GridBoundaryDomain;


  //===> initialize method ===>
  //============================>
  def initialize() {

    //===> Build periodic images of ghost cells ===>

    periodic_ghost_images = new GridBoundaryDomain(grid = grid);

    var shift: dimension*int;
    
    for d in dimensions do {
      [d_temp in dimensions] shift(d_temp) = 0;

      shift(d) = 2*grid.n_cells(d);
      periodic_ghost_images.low(d)  = grid.ghost_cells.low(d).translate(shift);

      shift(d) = -2*grid.n_cells(d);
      periodic_ghost_images.high(d) = grid.ghost_cells.high(d).translate(shift);
    }
    //<=== Build periodic images of ghost cells <===

  }
  //<=== initialize method <===
  //<==========================



  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: [grid.ext_cells] real, t: real) {
    //==== Periodic BCs are homogeneous ====
    homogeneousGhostFill(q);
  }
  //<=== ghostFill method <===
  //<=========================



  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: [grid.ext_cells] real) {

    //==== Copy values into ghost cells from their periodic images ====
    for d in dimensions do {
      q(grid.ghost_cells.low(d))  = q(periodic_ghost_images.low(d));
      q(grid.ghost_cells.high(d)) = q(periodic_ghost_images.high(d));
    }
  }
  //<=== homogeneousGhostFill method <===
  //<====================================


}
//<=============================
//<=== PeriodicGridBC class <===