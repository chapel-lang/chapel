use CodeletDist;

proc main {

  var N = [0..9] dmapped CodeletDist();
  var A: [N] int;

  forall i in N do {
    forall j in N do {
  	A(i) = i + j + 1234;
        writeln("i = ", i, " and ", A(i), " j = ", j);
    }
  }
  writeln(A);
}
