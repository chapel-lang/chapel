use LinearAlgebra, Random;

config const epsilon = 0.0000000001;

proc isClose(a: complex, b: complex) {
  return abs(a.re - b.re) + abs(a.im - b.im) < epsilon;
}

var A: [1..10, 1..10] real;
fillRandom(A);
var cplxA: [1..10, 1..10] complex = A;

{
  var eigenvalues = eigvals(A);

  // Check that:
  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }
}

{
  var eigenvalues = eig(A);

  // Check that:
  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }
}

{
  var (eigenvalues, right) = eig(A, right=true);

  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }

  // Check that:
  // Av = e * v
  // Where e and v are corresponding eigenvalues and right eigenvectors
  for (e, i) in zip(eigenvalues, 1..) {
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
  var (eigenvalues, left) = eig(A, left=true);

  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }

  // Check that
  // u^H * A = k * u^H
  // Where k and u are corresponding eigenvalues and left eigenvectors
  // and ^H means conjugate transpose.
  for (k, i) in zip(eigenvalues, 1..) {
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
  var (eigenvalues, left, right) = eig(A, left=true, right=true);
  // Check that the above properties hold when computing both eigenvectors
  // at the same time.

  // trace(A) = sum(eigenvalues)
  {
    var tr = trace(A);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(A) != sum(eigenvalues) ", tr, " != ", ev.re);
    }
  }

  // Av = e * v
  for (e, i) in zip(eigenvalues, 1..) {
    var eigVec:[1..10] complex = right[.., i];
    var Av = dot(cplxA, eigVec);
    var ev = dot(e, eigVec);

    for (a, e) in zip(Av, ev) {
      if !isClose(a, e) then
        writeln(a, " != ", e);
    }
  }

  // u^H * A = k * u^H
  for (k, i) in zip(eigenvalues, 1..) {
    var eigVec:[1..10] complex = conjg(left[.., i]);
    var Av = dot(eigVec, cplxA);
    var ku = dot(k, eigVec);

    for (a, k) in zip(Av, ku) {
      if !isClose(a, k) then
        writeln(a, " != ", k);
    }
  }
}


var B: [1..10, 1..10] complex;
fillRandom(B);

{
  var eigenvalues = eigvals(B);

  // Check that:
  // trace(B) = sum(eigenvalues)
  {
    var tr = trace(B);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(B) != sum(eigenvalues) ", tr, " != ", ev);
    }
  }
}

{
  var eigenvalues = eig(B);

  // Check that:
  // trace(B) = sum(eigenvalues)
  {
    var tr = trace(B);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(B) != sum(eigenvalues) ", tr, " != ", ev);
    }
  }
}

{
  var (eigenvalues, right) = eig(B, right=true);

  // trace(B) = sum(eigenvalues)
  {
    var tr = trace(B);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(B) != sum(eigenvalues) ", tr, " != ", ev);
    }
  }

  // Check that:
  // Bv = e * v
  // Where e and v are corresponding eigenvalues and right eigenvectors
  for (e, i) in zip(eigenvalues, 1..) {
    var eigVec:[1..10] complex = right[.., i];
    var Bv = dot(B, eigVec);
    var ev = dot(e, eigVec);

    for (b, e) in zip(Bv, ev) {
      if !isClose(b, e) then
        writeln(b, " != ", e);
    }
  }
}

{
  var (eigenvalues, left) = eig(B, left=true);

  // trace(B) = sum(eigenvalues)
  {
    var tr = trace(B);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(B) != sum(eigenvalues) ", tr, " != ", ev);
    }
  }

  // Check that
  // u^H * B = k * u^H
  // Where k and u are corresponding eigenvalues and left eigenvectors
  // and ^H means conjugate transpose.
  for (k, i) in zip(eigenvalues, 1..) {
    var eigVec:[1..10] complex = conjg(left[.., i]);
    var Bv = dot(eigVec, B);
    var ku = dot(k, eigVec);

    for (b, k) in zip(Bv, ku) {
      if !isClose(b, k) then
        writeln(b, " != ", k);
    }
  }
}

{
  var (eigenvalues, left, right) = eig(B, left=true, right=true);
  // Check that the above properties hold when computing both eigenvectors
  // at the same time.

  // trace(B) = sum(eigenvalues)
  {
    var tr = trace(B);
    var ev = + reduce eigenvalues;
    if !isClose(tr, ev) {
      writeln("trace(B) != sum(eigenvalues) ", tr, " != ", ev);
    }
  }

  // Bv = e * v
  for (e, i) in zip(eigenvalues, 1..) {
    var eigVec:[1..10] complex = right[.., i];
    var Bv = dot(B, eigVec);
    var ev = dot(e, eigVec);

    for (b, e) in zip(Bv, ev) {
      if !isClose(b, e) then
        writeln(b, " != ", e);
    }
  }

  // u^H * B = k * u^H
  for (k, i) in zip(eigenvalues, 1..) {
    var eigVec:[1..10] complex = conjg(left[.., i]);
    var Bv = dot(eigVec, B);
    var ku = dot(k, eigVec);

    for (b, k) in zip(Bv, ku) {
      if !isClose(b, k) then
        writeln(b, " != ", k);
    }
  }
}

