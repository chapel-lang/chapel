iter foo(n: int) {
  for i in 0..n-1 do
    yield i;
}

iter bar(n: int) {
  for i in 0..n-1 do
    for x in foo(n) do
      yield x+i;
}

var count = 0;
for i in bar(2) {
  count = count + 1;
  if count == 20 then break;
  writeln(i);
}
