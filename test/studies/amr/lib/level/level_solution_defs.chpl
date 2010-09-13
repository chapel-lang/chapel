
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





//===> LevelSolution.setToFunction method ===>
//===========================================>
def LevelSolution.setToFunction(
  initial_condition: func(dimension*real, real),
  time_in:           real
){

  //==== Set each LevelArray to the initial condition ====
  for i in [1..2] {
    space_data(i).setToFunction(initial_condition);
    time(i) = time_in;
  }
  

}
//<=== LevelSolution.setToFunction method <===
//<===========================================




//===> LevelSolution.clawOutput method ===>
//========================================>
def LevelSolution.clawOutput(
  frame_number: int
){


  //==== Use clawOutput for LevelArray ====
  space_data(2).clawOutput(time(2), frame_number);
  
}
//<=== LevelSolution.clawOutput method <===
//<========================================