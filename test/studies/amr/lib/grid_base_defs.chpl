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


//===> Definition of class RectangularGrid ===>
//============================================>
class RectangularGrid {
  
  var low_coord, high_coord:  dimension*real,
      low_index, high_index:  dimension*int,
      n_cells, n_ghost_cells: dimension*int;

  var dx: dimension*real;
          
  var all_cells:      domain(dimension, stridable=true),
      physical_cells: subdomain(all_cells);
  
  var low_ghost_cells:  [dimensions] subdomain(all_cells),
      high_ghost_cells: [dimensions] subdomain(all_cells);

  var boundary_data: BoundaryData;


  //===> initialize() method ===>
  def initialize() {

    sanity_checks();

    set_derived_fields();
  }
  //<=== initialize() method <===



  //===> sanity_checks() method ===>
  def sanity_checks() {
    var d_string: string;
    for d in dimensions do {
      d_string = format("%i", d);

      assert(low_coord(d) < high_coord(d),
	     "error: RectangularGrid: low_coord(" + d_string + ") must be strictly less than high_coord(" + d_string + ").");

      assert(n_cells(d) > 0,
             "error: RectangularGrid: n_cells(" + d_string + ") must be positive.");

      assert(n_ghost_cells(d) > 0,
	     "error: RectangularGrid: n_ghost_cells(" + d_string + ") must be positive.");
    }
  }
  //<=== sanity_checks() method <===



  //===> set_derived_fields() method ===>
  //--------------------------------------------------------------
  // After low_coord, high_coord, n_cells, and n_ghost_cells have
  // been provided, calculate:
  //     dx
  //     high_index
  //     physical_cells
  //     all_cells
  //     low_ghost_cells
  //     high_ghost_cells
  //--------------------------------------------------------------
  def set_derived_fields() {
    //==== dx ====
    dx = (high_coord - low_coord) / n_cells;


    //==== high_index ====
    [d in dimensions] high_index(d) = low_index(d) + 2*n_cells(d);


    //==== Physical cells ====
    var range_tuple: dimension*range(stridable = true);
    [d in dimensions] 
      range_tuple(d) = low_index(d)+1..low_index(d)+2*n_cells(d)-1 by 2;
    physical_cells = [(...range_tuple)];


    //==== All cells (including ghost cells) ====
    var size: dimension*int;
    [d in dimensions] size(d) = 2*n_ghost_cells(d);
    all_cells = physical_cells.expand(size);
         

    //===> Ghost cells ===>
    //------------------------------------------------------
    // Note that all ghost cell domains contain the corners.
    //------------------------------------------------------
    for d in dimensions do {
      [d_temp in dimensions] size(d_temp) = 0;

      size(d) = -(2*n_ghost_cells(d) - 1);
      low_ghost_cells(d) = all_cells.interior(size);

      size(d) = 2*n_ghost_cells(d) - 1;
      high_ghost_cells(d) = all_cells.interior(size);
    }   
    //<=== Ghost cells <===     

  }
  //<=== set_derived_fields() method <===
  
 
}
//<=== Definition of class RectangularGrid <===
//<============================================




//===> Definition of class GridFunction ===>
//=========================================>
class GridFunction {
  var parent_grid: RectangularGrid;
  var value: [parent_grid.all_cells] real;
  var time: real;
}
//<=========================================
//<=== Definition of class GridFunction ====




//===> Generate coordinates of cell centers ===>
//=============================================>
def RectangularGrid.coordinates (point_index: dimension*int) {

  var coord: dimension*real;

  if dimension == 1 then {
    coord(1) = low_coord(1) + point_index(1)*dx(1)/2.0;
  }
  else {
    forall d in dimensions do
    coord(d) = low_coord(d) + point_index(d)*dx(d)/2.0;
  }

  return coord;
}
//<=============================================
//<=== Generate coordinates of cell centers <===




//===> Evaluating an analytical function on the grid ===>
//======================================================>
def RectangularGrid.evaluate(f) {
  var q = new GridFunction(this);

  if dimension==1 then
    forall cell in physical_cells {
      q.value(cell) = f(coordinates(tuple(cell)));
    }
  else
    forall cell in physical_cells {
      q.value(cell) = f( coordinates(cell) );
    }

  return q;
}
//<======================================================
//<=== Evaluating an analytical function on the grid <===





