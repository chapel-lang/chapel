use Level_def;
use LevelVariable_def;



//|\""""""""""""""""""""""""""""|\
//| >    LevelSolution class    | >
//|/____________________________|/
class LevelSolution {
  const level:    Level;

  var old_data:     LevelVariable;
  var current_data: LevelVariable;
  var old_time:     real;
  var current_time: real;



  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  proc init(level: Level) {
    this.level  = level;
    old_data     = new LevelVariable(level = level);
    current_data = new LevelVariable(level = level);
  }
  // /|''''''''''''''''''''/|
  //< |    initializer    < |
  // \|....................\|



  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit()
  {
    delete old_data;
    delete current_data;
  }
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|

}
// /|""""""""""""""""""""""""""""/|
//< |    LevelSolution class    < |
// \|____________________________\|




//|\""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.setToFunction    | >
//|/____________________________________|/
proc LevelSolution.setToFunction(
  initial_condition: func(dimension*real, real),
  time_in:           real)
{

  //==== Set each LevelVariable to the initial condition ====
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
proc LevelSolution.clawOutput(
  frame_number: int
){

  //==== Use clawOutput for LevelVariable ====
  current_data.clawOutput(current_time, frame_number);
  
}
// /|"""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.clawOutput    < |
// \|_________________________________\|