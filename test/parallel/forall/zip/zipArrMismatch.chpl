var A: [1..4] real;
var B: [1..3] real = [1.0, 2.0, 3.0];

forall (a,b) in zip(A, B) do
  a = b;

writeln(A);
