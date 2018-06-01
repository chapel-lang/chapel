class C {
  var x: int = foo();
  proc foo() return 314;
}

var c = new unmanaged C();

writeln(c);

delete c;
