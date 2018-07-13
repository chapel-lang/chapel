
var A, B: [1..4] int;
forall xy in ([ab in zip(A,B)] ab) do
  xy = (1,5); // update A and B's elements
writeln(A);
writeln(B);
