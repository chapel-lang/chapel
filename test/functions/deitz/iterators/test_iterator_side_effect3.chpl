var j = 0;

iter myIterator(n: int) {
  for i in 1..n {
    j += 1;
    yield i;
  }
}

writeln(myIterator(5));
writeln(j);
writeln(myIterator(5));
writeln(j);
