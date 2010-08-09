use grid_base_defs;


//===> ScalarGridSolution class ===>
//=================================>
class ScalarGridSolution {
  const grid: RectangularGrid;

  var space_data: [1..2] [grid.ext_cells] real;
  var time: [1..2] real;
}
//<=== ScalarGridSolution class <===
//<=================================



//===> VectorGridSolution class ===>
//=================================>
class VectorGridSolution {
  param n_comps: int;
  const grid: RectangularGrid;
  const components = [1..n_comps];

  var space_data: [1..2] [grid.ext_cells] [components] real;
  var time: [1..2] real;
}
//<=== VectorGridSolution class <===
//<=================================



/* //===> ScalarSingleTimeGridSolution class ===> */
/* //===========================================> */
/* class ScalarSingleTimeGridSolution { */
/*   const grid: RectangularGrid; */
  
/*   var value:  [grid.ext_cells] real; */
/*   var time:   real; */
/* } */
/* //<=== ScalarSingleTimeGridSolution class ==== */
/* //<=========================================== */




/* //===> ScalarMultiTimeGridSolution class ===> */
/* //==========================================> */
/* class ScalarMultiTimeGridSolution { */
/*   const grid:      RectangularGrid; */
/*   const n_times:   int; */

/*   var time_layers: [1..n_times] ScalarSingleTimeGridSolution; */

/*   def initialize() { */
/*     for layer in time_layers do */
/*       layer = new ScalarSingleTimeGridSolution(grid = grid); */
/*   } */
/* } */
/* //<=== ScalarMultiTimeGridSolution class <=== */
/* //<========================================== */




/* //===> VectorSingleTimeGridSolution class ===> */
/* //===========================================> */
/* class VectorSingleTimeGridSolution { */
/*   const grid:    RectangularGrid; */
/*   const n_comps: int; */
/*   const components = [1..n_comps]; */
  
/*   var value: [grid.ext_cells] ([components] real); */
/*   var time; */
/* } */
/* //<=== VectorSingleTimeGridSolution class <=== */
/* //<=========================================== */




/* //===> VectorMultiTimeGridSolution class ===> */
/* //======================================> */
/* class VectorMultiTimeGridSolution { */
/*   const grid:    RectangularGrid; */
/*   const n_times: int; */
/*   const n_comps: int; */
  
/*   var time_layers: [1..n_times] ScalarSingleTimeGridSolution; */
  
/*   def initialize() { */
/*     for layer in time_layers do */
/*       layer = new VectorMultiTimeGridSolution(grid    = grid, */
/*                                               n_comps = n_comps); */
/*   } */
/* } */
/* //<=== VectorMultiTimeGridSolution class <=== */
/* //<========================================== */





// //===> copySolution method ===>
// //============================>
// def RectangularGrid.copySolution(q: GridSolution) {
//   
//   //==== Make sure q lives on this grid ====
//   assert(q.grid == this);
// 
//   
//   //==== Copy q ====
//   var q_copy = new GridSolution(grid  = this,
//                                 value = q.value,
//                                 time  = q.time);
// 
// }
// //<=== copySolution method <===
// //<============================




//===> initializeSolution method ===>
//==================================>
//---------------------------------------------------------------
// Provided an analytical function, evaluates it on the grid and
// returns a GridSolution.  As support for first-class functions
// develops, the input argument will become explicitly typed.
//---------------------------------------------------------------
def RectangularGrid.initializeSolution(
  sol:               ScalarGridSolution,
  initial_condition: func(dimension*real, real),
  time:              real
){

  //==== Check that q lives on this grid ====
  assert(sol.grid == this);


  //===> Evaluate and store initial_condition ===>
  write("Writing solution on grid...");
  forall precell in cells {
    var cell = tuplify(precell);
    sol.space_data(1)(cell) = initial_condition(xValue(cell));
    sol.space_data(2)(cell) = sol.space_data(1)(cell);
  }
  write("done.\n");
  //<=== Evaluate and store initial_condition <===
  
  
  //==== Set sol.time (note this is setting 2 values) ====
  sol.time = time;

}


//-------------------------------------------------------------------
// A similar method that does this for a TrueSolution is convenient.
//-------------------------------------------------------------------
def RectangularGrid.initializeSolution(
  sol:             ScalarGridSolution,
  true_solution: TrueSolution,
  time:          real
){

  //==== Check that q lives on this grid ====
  assert(sol.grid == this);


  //==== Evaluate and store true_solution at input time ====
  write("Writing solution on grid...");
  forall precell in cells {
    var cell = tuplify(precell);
    sol.space_data(1)(cell) = true_solution.qTrue(xValue(cell), time);
    sol.space_data(2)(cell) = sol.space_data(1)(cell);
  }
  write("done.\n");
  
  
  //==== Set sol.time (both values) ====
  sol.time = time;

}

//<==================================
//<=== initializeSolution method <===






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




//===> clawOutput method ===>
//==========================>
//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridSolution, at the given 
// frame_number.
//-------------------------------------------------------------------
def RectangularGrid.clawOutput(sol:            ScalarGridSolution,
                               frame_number: int
                              ){

  //==== Make sure solution lives on this grid ====
  assert(sol.grid == this);


  //==== Names of output files ====
  var frame_string:      string = format("%04i", frame_number),
      time_filename:     string = "_output/fort.t" + frame_string,
      solution_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var outfile = new file(time_filename, FileAccessMode.write);
  outfile.open();
  writeTimeFile(sol.time(2), 1, 1, 0, outfile);
  outfile.close();
  delete outfile;
  
  
  //==== Solution file ====
  outfile = new file(solution_filename, FileAccessMode.write);
  outfile.open();
  writeSolution(sol, 1, 1, outfile);
  outfile.close();
  delete outfile;

}
//<=== clawOutput method <===
//<==========================




//===> writeSolution method ====>
//==============================>
//---------------------------------------------------------
// Writes the data for a GridSolution living on this grid.
//---------------------------------------------------------
def RectangularGrid.writeSolution(sol:           ScalarGridSolution, 
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


  //===> Write solution values ===>
  if dimension == 1 then {
    for cell in cells do
      outfile.writeln(format(efmt, sol.space_data(2)(cell)));
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
      outfile.writeln(format(efmt, sol.space_data(2)(cell)));

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
  //<=== Write solution values <===
  
}
//<=== writeSolution method <===
//<=============================


//<=======================================================
//<=================== OUTPUT METHODS ====================
//<=======================================================