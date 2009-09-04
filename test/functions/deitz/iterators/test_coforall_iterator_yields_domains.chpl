def foo() {
  for i in 1..3 do
    yield [i..5];
}

coforall i in foo() do
  writeln(i);
