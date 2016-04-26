class C {
  var x = foo();
  proc foo() return x;
}

var c = new C();

writeln(c);
