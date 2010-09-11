use grid_base_defs;
use grid_array_defs;


//|""""""""""""""""""""""""""\
//|===> GridSolution class ===>
//|__________________________/
class GridSolution {

  const grid:     Grid;
  var space_data: [1..2] GridArray;
  var time:       [1..2] real;

  //==== Constructor ====
  def GridSolution(grid: Grid) {
    this.grid = grid;
    for i in [1..2] do
      space_data(i) = new GridArray(grid = grid);
  }
}
// /""""""""""""""""""""""""""|
//<=== GridSolution class <===|
// \__________________________|





//-----------------------------------------*
//===> GridSolution.setToFunction method ===>
//-----------------------------------------*
//----------------------------------------------------------
// Sets both time levels to a particular analytic function.
//----------------------------------------------------------
def GridSolution.setToFunction(
  initial_condition: func(dimension*real, real),
  time_in:           real
){

  //===> Evaluate and store initial_condition ===>
  write("Writing solution on grid...");
  for i in [1..2] {
    space_data(i).setToFunction(initial_condition);
    time(i) = time_in;
  }
  write("done.\n");
  //<=== Evaluate and store initial_condition <===


}
// *-----------------------------------------|
//<=== GridSolution.setToFunction method <===|
// *-----------------------------------------|




//===> GridSolution.clawOutput method ===>
//=======================================>
//-------------------------------------------------------------------
// Writes Clawpack-formatted output for a GridSolution, at the given
// frame_number.
//-------------------------------------------------------------------
def GridSolution.clawOutput(
  frame_number: int
){

  //==== Use clawOutput method for GridArray ====
  space_data(2).clawOutput(time(2), frame_number);

}
//<=== GridSolution.clawOutput method <===
//<=======================================