class C {
  var x: int = foo();
  proc foo() return 314;
}

var c = new C();

writeln(c);

delete c;
