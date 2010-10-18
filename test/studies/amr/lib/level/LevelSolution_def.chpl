use Level_def;
use LevelArray_def;



//|\""""""""""""""""""""""""""""|\
//| >    LevelSolution class    | >
//|/____________________________|/
class LevelSolution {
  const level:    Level;

  var old_data:     LevelArray;
  var current_data: LevelArray;
  var old_time:     real;
  var current_time: real;


  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    delete old_data;
    delete current_data;
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|


  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def LevelSolution(level: Level) {
    this.level  = level;
    old_data     = new LevelArray(level = level);
    current_data = new LevelArray(level = level);
  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|

}
// /|""""""""""""""""""""""""""""/|
//< |    LevelSolution class    < |
// \|____________________________\|




//|\""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.setToFunction    | >
//|/____________________________________|/
def LevelSolution.setToFunction(
  initial_condition: func(dimension*real, real),
  time_in:           real
){

  //==== Set each LevelArray to the initial condition ====
  old_data.setToFunction(initial_condition);
  old_time = time_in;
  
  current_data.setToFunction(initial_condition);
  current_time = time_in;

}
// /|""""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.setToFunction    < |
// \|____________________________________\|




//|\"""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.clawOutput    | >
//|/_________________________________|/
def LevelSolution.clawOutput(
  frame_number: int
){

  //==== Use clawOutput for LevelArray ====
  current_data.clawOutput(current_time, frame_number);
  
}
// /|"""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.clawOutput    < |
// \|_________________________________\|