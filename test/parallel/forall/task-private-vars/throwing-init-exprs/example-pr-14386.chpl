
var curTaskID: atomic int;

proc getTaskID() throws {
  return curTaskID.fetchAdd(1);
}

var dom = {1..8};

forall i in dom with (const taskID = getTaskID()) {
  writeln(i);
  writeln(taskID * 100 + 200);
}
