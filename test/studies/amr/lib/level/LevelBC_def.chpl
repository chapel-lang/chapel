use Level_def;
use LevelVariable_def;


//===> LevelBC class ===>
//======================>
class LevelBC {
  
  const level: Level;

  proc apply(q: LevelVariable, t: real) {}
  proc apply_Homogeneous(q: LevelVariable) {}

}
//<=== LevelBC class <===
//<======================