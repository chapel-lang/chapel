//===> Description ===>
//
// Basic class and method definitions for a rectangular grid of
// generic dimension (though it must be a compile-time constant.)
//
// This version is based on an underlying mesh of width dx/2, which
// contains all cell centers, interface centers, and vertices.  As
// a result, the domain of cell centers is strided by 2, corresponding
// to jumps of dx.
//
//<=== Description <===


config param dimension = 2;
const dimensions = [1..dimension];

enum loc1d {low=-1, interior=0, high=1};
const interior_location: dimension*int; // since loc1d.interior=0, this initializes correctly
const ghost_locations = (loc1d.low..loc1d.high)**dimension;


//==== Helper constants ====
/* const zeros: dimension*int; */
/* const e: dimension*(dimension*int); */
/* for d in dimensions do e(d)(d) = 1; */
/* const empty_domain: domain(dimension, stridable=true); */



//|""""""""""""""""""\
//|===> Grid class ===>
//|__________________/
class Grid {
  
  const x_low, x_high:          dimension*real;
  const i_low, i_high:          dimension*int;
  const n_cells, n_ghost_cells: dimension*int;

  const dx: dimension*real;
          
  const ext_cells: domain(dimension, stridable=true);
  const cells:     subdomain(ext_cells);
  
  const ghost_domain_set: GhostDomainSet;


  //|-------------------->
  //|===> Constructor ===>
  //|-------------------->
  def Grid(
    x_low:         dimension*real,
    x_high:        dimension*real,
    i_low:         dimension*int,
    n_cells:       dimension*int,
    n_ghost_cells: dimension*int)
  {

    //==== Assign inputs to fields ====
    this.x_low         = x_low;
    this.x_high        = x_high;
    this.i_low         = i_low;
    this.n_cells       = n_cells;
    this.n_ghost_cells = n_ghost_cells;

    //==== Sanity check ====
    sanityChecks();

    //==== dx ====
    dx = (x_high - x_low) / n_cells;

    //==== i_high ====
    for d in dimensions do
      i_high(d) = i_low(d) + 2*n_cells(d);

    //==== Physical cells ====
    var ranges: dimension*range(stridable = true);
    for d in dimensions do ranges(d) = (i_low(d)+1 .. by 2) #n_cells(d);
    cells = ranges;

    //==== Extended cells (includes ghost cells) ====
    var size: dimension*int;
    for d in dimensions do size(d) = 2*n_ghost_cells(d);
    ext_cells = cells.expand(size);

    //==== Ghost cells ====
    ghost_domain_set = new GhostDomainSet(this);
  }
  //<--------------------|
  //<=== Constructor <===|
  //<--------------------|



  //|---------------------------->
  //|===> sanityChecks method ===>
  //|---------------------------->
  //--------------------------------------------------------------
  // Performs some basic sanity checks on the constructor inputs.
  //--------------------------------------------------------------
  def sanityChecks() {
    var d_string: string;
    for d in dimensions do {
      d_string = format("%i", d);

      assert(x_low(d) < x_high(d),
	     "error: Grid: x_low(" + d_string + ") must be strictly less than x_high(" + d_string + ").");

      assert(n_cells(d) > 0,
             "error: Grid: n_cells(" + d_string + ") must be positive.");

      assert(n_ghost_cells(d) > 0,
	     "error: Grid: n_ghost_cells(" + d_string + ") must be positive.");
    }
  }
  //<----------------------------|
  //<=== sanityChecks method <===|
  //<----------------------------|



  //|------------------------->
  //|===> writeThis method ===>
  //|------------------------->
  def writeThis(w: Writer) {
    writeln("x_low: ", x_low, ",  x_high: ", x_high);
    write("i_low: ", i_low, ",  i_high: ", i_high);
  }
  //<-------------------------|
  //<=== writeThis method <===|
  //<-------------------------|

}
// /""""""""""""""""""|
//<=== Grid class <===|
// \__________________|







