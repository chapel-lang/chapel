extern class C { var x: int(64); }

proc my_foo(c: C, x: int(64)) {
  c.x = x;
}

var myC = new C(5);

writeln(myC);

my_foo(myC, 3);

writeln(myC);

delete myC;
