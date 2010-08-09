
use grid_base_defs;
use grid_solution_defs;


//============================================================>
//==================== BOUNDARY CONDITIONS ===================>
//============================================================>


//===> GridBC class ===>
//=====================>
class GridBC {

  var grid: RectangularGrid;
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
  //<================================
  //<=== setBoundaryFaces method <===


  //==== Dummy routines; to be provided in derived classes ====
  def ghostFill(q: ScalarSingleTimeGridSolution){}
  def homogeneousGhostFill(q: ScalarSingleTimeGridSolution){}
  
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
  //<============================
  //<=== initialize method <===



  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: ScalarSingleTimeGridSolution) {
    //==== Periodic BCs are homogeneous ====
    homogeneousGhostFill(q);
  }
  //<=========================
  //<=== ghostFill method <===



  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: ScalarSingleTimeGridSolution) {

    //==== Make sure q lives on the same grid as the BC ====
    assert(q.grid == grid);

    //==== Copy values into ghost cells from their periodic images ====
    for d in dimensions do {
      q.value(grid.low_ghost_cells(d))  = q.value(low_ghost_periodic(d));
      q.value(grid.high_ghost_cells(d)) = q.value(high_ghost_periodic(d));
    }
  }
  //<====================================
  //<=== homogeneousGhostFill method <===


}
//<=============================
//<=== PeriodicGridBC class <===




//===> ZeroOrderExtrapGridBC class ===>
//=====================================>
class ZeroOrderExtrapGridBC: GridBC {

  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: ScalarSingleTimeGridSolution) {
    //==== This type of BC is homogeneous ====
    homogeneousGhostFill(q);
  }
  //<=========================
  //<=== ghostFill method <===
  


  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: ScalarSingleTimeGridSolution) {

    for d in dimensions do {

      var shift: dimension*int;
      shift(d) = 1;
      
      
      forall precell in low_ghost_cells(d) {
        var cell = tuplify(precell);
        var target_cell: dimension*int;
        for d_cell in dimensions do
          target_cell(d_cell) = max(cell(d_cell), grid.cells.dim(d_cell).low);
        q.value(cell) = q.value(target_cell);
      }
      
      
      //==== High ghost cells ====
      forall precell in low_ghost_cells(d) {
        var cell = tuplify(precell);
        var target_cell: dimension*int;
        for d_cell in dimensions do
          target_cell(d_cell) = min(cell(d_cell), grid.cells.dim(d_cell).high);
        q.value(cell) = q.value(target_cell);
      }
    }

  }
  //<====================================
  //<=== homogeneousGhostFill method <===



}
//<====================================
//<=== ZeroOrderExtrapGridBC class <===



//===> ZeroInflowGridBC class ===>
//===============================>
class ZeroInflowGridBC: GridBC {
  
  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: ScalarSingleTimeGridSolution) {
    //==== This type of BC is homogeneous ====
    homogeneousGhostFill(q);
  }
  //<=========================
  //<=== ghostFill method <===
  
  
  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: ScalarSingleTimeGridSolution) {
    
    for d in dimensions {
      forall cell in grid.low_ghost_cells(d) do
        q.value(cell) = 0.0;

      forall cell in grid.high_ghost_cells(d) do
        q.value(cell) = 0.0;
    }
  }
  //<====================================
  //<=== homogeneousGhostFill method <===
  
}
//<===============================
//<=== ZeroInflowGridBC class <===






//===> TrueDirichletGridBC class ===>
//==================================>
//----------------------------------------------------------------
// Provides Dirichlet boundary conditions corresponding to method
// qTrue.  Derive this class to define qTrue.
//----------------------------------------------------------------
class TrueDirichletGridBC: GridBC {

  var true_solution: TrueSolution;

  
  //===> initialize method ===>
  //==========================>
  def initialize() {
    setBoundaryFaces();
  }
  //<=== initialize method <===
  //<==========================


