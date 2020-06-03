class MyClass {
  var x:int;
  proc init() {
    writeln("in MyClass.init");
  }
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

var global: borrowed MyClass?; // 'borrowed' optional here
proc saveit(arg: borrowed MyClass) { // and here
  global = arg; // Error! trying to store borrow from local 'x' into 'global'
  delete arg;   // Error! trying to delete a borrow
}
proc test() {
  var x = new owned MyClass();
  saveit(x); // x coerced to borrow on call
  // instance destroyed here
}
test();
writeln(global);  // uh-oh! use-after free
