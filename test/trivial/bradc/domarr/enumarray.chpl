enum classVals = S | W | A | B | C | D | O;

var DClass: domain(1) = S..O;

var probSize: [DClass] integer;

probSize(S) = 123;
probSize(W) = 456;
probSize(A) = 789;
probSize(B) = 012;
probSize(C) = 345;
probSize(D) = 678;
probSize(O) = 901;

writeln("probSize(S) = ", probSize(S));
for i in DClass {   -- vvvv  BLC: can I get rid of this cast?
  writeln("probSize(", classVals(i), ") = ", ("%3d", probSize(i)));
}
writeln("probSize = ", ("%.3d", probSize));
