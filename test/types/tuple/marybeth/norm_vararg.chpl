proc norm(x:?t...?n) {
  var sum:t;

  for i in 0..#n {
    sum += x(i)*x(i);
  }
  return sqrt(sum);
}

writeln(norm(1.0, 1.0, 1.0));
writeln(norm(5.0, 5.0));
