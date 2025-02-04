proc helper(arg: int) {
  var a = b;
  var b = arg;
  return b;
}

proc other(x: int) {
  var a = x;
  var x = 2.0;
  return x;
}

proc uhoh(v: int) {
  var x: t;
  type t;
  t = v.type;
}

proc anotherone() {
  var gx = g + 2;
  var g = 1;
  return gx;
}

proc bar() {
  var bx = 5;
  bx += ly * 2;
  var ly = 3;
  return bx;
}

anotherone();
bar();
var x = helper(2);
var y = other(2);

var ax = g;
var g = 1;

var bx = 5;
bx += ly * 2;
var ly = 3;

foo();

proc foo(arg:int = x, x: int = 5) { return 0; }
