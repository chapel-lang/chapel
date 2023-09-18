// Test a class initializer with an empty body

class MyClass {
  var x : int;
  var y : int = 1;
  var z       = 2;

  proc init() {

  }
}

proc main() {
  var ownCls = new owned MyClass();
  var cls : borrowed MyClass = ownCls.borrow();

  writeln('cls: ', cls);
}