//===> Output a GridFunction in Clawpack format ===>
//=================================================>
def RectangularGrid.clawpack_output(q: GridFunction, frame_number: int) {


  //---- Make sure that q lives on this grid ----
  assert(q.parent_grid == this);


  //---- Parameters needed by the output file ----
  var meqn:        int = 1,
      ngrids:      int = 1,
      maux:        int = 0,
      grid_number: int = 1,
      AMR_level:   int = 1;


  //---- Formatting parameters ----
  var efmt:  string = "%26.16E",
      ifmt:  string = "%5i",
      sfmt:  string = "%20s",
      linelabel: string;


  //---- Names of output files ----
  var frame_string:          string = format("%04i", frame_number),
      name_of_time_file:     string = "_output/fort.t" + frame_string,
      name_of_solution_file: string = "_output/fort.q" + frame_string;


  //---- Write time file ----
  var outfile = new file(name_of_time_file, FileAccessMode.write);
  outfile.open();
  outfile.writeln( format(efmt, q.time),    "    time");
  outfile.writeln( format(ifmt, meqn),      "                 meqn");
  outfile.writeln( format(ifmt, ngrids),    "                 ngrids");
  outfile.writeln( format(ifmt, maux),      "                 naux");
  outfile.writeln( format(ifmt, dimension), "                 ndim");
  outfile.writeln("");
  outfile.writeln("");
  outfile.close();
  delete outfile;


  //---> Write solution file --->
  //---------------------------->
  outfile = new file(name_of_solution_file, FileAccessMode.write);
  outfile.open();
  outfile.writeln( format(ifmt, grid_number), "                 grid_number");
  outfile.writeln( format(ifmt, AMR_level),   "                 AMR_level");


  //---- Write n_cells ----
  for d in dimensions do {
    select d {
      when 1 do linelabel = "                 mx";
      when 2 do linelabel = "                 my";
      when 3 do linelabel = "                 mz";
      otherwise linelabel = "                 mx(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(ifmt, n_cells(d)),  linelabel);
  }


  //---- Write low_coord ----
  for d in dimensions do {
    select d {
      when 1 do linelabel = "    xlow";
      when 2 do linelabel = "    ylow";
      when 3 do linelabel = "    zlow";
      otherwise linelabel = "    xlow(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(efmt, low_coord(d)),  linelabel);
  }


  //---- Write dx ----
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
    for cell in physical_cells do
      outfile.writeln(format(efmt, q.value(cell)));
  }
  else {
    //------------------------------------------------------------
    //---- Transpose physical_cells; iterating over the transpose
    //---- in row major order achieves column major order on the
    //---- original domain. --------------------------------------
    //------------------------------------------------------------
    var range_tuple: dimension*range(stridable=true);
    [d in dimensions]
      range_tuple(d) = physical_cells.dim(1 + dimension - d);

    var physical_cells_transposed: domain(dimension, stridable=true);
    physical_cells_transposed = [(...range_tuple)];

    //---- Write values ----
    var cell: dimension*int;
    for cell_transposed in physical_cells_transposed do {
      [d in dimensions] cell(d) = cell_transposed(1 + dimension - d);
      outfile.writeln(format(efmt, q.value(cell)));
      for d in dimensions do {
        if cell(d) == physical_cells.dim(d).high then
          outfile.writeln("  ");
        else
          break;
      }
    }
  }
  //<=== Write solution values <===


  //---- Finish up ----
  outfile.close();
  delete outfile;
  //<----------------------------
  //<--- Write solution file <---

}
//<=================================================
//<=== Output a GridFunction in Clawpack format <===




//===> BoundaryData class ===>
//===========================>
class BoundaryData {

  var parent: RectangularGrid;
  
  //==== fill_ghost_cells() method ====
  //--------------------------------------------------
  // This method is empty, and meant to be overridden
  // by a like-named method for a derived class.
  //--------------------------------------------------
  def fill_ghost_cells(value: [parent.all_cells] real) {
    assert(false, "The fill_ghost_cells() method of class BoundaryData " +
	   "must be overriddend by a derived class.");
  }
}
//<=== BoundaryData class <===
//<===========================




//===> PeriodicBoundaryData class ===>
//===================================>
class PeriodicBoundaryData: BoundaryData {

  var low_ghost_periodic:  [dimensions] subdomain(parent.all_cells);
  var high_ghost_periodic: [dimensions] subdomain(parent.all_cells);


  //===> initialize() method ===>
  def initialize() {

    //==== Set parent's boundary data ====
    parent.boundary_data = this;


    //===> Build periodic images of ghost cells ===>
    var shift: dimension*int;
    
    for d in dimensions do {
      [d_temp in dimensions] shift(d_temp) = 0;

      shift(d) = 2*parent.n_cells(d);
      low_ghost_periodic(d)  = parent.low_ghost_cells(d).translate(shift);

      shift(d) = -2*parent.n_cells(d);
      high_ghost_periodic(d) = parent.high_ghost_cells(d).translate(shift);
    }
    //<=== Build periodic images of ghost cells <===

  }
  //<=== initialize() method <===



  //===> fill_ghost_cells() method ===>
  def fill_ghost_cells(value: [parent.all_cells] real) {
    for d in dimensions do {
      value(parent.low_ghost_cells(d))  = value(low_ghost_periodic(d));
      value(parent.high_ghost_cells(d)) = value(high_ghost_periodic(d));
    }
  }
  //<=== fill_ghost_cells() method <===

}
//<===================================
//<=== PeriodicBoundaryData class <===





//===> ZeroOrderExtrapolation class ===>
//=====================================>
class ZeroOrderExtrapolation: BoundaryData {

  //===> initialize() method ===>
  def initialize() {

    //==== Set parent's boundary data ====
    parent.boundary_data = this;

  }
  //<=== initialize() method <===



  //===> fill_ghost_cells() method ===>
  def fill_ghost_cells(value: [parent.all_cells] real) {

    for d in dimensions do {
      //==== Low ghost cells ====
      forall cell in parent.low_ghost_cells(d) {
	var target_cell = cell;
	target_cell(d) = parent.physical_cells.dim(d).low;
	value(cell) = value(target_cell);
      }

      //==== High ghost cells ====
      forall cell in parent.high_ghost_cells(d) {
	var target_cell = cell;
	target_cell(d) = parent.physical_cells.dim(d).high;
	value(cell) = value(target_cell);
      }
    }

  }
  //<=== fill_ghost_cells() method <===

}
//<=====================================
//<=== ZeroOrderExtrapolation class <===