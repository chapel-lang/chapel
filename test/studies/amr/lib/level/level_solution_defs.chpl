
use level_base_defs;
use grid_solution_defs;


//========================================================>
//==================== LEVEL SOLUTIONS ===================>
//========================================================>


//===> LevelSolution class ===>
//============================>
class LevelSolution {
  const level:    BaseLevel;

  var space_data: [1..2] LevelArray;
  var time:       [1..2] real;

  def initialize() {
    for i in [1..2] do
      space_data(i) = new LevelArray(level);
  }
}
//<=== LevelSolution class <===
//<============================





//===> BaseLevel.initializSolution method ===>
//===========================================>
//------------------------------------------------------------------
// The argument 'initializer' can be any object allowable by a used
// BaseGrid.initializeSolution method.
//------------------------------------------------------------------
def BaseLevel.initializeSolution(
  sol: LevelSolution,
  initial_condition: func(dimension*real, real)
){

  //==== Check that q lives on this level ====
  assert(sol.level == this);


  //==== Form LevelGridSolutions ====
  coforall grid in child_grids {

    write("Creating new GridSolution...");
    var grid_sol = new LevelGridSolution(grid   = grid, 
					 parent = q);
		write("done.\n");

    grid.initializeSolution(grid_sol, initializer);

    q.child_sols(grid) = grid_sol;
  }
  

}
//<=== BaseLevel.initializeSoution method <===
//<===========================================