//|--------------------------->
//|===> Grid.xValue method ===>
//|--------------------------->
//----------------------------------------
// Converts indices to coordinate values.
//----------------------------------------
def Grid.xValue (point_index: dimension*int) {

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
//<---------------------------|
//<=== Grid.xValue method <===|
//<---------------------------|







//|""""""""""""""""""""""""""""\
//|===> GhostDomainSet class ===>
//|____________________________/
class GhostDomainSet {

  const grid:    Grid;
  var   domains: [ghost_locations] domain(dimension, stridable=true);

  //|-------------------->
  //|===> Constructor ===>
  //|-------------------->
  def GhostDomainSet(grid: Grid) {

    this.grid = grid;

    var ranges: dimension*range(stridable=true);
    var interior_location: dimension*int;
    for d in dimensions do interior_location(d) = loc1d.interior;

    for loc in ghost_locations {
      if loc != interior_location {
	for d in dimensions do ranges(d) = setRange(d,loc(d));
	domains(loc) = ranges;
      }
    }

  }
  //<--------------------|
  //<=== Constructor <===|
  //<--------------------|



  //|------------------------>
  //|===> setRange method ===>
  //|------------------------>
  //-----------------------------------------------------------------
  // Given a dimension and position, sets the corresponding range of
  // ghost cells.
  //-----------------------------------------------------------------
  def setRange(d: int, p: int) {
    var R: range(stridable=true);

    if p == loc1d.low then
      R = (grid.ext_cells.low(d).. by 2) #grid.n_ghost_cells(d);
    else if p == loc1d.interior then
      R = grid.cells.dim(d);
    else
      R = (..grid.ext_cells.high(d) by 2) #grid.n_ghost_cells(d);

    return R;
  }
  //<------------------------|
  //<=== setRange method <===|
  //<------------------------|



  def this(loc: dimension*int) var {
    return domains(loc);
  }

}
// /""""""""""""""""""""""""""""/
//<=== GhostDomainSet class <==<
// \____________________________\




//|""""""""""""""""""""""""""""""""""\
//|===> SparseGhostDomainSet class ===>
//|__________________________________/
class SparseGhostDomainSet {
  const grid:    Grid;
  var locations: sparse subdomain(ghost_locations);
  var domains:   [locations] domain(dimension, stridable=true);
}
// /""""""""""""""""""""""""""""""""""/
//<=== SparseGhostDomainSet class <==<
// \__________________________________\




//|"""""""""""""""""""""""""""\
//|===> GhostArraySet class ===>
//|___________________________/
//---------------------------------------------------------------------
// Creates a set of arrays for storing data on a grid's ghost cells.
// Note that the IsolatedArray field stores it's data in the 'value'
// field, so data access should look like ghost_array(loc).value(idx).
//---------------------------------------------------------------------
class GhostArraySet {
  const grid: Grid;
  var arrays: [ghost_locations] IsolatedArray;

  def GhostArraySet(grid: Grid) {
    this.grid = grid;
    for loc in ghost_locations do
      arrays(loc) = new IsolatedArray(grid.ghost_domain_set(loc));
  }

  def this(loc: dimension*int) var {
    return arrays(loc);
  }
}
// /"""""""""""""""""""""""""""/
//<=== GhostArraySet class <==<
// \___________________________\







//|------------------------->
//|===> readGrid routine ===>
//|------------------------->
//---------------------------------------
// Constructs a grid from an input file.
//---------------------------------------
def readGrid(file_name: string) {

  var input_file = new file(file_name, FileAccessMode.read);
  input_file.open();

  var dim_in: int;
  input_file.readln(dim_in);
  assert(dim_in == dimension, 
         "error: dimension of space.txt must equal " + format("%i",dimension));
  input_file.readln(); // empty line

  var x_low, x_high:                 dimension*real;
  var n_cells, n_ghost_cells: dimension*int;

  input_file.readln( (...x_low) );
  input_file.readln( (...x_high) );
  input_file.readln( (...n_cells) );
  input_file.readln( (...n_ghost_cells) );

  input_file.close();

  //==== Set low indices ====
  //-------------------------------------------------------------
  // This routine should only be used for a single-grid problem,
  // so assume lower indicies are all zero.
  //-------------------------------------------------------------
  var i_low: dimension*int;


  return new Grid(x_low, x_high, i_low, n_cells, n_ghost_cells);

}
//<-------------------------|
//<=== readGrid routine <===|
//<-------------------------|










//|------------------------>
//|===> tuplify routine ===>
//|------------------------>
//-----------------------------------------------------------
// This is used to fix the "1D problem", in that indices of
// a one-dimensional domain are of type int, whereas for all
// other dimensions, they're dimension*int.
//-----------------------------------------------------------
def tuplify(idx) {
  if isTuple(idx) then return idx;
  else return tuple(idx);
}
//<------------------------|
//<=== tuplify routine <===|
//<------------------------|



//|----------------------------*
//|==== range exponentiation ===>
//|----------------------------*
def **(R: range(stridable=?s), param n: int) {
  var ranges: n*R.type;
  for i in [1..n] do ranges(i) = R;

  var D: domain(n,stridable=s) = ranges;
  return D;
}
// *----------------------------|
//<=== range exponentiation <===|
// *----------------------------|



//|---------------------------*
//|===> IsolatedArray class ===>
//|---------------------------*
//-----------------------------------------------------------------------
// This class defines an "isolated" array in the sense that the object
// contains its own domain.  This allows arrays of arrays to be created.
// I'm only creating it for the single type of domain I'm using to store
// solution data.
//-----------------------------------------------------------------------
class IsolatedArray {
  const dom: domain(dimension, stridable=true);
  var value: [dom] real;
}
// *---------------------------|
//<=== IsolatedArray class <===|
// *---------------------------|



//|-------------------------------*
//|===> Scalar/tuple arithmetic ===>
//|-------------------------------*
def *(a, T)
where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type {
  var U: T.type;
  for i in 1..T.size do
    U(i) = a*T(i);
  return U;
}

def *(T,a)
where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  return a*T;
}
// *-------------------------------|
//<=== Scalar/tuple arithmetic <===|
// *-------------------------------|




