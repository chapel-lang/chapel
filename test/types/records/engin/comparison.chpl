write("Basic record... ");
{
  record R {
    var x: int;
  }

  var small = new R(1);
  var small2 = new R(1);
  var big = new R(2);

  test(small, small2, big);
}
writeln("success");

write("Multiple fields... ");
{
  record R {
    var x, y: int;
  }

  var small = new R(1, 1);
  var small2 = new R(1, 1);
  var big = new R(2, 2);

  test(small, small2, big);
}
writeln("success");

write("Generic record... ");
{
  record R {
    type t = int;
    var x: t;
  }

  var small = new R(int, 1);
  var small2 = new R(int, 1);
  var big = new R(int, 2);

  test(small, small2, big);
}
writeln("success");

write("Class field with operators overloaded... ");
{
  class C { var x = 0; }
  proc ==(c1: borrowed C, c2: borrowed C) return c1.x == c2.x;
  proc !=(c1: borrowed C, c2: borrowed C) return c1.x != c2.x;
  proc <(c1: borrowed C, c2: borrowed C) return c1.x < c2.x;
  proc <=(c1: borrowed C, c2: borrowed C) return c1.x <= c2.x;
  proc >(c1: borrowed C, c2: borrowed C) return c1.x > c2.x;
  proc >=(c1: borrowed C, c2: borrowed C) return c1.x >= c2.x;

  record R {
    var c: owned C;
    proc init(x: int) { this.c = new C(x); }
  }

  var small = new R(1);
  var small2 = new R(1);
  var big = new R(2);

  test(small, small2, big);
}
writeln("success");

proc test(small, small2, big) {
  assert((small == small2) == true);
  assert((small != small2) == false);
  assert((small == big) == false);
  assert((small != big) == true);

  assert((small < small2) == false);
  assert((small < big) == true);
  assert((big < small) == false);
  assert((small <= small2) == true);
  assert((small <= big) == true);
  assert((big <= small) == false);

  assert((small > small2) == false);
  assert((small > big) == false);
  assert((big > small) == true);
  assert((small >= small2) == true);
  assert((small >= big) == false);
  assert((big >= small) == true);
}
