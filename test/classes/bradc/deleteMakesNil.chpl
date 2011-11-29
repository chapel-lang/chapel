class C {
  var x: int;
}

var newC = new C(3);

writeln("newC is: ", newC);

delete newC;

writeln("newC is: ", newC);
