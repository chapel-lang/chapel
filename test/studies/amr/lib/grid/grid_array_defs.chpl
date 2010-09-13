use grid_base_defs;


//====================================================>
//==================== GRID ARRAYS ===================>
//====================================================>


//|"""""""""""""""""""""""\
//|===> GridArray class ===>
//|_______________________/
class GridArray {

  const grid:         Grid;
  var value:          [grid.ext_cells] real;
  var _promotionType: real;


  //|------------------------*
  //|===> these() iterator ===>
  //|------------------------*
  //--------------------------------------------------------------------
  // Iterates over interior values.  The aim is to allow arithmetic
  // operations without directly accessing the 'value' field, such as
  // q += 2.0, or q = q1 + q2.
  //--------------------------------------------------------------------
  // Brad mentioned that there ought to be a way to use the default
  // leader/follower iterators for arithmetic domains.  For now, though
  // this is acceptably fast given the simplicity of the code.
  //--------------------------------------------------------------------
  def these() {
    for cell in grid.cells do
      yield value(cell);
  }

  def these(param tag: iterator) 
  where tag == iterator.leader {
    forall cell in grid.cells do
      yield cell;
  }

  def these(param tag: iterator, follower) var
  where tag == iterator.follower {
    yield value(follower);
  }
  // *------------------------|
  //<=== these() iterator <===|
  // *------------------------|

}
// /"""""""""""""""""""""""|
//<=== GridArray class <===|
// \_______________________|




//|--------------------------------------*
//|===> GridArray assignment overloads ===>
//|--------------------------------------*
def =(q: GridArray, rvalue) 
  where rvalue.type != GridArray && rvalue.type != real
{
  forall (x,y) in (q,rvalue) do x = y;
}

def =(q: GridArray, rvalue: real) {
  forall x in q do x = rvalue;
}
// *--------------------------------------|
//<=== GridArray assignment overloads <===|
// *--------------------------------------|




//|--------------------------------------*
//|===> GridArray.setToFunction method ===>
//|--------------------------------------*
def GridArray.setToFunction(
  f: func(dimension*real, real)
){

  //==== Evaluate and store f(x) ====
  forall precell in grid.cells {
    var cell = tuplify(precell);
    value(cell) = f(grid.xValue(cell));
  }
}
// *--------------------------------------|
//<=== GridArray.setToFunction method <===|
// *--------------------------------------|


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
def writeTimeFile(
  time:    real,
  meqn:    int,
  ngrids:  int,
  naux:    int,
  outfile: file)
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
//<=== writeTimeFile method <=====
//<===============================




//===> GridArray.write method ===>
//===============================>
def GridArray.write(
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
    [d in dimensions] range_tuple(d) = grid.cells.dim(1 + dimension - d);

    var cells_transposed: domain(dimension, stridable=true);
    cells_transposed = [(...range_tuple)];

    var cell:       dimension*int;
    var n_newlines: int;
    for cell_transposed in cells_transposed do {

      //==== Write value ====
      [d in dimensions] cell(d) = cell_transposed(1 + dimension - d);
      outfile.writeln(format(efmt, value(cell)));

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

      for n in [1..n_newlines] do outfile.writeln("  ");
      //<=== Newlines at the end of each dimension <===
    }
  }
  //<=== Write array values <===
  
}
//<=== GridArray.write method <===
//<===============================




//===> GridArray.clawOutput method ===>
//====================================>
//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridArray, at the given 
// time and frame_number.
//-------------------------------------------------------------------
def GridArray.clawOutput(
  time:         real,
  frame_number: int
){

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
  write(1, 1, outfile);
  outfile.close();
  delete outfile;

}
//<=== GridArray.clawOutput method <===
//<====================================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================
