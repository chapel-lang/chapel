var A: [1..3] int;
var B: [1..4] int = [1,2,3,4];
forall (a,b) in zip(A,B) do
  a = b;
writeln("B is: ", B);
writeln("A is: ", A);
