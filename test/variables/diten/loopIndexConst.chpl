iter myiter(n: int) {
  for i in 1..n do yield i;
}

for i in myiter(3) {
  i += 1;
  writeln(i);
}

for i in 1..3 do {
  i += 1;
  writeln(i);
}
