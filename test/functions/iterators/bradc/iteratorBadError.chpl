def iter() {
  startval = 0;
  for i in startval..10 do
    yield i;
}

for j in iter() {
  writeln("j is: ", j);
}
