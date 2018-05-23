class C {
  type t;
  var x: t = foo();
  proc foo() return 314:t;
}

var c = new unmanaged C(int(64));

writeln(c);

delete c;