//|------------------------------*
//|===> setOutputTimes routine ===>
//|------------------------------*
def setOutputTimes(file_name: string) {

  var input_file = new file(file_name, FileAccessMode.read);
  input_file.open();

  var initial_time, final_time: real;
  var n_output: int;

  input_file.readln(initial_time);
  input_file.readln(final_time);
  input_file.readln(n_output);
  input_file.close();

  var output_times: [1..n_output] real;
  var dt_output:    real = (final_time - initial_time) / n_output;
  
  for i in [1..n_output] do
    output_times(i) = initial_time + i*dt_output;

  return output_times;

}
// *------------------------------|
//<=== setOutputTimes routine <===|
// *------------------------------|





def main {

  var x_low = (0.0,1.0);
  var x_high = (2.0,3.0);
  var i_low = (0,0);
  var n_cells = (20,40);
  var n_ghost_cells = (2,2);

  var grid = new Grid(x_low, x_high, i_low, n_cells, n_ghost_cells);

  writeln(grid);
  writeln("grid.cells = ", grid.cells);
  writeln("grid.ext_cells = ", grid.ext_cells);

  writeln("");
  writeln("Ghost cell domains:");
  for loc in ghost_locations do
    writeln( grid.ghost_domain_set(loc) );

  writeln("");
  var ghost_array = new GhostArraySet(grid);
  for loc in ghost_locations {
    writeln( grid.ghost_domain_set(loc) );
    writeln( ghost_array(loc).value);
    writeln("");
  }

  var sparse_ghost_set = new SparseGhostDomainSet(grid);
  for loc in ghost_locations {
    var ghost_domain = grid.ghost_domain_set(loc);
    if ghost_domain.high(2) > 40 {
      sparse_ghost_set.locations.add(loc);
      sparse_ghost_set.domains(loc) = ghost_domain;
    }
  }

  for loc in sparse_ghost_set.locations {
    writeln(loc);
    writeln(sparse_ghost_set.domains(loc));
    writeln("");
  }


}