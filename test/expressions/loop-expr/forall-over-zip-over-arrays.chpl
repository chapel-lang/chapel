
var A, B: [1..4] int;
var R: [1..4] real;
var S: [1..4] string;

forall xy in ([ab in zip(A,B)] ab) do
  xy = (1,5); // update A and B's elements
writeln(A);
writeln(B);

forall xyz in zip([ab in zip(B,R)] ab, S) do
  xyz = ((2,6.6),"hi");
writeln(B);
writeln(R);
writeln(S);

forall zyx in zip(S, [ab in zip(R,A)] ab) do
  zyx = ("bye", (3.1,4));
writeln(A);
writeln(R);
writeln(S);
