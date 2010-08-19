
use grid_base_defs;
use grid_solution_defs;



//===> GridBC class ===>
//=====================>
class GridBC {

  var grid: BaseGrid;
  var low_boundary_faces:  [dimensions] domain(dimension, stridable=true);
  var high_boundary_faces: [dimensions] domain(dimension, stridable=true);
  

  //===> setBoundaryFaces method ===>
  //================================>
  //------------------------------------------------------
  // Sets up domains for the low and high boundary faces.
  //------------------------------------------------------
  def setBoundaryFaces() {

    var range_tuple: dimension*range(stridable=true);

    for d in dimensions {

      //==== All components except for d will equal grid.cells ====
      for d_range in dimensions do
        range_tuple(d_range) = grid.cells.dim(d_range);

      //==== low boundary faces ====
      range_tuple(d)        = grid.i_low(d) .. grid.i_low(d) by 2;
      low_boundary_faces(d) = [(...range_tuple)];


      //==== high_boundary_faces ====
      range_tuple(d)         = grid.i_high(d) .. grid.i_high(d) by 2;
      high_boundary_faces(d) = [(...range_tuple)];

    }
  }
  //<=== setBoundaryFaces method <===
  //<================================


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