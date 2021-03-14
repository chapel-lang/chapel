proc factorial(n) {
  proc innerfac(ref result, n) {
    if n > 0 {
      innerfac(result, n-1);
      result *= n;
    }
  }
  var result = 1;
  innerfac(result, n);
  return result;
}

for i in 1..10 {
  writeln(factorial(i));
}
