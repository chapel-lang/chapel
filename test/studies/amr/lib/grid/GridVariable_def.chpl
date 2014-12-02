use Grid_def;



//|\"""""""""""""""""""""""""""|\
//| >    GridVariable class    | >
//|/___________________________|/
//------------------------------------------------------------
// This class is represents a variable defined on the spatial 
// region described by a Grid.  Note that any number of
// GridVariables may be defined on a given Grid, as the Grid
// class just describes the region of space, and not any
// associated storage.
//
// The GridVariable class is used, as opposed to simply an
// array defined on Grid.cells, for two reasons:
//   (1) This allows the definition of a LevelVariable, as
//       (mostly) an array of GridVariables.
//   (2) Better organization of code, as the algorithms for
//       various numerical solvers can be sensibly defined
//       as methods of class GridVariable.
//------------------------------------------------------------

class GridVariable {

  const grid: Grid;
  var  value: [grid.extended_cells] real;



  //|\'''''''''''''|\
  //| >    this    | >
  //|/.............|/  
  //----------------------------------------------------
  // Provides an alias directly into the 'value' array.
  //----------------------------------------------------

  proc this(D: domain) ref
  {
    var alias => value(D);
    return alias;
  }
  // /|'''''''''''''/|
  //< |    this    < |
  // \|.............\|
  


  //|\'''''''''''''''''''|\
  //| >    destructor    | >
  //|/...................|/
  
  proc ~GridVariable () {}
  
  // /|'''''''''''''''''''/|
  //< |    destructor    < |
  // \|...................\|

}
// /|"""""""""""""""""""""""""""/|
//< |    GridVariable class    < |
// \|___________________________\|



//|\"""""""""""""""""""""""""""""""""""|\
//| >    GridVariable.setToFunction    | >
//|/___________________________________|/

//-------------------------------------------------------
// Sets the value to a specified function f, as would be
// done when setting up the initial condition for a PDE.
//-------------------------------------------------------

proc GridVariable.setToFunction(
  f: func(dimension*real, real)
){

  //==== Evaluate and store f(x) ====
  forall precell in grid.cells {
    var cell = tuplify(precell);
    value(cell) = f(grid.xValue(cell));
  }
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    GridVariable.setToFunction    < |
// \|___________________________________\|






//------------------------------------
// Only I/O methods below this line
//------------------------------------
// |   |   |   |   |   |   |   |   |
//\|/ \|/ \|/ \|/ \|/ \|/ \|/ \|/ \|/
// V   V   V   V   V   V   V   V   V



//|\""""""""""""""""""""""""""""""|\
//| >    writeTimeFile routine    | >
//|/______________________________|/

//-------------------------------------------------------------
// Global routine; meant to be called by output method of any
// spatial object.  The time file is really simple, and this
// just formats a few parameters.
//-------------------------------------------------------------

proc writeTimeFile(
  time:    real,
  meqn:    int,
  ngrids:  int,
  naux:    int,
  outfile: channel)
{

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
// /|""""""""""""""""""""""""""""""/|
//< |    writeTimeFile routine    < |
// \|______________________________\|




//|\"""""""""""""""""""""""""""""""|\
//| >    GridVariable.writeData    | >
//|/_______________________________|/

proc GridVariable.writeData (
  grid_number: int,
  AMR_level:   int,
  outfile: channel)
{

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
    outfile.writeln( format(ifmt, grid.n_cells(d)),  linelabel);
  }


  //==== Write x_low ====
  for d in dimensions do {
    select d {
      when 1 do linelabel = "    xlow";
      when 2 do linelabel = "    ylow";
      when 3 do linelabel = "    zlow";
      otherwise linelabel = "    xlow(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(efmt, grid.x_low(d)),  linelabel);
  }


  //==== Write dx ====
  for d in dimensions do {
    select d {
      when 1 do linelabel = "    dx";
      when 2 do linelabel = "    dy";
      when 3 do linelabel = "    dz";
      otherwise linelabel = "    dx(" + format("%1i",d) + ")";
    }
    outfile.writeln( format(efmt, grid.dx(d)),  linelabel);
  }
  outfile.writeln("");


  //===> Write array values ===>
  if dimension == 1 then {
    for cell in grid.cells do
      outfile.writeln(format(efmt, value(cell)));
  }
  else {
    //------------------------------------------------------------
    // Transpose cells; iterating over the transpose in row major
    // order achieves column major order on the original domain.
    //------------------------------------------------------------
    var range_tuple: dimension*range(stridable=true);
    [d in dimensions with (ref range_tuple)] // could also be 'for param d'
      range_tuple(d) = grid.cells.dim(1 + dimension - d);

    var cells_transposed: domain(dimension, stridable=true);
    cells_transposed = {(...range_tuple)};

    var cell:       dimension*int;
    var n_newlines: int;
    for cell_transposed in cells_transposed do {

      //==== Write value ====
      [d in dimensions with (ref cell)] // could also be 'for param d'
        cell(d) = cell_transposed(1 + dimension - d);
      if abs(value(cell)) > 1.0e-99 then
        outfile.writeln(format(efmt, value(cell)));
      else
        outfile.writeln(format(efmt, 0.0));

      //===> Newlines at the end of each dimension ===>
      //--------------------------------------------------------------
      // There are no newlines at the very end of output.  How that's
      // handled is up to the main output routine.
      //--------------------------------------------------------------
      n_newlines = 0;

      for d in dimensions do {
        if cell(d) == grid.cells.high(d) then
          n_newlines += 1;
        else
          break;
      }

      if n_newlines == dimension then n_newlines = 0;

      for n in {1..n_newlines} do outfile.writeln("  ");
      //<=== Newlines at the end of each dimension <===
    }
  }
  //<=== Write array values <===
  
}
// /|"""""""""""""""""""""""""""""""/|
//< |    GridVariable.writeData    < |
// \|_______________________________\|



//|\""""""""""""""""""""""""""""""""|\
//| >    GridVariable.clawOutput    | >
//|/________________________________|/

//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridVariable, at the given 
// time and frame_number.
//-------------------------------------------------------------------

proc GridVariable.clawOutput(
  time:         real,
  frame_number: int)
{

  //==== Names of output files ====
  var frame_string:  string = format("%04i", frame_number),
      time_filename: string = "_output/fort.t" + frame_string,
      data_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var outfile = open(time_filename, iomode.cw).writer();
  writeTimeFile(time, 1, 1, 0, outfile);
  outfile.close();
  
  
  //==== Data file ====
  outfile = open(data_filename, iomode.cw).writer();
  this.writeData(1, 1, outfile);
  outfile.close();

}
// /|""""""""""""""""""""""""""""""""/|
//< |    GridVariable.clawOutput    < |
// \|________________________________\|


