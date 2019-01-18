class C {
  var x = foo();
  proc foo() return x;
}

var c = new owned C();

writeln(c);
