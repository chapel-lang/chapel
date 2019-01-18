iter myiter(in arg) {
  for i in 1..10 {
    arg += 1;
    yield arg;
  }
}

for i in myiter(1) do writeln(i);
