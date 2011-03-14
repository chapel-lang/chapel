proc main() {
  var x: [1..10] uint(64);
  for i in 1..10 do
    local {
      atomic x(i) = x(i) + 1;
    }
  writeln("x = ", x);
}

