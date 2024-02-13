
var A, B: [1..4] int;
var R: [1..4] real;
var S: [1..4] string;

A = 0; B = 0; R = 0.0; S = "";
foreach xy in (foreach ab in zip(A,B) do ab) do
  xy = (1,5); // update A and B's elements
writeln(A);
writeln(B);

A = 0; B = 0; R = 0.0; S = "";
foreach xyz in zip(foreach ab in zip(B,R) do ab, S) do
  xyz = ((2,6.6),"hi");
writeln(B);
writeln(R);
writeln(S);

A = 0; B = 0; R = 0.0; S = "";
foreach zyx in zip(S, for ab in zip(R,A) do ab) do
  zyx = ("bye", (3.1,4));
writeln(A);
writeln(R);
writeln(S);
