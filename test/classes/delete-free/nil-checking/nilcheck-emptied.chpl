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

proc bad1q() {
  var x = new owned MyClass?(1);
  var y = x.release();
  x!.method();
}
bad1q();

proc bad2() {
  var x = new owned MyClass(1);
  var y = x.release();
  x.x = 42;
}
bad2();

proc bad2q() {
  var x = new owned MyClass?(1);
  var y = x.release();
  x!.x = 42;
}
bad2q();

proc bad3() {
  var x = new owned MyClass(1);
  var y = x; // transfers from x to y
  x.method();
}
bad3();

proc bad3q() {
  var x = new owned MyClass?(1);
  var y = x; // transfers from x to y
  x!.method();
}
bad3q();

proc bad4() {
  var x = new owned MyClass(1);
  ref y = x;
  var z = x;
  y.clear();
  writeln(x);
  x.method();
}
bad4();

proc bad4q() {
  var x = new owned MyClass?(1);
  ref y = x;
  var z = x;
  y.clear();
  writeln(x);
  x!.method();
}
bad4q();
