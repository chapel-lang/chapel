proc sum(n: int) {
  var sum = 0;
  for i in 1..n {
    sum += i;
  }
  return sum;
}

var sum = asum(100);
writeln(sum);