module myParams {
  //
  // config param to choose whether update loops need to be protected
  // declared as param to avoid the additional check at runtime 
  //
  config param safeUpdates = false;

  //
  // turn on/off stm statistics collection
  //
  config param trackStmStats = false;
 
  //
  // choose fork_fast over fork
  //
 config param forkFast = false;

 config param seed: int(64) = 0x2;

}