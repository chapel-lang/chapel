const D1 = {1..5,1..3};
const D2 = {1..7,1..7};
var A:[D1] real;
var B:[D2] real;

initMatrix(A);
initMatrix(B);

writeln("A = ");
writeln(A);
writeln("B = ");
writeln(B);

proc initMatrix(A:[?D]) {
  if (D.rank == 2) {
    if (D.dim(1) == D.dim(2)) {
      A = 1.0;
    }
    else {
      var minRange = min(D.dim(1),D.dim(2));
      for i in minRange {
        A(i,i) = 1.0;
      }
    }
  }
  else halt("Input matrix must be 2 dimensional");
}
