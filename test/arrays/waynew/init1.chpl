proc jam( j: int, k: int): int {
  return j*k;
}

var D: domain(1) = {1..5};

var A: [D] int = [i in D] jam( i, 2);
writeln( A);

var B: [0..10] int = [i in 0..10] jam( i, 3);
writeln( B);

var C: [0..10 by 3] int = [i in 0..10 by 3] jam( i, 3);
C[9] = 99;
writeln( C);
