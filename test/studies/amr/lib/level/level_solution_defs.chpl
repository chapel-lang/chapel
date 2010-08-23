
use level_base_defs;
use level_array_defs;



//===> LevelSolution class ===>
//============================>
class LevelSolution {
  const level:    BaseLevel;

  var space_data: [1..2] LevelArray;
  var time:       [1..2] real;

  def initialize() {
    for i in [1..2] do
      space_data(i) = new LevelArray(level = level);
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
  sol:               LevelSolution,
  initial_condition: func(dimension*real, real),
  time:              real
){

  //==== Check that q lives on this level ====
  assert(sol.level == this);


  //==== Set each LevelArray to the initial condition ====
  for i in [1..2] {
    setLevelArray(sol.space_data(i), initial_condition);
    sol.time(i) = time;
  }
  //==== Set each LevelArray to the initial condition ====
  

}
//<=== BaseLevel.initializeSoution method <===
//<===========================================




//===> BaseLevel.writeSolution method ===>
//=======================================>
//---------------------------------------------------------
// Writes the data for a LevelSolution living on this level.
//---------------------------------------------------------
def BaseLevel.clawOutput(
  sol:            LevelSolution,
  frame_number: int
){

  //==== Make sure solution lives on this level ====
  assert(sol.level == this);

  //==== Use clawOutput for LevelArray ====
  clawOutput(sol.space_data(2), sol.time(2), frame_number);
  
}
//<=== BaseLevel.writeSolution method <===
//<=======================================