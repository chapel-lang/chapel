use grid_base_defs;



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




//===> GridArray.setToFunction method ===>
//=======================================>
def BaseGrid.setGridArray(
  q: GridArray,
  f: func(dimension*real, real)
){

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
