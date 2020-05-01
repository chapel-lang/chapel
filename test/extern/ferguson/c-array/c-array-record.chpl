record R {
  var x:int;
  proc init() {
    writeln("R.init");
    this.x = 1;
  }
  proc init(x:int) {
    writeln("R.init ", x);
    this.x = x; 
  }
  proc init=(other:R) {
    writeln("R.init= ", other);
    this.x = other.x; 
  }
  proc deinit() {
    writeln("R.deinit ", this.x);
  }
}
proc =(ref lhs:R, rhs:R) {
  writeln(lhs.x, " = ", rhs.x);
  lhs.x = rhs.x;
}

proc test1() {
  writeln("\ntest1");
  var x: c_array(R, 1);

  writeln(x);
}
test1();

proc test2() {
  writeln("\ntest2");
  var x: c_array(R, 2);

  writeln(x);

  for i in 0..1 do
    x[i] = new R(100*i);

  writeln(x);
}
test2();

proc test3() {
  writeln("\ntest3");
  var x: c_array(R, 2);

  for i in 0..1 do
    x[i] = new R(100*i);

  writeln(x);

  // test copy init
  var y = x;
  writeln(y);
  writeln(x); // avoiding copy elision to test copy init
}
test3();

proc test4() {
  writeln("\ntest4");
  var x: c_array(R, 2);

  for i in 0..1 do
    x[i] = new R(100*i);

  writeln(x);

  // test assign
  var y: c_array(R, 2);
  y; // avoid copy-init
  y = x;
  writeln(y);
}
test4();
