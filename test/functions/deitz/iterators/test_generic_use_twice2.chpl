iter foo() {
  for i in 1..10 do
    yield i;
}

proc bar(x) {
  for i in x do
    for j in x do
      write((i,j));
  writeln();
}

bar(foo());

