var A: [1..3] int;

def foo(out B: [1..3] int) {
  for i in B.domain do
    B(i) = i;
}

writeln(A);
foo(A);
writeln(A);
