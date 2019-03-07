class MyClass {
  var x: int;
  proc method() {
    writeln("in method");
  }
}

proc bad1() {
  var x = new owned MyClass(1);
  var y = x.release();
  x.method();
}
bad1();

proc bad2() {
  var x = new owned MyClass(1);
  var y = x.release();
  x.x = 42;
}
bad2();

proc bad3() {
  var x = new owned MyClass(1);
  var y = x; // transfers from x to y
  x.method();
}
bad3();
