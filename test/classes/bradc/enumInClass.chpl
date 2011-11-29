class C {
  enum probClass { A, B, C, D, E };

  var x: probClass;
}

var myC = new C();

writeln("myC = ", myC);

delete myC;