  //===> ghostFill method ===>
  //=========================>
  //---------------------------------------------------
  // Evaluates true_solution.qTrue on each ghost cell.
  //---------------------------------------------------
  def ghostFill(q: ScalarSingleTimeGridSolution) {
    
    for d in dimensions {

      forall precell in grid.low_ghost_cells(d) {
        var cell = tuplify(precell);
        q.value(cell) = true_solution.qTrue(grid.xValue(cell), q.time);
      }

      forall precell in grid.high_ghost_cells(d) {
        var cell = tuplify(precell);
        q.value(cell) = true_solution.qTrue(grid.xValue(cell), q.time);
      }

    }

  }
  //<=========================
  //<=== ghostFill method <===


  //===> homogeneousGhostFill method ===>
  //====================================>
  //------------------------------------------------------------
  // For homogeneous Dirichlet BCs.  At each boundary face, the 
  // nearest interior value is reflected about 0 to the nearest 
  // ghost cell value.
  //------------------------------------------------------------
  def homogeneousGhostFill(q: ScalarSingleTimeGridSolution) {

    for d in dimensions {

      //==== For shifting from a face to a cell ====
      var shift: dimension*int;
      shift(d) = 1;

      
      //==== Low boundary ====
      forall preface in low_boundary_faces(d) {
        var face = tuplify(preface);
        q.value(face - shift) = -q.value(face + shift);
      }


      //==== High boundary ====
      forall preface in high_boundary_faces(d) {
        var face = tuplify(preface);
        q.value(face + shift) = -q.value(face - shift);
      }
    }

  }
  //<====================================
  //<=== homogeneousGhostFill method <===

}
//<==================================
//<=== TrueDirichletGridBC class <===



//===> TrueNeumannGridBC class ===>
//================================>
class TrueNeumannGridBC: GridBC {

  var true_solution: TrueSolution;


  //===> initialize method ===>
  //==========================>
  def initialize() {
    setBoundaryFaces();
  }
  //<=========================
  //<=== intialize method <===



  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: ScalarSingleTimeGridSolution) {

    writeln("Start of ghostFill");
    
    for d in dimensions {

      //==== dx in this dimension ====
      var dx = grid.dx(d);

      
      //==== For shifting faces to cells ====
      var shift: dimension*int;
      shift(d) = 1;


      //==== Low boundary ====
      forall preface in low_boundary_faces(d) {
        var face            = tuplify(preface);
        var normal_flux     = -true_solution.fluxComponent(grid.xValue(face), q.time, d);
        q.value(face-shift) = true_solution.normalFluxToGhost(normal_flux, 
                                                              q.value(face+shift), 
                                                              dx);
      }


      //==== High boundary ====
      forall preface in high_boundary_faces(d) {
        var face            = tuplify(preface);
        var normal_flux     = true_solution.fluxComponent(grid.xValue(face), q.time, d);
        q.value(face+shift) = true_solution.normalFluxToGhost(normal_flux,
                                                              q.value(face-shift),
                                                              dx);
      }

    }

  }
  //<=========================
  //<=== ghostFill method <===



  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: ScalarSingleTimeGridSolution) {

    for d in dimensions do {
      //==== Low ghost cells ====
      forall cell in grid.low_ghost_cells(d) {
        var target_cell = cell;
        target_cell(d)  = grid.cells.dim(d).low;
        q.value(cell)   = q.value(target_cell);
      }

      //==== High ghost cells ====
      forall cell in grid.high_ghost_cells(d) {
        var target_cell = cell;
        target_cell(d)  = grid.cells.dim(d).high;
        q.value(cell)   = q.value(target_cell);
      }
    }

  }
  //<====================================
  //<=== homogeneousGhostFill method <===
}
//<================================
//<=== TrueNeumannGridBC class <===


//<============================================================
//<=================== BOUNDARY CONDITIONS ====================
//<============================================================