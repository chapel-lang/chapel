iter foo() {
  forall i in 1..10 do
    yield i;
}

for i in foo() do
  writeln(i);
