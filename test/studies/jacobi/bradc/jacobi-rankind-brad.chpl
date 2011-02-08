config param rank = 2;

config const n = 10;
config const epsilon = 0.0001;

const D: domain(rank) = [1..n]**rank;

param StencilD: domain(rank) = [-1..1]**rank;

var weights: [StencilD] real;

for d in 1..rank {
  var ind: index(rank) = 0;
  ind(d) = -1;
  weights(ind) = 0.25;
  ind(d) = 1;
  weights(ind) = 0.25;
}

proc main() {
  var A, B: [D] real = 0.0;

  A.setBoundary(computeBoundary);

  // boy, it's a shame not to be able to declare this when it's used
  var bigdiff: real;

  do {
    // the hope is that some major constant folding/stencil
    // optimization will take place here
    [ijk in D] B(ijk) = sum reduce [off in StencilD] A(ijk+off);

    bigdiff = max reduce abs(A-B);

    A = B;
  } while (bigdiff > epsilon);

  writeln("Final A is: \n", A);
}


proc computeBoundary(ijk: index(rank)) {
  for d in 1..rank {
    if (ijk(d) < 0 || ijk(d) > n+1) {
      halt("Out of bounds error in computeBoundary");
    }
  }
  if (ijk(1) == n+1) {
    return 1.0;
  } else {
    return 0.0;
  }
}
