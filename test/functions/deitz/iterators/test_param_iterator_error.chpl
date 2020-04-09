iter f() param {
  yield 1;
  yield 2;
}

for i in f() do
  writeln(i);
