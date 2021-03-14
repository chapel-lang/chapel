var n1, n2, n3, n4: int;

iter foo() {
  n1 += 1;
  for i in 2..11 {
    n2 += 2;
    yield i;
    n3 += 3;
  }
  n4 += 4;
}

for i in zip(1..10, foo()) do
  writeln(i);

writeln((n1, n2, n3, n4));
