use Level_def;
use GridArray_def;


//|\"""""""""""""""""""""""""|\
//| >    LevelArray class    | >
//|/_________________________|/
class LevelArray {
  const level:        Level;
  var grid_arrays:    [level.grids] GridArray;
  var _promotionType: real;


  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    //==== Nothing to do ====
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
      grid_arrays(grid) = new GridArray(grid = grid);                          
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
    var pointer => grid_arrays(grid).value(D);
    return pointer;
  }
  // /|'''''''''''''''''''''''/|
  //< |    this() methods    < |
  // \|.......................\|


  //|\''''''''''''''''''''''''''|\
  //| >    these() iterators    | >
  //|/..........................|/
  //---------------------------------------------------------------------
  // Seems like it would be ideal to chain this with the leader/follower
  // for GridArrays.
  //---------------------------------------------------------------------
  def these() {
    for grid in level.grids {
      for cell in grid.cells do
	      yield grid_arrays(grid).value(cell);
    }
  }

  def these(param tag: iterator)
  where tag == iterator.leader {
    for grid in level.grids {
      for cell in grid.cells do
	yield (grid,cell);
    }
  }

  def these(param tag: iterator, follower) var
  where tag == iterator.follower {
    yield grid_arrays(follower(1)).value(follower(2));
  }
  // /|''''''''''''''''''''''''''/|
  //< |    these() iterators    < |
  // \|..........................\|


}
// /|"""""""""""""""""""""""""/|
//< |    LevelArray class    < |
// \|_________________________\|




//===> LevelArray assignment overloads ===>
//========================================>
def =(q: LevelArray, rvalue)
where rvalue.type != LevelArray && rvalue.type != real
{
  forall (x,y) in (q,rvalue) do
    x = y;
}

def =(q: LevelArray, rvalue: real) {
  forall x in q do
    x = rvalue;
}
//<=== LevelArray assignment overloads <===
//<========================================




//|\"""""""""""""""""""""""""""""""""|\
//| >    LevelArray.setToFunction    | >
//|/_________________________________|/
//-----------------------------------------------------------------
// Sets LevelArray.value to the analytical function f evaluated on
// the level.
//-----------------------------------------------------------------
def LevelArray.setToFunction(
  f: func(dimension*real, real)
){

  for grid in level.grids {
    grid_arrays(grid).setToFunction(f);
  }

}
// /|"""""""""""""""""""""""""""""""""/|
//< |    LevelArray.setToFunction    < |
// \|_________________________________\|




//|\""""""""""""""""""""""""""""""""""""""|\
//| >    LevelArray.fillOverlaps    | >
//|/______________________________________|/
def LevelArray.fillOverlaps() {
  
  for grid in level.grids {
    for (nbr, overlap) in level.sibling_overlaps(grid) do
      this(grid,overlap) = this(nbr,overlap);
  }
  
}
// /|""""""""""""""""""""""""""""""""""""""/|
//< |    LevelArray.fillOverlaps    < |
// \|______________________________________\|





//|\""""""""""""""""""""""""""""""|\
//| >    LevelArray.clawOutput    | >
//|/______________________________|/
//-----------------------------------------------------------------------
// Writes both a time file and a solution file for a given frame number.
//-----------------------------------------------------------------------
def LevelArray.clawOutput(
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
  write(1, 1, outfile);  // AMR_level=1 and base_grid_number=1 for single-level output
  outfile.close();
  delete outfile;

}
// /|""""""""""""""""""""""""""""""/|
//< |    LevelArray.clawOutput    < |
// \|______________________________\|





//|\"""""""""""""""""""""""""|\
//| >    LevelArray.write    | >
//|/_________________________|/
//--------------------------------------------------
// Writes out all data contained in the LevelArray.
//--------------------------------------------------
def LevelArray.write(
  AMR_level:        int,
  base_grid_number: int,
  outfile:          file)
{

  var grid_number = base_grid_number;
  for grid in level.ordered_grids {
    grid_arrays(grid).write(grid_number, AMR_level, outfile);
    outfile.writeln("  ");
    grid_number += 1;
  }

}
// /|"""""""""""""""""""""""""/|
//< |    LevelArray.write    < |
// \|_________________________\|






//|\"""""""""""""""""""""""""""""""""""""|\
//| >    extrapolateGhostData methods    | >
//|/_____________________________________|/
//-----------------------------------------------------------------
// Fills the first layer of ghost cells with linearly extrapolated
// data from the interior.  Note that if there are more layers of
// ghost cells, they will be filled with incorrect values.
//-----------------------------------------------------------------
def LevelArray.extrapolateGhostData() {
  
  for grid_array in grid_arrays do
    grid_array.extrapolateGhostData();

}


def GridArray.extrapolateGhostData() {

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