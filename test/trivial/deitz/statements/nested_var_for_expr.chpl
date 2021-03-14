var A: [1..3] int;

iter foo() ref {
  for i in 1..3 do
    yield A(i);
}

writeln(A);

for e in foo() do
  e = 1;

writeln(A);

for e in for f in foo() do f do
  e = 2;

writeln(A);
