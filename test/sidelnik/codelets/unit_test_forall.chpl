use CodeletDist;

proc main {

  var N = [0..9] dmapped CodeletDist();
  var A: [N] int;

  forall i in N do {
  	A(i) = i + 1234;
        writeln("i = ", i, " and ", A(i));
  }
  writeln(A);
}
