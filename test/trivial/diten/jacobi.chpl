config var N = 4;
config var epsilon = 1.0e-5;
var D = {1..N, 1..N};
var BD= {0..N+1, 0..N+1};


proc main(){
  var A : [BD] real(64);
  var tmp : [D] real(64);
  var delta : real(64);

  // set bottom border to 1.0
  [i in 1..N] A(N+1, i) = 1.0;
  do {
    [ij in D] tmp(ij) = (A(ij + (0, 1))  + 
                         A(ij + (1, 0))  + 
                         A(ij + (0, -1)) + 
                         A(ij + (-1, 0))) / 4.0;

    delta = + reduce abs(tmp - A[D]);
    A[D] = tmp;
  } while (delta > epsilon);
  writeln(A(D));
}

proc +(a: (int, int), b: (int, int)){
  return (a(0) + b(0), a(1) + b(1));
}
