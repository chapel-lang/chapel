class Parent {
  proc runTask() {
  }
}
class Child : Parent {
  override proc runTask() {
    begin {
      writeln("IN TASK 1");
      writeln("IN TASK 2");
    }
  }
}
proc run() {
  sync {
    var ownObj = new owned Child();
    var obj:borrowed Parent = ownObj.borrow();
    obj.runTask();
  }
}
run();
