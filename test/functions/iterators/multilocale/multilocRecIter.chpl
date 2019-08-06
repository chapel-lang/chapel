iter foo(depth: int): int {
  if depth >= numLocales then
    return;

  on Locales[depth] do
    yield here.id;

  for f in foo(depth+1) do
    yield f;
}

for f in foo(0) do
  writeln("Got ", f, " on locale ", here.id);
