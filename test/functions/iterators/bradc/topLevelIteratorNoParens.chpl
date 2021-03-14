iter foo {
  for i in 1..3 {
    yield i;
  }
}

for i in foo {
  writeln("i is: ", i);
}
