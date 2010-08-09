
use level_base_defs;
use grid_solution_defs;


//========================================================>
//==================== LEVEL SOLUTIONS ===================>
//========================================================>


//===> LevelSolution class ===>
//============================>
class LevelSolution {
  var level:      BaseLevel;                              // parent level
  var child_sols: [level.child_grids] LevelGridSolution;  // child functions
  var time:       real;                                   // duh
}
//<=== LevelSolution class <===
//<============================




//===> LevelGridSolution derived class ===>
//========================================>
class LevelGridSolution: GridSolution {

  var parent: LevelSolution;

}
//<=== LevelGridSolution derived class <===
//<========================================




//===> BaseLevel.initializSolution method ===>
//===========================================>
//--------------------------------------------------------------------
// The argument 'initializer' can be either a first-class function
// or a TrueSolution.
//--------------------------------------------------------------------
def BaseLevel.initializeSolution(q: LevelSolution,
				 initializer
				){

  //==== Check that q lives on this level ====
  assert(q.level == this);


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


