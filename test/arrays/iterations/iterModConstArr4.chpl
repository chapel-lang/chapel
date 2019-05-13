const A = [1.0, 2.0, 3.0];

forall (i,a) in zip(1..3,A) do
  a = -a;

writeln(A);
