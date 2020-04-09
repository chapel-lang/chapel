iter myiter(r: range) {
  for r in r do
    yield r;
}

iter myiter(param tag: iterKind, r: range) {
  for r in r do
    yield r;
}

forall i in myiter(iterKind.standalone, 1..10) do
  writeln(i);
