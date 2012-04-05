use CodeletDist;

proc main {

  var N = [0..9] dmapped CodeletDist();
  var A: [N] int;

  writeln(A(0)); // code 1

  for i in [0..9] do {
    forall j in N do {
      A(i) = i + j;
    }
  }

  writeln(A(0));
}
