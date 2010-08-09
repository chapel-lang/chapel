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
    for d in dimensions do
      i_high(d) = i_low(d) + 2*n_cells(d);


    //==== Physical cells ====
    var range_tuple: dimension*range(stridable = true);
    for d in dimensions do
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
      forall d_size in dimensions do size(d_size) = 0;

      size(d) = -(2*n_ghost_cells(d) - 1);
      low_ghost_cells(d) = ext_cells.interior(size);

      size(d) = 2*n_ghost_cells(d) - 1;
      high_ghost_cells(d) = ext_cells.interior(size);
    }   
    //<=== Ghost cells <===     

  }
  //<=== setDerivedFields method <===
  
 

  //===> GhostData subclass ===>
  //===========================>
  //--------------------------------------------------------------
  // Ideally, the fields low and high would be direct collections
  // of arrays, but I don't think that's possible at the moment.
  //--------------------------------------------------------------
  class GhostData {

    type data_type;
    var low, high: [dimensions] IsolatedArray(dimension, data_type);

    def initialize() {
      for d in dimensions {
	low(d) = new IsolatedArray(dim       = dimension,  
				   data_type = data_type, 
				   dom       = low_ghost_cells(d));

	high(d) = new IsolatedArray(dim       = dimension, 
				    data_type = data_type, 
				    dom       = high_ghost_cells(d));
      }

    }
  }
  //<===========================
  //<=== GhostData subclass <===


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


//<=====================================================================
//<=================== FUNDAMENTAL GRID DEFINITIONS ====================
//<=====================================================================






//==============================================>
//==================== OTHER ===================>
//==============================================>


//===> GridArray class ===>
//========================>
class GridArray {
  const grid: RectangularGrid;
  var value: [grid.ext_cells] real;

  def this(cell: dimension*int) {
    return value(cell);
  }
}
//<=== GridArray class <===
//<========================


//===> Fix for the 1D/tuple problem ===>
def tuplify(idx) {
  if isTuple(idx) then
    return idx;
  else
    return tuple(idx);
}
//<=== Fix for the 1D/tuple problem <===



//===> IsolatedArray record ===>
//=============================>
//-------------------------------------------------------------
// Contains an array and its domain.  "Isolated" in that the
// domain will be assigned as a copy.  Not sharp in that sense
// for my purposes, but needed to dodge the problem of making
// a tuple of generic length and generic type for now.
//-------------------------------------------------------------
record IsolatedArray {
  param dim: int;
  type data_type;
  var dom: domain(dim,stridable=true);
  var value: [dom] data_type;
}
//<=============================
//<=== IsolatedArray record <===



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
//<=================================
//<=== intersectDomains routine <===


//<==============================================
//<=================== OTHER ====================
//<==============================================
