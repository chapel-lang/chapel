class MyClass {
  var x:int;
}

proc test() {
  var x: borrowed MyClass?;
  writeln(x);
}
test();
