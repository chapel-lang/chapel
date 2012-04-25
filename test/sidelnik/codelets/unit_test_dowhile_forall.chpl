use CodeletDist;

proc main {

  var x = 0;
  var N = [0..9] dmapped CodeletDist();
  var A: [N] int;

  writeln(A(0)); // code 1

  do {
    writeln("Before A :", x);
    forall j in N do {
      writeln("HELLO! : ", j);
      A(j) = A(j) + x + j;
    }
    writeln("AFter A ", x);
    x = x + 1;
  } while x < 10;

  
  if A(9) == 135 then
    writeln("Success");
  else
  writeln("Failure: ", A(9), " != 135");
}
