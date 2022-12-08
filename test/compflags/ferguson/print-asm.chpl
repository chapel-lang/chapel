config const n = 10_000;
proc foo() {
  var result = 0.0;
  for i in 1..n {
    result += sqrt(i*i);
  }
  return result;
}

proc bar() {
  var total = 0.0;
  for i in 1..10 {
    total += foo();
  }
  return total;
}

writeln(foo());
writeln(bar());
