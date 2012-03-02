use ILDist;

proc main {

  var N = [0..9] dmapped ILDist();
  var A: [N] int;

  writeln(A(0)); // code 1

  for i in [0..9] do {

    writeln(i);

    forall j in N do {
      writeln("HELLO WORLD");
      A(i) = i + j;
    }

    writeln(i);
  }

  writeln(A(0));
}
