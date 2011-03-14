var x: int = 0;

for loc in Locales do
  on loc {
    writeln("In locale ", here.id);
    atomic x += 1;
  }

writeln("x = ", x);
  