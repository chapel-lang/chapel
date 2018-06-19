class MyClass {
  var x:int;
  proc init() {
    writeln("in MyClass.init");
  }
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

proc test() {
  var a: owned MyClass = new owned MyClass();
  // the instance referred to by a is deleted at end of scope
  var c: borrowed MyClass = a.borrow();
  // c "borrows" to the instance managed by a
  return c; // lifetime checker error! returning borrow from local variable
  // a is deleted here
}
test();
