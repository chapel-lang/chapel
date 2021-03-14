class C {
  enum probClass { A, B, C, D, E };

  var x: probClass;
}

var myC = new borrowed C();

writeln("myC = ", myC);
