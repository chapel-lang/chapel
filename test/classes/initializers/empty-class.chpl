// Test a class initializer with an empty body

class MyClass {
  var x : int;
  var y : int = 1;
  var z       = 2;

  proc init() {

  }
}

proc main() {
  var cls : borrowed MyClass = (new owned MyClass()).borrow();

  writeln('cls: ', cls);
}
