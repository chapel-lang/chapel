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
    var obj:borrowed Parent = new borrowed Child();
    obj.runTask();
  }
}
run();
