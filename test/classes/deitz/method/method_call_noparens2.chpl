class C {
  var x = 2;
  proc foo return 3;
  proc bar return foo;
}

var c = new C(4);

writeln(c.bar);

delete c;
