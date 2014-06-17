var j = 0;

iter myIterator(n: int) {
  for i in 1..n {
    j += 1;
    yield i;
  }
}

for i in myIterator(5) do
  writeln((i, j));
