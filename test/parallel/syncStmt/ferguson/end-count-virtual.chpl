class Parent {
  proc runTask() {
  }
}
class Child : Parent {
  proc runTask() {
    begin {
      writeln("IN TASK 1");
      writeln("IN TASK 2");
    }
  }
}
proc run() {
  sync {
    var obj:Parent = new borrowed Child();
    obj.runTask();
  }
}
run();
