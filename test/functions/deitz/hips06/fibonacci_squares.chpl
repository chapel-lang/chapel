iter fibonacci(n):int {
  var i1 = 0, i2 = 1;
  var i = 0;
  while i <= n {
    yield i1;
    var i3 = i1 + i2;
    i1 = i2;
    i2 = i3;
    i += 1;
  }
}

iter squares(n):int {
  var i = 0;
  while i <= n {
    yield i * i;
    i += 1;
  }
}

for i in fibonacci(12) do
  writeln(i);
writeln();
for j in squares(12) do
  writeln(j);
