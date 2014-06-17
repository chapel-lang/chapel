var A: [1..3] domain(1);

iter foo() {
  for i in 1..3 do
    yield {i..5};
}

coforall (i, j) in zip(foo(), 1..3) do
  A(j) = i;

writeln(A);
