use CodeletDist;

proc main {

  var x = 0;
  var N = [0..4] dmapped CodeletDist();
  var A: [N] int;

  writeln(A(0)); // code 1

  while x < 10 {
    writeln("Before A :", x);
    forall j in N do {
      writeln("HELLO!");
      A(j) = x + j;
    }
    x = x + 1;
    writeln("AFter A ", x);
  }

  writeln(A(9));
}
