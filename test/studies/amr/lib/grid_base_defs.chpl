//===> Description ===>
//
// Basic class and method definitions for a rectangular grid of
// generic dimension (though it must be a compile-time constant.)
//
// This version is based on an underlying mesh of width dx/2, which
// contains all cell centers, interface centers, and vertices.  As
// a result, the domin of cell centers is strided by 2, corresponding
// to jumps of dx.
//
//<=== Description <===


config param dimension = 2;
const dimensions = [1..dimension];


//===> RectangularGrid class ===>
//==============================>
class RectangularGrid {
  
  var x_low, x_high:          dimension*real,
      i_low, i_high:          dimension*int,
      n_cells, n_ghost_cells: dimension*int;

  var dx: dimension*real;
          
  var ext_cells:      domain(dimension, stridable=true),
      cells: subdomain(ext_cells);
  
  var low_ghost_cells:  [dimensions] subdomain(ext_cells),
      high_ghost_cells: [dimensions] subdomain(ext_cells);


  //===> initialize method ===>
  def initialize() {

    sanityChecks();

    setDerivedFields();
  }
  //<=== initialize method <===



  //===> sanityChecks method ===>
  def sanityChecks() {
    var d_string: string;
    for d in dimensions do {
      d_string = format("%i", d);

      assert(x_low(d) < x_high(d),
	     "error: RectangularGrid: x_low(" + d_string + ") must be strictly less than x_high(" + d_string + ").");

      assert(n_cells(d) > 0,
             "error: RectangularGrid: n_cells(" + d_string + ") must be positive.");

      assert(n_ghost_cells(d) > 0,
	     "error: RectangularGrid: n_ghost_cells(" + d_string + ") must be positive.");
    }
  }
  //<=== sanityChecks method <===



  //===> setDerivedFields method ===>
  //--------------------------------------------------------------
  // After x_low, x_high, n_cells, and n_ghost_cells have
  // been provided, calculate:
  //     dx
  //     i_high
  //     cells
  //     ext_cells
  //     low_ghost_cells
  //     high_ghost_cells
  //--------------------------------------------------------------
  def setDerivedFields() {
    //==== dx ====
    dx = (x_high - x_low) / n_cells;


    //==== i_high ====
    [d in dimensions] i_high(d) = i_low(d) + 2*n_cells(d);


    //==== Physical cells ====
    var range_tuple: dimension*range(stridable = true);
    [d in dimensions] 
      range_tuple(d) = i_low(d)+1 .. i_low(d)+2*n_cells(d)-1 by 2;
    cells = [(...range_tuple)];


    //==== Extended cells (includes ghost cells) ====
    var size: dimension*int;
    [d in dimensions] size(d) = 2*n_ghost_cells(d);
    ext_cells = cells.expand(size);
         

    //===> Ghost cells ===>
    //------------------------------------------------------
    // Note that all ghost cell domains contain the corners.
    //------------------------------------------------------
    for d in dimensions do {
      [d_temp in dimensions] size(d_temp) = 0;

      size(d) = -(2*n_ghost_cells(d) - 1);
      low_ghost_cells(d) = ext_cells.interior(size);

      size(d) = 2*n_ghost_cells(d) - 1;
      high_ghost_cells(d) = ext_cells.interior(size);
    }   
    //<=== Ghost cells <===     

  }
  //<=== setDerivedFields method <===
  
 
}
//<=== RectangularGrid class <===
//<==============================




//===> xValue method ===>
//======================>
//---------------------------------------
// Converts indices to coordinate values.
//----------------------------------------
def RectangularGrid.xValue (point_index: dimension*int) {

  var coord: dimension*real;

  if dimension == 1 then {
    coord(1) = x_low(1) + (point_index(1) - i_low(1)) * dx(1)/2.0;
  }
  else {
    forall d in dimensions do
    coord(d) = x_low(d) + (point_index(d) - i_low(d)) * dx(d)/2.0;
  }

  return coord;
}
//<======================
//<=== xValue method <===






//=======================================================>
//==================== GRID SOLUTIONS ===================>
//=======================================================>


//===> GridSolution class ===>
//===========================>
class GridSolution {
  var grid:  RectangularGrid;
  var value: [grid.ext_cells] real;
  var time:  real;
}
//<===========================
//<=== GridSolution class ====




//===> copyGridSolution method ===>
//==================================>
def RectangularGrid.copyGridSolution(q: GridSolution) {
  
  //==== Make sure q lives on this grid ====
  assert(q.grid == this);

  
  //==== Copy q ====
  var q_copy = new GridSolution(grid  = this,
			        value = q.value,
			        time  = q.time);

}
//<=== copyGridSolution method <===
//<==================================




//===> initializeGridSolution method ===>
//======================================>
//---------------------------------------------------------------
// Provided an analytical function, evaluates it on the grid and
// returns a GridSolution.  As support for first-class functions
// develops, the input argument will become explicitly typed.
//---------------------------------------------------------------
def RectangularGrid.initializeGridSolution(initial_condition) {
  var q = new GridSolution(this);

  if dimension==1 then
    forall cell in cells {
      q.value(cell) = initial_condition(xValue(tuple(cell)));
    }
  else
    forall cell in cells {
      q.value(cell) = initial_condition(xValue(cell));
    }

  return q;
}
//<======================================
//<=== initializeGridSolution method <===


