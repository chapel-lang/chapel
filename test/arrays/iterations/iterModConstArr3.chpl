const A = [1.0, 2.0, 3.0];

forall (a,i) in zip(A,1..3) do
  a = -a;

writeln(A);
