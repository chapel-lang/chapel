class C {
  var x = 2;
  proc foo return 3;
}

var c = new C(4);

writeln(c.foo);

delete c;
