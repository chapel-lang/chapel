use grid_base_defs;
use grid_array_defs;


//===> ScalarGridSolution class ===>
//=================================>
class ScalarGridSolution {
  const grid: BaseGrid;

  var space_data: [1..2] GridArray;
  var time: [1..2] real;

  def initialize() {
    for i in [1..2] do
      space_data(i) = new GridArray(grid = grid);
  }
}
//<=== ScalarGridSolution class <===
//<=================================




//===> BaseGrid.initializeSolution method ===>
//===========================================>
//---------------------------------------------------------------
// Provided an analytical function, evaluates it on the grid and
// returns a GridSolution.  As support for first-class functions
// develops, the input argument will become explicitly typed.
//---------------------------------------------------------------
def BaseGrid.initializeSolution(
  sol:               ScalarGridSolution,
  initial_condition: func(dimension*real, real),
  time:              real
){

  //==== Check that q lives on this grid ====
  assert(sol.grid == this);


  //===> Evaluate and store initial_condition ===>
  write("Writing solution on grid...");
  for i in [1..2] {
    setGridArray(sol.space_data(i), initial_condition);
    sol.time(i) = time;
  }
  write("done.\n");
  //<=== Evaluate and store initial_condition <===


}
//<=== BaseGrid.initializeSolution method <===
//<===========================================




//===> clawOutput method ===>
//==========================>
//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridSolution, at the given
// frame_number.
//-------------------------------------------------------------------
def BaseGrid.clawOutput(
  sol:          ScalarGridSolution,
  frame_number: int
){

  //==== Make sure solution lives on this grid ====
  assert(sol.grid == this);

  //==== Use clawOutput method for GridArray ====
  clawOutput(sol.space_data(2), sol.time(2), frame_number);
}
//<=== clawOutput method <===
//<==========================