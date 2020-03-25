record R {
  var field: int;
  proc init() {
    writeln("init()");
  }
  proc init(arg:int) {
    writeln("init(", arg, ")");
    this.field = arg;
  }
  proc init=(other: R) {
    writeln("init=(", other, ")");
    this.field = other.field;
  }
}
proc =(ref lhs: R, rhs: R) {
  writeln(lhs, " = ", rhs);
  lhs.field = rhs.field;
}

proc acceptsOut(out arg) { }

config const option = true;

// test 1
var x1:R;
inner1();
x1 = new R(2);

proc inner1() {
  writeln(x1);
}

// test 2
var x2:R;
{
  inner2();
  if option then
    x2 = new R(1);
  else
    x2 = new R(1);
}

proc inner2() {
  writeln(x2);
}

// test 3
var x3:int;
inner3();
x3 = 34;

proc inner3() {
  writeln(x3);
}

// test 4
var x4:R;
inner4();
acceptsOut(x4);

proc inner4() {
  writeln(x4);
}

// test 5
var x5:int;
inner5();
acceptsOut(x5);

proc inner5() {
  writeln(x5);
}
