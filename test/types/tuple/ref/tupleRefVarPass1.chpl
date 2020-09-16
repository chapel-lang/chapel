// Test ref tuple variable composed of local vars and global vars.
record r { var x: int = 0; }

var g1 = 0;
var g2 = new r();

proc test1() {
  var a = 0;
  var b = new r();

  writeln(a);
  writeln(b);

  ref foo = (a, b);
  foo[0] = 128;
  foo[1] = new r(128);

  writeln(a);
  writeln(b);
}
test1();

proc test2() {
  writeln(g1);
  writeln(g2);

  ref foo = (g1, g2);
  foo[0] = 128;
  foo[1] = new r(128);

  writeln(g1);
  writeln(g2);
}
test2();

