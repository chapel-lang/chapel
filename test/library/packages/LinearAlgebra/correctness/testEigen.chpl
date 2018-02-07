use LinearAlgebra, Random;

config const epsilon = 0.0000000001;

proc isClose(a: complex, b: complex) {
  return abs(a.re - b.re) + abs(a.im - b.im) < epsilon;
}

var A: [1..10, 1..10] real;
fillRandom(A);
var cplxA: [1..10, 1..10] complex = A;

{
  var eig = eigvals(A);

  // Check that:
  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eig;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }
}

{
  var (eig, right) = eigvals(A, right=true);

  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eig;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }

  // Check that:
  // Av = e * v
  // Where e and v are corresponding eigenvalues and right eigenvectors
  for (e, i) in zip(eig, 1..) {
    var eigVec:[1..10] complex = right[.., i];
    var Av = dot(cplxA, eigVec);
    var ev = dot(e, eigVec);

    for (a, e) in zip(Av, ev) {
      if !isClose(a, e) then
        writeln(a, " != ", e);
    }
  }
}

{
  var (eig, left) = eigvals(A, left=true);

  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eig;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }

  // Check that
  // u^H * A = k * u^H
  // Where k and u are corresponding eigenvalues and left eigenvectors
  // and ^H means conjugate transpose.
  for (k, i) in zip(eig, 1..) {
    var eigVec:[1..10] complex = conjg(left[.., i]);
    var Av = dot(eigVec, cplxA);
    var ku = dot(k, eigVec);

    for (a, k) in zip(Av, ku) {
      if !isClose(a, k) then
        writeln(a, " != ", k);
    }
  }
}

{
  var (eig, left, right) = eigvals(A, left=true, right=true);
  // Check that the above properties hold when computing both eigenvectors
  // at the same time.

  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eig;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }

  // Av = e * v
  for (e, i) in zip(eig, 1..) {
    var eigVec:[1..10] complex = right[.., i];
    var Av = dot(cplxA, eigVec);
    var ev = dot(e, eigVec);

    for (a, e) in zip(Av, ev) {
      if !isClose(a, e) then
        writeln(a, " != ", e);
    }
  }

  // u^H * A = k * u^H
  for (k, i) in zip(eig, 1..) {
    var eigVec:[1..10] complex = conjg(left[.., i]);
    var Av = dot(eigVec, cplxA);
    var ku = dot(k, eigVec);

    for (a, k) in zip(Av, ku) {
      if !isClose(a, k) then
        writeln(a, " != ", k);
    }
  }
}

