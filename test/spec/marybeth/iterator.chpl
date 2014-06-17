iter evens(n): int {
  for i in 1..n do yield 2*i;
}

for i in evens(5) do writeln(i);

var y = evens(5);
writeln(y);
