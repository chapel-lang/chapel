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


//======================================================================>
//==================== FUNDAMENTAL GRID DEFINITIONS  ===================>
//======================================================================>


//===> BaseGrid class ===>
//=======================>
class BaseGrid {
  
  var x_low, x_high:          dimension*real,
      i_low, i_high:          dimension*int,
      n_cells, n_ghost_cells: dimension*int;

  var dx: dimension*real;
          
  var ext_cells:      domain(dimension, stridable=true),
      cells: subdomain(ext_cells);
  
  var low_ghost_cells:  [dimensions] subdomain(ext_cells),
      high_ghost_cells: [dimensions] subdomain(ext_cells);


  //===> initialize method ===>
  //==========================>
  def initialize() {

    sanityChecks();

    setDerivedFields();
  }
  //<=== initialize method <===
  //<==========================


  //===> sanityChecks method ===>
  //============================>
  def sanityChecks() {
    var d_string: string;
    for d in dimensions do {
      d_string = format("%i", d);

      assert(x_low(d) < x_high(d),
	     "error: BaseGrid: x_low(" + d_string + ") must be strictly less than x_high(" + d_string + ").");

      assert(n_cells(d) > 0,
             "error: BaseGrid: n_cells(" + d_string + ") must be positive.");

      assert(n_ghost_cells(d) > 0,
	     "error: BaseGrid: n_ghost_cells(" + d_string + ") must be positive.");
    }
  }
  //<=== sanityChecks method <===
  //<============================




  //===> setDerivedFields method ===>
  //================================>
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
    for d in dimensions do
      i_high(d) = i_low(d) + 2*n_cells(d);


    //==== Physical cells ====
    var range_tuple: dimension*range(stridable = true);
    for d in dimensions do
      range_tuple(d) = i_low(d)+1 .. i_low(d)+2*n_cells(d)-1 by 2;
    cells = range_tuple;


    //==== Extended cells (includes ghost cells) ====
    var size: dimension*int;
    [d in dimensions] size(d) = 2*n_ghost_cells(d);
    ext_cells = cells.expand(size);
         

    //===> Ghost cells ===>
    //------------------------------------------------------
    // Note that all ghost cell domains contain the corners.
    //------------------------------------------------------
    for orientation in dimensions do {

      var range_tuple: dimension*range(stridable=true);

      //===> Set up off-dimensions ===>
      for d in [1..orientation-1] do
	range_tuple(d) = cells.dim(d);

      for d in [orientation+1 .. dimension] do
	range_tuple(d) = ext_cells.dim(d);
      //<=== Set up off-dimensions <===


      //===> Low ghost cells ===>
      range_tuple(orientation) = ext_cells.dim(orientation).low 
                                 .. cells.dim(orientation).low-2 
                                 by 2;

      low_ghost_cells(orientation) = range_tuple;
      //<=== Low ghost cells <===


      //===> High ghost cells ===>
      range_tuple(orientation) = cells.dim(orientation).high+2
                                 .. ext_cells.dim(orientation).high
                                 by 2;

      high_ghost_cells(orientation) = range_tuple;
      //<=== High ghost cells <===



/*       forall d_size in dimensions do size(d_size) = 0; */

/*       size(d) = -(2*n_ghost_cells(d) - 1); */
/*       low_ghost_cells(d) = ext_cells.interior(size); */

/*       size(d) = 2*n_ghost_cells(d) - 1; */
/*       high_ghost_cells(d) = ext_cells.interior(size); */
    }   
    //<=== Ghost cells <===     

  }
  //<=== setDerivedFields method <===
  //<================================



  //===> ghost_cells iterator ===>
  //=============================>
  def ghost_cells {
    for d in dimensions {
      for cell in low_ghost_cells(d) do
	yield cell;
      for cell in high_ghost_cells(d) do
	yield cell;
    }
  }
  //<=== ghost_cells iterator <===
  //<=============================
  

}
//<=== BaseGrid class <===
//<==============================




//===> xValue method ===>
//======================>
//---------------------------------------
// Converts indices to coordinate values.
//----------------------------------------
def BaseGrid.xValue (point_index: dimension*int) {

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
//<=== xValue method <===
//<======================


//<=====================================================================
//<=================== FUNDAMENTAL GRID DEFINITIONS ====================
//<=====================================================================






//====================================================>
//==================== GRID ARRAYS ===================>
//====================================================>

//===> GridArray class ===>
//========================>
class GridArray {
  const grid: BaseGrid;
  var value: [grid.ext_cells] real;
}
//<=== GridArray class <===
//<========================




//===> BaseGrid.setGridArray method ===>
//=====================================>
def BaseGrid.setGridArray(
  q: GridArray,
  f: func(dimension*real, real)
){
  //==== Check that q lives on this grid ====
  assert(q.grid == this);


  //==== Evaluate and store f(x) ====
  forall precell in cells {
    var cell = tuplify(precell);
    q.value(cell) = f(xValue(cell));
  }
}
//<=== BaseGrid.setGridArray method <===
//<=====================================


//<====================================================
//<=================== GRID ARRAYS ====================
//<====================================================





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




//===> BaseGrid.writeGridArray method ===>
//=======================================>
def BaseGrid.writeGridArray(
  q:           GridArray,
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


  //===> Write array values ===>
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
  //<=== Write array values <===
  
}
//<=== BaseGrid.writeGridArray method <===
//<=======================================




//===> clawOutput method ===>
//==========================>
//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridArray, at the given 
// time and frame_number.
//-------------------------------------------------------------------
def BaseGrid.clawOutput(
  q:            GridArray,
  time:         real,
  frame_number: int
){

  //==== Make sure solution lives on this grid ====
  assert(q.grid == this);


  //==== Names of output files ====
  var frame_string:  string = format("%04i", frame_number),
      time_filename: string = "_output/fort.t" + frame_string,
      data_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var outfile = new file(time_filename, FileAccessMode.write);
  outfile.open();
  writeTimeFile(time, 1, 1, 0, outfile);
  outfile.close();
  delete outfile;
  
  
  //==== Data file ====
  outfile = new file(data_filename, FileAccessMode.write);
  outfile.open();
  writeGridArray(q, 1, 1, outfile);
  outfile.close();
  delete outfile;

}
//<=== clawOutput method <===
//<==========================


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



//===> intersectDomains routine ===>
//=================================>
//--------------------------------------------------------------
// Determines the intersection of two arithmetic domains of the
// same type.  Returns the domain of intersection, or an empty
// domain if the inputs don't intersect.
//--------------------------------------------------------------
def intersectDomains(D1, D2) {
  
  assert(D1.type == D2.type,
	 "error: intersectDomains: input domains must be of same type");


  //==== Assume the domains intersect ====
  var intersect = true;

  var overlap: D1.rank*range(stridable=true);

  for d in [1..D1.rank] {

    //==== Check that strides match ====
    assert(D1.dim(d).stride == D2.dim(d).stride,
           "error: intersectDomains: domains must have equal stride");

    //==== Compute the overlap in this dimension ====
    overlap(d) = max(D1.dim(d).low, D2.dim(d).low) 
                 .. 
                 min(D1.dim(d).high, D2.dim(d).high) 
                 by 
                 D1.dim(d).stride;
    
    //==== Check whether this rules out intersection ====
    if overlap(d).length == 0 then intersect = false;
  }


  //==== Return ====
  if intersect then
    return [(...overlap)];
  else {
    var empty_domain: D1.type;
    return empty_domain;
  }


}
//<=== intersectDomains routine <===
//<=================================


//<==============================================
//<=================== OTHER ====================
//<==============================================
