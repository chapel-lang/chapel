iter itr() {
  startval = 0;
  for i in startval..10 do
    yield i;
}

for j in itr() {
  writeln("j is: ", j);
}