// NOTE : Tolerances should probably be set by type.
proc testArray(truth : [?Dom] ?t, check : [Dom] t, src : string) {
  var atol, rtol;
  select t {
      when complex(64) {atol=1.0e-6; rtol=1.0e-6;}
      when complex(128) {atol=1.0e-15; rtol=1.0e-14;}
      when real(32) {atol=1.0e-6; rtol=1.0e-6;}
      when real(64) {atol=1.0e-15; rtol=1.0e-14;}
      otherwise {atol=0.0; rtol=1.0e-5;} // Use some random defaults
  }
  for (ndx, t1, c1) in zip(Dom, truth, check) {
    if !isclose(t1, c1, atol=atol, rtol=rtol) then
      writeln("Expected ", t1, " != obtained ", c1, " at index ", ndx, " in ",src);
  }
}


proc testEighHelper(mat0 : [?Dom]?t, true0, eig0) {

  param nBits = if isComplexType(t) then numBits(t)/2 else numBits(t);
  var one = eye(Dom, eltType=real(nBits));

  const typ = if isComplexType(t) then "herm" else "symm";
  
  // Lower tri, overwrite=false
  {
    const msg = "lower tri, overwrite=false, %s".format(typ);
    var mat1 = tril(mat0);
    var w = eigvalsh(mat1, lower=true);
    var (w1, eig1) = eigh(mat1, lower=true);
    testArray(true0, w, msg);
    testArray(true0, w1, msg);
    var eet = abs(dot(eig0, eig1)); // element-wise square -- should be the identity matrix
    testArray(one, eet, msg); // not overwritten
    testArray(tril(mat0), mat1, msg); // not overwritten
  }

  // Upper tri, overwrite=false
  {
    const msg = "upper tri, overwrite=false, %s".format(typ);
    var mat1 = triu(mat0);
    var w = eigvalsh(mat1, lower=false);
    var (w1, eig1) = eigh(mat1, lower=false);
    testArray(true0, w, msg);
    testArray(true0, w1, msg);
    var eet = abs(dot(eig0, eig1)); // element-wise square -- should be the identity matrix
    testArray(one, eet, msg); // not overwritten
    testArray(triu(mat0), mat1, msg); //not overwritten
  }

  // Lower tri, overwrite
  {
    const msg = "lower tri, %s".format(typ);
    var mat1 = tril(mat0);
    var w = eigvalsh(mat1, lower=true, overwrite=true);
    testArray(true0, w, msg);
    mat1 = tril(mat0);
    w = eigh(mat1, lower=true, overwrite=true);
    var eet = abs(dot(eig0, mat1));
    testArray(one, eet, msg); // not overwritten
  }

  // Upper tri, overwrite
  {
    const msg = "upper tri, %s".format(typ);
    var mat1 = triu(mat0);
    var w = eigvalsh(mat1, lower=false, overwrite=true);
    testArray(true0, w, msg);
    mat1 = triu(mat0);
    w = eigh(mat1, lower=false, overwrite=true);
    var eet = abs(dot(eig0, mat1));
    testArray(one, eet, msg); // not overwritten
  }
}


proc testEigenvalSymm1(type t) {

  // Set the first element of each eigenvector to be positive
  var mat0 = Matrix([-2,1,0],[1,-2,1],[0,1,-2],eltType=t);
  var true0 = Vector(-sqrt_2,0.0,sqrt_2, eltType=t) - 2.0 : t;
  var eig0 = Matrix([0.5,-recipr_sqrt_2,0.5],
                    [-recipr_sqrt_2,0.0,recipr_sqrt_2],
                    [0.5,recipr_sqrt_2,0.5],eltType=t);

  testEighHelper(mat0, true0, eig0);
}
testEigenvalSymm1(real(32));
testEigenvalSymm1(real(64));


proc testEigenvalHerm1(type t) {
  var mat0 = Matrix([-2+0i,0+1i,3+0i],[0-1i,-2+0i,2+2i],[3+0i,2-2i,-2+0i],eltType=t);
  var true0 = Vector(-6.5432665971054891651,
                     -1.3155171261767867149,
                      1.8587837232822758800, eltType=real(numBits(t)/2));
  // These are the eigenvectors
  var eig0 = Matrix([-0.53213104215190341939 + 0.068091856486548515348i,
                     -0.30935945711023660270 - 0.41149724184005937572i,
                     +0.66870531575053882965 + 0.0i],
                    [-0.50426352398839197001 + 0.44673689964442607227i,
                     +0.71701929683028175571 + 0.0i,       
                     -0.11505324868793179547 - 0.13707851330626418953i],
                    [+0.49754108324625095552 + 0.10391036719974609152i,
                     +0.40096763363066469802 + 0.24510208283104556075i,
                     +0.72166850550863686527 + 0.0i], eltType=t);
  var eig0h = conjg(eig0);
  
  testEighHelper(mat0, true0, eig0h);
}
testEigenvalHerm1(complex(64));
testEigenvalHerm1(complex(128));

