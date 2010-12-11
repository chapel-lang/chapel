use Level_def;
use GridVariable_def;


//|\""""""""""""""""""""""""""""|\
//| >    LevelVariable class    | >
//|/____________________________|/

//---------------------------------------------------------------
// Similar to the GridVariable class, this represents a variable
// defined on the spatial region described by a Level.  This
// allows data to be stored on a union of rectangles.
//---------------------------------------------------------------

class LevelVariable {
  
  const level:     Level;
  var grid_arrays: [level.grids] GridVariable;


  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    // Nothing to do
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|


  //|\''''''''''''''''''''''''''''|\
  //| >    initialize() method    | >
  //|/............................|/
  //--------------------------------------------------------
  // Needed instead of a constructor, so grid_arrays can be
  // referred to level.grids by the default constructor.
  //--------------------------------------------------------
  def initialize() {
    for grid in level.grids do
      grid_arrays(grid) = new GridVariable(grid = grid);                          
  }
  // /|''''''''''''''''''''''''''''/|
  //< |    initialize() method    < |
  // \|............................\|


  //|\'''''''''''''''''''''''|\
  //| >    this() methods    | >
  //|/.......................|/
  def this(grid: Grid) var {
    return grid_arrays(grid);
  }

  def this(
    grid: Grid, 
    D: domain(dimension, stridable=true)) 
  var {
    var alias => grid_arrays(grid).value(D);
    return alias;
  }
  // /|'''''''''''''''''''''''/|
  //< |    this() methods    < |
  // \|.......................\|


}
// /|""""""""""""""""""""""""""""/|
//< |    LevelVariable class    < |
// \|____________________________\|









//|\""""""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.setToFunction    | >
//|/____________________________________|/

//-------------------------------------------------------
// Sets the value to a specified function f, as would be
// done when setting up the initial condition for a PDE.
//-------------------------------------------------------

def LevelVariable.setToFunction(
  f: func(dimension*real, real)
){

  for grid in level.grids {
    grid_arrays(grid).setToFunction(f);
  }

}
// /|"""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.setToFunction    < |
// \|_________________________________\|




//|\""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.fillOverlaps    | >
//|/________________________________|/

def LevelVariable.fillOverlaps () {
  
  for grid in level.grids {
    for (nbr, overlap) in level.sibling_overlaps(grid) do
      this(grid,overlap) = this(nbr,overlap);
  }
  
}
// /|""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.fillOverlaps    < |
// \|________________________________\|





//|\"""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.clawOutput    | >
//|/_________________________________|/

//-----------------------------------------------------------------------
// Writes both a time file and a solution file for a given frame number.
//-----------------------------------------------------------------------

def LevelVariable.clawOutput(
  time:         real,
  frame_number: int)
{

  //==== Names of output files ====
  var frame_string:      string = format("%04i", frame_number),
      time_filename:     string = "_output/fort.t" + frame_string,
      solution_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var n_grids = level.grids.numIndices;

  var outfile = new file(time_filename, FileAccessMode.write);
  outfile.open();  
  writeTimeFile(time, 1, n_grids, 0, outfile);
  outfile.close();
  delete outfile;
  
  
  //==== Solution file ====
  outfile = new file(solution_filename, FileAccessMode.write);
  outfile.open();
  this.writeData(1, 1, outfile);  // AMR_level=1 and base_grid_number=1 for single-level output
  outfile.close();
  delete outfile;

}
// /|"""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.clawOutput    < |
// \|_________________________________\|





//|\""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.writeData    | >
//|/________________________________|/

//--------------------------------------------------
// Writes out all data contained in the LevelVariable.
//--------------------------------------------------

def LevelVariable.writeData(
  AMR_level:        int,
  base_grid_number: int,
  outfile:          file)
{

  var grid_number = base_grid_number;
  for grid in level.ordered_grids {
    grid_arrays(grid).writeData(grid_number, AMR_level, outfile);
    outfile.writeln("  ");
    grid_number += 1;
  }

}
// /|""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.writeData    < |
// \|________________________________\|






//|\"""""""""""""""""""""""""""""""""""""|\
//| >    extrapolateGhostData methods    | >
//|/_____________________________________|/

//-----------------------------------------------------------------
// Fills the first layer of ghost cells with linearly extrapolated
// data from the interior.  Note that if there are more layers of
// ghost cells, they will be filled with incorrect values.
//-----------------------------------------------------------------

def LevelVariable.extrapolateGhostData () {
  
  for grid_array in grid_arrays do
    grid_array.extrapolateGhostData();

}


def GridVariable.extrapolateGhostData () {

  for ghost_domain in grid.ghost_multidomain {
    var loc = grid.relativeLocation(ghost_domain);
    var shift = -1*loc;

    forall cell in ghost_domain {
      value(cell) = 2.0*value(cell+shift) - value(cell+2*shift);
    }
  }

}
// /|"""""""""""""""""""""""""""""""""""""/|
//< |    extrapolateGhostData methods    < |
// \|_____________________________________\|