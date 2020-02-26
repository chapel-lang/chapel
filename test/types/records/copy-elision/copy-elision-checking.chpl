class MyClass { var x: int; }

record R {
  var x: int = 1;
  var c: shared MyClass = new shared MyClass(x);
}
proc R.init(arg: int) {
  this.x = arg;
  this.c = new shared MyClass(arg);
}
proc R.init=(other: R) {
  this.x = other.x;
  this.c = new shared MyClass(other.c.x);
}
proc =(ref lhs: R, rhs: R) {
  lhs.x = rhs.x;
  lhs.c = new shared MyClass(rhs.c.x);
}

proc test1() {
  var x = new R(1);
  ref r = x;
  var y = x;
  r.x = 2;   // yeah but x is dead at this point
  r.c.x = 2; // should not modify y's value
  writeln(y);
}
test1();

proc test2() {
  var x = new R(1);
  ref r = x;
  var y = x;
  r.c.x = 2; // x is dead at this point
  writeln(y);
}
test2();
