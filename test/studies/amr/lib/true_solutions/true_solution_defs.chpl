
use grid_base_defs;
use grid_bc_defs;

//===> TrueSolution class ===>
//===========================>
//----------------------------------------------------------------------
// Create a true solution to a PDE by deriving this class.  In general,
// it will contain a few parameters in addition to the qTrue method.
// (Hence, using a class is probably more suitable than a first-class
// functin.)
//----------------------------------------------------------------------
class TrueSolution {

  def qTrue(x: dimension*real, t: real) {
    return 0.0;
  }

  def fluxComponent(x: dimension*real, t: real, comp: int) {
    return 0.0;
  }

  def normalFluxToGhost(flux: real,
                        inner_value: real,
                        dx: real
                       ){
    return 0.0;
  }
}
//<===========================
//<=== TrueSolution class <===




//===> RectangularGrid.initializeSolution method ===>
//==================================================>
def RectangularGrid.initializeSolution(
  sol:             ScalarGridSolution,
  true_solution: TrueSolution,
  time:          real
){

  //==== Check that q lives on this grid ====
  assert(sol.grid == this);


  //==== Evaluate and store true_solution at input time ====
  write("Writing solution on grid...");
  forall precell in cells {
    var cell = tuplify(precell);
    sol.space_data(1)(cell) = true_solution.qTrue(xValue(cell), time);
    sol.space_data(2)(cell) = sol.space_data(1)(cell);
  }
  write("done.\n");
  
  
  //==== Set sol.time (both values) ====
  sol.time = time;

}
//<=== RectangularGrid.initializeSolution method <===
//<==================================================





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
  def ghostFill(q: [grid.ext_cells] real, t: real) {
    
    for d in dimensions {

      forall precell in grid.low_ghost_cells(d) {
        var cell = tuplify(precell);
        q(cell) = true_solution.qTrue(grid.xValue(cell), q.time);
      }

      forall precell in grid.high_ghost_cells(d) {
        var cell = tuplify(precell);
        q(cell) = true_solution.qTrue(grid.xValue(cell), q.time);
      }

    }

  }
  //<=== ghostFill method <===
  //<=========================


  //===> homogeneousGhostFill method ===>
  //====================================>
  //------------------------------------------------------------
  // For homogeneous Dirichlet BCs.  At each boundary face, the 
  // nearest interior value is reflected about 0 to the nearest 
  // ghost cell value.
  //------------------------------------------------------------
  def homogeneousGhostFill(q: [grid.ext_cells] real) {

    for d in dimensions {

      //==== For shifting from a face to a cell ====
      var shift: dimension*int;
      shift(d) = 1;

      
      //==== Low boundary ====
      forall preface in low_boundary_faces(d) {
        var face = tuplify(preface);
        q(face - shift) = -q(face + shift);
      }


      //==== High boundary ====
      forall preface in high_boundary_faces(d) {
        var face = tuplify(preface);
        q(face + shift) = -q(face - shift);
      }
    }

  }
  //<=== homogeneousGhostFill method <===
  //<====================================

}
//<=== TrueDirichletGridBC class <===
//<==================================



//===> TrueNeumannGridBC class ===>
//================================>
class TrueNeumannGridBC: GridBC {

  var true_solution: TrueSolution;


  //===> initialize method ===>
  //==========================>
  def initialize() {
    setBoundaryFaces();
  }
  //<=== intialize method <===
  //<=========================



  //===> ghostFill method ===>
  //=========================>
  def ghostFill(q: [grid.ext_cells] real, t: real) {

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
        q(face-shift) = true_solution.normalFluxToGhost(normal_flux, 
                                                        q(face+shift), 
                                                        dx);
      }


      //==== High boundary ====
      forall preface in high_boundary_faces(d) {
        var face            = tuplify(preface);
        var normal_flux     = true_solution.fluxComponent(grid.xValue(face), q.time, d);
        q(face+shift) = true_solution.normalFluxToGhost(normal_flux,
                                                        q(face-shift),
                                                        dx);
      }

    }

  }
  //<=== ghostFill method <===
  //<=========================



  //===> homogeneousGhostFill method ===>
  //====================================>
  def homogeneousGhostFill(q: [grid.ext_cells] real) {

    for d in dimensions do {
      //==== Low ghost cells ====
      forall cell in grid.low_ghost_cells(d) {
        var target_cell = cell;
        target_cell(d)  = grid.cells.dim(d).low;
        q(cell)         = q(target_cell);
      }

      //==== High ghost cells ====
      forall cell in grid.high_ghost_cells(d) {
        var target_cell = cell;
        target_cell(d)  = grid.cells.dim(d).high;
        q(cell)         = q(target_cell);
      }
    }

  }
  //<=== homogeneousGhostFill method <===
  //<====================================
}
//<=== TrueNeumannGridBC class <===
//<================================







//===> Gaussian class ===>
//=======================>
class Gaussian: TrueSolution {

  //==== Parameters ====
  const pi = 4.0*atan(1.0);
  var diffusivity = 0.1;
  var t0 = 0.1;


  //===> qTrue method ===>
  //=====================>
  def qTrue(x: dimension*real, t: real) {

    //==== Calculate norm squared of x ====
    var norm_squared_x = 0.0;
    for d in dimensions do norm_squared_x += x(d)**2;

    return (4.0 * pi * diffusivity * (t + t0))**(-dimension:real / 2.0)
      * exp(-norm_squared_x / (4.0 * diffusivity * (t + t0)));

  }
  //<=== qTrue method <===
  //<=====================


  //===> fluxComponent method ===>
  //=============================>
  def fluxComponent(x: dimension*real, t: real, comp: int) {
    var delta = 1.0e-4;
    var shift: dimension*real;
    shift(comp) = delta;
    return -diffusivity * (qTrue(x+shift,t) - qTrue(x-shift,t))
           / (2.0 * delta);
  }
  //<=== fluxComponent method <===
  //<=============================


  //===> normalFluxToGhost method ===>
  //=================================>
  def normalFluxToGhost(flux:        real,
			inner_value: real,
			dx:          real) {

    return inner_value - dx/diffusivity * flux;
  }
  //<=== normalFluxToGhost method <===
  //<=================================

}
//<=======================
//<=== Gaussian class <===