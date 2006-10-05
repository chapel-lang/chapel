def jam( j: int, k: int): int {
  return j*k;
}

var D: domain(1) = [1..5];

var C: [i in 0..10 by 3] int = jam( i, 3);
C[1] = 99;
writeln( C);
