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

  var low_ghost_periodic:  [dimensions] subdomain(grid.ext_cells);
  var high_ghost_periodic: [dimensions] subdomain(grid.ext_cells);


  //===> initialize method ===>
  //============================>
  def initialize() {

    //===> Build periodic images of ghost cells ===>
    var shift: dimension*int;
    
    for d in dimensions do {
      [d_temp in dimensions] shift(d_temp) = 0;

      shift(d) = 2*grid.n_cells(d);
      low_ghost_periodic(d)  = grid.low_ghost_cells(d).translate(shift);

      shift(d) = -2*grid.n_cells(d);
      high_ghost_periodic(d) = grid.high_ghost_cells(d).translate(shift);
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
      q(grid.low_ghost_cells(d))  = q(low_ghost_periodic(d));
      q(grid.high_ghost_cells(d)) = q(high_ghost_periodic(d));
    }
  }
  //<=== homogeneousGhostFill method <===
  //<====================================


}
//<=============================
//<=== PeriodicGridBC class <===