var A3: [1..3] real;
var A4: [1..4] real = [1.0, 2.0, 3.0, 4.0];

forall (i,j) in zip(A4, A3) do
  i += j;

writeln(A4);
