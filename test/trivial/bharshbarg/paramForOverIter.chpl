config const n = 10;

iter foo() {
  for i in 1..n do yield i;
}

for param i in foo() do
  writeln(i);
