record r { var x: int = 0; }

var g1 = (0, new r());

proc test1() {
  writeln(g1);
  ref x = g1;
  x = (128, new r(256));
  writeln(g1);
}
test1();

g1 = (0, new r());

proc test2() {
  var tup = (0, new r());
  writeln(tup);
  ref x = tup;
  x = (128, new r(256));
  writeln(tup);
}
test2();

proc test3() {
  var a = 0;
  var b = new r();
  writeln(a);
  writeln(b);
  ref x = (a, b);
  x = (128, new r(256));
  writeln(a);
  writeln(b);
}
test3();

