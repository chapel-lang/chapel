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

public use SpaceDimension;
public use LanguageExtensions;
public use BasicDataStructures;

private use IO;

//-----------------------------------------------------------
// Define an enum type to describe the relative position of
// an index to a range.  Tuples of these will describe how
// a multi-dimensional index (and potentially a full domain)
// is positioned relative to a full grid.
//-----------------------------------------------------------

enum  loc1d {below=-2, low=-1, inner=0, high=1, above=2};




//|\"""""""""""""""""""|\
//| >    Grid class    | >
//|/___________________|/

class Grid {
  
  const x_low, x_high:          dimension*real;
  const i_low, i_high:          dimension*int;
  const n_cells, n_ghost_cells: dimension*int;

  const dx: dimension*real;
          
  const cells:          domain(dimension, stridable=true);
  const extended_cells: domain(dimension, stridable=true);
  
  // const ghost_domains: MultiDomain(dimension, stridable=true);
  const ghost_domains: unmanaged List( domain(dimension, stridable=true) );



  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  
  proc init (
    x_low:         dimension*real,
    x_high:        dimension*real,
    i_low:         dimension*int,
    n_cells:       dimension*int,
    n_ghost_cells: dimension*int )
  {

    //==== Assign inputs to fields ====
    this.x_low         = x_low;
    this.x_high        = x_high;
    this.i_low         = i_low;
    this.n_cells       = n_cells;
    this.n_ghost_cells = n_ghost_cells;

    //==== dx ====
    dx = (x_high - x_low) / n_cells;

    //==== i_high ====
    for d in dimensions do
      i_high(d) = i_low(d) + 2*n_cells(d);

    //==== Physical cells ====
    var ranges: dimension*range(stridable = true);
    for d in dimensions
    {
      ranges(d) = (i_low(d)+1 .. by 2) #n_cells(d);
      ranges(d).alignHigh();
    }
    cells = ranges;


    //==== Extended cells (includes ghost cells) ====
    //---------------------------------------------------------------
    // The 'expand' method of an arithmetic domain extends both its
    // lower and upper bounds by the input.  In this case, the input
    // must be multiplied by 2 because a cell is 2 indices wide.
    //---------------------------------------------------------------
    extended_cells = cells.expand(2*n_ghost_cells);


    //===> Ghost cells ===>
    //-------------------------------------------------------------
    // Ghost cells split into chunks based on relative location to
    // the grid and stored in a linked list.
    //-------------------------------------------------------------

    ghost_domains = new unmanaged List( domain(dimension,stridable=true) );
    this.complete();

    //==== Sanity check ====
    sanityChecks();

    var inner_location: dimension*loc1d;
    for d in dimensions do inner_location(d) = loc1d.inner;

    var ghost_domain: domain(dimension, stridable=true);
    for loc in (loc1d.below .. loc1d.above by 2)**dimension {
      if loc != inner_location {
        for d in dimensions {
          if loc(d) == loc1d.below {
            var tmp = ((extended_cells.low(d).. by 2) #n_ghost_cells(d));
            ranges(d) = tmp.alignHigh();
          } else if loc(d) == loc1d.inner {
            ranges(d) = cells.dim(d);
          } else {
            // ((..extended_cells.high(d) by 2) #-n_ghost_cells(d)).alignLow();
            // hilde sez: Mathematical precision meets ease of use
            var tmp = ((..extended_cells.high(d) by 2 align extended_cells.high(d)) #-n_ghost_cells(d));
            ranges(d) = tmp.alignLow();
          }
        }
        ghost_domain = ranges;
        ghost_domains.add(ghost_domain);
      }
    }
    //<=== Ghost cells <===

  }
  // /|''''''''''''''''''''/|
  //< |    Initializer    < |
  // \|....................\|



  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/

  proc deinit () { delete ghost_domains; }
  
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|



  //|\''''''''''''''''''''''''''|\
  //| >   sanityChecks method   | >
  //|/..........................|/

  //--------------------------------------------------------------
  // Performs some basic sanity checks on the initializer inputs.
  //--------------------------------------------------------------

  proc sanityChecks () {
    var d_string: string;
    for d in dimensions do {
      d_string = "%i".format(d);

      assert(x_low(d) < x_high(d),
	     "error: Grid: x_low(" + d_string + ") must be strictly less than x_high(" + d_string + ").");

      assert(n_cells(d) > 0,
             "error: Grid: n_cells(" + d_string + ") must be positive.");

      assert(n_ghost_cells(d) > 0,
	     "error: Grid: n_ghost_cells(" + d_string + ") must be positive.");
    }
  }
  // /|'''''''''''''''''''''''''''/|
  //< |   sanityChecks method    < |
  // \|...........................\|



  //|\''''''''''''''''''''''''''''''''|\
  //| >    relativeLocation method    | >
  //|/................................|/
  
  proc relativeLocation(idx: dimension*int) {
    var loc: dimension*int;

    for d in dimensions {
           if idx(d) <  i_low(d)  then loc(d) = loc1d.below: int;
      else if idx(d) == i_low(d)  then loc(d) = loc1d.low: int;
      else if idx(d) <  i_high(d) then loc(d) = loc1d.inner: int;
      else if idx(d) == i_high(d) then loc(d) = loc1d.high: int;
      else                             loc(d) = loc1d.above: int;
    }

    return loc;
  }

  proc relativeLocation(D: domain(dimension, stridable=true)){
    var loc_low  = relativeLocation(D.low);
    var loc_high = relativeLocation(D.high);

    if loc_low == loc_high then
      return loc_low;
    else
      halt("error: Grid.locateDomain\n:" +
	   "Provided domain spans multiple location categories.");
  }
  // /|''''''''''''''''''''''''''''''''/|
  //< |    relativeLocation method    < |
  // \|................................\|



  //|\'''''''''''''''''''''''''|\
  //| >    writeThis method    | >
  //|/.........................|/
  
  //-----------------------------------------------------------
  // Defines the output of the intrinsic 'write' and 'writeln' 
  // procedures, so that write(Grid) will produce something
  // sensible.  Mainly for testing and debugging.
  //-----------------------------------------------------------
  
  proc writeThis (w) {
    writeln("x_low: ", x_low, ",  x_high: ", x_high);
    write("i_low: ", i_low, ",  i_high: ", i_high);
  }
  // /|'''''''''''''''''''''''''/|
  //< |    writeThis method    < |
  // \|.........................\|

}
// /|"""""""""""""""""""/|
//< |    Grid class    < |
// \|___________________\|







//|\"""""""""""""""""""""""""""|\
//| >    Grid.xValue method    | >
//|/___________________________|/

//-------------------------------------------------------------
// Converts an index to a coordinate tuple.  This is primarily
// used when evaluating an analyitical function on the Grid,
// as when setting up in initial condition.
//-------------------------------------------------------------

proc Grid.xValue (point_index: dimension*int) {

  var coord: dimension*real;

  if dimension == 1 then {
    coord(0) = x_low(0) + (point_index(0) - i_low(0)) * dx(0)/2.0;
  }
  else {
    for d in dimensions do
      coord(d) = x_low(d) + (point_index(d) - i_low(d)) * dx(d)/2.0;
  }

  return coord;
}
// /|"""""""""""""""""""""""""""/|
//< |    Grid.xValue method    < |
// \|___________________________\|






//|\"""""""""""""""""""""""""|\
//| >    readGrid routine    | >
//|/_________________________|/

//---------------------------------------------------------------
// Constructs a grid from an input file.  Used when implementing
// single-grid problems, but not for AMR.
//---------------------------------------------------------------

proc readGrid(file_name: string) {

  var input_file = open(file_name, iomode.r).reader();

  var dim_in: int;
  input_file.readln(dim_in);
  assert(dim_in == dimension, 
         "error: dimension of space.txt must equal " + "%i".format(dimension));
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


  return new unmanaged Grid(x_low, x_high, i_low, n_cells, n_ghost_cells);

}
// /|"""""""""""""""""""""""""/|
//< |    readGrid routine    < |
// \|_________________________\|




//|\"""""""""""""""""""""""""""""""|\
//| >    setOutputTimes routine    | >
//|/_______________________________|/

proc setOutputTimes (file_name: string) {

  var input_file = open(file_name, iomode.r).reader();

  var initial_time, final_time: real;
  var n_output: int;

  input_file.readln(initial_time);
  input_file.readln(final_time);
  input_file.readln(n_output);
  input_file.close();

  var output_times: [0..n_output] real;
  var dt_output:    real = (final_time - initial_time) / n_output;
  
  for i in 0..n_output do
    output_times(i) = initial_time + i*dt_output;

  return output_times;

}
// /|"""""""""""""""""""""""""""""""/|
//< |    setOutputTimes routine    < |
// \|_______________________________\|












// proc main {
// 
//   var x_low = (0.0,1.0);
//   var x_high = (2.0,3.0);
//   var i_low = (0,0);
//   var n_cells = (20,40);
//   var n_ghost_cells = (2,2);
// 
//   var grid = new Grid(x_low, x_high, i_low, n_cells, n_ghost_cells);
// 
//   writeln(grid);
//   writeln("grid.cells = ", grid.cells);
//   writeln("grid.extended_cells = ", grid.extended_cells);
// 
// 
// }
