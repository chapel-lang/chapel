iter foo() {
  for i in 1..10 do
    if i%2 == 0 then
      yield "hi";
    else
      yield 1.2;
}

for i in foo() do
  writeln(i);
