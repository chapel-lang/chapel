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
  
  var ghost_cells: GridBoundaryDomain;
/*   var low_ghost_cells:  [dimensions] subdomain(ext_cells), */
/*       high_ghost_cells: [dimensions] subdomain(ext_cells); */


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
      range_tuple(d) = i_low(d)+1 .. #2*n_cells(d) by 2;
    cells = range_tuple;


    //==== Extended cells (includes ghost cells) ====
    var size: dimension*int;
    for d in dimensions do
      size(d) = 2*n_ghost_cells(d);
    ext_cells = cells.expand(size);
         

    //===> Ghost cells ===>
    ghost_cells = new GridBoundaryDomain(grid = this);

    for orientation in dimensions do {

      var range_tuple: dimension*range(stridable=true);

      //===> Set up off-dimensions ===>
      for d in [1..orientation-1] do
	range_tuple(d) = cells.dim(d);

      for d in [orientation+1 .. dimension] do
	range_tuple(d) = ext_cells.dim(d);
      //<=== Set up off-dimensions <===



      //===> Low ghost cells ===>
      range_tuple(orientation) = ext_cells.low(orientation)
                                 .. cells.low(orientation) - 2 
                                 by 2;

      ghost_cells.low(orientation) = range_tuple;

/*       low_ghost_cells(orientation) = range_tuple; */
      //<=== Low ghost cells <===


      //===> High ghost cells ===>
      range_tuple(orientation) = cells.high(orientation) + 2
                                 .. ext_cells.high(orientation)
                                 by 2;

      ghost_cells.high(orientation) = range_tuple;
/*       high_ghost_cells(orientation) = range_tuple; */
      //<=== High ghost cells <===

    }   
    //<=== Ghost cells <===     

  }
  //<=== setDerivedFields method <===
  //<================================



/*   //===> ghost_cells iterator ===> */
/*   //=============================> */
/*   def ghost_cells { */
/*     for d in dimensions { */
/*       for cell in low_ghost_cells(d) do */
/* 	yield cell; */
/*       for cell in high_ghost_cells(d) do */
/* 	yield cell; */
/*     } */
/*   } */
/*   //<=== ghost_cells iterator <=== */
/*   //<============================= */
  

}
//<=== BaseGrid class <===
//<==============================



//===> GridBoundaryDomain class ===>
//=================================>
//------------------------------------------------------------------
// Sets up a "domain" with a low and high entry for each dimension.
// Also provides a these() method for use as a (currently serial)
// iterator.
//------------------------------------------------------------------
class GridBoundaryDomain {
  const grid: BaseGrid;
  var low:  [dimensions] domain(dimension, stridable=true);
  var high: [dimensions] domain(dimension, stridable=true);

  def these() {
    for d in dimensions {
      for idx in low(d) do
	yield idx;
      for idx in high(d) do
	yield idx;
    }
  }

}
//<=== GridBoundaryDomain class <===
//<=================================




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
    overlap(d) = max(D1.low(d), D2.low(d)) 
                 .. 
                 min(D1.high(d), D2.high(d))
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
