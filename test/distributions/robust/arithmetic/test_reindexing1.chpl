use driver;

const TD2D: domain(2) distributed Dist2D = Space2 + (1, 1);

def foo(A: [TD2D] int) {
  for i in A.domain do
    writeln(i, ": ", A(i));
}

for e in A2D do e = next();

for i in A2D.domain do
  writeln(i, ": ", A2D(i));

foo(A2D);
