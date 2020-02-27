public use Level_def;
public use GridVariable_def;

private use IO;

//|\""""""""""""""""""""""""""""|\
//| >    LevelVariable class    | >
//|/____________________________|/

//---------------------------------------------------------------
// Similar to the GridVariable class, this represents a variable
// defined on the spatial region described by a Level.  This
// allows data to be stored on a union of rectangles.
//---------------------------------------------------------------

class LevelVariable {
  
  const level:        unmanaged Level;
  var grid_variables: [level.grids] unmanaged GridVariable?;



  //|\''''''''''''''''''''''''''''|\
  //| >    postinit() method    | >
  //|/............................|/
  
  //-----------------------------------------------------------
  // Needed instead of a constructor, so grid_variables can be
  // referred to level.grids by the default constructor.  The
  // intended constructor signature is
  //   LevelVariable(level: Level)
  //-----------------------------------------------------------

  proc postinit() {
    for grid in level.grids do
      grid_variables(grid) = new unmanaged GridVariable(grid = grid);                          
  }
  // /|''''''''''''''''''''''''''''/|
  //< |    postinit() method    < |
  // \|............................\|



  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/

  proc deinit ()
  {
    for grid_var in grid_variables do delete grid_var;
  }

  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|



  //|\'''''''''''''''''''''|\
  //| >    this methods    | >
  //|/.....................|/
  
  //---------------------------------------------------------------
  // A 'this' method lets the object be accessed as a function.
  // And, like any other function, it may be overloaded like we've
  // done here.
  //
  // A LevelVariable may be accessed by a Grid, in which case it
  // returns the associated GridVariable.  Or it may be accessed
  // by both a Grid and a subdomain of that grid, in which case
  // it provides a pointer directly into the relevant portion of
  // the GridVariable's 'value' field.
  //---------------------------------------------------------------
  
  proc this(grid: unmanaged Grid) ref {
    return grid_variables(grid);
  }

  pragma "no copy return"
  proc this(
    grid: unmanaged Grid, 
    D: domain(dimension, stridable=true)) 
  {
    return grid_variables(grid)!.value(D);
  }
  // /|'''''''''''''''''''''/|
  //< |    this methods    < |
  // \|.....................\|

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

proc LevelVariable.setToFunction(
  f: func(dimension*real, real)
){

  for grid in level.grids do
    grid_variables(grid)!.setToFunction(f);

}
// /|""""""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.setToFunction    < |
// \|____________________________________\|




//|\"""""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.fillOverlaps    | >
//|/___________________________________|/

//-----------------------------------------------------------
// This method fills all overlap regions, i.e. each grid's
// ghost cells that overlap with one of its neighbors.  The
// overlap regions have already been stored in the structure
// level.sibling_ghost_regions.
//
// Note how SiblingGhostRegion.these and LevelVariable.this have
// been defined to greatly simplify the syntax of this
// operation.
//-----------------------------------------------------------

proc LevelVariable.fillOverlaps ()
{
  
  for grid in level.grids {
    for (neighbor, region) in level.sibling_ghost_regions(grid)! do
      this(grid,region) = this(neighbor,region);
  }
  
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.fillOverlaps    < |
// \|___________________________________\|





//|\"""""""""""""""""""""""""""""""""""""|\
//| >    extrapolateGhostData methods    | >
//|/_____________________________________|/

//-----------------------------------------------------------------
// Fills the first layer of ghost cells with linearly extrapolated
// data from the interior.  Note that if there are more layers of
// ghost cells, they will be filled with incorrect values.
//
// This is used in a few settings to prevent the special treatment
// of boundary cases.
//-----------------------------------------------------------------

proc LevelVariable.extrapolateGhostData () {
  
  for grid_variable in grid_variables do
    grid_variable!.extrapolateGhostData();

}


proc GridVariable.extrapolateGhostData () {

  for ghost_domain in grid.ghost_domains {
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





//------------------------------------
// Only I/O methods below this line
//------------------------------------
// |   |   |   |   |   |   |   |   |
//\|/ \|/ \|/ \|/ \|/ \|/ \|/ \|/ \|/
// V   V   V   V   V   V   V   V   V


//|\"""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.clawOutput    | >
//|/_________________________________|/

//-----------------------------------------------------------------------
// Writes both a time file and a solution file for a given frame number.
//-----------------------------------------------------------------------

proc LevelVariable.clawOutput (
  time:         real,
  frame_number: int)
{

  //==== Names of output files ====
  var frame_string:      string = "%04i".format(frame_number),
      time_filename:     string = "_output/fort.t" + frame_string,
      solution_filename: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var n_grids = level.grids.numIndices;

  var outfile = open(time_filename, iomode.cw).writer();
  writeTimeFile(time, 1, n_grids, 0, outfile);
  outfile.close();
  
  
  //==== Solution file ====
  outfile = open(solution_filename, iomode.cw).writer();
  this.writeData(1, 1, outfile);  // AMR_level=1 and base_grid_number=1 for single-level output
  outfile.close();

}
// /|"""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.clawOutput    < |
// \|_________________________________\|





//|\""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.writeData    | >
//|/________________________________|/

//-----------------------------------------------------
// Writes out all data contained in the LevelVariable.
//-----------------------------------------------------

proc LevelVariable.writeData (
  AMR_level:        int,
  base_grid_number: int,
  outfile:          channel)
{

  var grid_number = base_grid_number;
  for grid in level.ordered_grids {
    grid_variables(grid)!.writeData(grid_number, AMR_level, outfile);
    outfile.writeln("  ");
    grid_number += 1;
  }

}
// /|""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.writeData    < |
// \|________________________________\|
