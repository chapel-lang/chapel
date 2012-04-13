use CodeletDist;

proc main {

  var N = [0..9] dmapped CodeletDist();
  var A: [N] int;

  writeln(A(0)); // code 1

  for i in [0..9] do {
    writeln("Before A :", i);
    forall j in N do {
      writeln("HELLO!");
      A(j) = i + j;
    }
    writeln("AFter A ", i);
  }

  writeln(A(9));
}
