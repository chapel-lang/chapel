proc main() {
  var x: [1..10] uint(64);
  coforall i in 1..10 do
    atomic {
      on x(i) do x(i) = x(i) + 1;
    }
  writeln("x = ", x);
}

