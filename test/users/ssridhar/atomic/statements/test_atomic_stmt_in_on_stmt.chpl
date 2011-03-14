proc main() {
  var x: [1..10] uint(64);
  for i in 1..10 do
    on x(i) do atomic x(i) = x(i) + 1;
  writeln("x = ", x);
}

