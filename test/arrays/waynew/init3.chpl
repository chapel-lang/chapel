proc jam( j: int, k: int): int {
  return j*k;
}

var D: domain(2) = {1..5, 6..7};

var A: [D] int = [(i,j) in D] jam( i, j);
writeln( A);
