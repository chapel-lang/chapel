// This test is focused on making sure the order
// of copy-initialization and write-back for inout
// mixed with in and out intents is reasonable.

proc inout_inout(inout a: R, inout b: R) {
  writeln("a is ", a);
  writeln("b is ", b);
  a = new R(3);
  b = new R(4);
}

proc test_inout_inout() {
  writeln();
  writeln("test_inout_inout");
  var a = new R(1);
  var b = new R(2);
  inout_inout(a, b);
  writeln(a, " ", b);
}
test_inout_inout();

proc test_inout_inout2() {
  writeln();
  writeln("test_inout_inout2");
  var a = new R(1);
  var b = new R(2);
  inout_inout(a, b);
}
test_inout_inout2();


proc in_inout(in a: R, inout b: R) {
  writeln("a is ", a);
  b = new R(4);
}

proc test_in_inout() {
  writeln();
  writeln("test_in_inout");
  var a = new R(1);
  var b = new R(2);
  in_inout(a, b);
  writeln(a, " ", b); // disabling copy elision for a
}
test_in_inout();

proc inout_in(inout a: R, in b: R) {
  writeln("a is ", a);
  writeln("b is ", a);
  a = new R(3);
}

proc test_inout_in() {
  writeln();
  writeln("test_inout_in");
  var a = new R(1);
  var b = new R(2);
  inout_in(a, b);
  writeln(a, " ", b); // disabling copy elision for b
}
test_inout_in();

proc in_in(in a: R, in b: R) {
  writeln("a is ", a);
  writeln("b is ", b);
}

proc test_in_in() {
  writeln();
  writeln("test_in_in");
  var a = new R(1);
  var b = new R(2);
  in_in(a, b);
  writeln(a, " ", b); // disabling copy elision for a and b
}
test_in_in();

proc out_out(out a: R, out b: R) {
  writeln("a is ", a);
  writeln("b is ", b);
  a = new R(10);
  b = new R(20);
}

proc test_out_out() {
  writeln();
  writeln("test_out_out");
  var a = new R(1);
  var b = new R(2);
  out_out(a, b);
  writeln(a, " ", b);
}
test_out_out();

proc out_out2(out a: R, out b: R) {
  a = new R(10);
  b = new R(20);
}

proc test_out_out2() {
  writeln();
  writeln("test_out_out2");
  var a = new R(1);
  var b = new R(2);
  out_out(a, b);
  writeln(a, " ", b);
}
test_out_out2();

proc out_inout(out a: R, inout b: R) {
  writeln("a is ", a);
  writeln("b is ", b);
  a = new R(10);
  b = new R(20);
}

proc test_out_inout() {
  writeln();
  writeln("test_out_inout");
  var a = new R(1);
  var b = new R(2);
  out_inout(a, b);
  writeln(a, " ", b);
}
test_out_inout();

proc inout_out(inout a: R, out b: R) {
  writeln("a is ", a);
  writeln("b is ", b);
  a = new R(10);
  b = new R(20);
}

proc test_inout_out() {
  writeln();
  writeln("test_inout_out");
  var a = new R(1);
  var b = new R(2);
  inout_out(a, b);
  writeln(a, " ", b);
}
test_inout_out();


// Also check writeback order with same argument
proc foo1(out   x, out   y) {x = 1; y = 2;}
proc foo2(out   x, inout y) {x = 1; y = 2;}
proc foo3(inout x, out   y) {x = 1; y = 2;}
proc foo4(inout x, inout y) {x = 1; y = 2;}

{
  writeln("testing writeback order with integers");
  var x = 0;

  x=0; foo1(x, x); writeln(x);
  x=0; foo2(x, x); writeln(x);
  x=0; foo3(x, x); writeln(x);
  x=0; foo4(x, x); writeln(x);
}


class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared C(0);
    writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    writeln("init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    writeln("init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    writeln("deinit ", x, " ", ptr.xx);
  }
  proc toString() {
    return "(" + this.x:string + " " + this.ptr.xx:string + ")";
  }
  proc set1() {
    this.x = 1;
    this.ptr.xx = 1;
    return this;
  }
}
proc =(ref lhs:R, rhs:R) {
  writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}
