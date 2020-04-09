proc jam( j: int, k: int): int {
  return j*k;
}

var D: domain(1) = {1..5};

var C: [0..10 by 3] int = [i in 0..10 by 3] jam( i, 3);
C[1] = 99;
writeln( C);
