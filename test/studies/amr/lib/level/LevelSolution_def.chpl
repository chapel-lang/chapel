public use Level_def;
public use LevelVariable_def;



//|\""""""""""""""""""""""""""""|\
//| >    LevelSolution class    | >
//|/____________________________|/
class LevelSolution {
  const level:    unmanaged Level;

  var old_data:     unmanaged LevelVariable;
  var current_data: unmanaged LevelVariable;
  var old_time:     real;
  var current_time: real;



  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  proc init(level: unmanaged Level) {
    this.level  = level;
    old_data     = new unmanaged LevelVariable(level = level);
    current_data = new unmanaged LevelVariable(level = level);
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
