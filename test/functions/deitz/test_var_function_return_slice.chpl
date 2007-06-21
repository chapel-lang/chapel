var A: [1..5] int;

def foo() var {
  return A(2..4);
}

foo() = 2;

writeln(A);
