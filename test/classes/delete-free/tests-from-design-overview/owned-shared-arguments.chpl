class MyClass {
  var x:int;
  proc init() {
    writeln("in MyClass.init");
  }
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

var global: owned MyClass?;
proc saveit(in arg: owned MyClass) {
  global = arg; // OK! Transfers ownership from 'arg' to 'global'
  // now instance will be deleted at end of program
}
proc test() {
  var x = new owned MyClass();
  saveit(x); // leaves x 'nil' - instance transferred to arg & then to global
  // instance not destroyed here since x is 'nil'
}
test();
writeln(global);  // OK