//<=======================================================
//<=================== GRID SOLUTIONS ====================
//<=======================================================






//============================================================>
//==================== BOUNDARY CONDITIONS ===================>
//============================================================>


//===> GridBC class ===>
//=====================>
class GridBC {

  var grid: RectangularGrid;

  //==== Dummy routines; to be provided in derived classes ====
  def ghostFill(q: GridSolution){}
  def homogeneousGhostFill(q: GridSolution){}
  
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
  def ghostFill(q: GridSolution) {
    //==== Periodic BCs are homogeneous ====
    homogeneousGhostFill(q);
  }
  //<=========================
  //<=== ghostFill method <===



  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: GridSolution) {

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




//===> ZeroOrderExtrapolationGridBC class ===>
//===========================================>
class ZeroOrderExtrapolationGridBC: GridBC {


  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: GridSolution) {
    //==== Constant Dirichlet BCs are homogeneous ====
    homogeneousGhostFill(q);
  }
  //<========================
  //<=== ghostFill method <===
  


  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: GridSolution) {

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
//<======================================
//<=== ConstantDirichletGridBC class <===









//===> TrueSolutionDirichletGridBC class ===>
//==========================================>
//----------------------------------------------------------------
// Provides Dirichlet boundary conditions corresponding to method
// qTrue.  Derive this class to define qTrue.
//----------------------------------------------------------------
class TrueSolutionDirichletGridBC: GridBC {

  var true_solution: TrueSolution;

  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: GridSolution) {
    
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
  //-------------------------------------------------------------------
  // For homogeneous Dirichlet BCs.  Each ghost cell in the layer
  // nearest the boundary is filled by reflecting the nearest interior
  // value about zero.  This corresponds to linear extrapolation
  // using that interior value, and a value of zero on the boundary.
  //-------------------------------------------------------------------
  def homogeneousGhostFill(q: GridSolution) {

    for d in dimensions {

      //==== For shifting cell indices ====
      var cell_shift: dimension*int;
      cell_shift(d) = 2;

      //==== Low ghost cells ====
      forall cell_pretuple in grid.low_ghost_cells(d) {
        var cell = tuplify(cell_pretuple);
        if cell(d) == grid.i_low(d)-1 then
        q.value(cell) = -q.value(cell + cell_shift);
      }

      //==== High ghost cells ====
      forall cell_pretuple in grid.high_ghost_cells(d) {
        var cell = tuplify(cell_pretuple);
        if cell(d) == grid.i_high(d)+1 then
        q.value(cell) = -q.value(cell - cell_shift);
      }

    }

  }
  //<====================================
  //<=== homogeneousGhostFill method <===

}
//<==========================================
//<=== TrueSolutionDirichletGridBC class <===



// //===> TrueSolutionNeumannGridBC class ===>
// //========================================>
// class TrueSolutionNeumannGridBC: GridBC {
// 
//   var true_solution: TrueSolution;
// 
//   //===> ghostFill method ===>
//   //=========================>
//   def ghostFill(q: GridSolution) {
//     
//     for d in dimensions {
// 
//     }
// 
//   }
//   //<=========================
//   //<=== ghostFill method <===
// 
// 
//   //===> homogeneousGhostFill method ===>
//   //====================================>
//   def homogeneousGhostFill(q: GridSolution) {
// 
//     for d in dimensions do {
//       //==== Low ghost cells ====
//       forall cell in grid.low_ghost_cells(d) {
//         var target_cell = cell;
//         target_cell(d)  = grid.cells.dim(d).low;
//         q.value(cell)   = q.value(target_cell);
//       }
// 
//       //==== High ghost cells ====
//       forall cell in grid.high_ghost_cells(d) {
//         var target_cell = cell;
//         target_cell(d)  = grid.cells.dim(d).high;
//         q.value(cell)   = q.value(target_cell);
//       }
//     }
// 
//   }
//   //<====================================
//   //<=== homogeneousGhostFill method <===
// }
// //<========================================
// //<=== TrueSolutionNeumannGridBC class <===


//<============================================================
//<=================== BOUNDARY CONDITIONS ====================
//<============================================================






//=======================================================>
//==================== OUTPUT METHODS ===================>
//=======================================================>


//===> writeTimeFile function =====>
//=================================>
//-------------------------------------------------------------
// Global routine; meant to be called by output method of any
// spatial object.  The time file is really simple, and this
// just formats a few parameters.
//-------------------------------------------------------------
def writeTimeFile(time:    real,
                  meqn:    int,
                  ngrids:  int,
                  naux:    int,
                  outfile: file
                 ) {


  //==== Formatting parameters ====
  var efmt:  string = "%26.16E",
      ifmt:  string = "%5i";

  //==== Write time file ====
  outfile.writeln( format(efmt, time),      "    time");
  outfile.writeln( format(ifmt, meqn),      "                 meqn");
  outfile.writeln( format(ifmt, ngrids),    "                 ngrids");
  outfile.writeln( format(ifmt, naux),      "                 naux");
  outfile.writeln( format(ifmt, dimension), "                 ndim");
  outfile.writeln("");
  outfile.writeln("");

}
//<=== writeTimeFile method <=====
//<===============================




//===> clawOutput method ===>
//==========================>
//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridSolution, at the given 
// frame_number.
//-------------------------------------------------------------------
def RectangularGrid.clawOutput(q:             GridSolution,
                               frame_number: int
                              ){

  //==== Make sure q lives on this grid ====
  assert(q.grid == this);


  //==== Names of output files ====
  var frame_string:      string = format("%04i", frame_number),
      time_filename:     string = "_output/fort.t" + frame_string,
      solution_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var outfile = new file(time_filename, FileAccessMode.write);
  outfile.open();
  writeTimeFile(q.time, 1, 1, 0, outfile);
  outfile.close();
  delete outfile;
  
  
  //==== Solution file ====
  outfile = new file(solution_filename, FileAccessMode.write);
  outfile.open();
  writeSolution(q, 1, 1, outfile);
  outfile.close();
  delete outfile;

}
//<=== clawOutput method <===
//<==========================




//===> writeSolution method ====>
//==============================>
//---------------------------------------------------------
// Writes the data for a GridSolution living on this grid.
//---------------------------------------------------------
def RectangularGrid.writeSolution(q:           GridSolution, 
                                  grid_number: int, 
                                  AMR_level:   int,
                                  outfile:     file
                                 ){

  //==== Formatting parameters ====
  var efmt:  string = "%26.16E",
      ifmt:  string = "%5i",
      sfmt:  string = "%20s",
      linelabel: string;

                                    
  //==== Header ====
  outfile.writeln( format(ifmt, grid_number), "                 grid_number");
  outfile.writeln( format(ifmt, AMR_level),   "                 AMR_level");


  //==== Write n_cells ====
  for d in dimensions do {
    select d {
      when 1 do linelabel = "                 mx";
      when 2 do linelabel = "                 my";
      when 3 do linelabel = "                 mz";
      otherwise linelabel = "                 mx(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(ifmt, n_cells(d)),  linelabel);
  }


  //==== Write x_low ====
  for d in dimensions do {
    select d {
      when 1 do linelabel = "    xlow";
      when 2 do linelabel = "    ylow";
      when 3 do linelabel = "    zlow";
      otherwise linelabel = "    xlow(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(efmt, x_low(d)),  linelabel);
  }


  //==== Write dx ====
  for d in dimensions do {
    select d {
      when 1 do linelabel = "    dx";
      when 2 do linelabel = "    dy";
      when 3 do linelabel = "    dz";
      otherwise linelabel = "    dx(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(efmt, dx(d)),  linelabel);
  }
  outfile.writeln("");


  //===> Write solution values ===>
  if dimension == 1 then {
    for cell in cells do
      outfile.writeln(format(efmt, q.value(cell)));
  }
  else {
    //------------------------------------------------------------
    //---- Transpose cells; iterating over the transpose
    //---- in row major order achieves column major order on the
    //---- original domain.
    //------------------------------------------------------------
    var range_tuple: dimension*range(stridable=true);
    [d in dimensions] range_tuple(d) = cells.dim(1 + dimension - d);

    var cells_transposed: domain(dimension, stridable=true);
    cells_transposed = [(...range_tuple)];

    var cell:       dimension*int;
    var n_newlines: int;
    for cell_transposed in cells_transposed do {

      //==== Write value ====
      [d in dimensions] cell(d) = cell_transposed(1 + dimension - d);
      outfile.writeln(format(efmt, q.value(cell)));

      //===> Newlines at the end of each dimension ===>
      //--------------------------------------------------------------
      // There are no newlines at the very end of output.  How that's
      // handled is up to the main output routine.
      //--------------------------------------------------------------
      n_newlines = 0;

      for d in dimensions do {
        if cell(d) == cells.dim(d).high then
          n_newlines += 1;
        else
          break;
      }

      if n_newlines == dimension then n_newlines = 0;

      for n in [1..n_newlines] do outfile.writeln("  ");
      //<=== Newlines at the end of each dimension <===
    }
  }
  //<=== Write solution values <===
  
}
//<=== writeSolution method <===
//<=============================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================








//==============================================>
//==================== OTHER ===================>
//==============================================>


//===> Fix for the 1D/tuple problem ===>
def tuplify(idx) {
  if isTuple(idx) then
    return idx;
  else
    return tuple(idx);
}
//<=== Fix for the 1D/tuple problem <===



//===> TrueSolution class ===>
//===========================>
class TrueSolution {

  def qTrue(x: dimension*real, t: real) {
    return 0.0;
  }

}
//<===========================
//<=== TrueSolution class <===


//<==============================================
//<=================== OTHER ====================
//<==============================================
