public use Grid_def;

private use IO;

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

  const grid: unmanaged Grid;
  var  value: [grid.extended_cells] real;



  //|\'''''''''''''|\
  //| >    this    | >
  //|/.............|/  
  //----------------------------------------------------
  // Provides an alias directly into the 'value' array.
  //----------------------------------------------------

  pragma "no copy return"
  proc this(D: domain)
  {
    return value(D);
  }
  // /|'''''''''''''/|
  //< |    this    < |
  // \|.............\|
  


  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit () {}
  
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
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
  var efmt:  string = "%26.16Er",
      ifmt:  string = "%5i";

  //==== Write time file ====
  outfile.writef(efmt+"    time\n", time);
  outfile.writef(ifmt+"                 meqn\n", meqn);
  outfile.writef(ifmt+"                 ngrids\n", ngrids);
  outfile.writef(ifmt+"                 naux\n", naux);
  outfile.writef(ifmt+"                 ndim\n", dimension);
  outfile.writef("\n");
  outfile.writef("\n");

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
  var efmt:  string = "%26.16Er",
      ifmt:  string = "%5i",
      linelabel: string;

  //==== Header ====
  outfile.writef(ifmt+"                 grid_number\n", grid_number);
  outfile.writef(ifmt+"                 AMR_level\n", AMR_level);


  //==== Write n_cells ====
  for d in dimensions do {
    select d {
      when 0 do linelabel = "                 mx\n";
      when 1 do linelabel = "                 my\n";
      when 2 do linelabel = "                 mz\n";
      otherwise linelabel = "                 mx(" + "%1i".format(d) + ")\n";
    }
    outfile.writef(ifmt+linelabel, grid.n_cells(d));
  }


  //==== Write x_low ====
  for d in dimensions do {
    select d {
      when 0 do linelabel = "    xlow\n";
      when 1 do linelabel = "    ylow\n";
      when 2 do linelabel = "    zlow\n";
      otherwise linelabel = "    xlow(" + "%1i".format(d) + ")\n";
    }
    outfile.writef(efmt+linelabel, grid.x_low(d));
  }


  //==== Write dx ====
  for d in dimensions do {
    select d {
      when 0 do linelabel = "    dx\n";
      when 1 do linelabel = "    dy\n";
      when 2 do linelabel = "    dz\n";
      otherwise linelabel = "    dx(" + "%1i".format(d) + ")\n";
    }
    outfile.writef(efmt+linelabel, grid.dx(d));
  }
  outfile.writeln("");


  //===> Write array values ===>
  if dimension == 0 then {
    for cell in grid.cells do
      outfile.writef(efmt+"\n", value(cell));
  }
  else {
    //------------------------------------------------------------
    // Transpose cells; iterating over the transpose in row major
    // order achieves column major order on the original domain.
    //------------------------------------------------------------
    var range_tuple: dimension*range(stridable=true);
    [d in dimensions with (ref range_tuple)] // could also be 'for param d'
      range_tuple(d) = grid.cells.dim(dimension - (d+1));

    var cells_transposed: domain(dimension, stridable=true);
    cells_transposed = {(...range_tuple)};

    var cell:       dimension*int;
    var n_newlines: int;
    for cell_transposed in cells_transposed do {

      //==== Write value ====
      [d in dimensions with (ref cell)] // could also be 'for param d'
        cell(d) = cell_transposed(dimension - (d+1));
      if abs(value(cell)) > 1.0e-99 then
        outfile.writef(efmt+"\n", value(cell));
      else
        outfile.writef(efmt+"\n", 0.0);

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
  var frame_string:  string = "%04i".format(frame_number),
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


