public use Level_def;
public use LevelVariable_def;


//===> LevelBC class ===>
//======================>
class LevelBC {
  
  const level: unmanaged Level;

  proc apply(q: unmanaged LevelVariable, t: real) {}
  proc apply_Homogeneous(q: unmanaged LevelVariable) {}

}
//<=== LevelBC class <===
//<======================
