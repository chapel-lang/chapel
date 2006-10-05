def jam( j: int, k: int): int {
  return j*k;
}

var D: domain(1) = [1..5];

var A: [i in D] int = jam( i, 2);
writeln( A);

var B: [i in 0..10] int = jam( i, 3);
writeln( B);

var C: [i in 0..10 by 3] int = jam( i, 3);
C[9] = 99;
writeln( C);
