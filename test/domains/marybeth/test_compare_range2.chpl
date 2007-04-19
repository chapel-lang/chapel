const D1 = [1..5,1..3];
const D2 = [1..7,1..7];
var A:[D1] real;
var B:[D2] real;

initMatrix(A);
initMatrix(B);

writeln("A = ");
writeln(A);
writeln("B = ");
writeln(B);

def initMatrix(A:[?D]) {
  if (D.rank == 2) {
    if ((D(1).low == D(2).low) & (D(1).high == D(2).high)) {
      A = 1.0;
    }
    else {
      var minRange = if (D(1).length < D(2).length) then D(1) else D(2); 
      for i in minRange {
        A(i,i) = 1.0;
      }
    }
  }
  else halt("Input matrix must be 2 dimensional");
}
