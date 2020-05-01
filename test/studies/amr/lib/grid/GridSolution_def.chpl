public use Grid_def;
public use GridVariable_def;


//|\"""""""""""""""""""""""""""|\
//| >    GridSolution class    | >
//|/___________________________|/

//------------------------------------------------------------
// Approximates the solution to a time-dependent PDE defined
// on a Grid.  Spatial data is stored at two time levels (old
// and current).  Each level of spatial data is stored in a
// GVariable.
//------------------------------------------------------------

class GridSolution {

  const grid:     unmanaged Grid;
  
  var old_data:     unmanaged GridVariable;
  var current_data: unmanaged GridVariable;
  var old_time:     real;
  var current_time: real;
  
  

  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  
  proc init (grid: unmanaged Grid) {
    this.grid = grid;
    old_data =     new unmanaged GridVariable(grid = grid);
    current_data = new unmanaged GridVariable(grid = grid);
  }
  // /|''''''''''''''''''''/|
  //< |    initializer    < |
  // \|....................\|
  
  
  
  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit () 
  {
    delete old_data;
    delete current_data;
  }
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|
  
  
}
// /|"""""""""""""""""""""""""""/|
//< |    GridSolution class    < |
// \|___________________________\|





//|\"""""""""""""""""""""""""""""""""""|\
//| >    GridSolution.setToFunction    | >
//|/___________________________________|/

//------------------------------------------------------------
// Sets both old_data and current_data equal to the provided 
// function initial_condition.  This is meant to be used
// when setting up the initial condition for a PDE.
//
// For efficiency, it would probably be better to have a flag
// indicating that only one time point worth of data is
// currently stored, so that the values generated here would
// only need to be stored once.  But for the time being, the
// current behavior gets the point across.
//------------------------------------------------------------

proc GridSolution.setToFunction (
  initial_condition: func(dimension*real, real),
  time_in:           real)
{

  //===> Evaluate and store initial_condition ===>
  write("Writing solution on grid...");

  old_data.setToFunction(initial_condition);
  old_time = time_in;
  
  current_data(grid.cells) = old_data(grid.cells);
  current_time = time_in;
  write("done.\n");
  //<=== Evaluate and store initial_condition <===


}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    GridSolution.setToFunction    < |
// \|___________________________________\|




//|\""""""""""""""""""""""""""""""""|\
//| >    GridSolution.clawOutput    | >
//|/________________________________|/

//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridSolution, at the given
// frame_number.
//-------------------------------------------------------------------

proc GridSolution.clawOutput(
  frame_number: int
){

  //==== Use clawOutput method for GridVariable ====
  current_data.clawOutput(current_time, frame_number);

}
// /|""""""""""""""""""""""""""""""""/|
//< |    GridSolution.clawOutput    < |
// \|________________________________\|
