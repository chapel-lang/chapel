proc foo(inout x: int) {
  x = x + 1;
}

iter bar(n: int) {
  var x = 0;
  for i in 1..n {
    foo(x);
    yield x;
  }
}

for i in bar(5) do
  writeln(i);

var x: [1..5] int = bar(5);
writeln(x);
