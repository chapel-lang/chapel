// a test file for sparse matrix matrix multiplication that has a zero indexed,
// 1-indexed, and a wonky index example
import LinearAlgebra;
use LinearAlgebra.Sparse;
use IO;

// helper function for printing dense layout of sparse matrix
proc writeArr(spsArr: [?Dom] ?eltType, parentDom: domain(2)) {
  for (i,j) in parentDom {
    write(spsArr(i,j), " ");
    if (j == parentDom.dim(1).high) then writeln();
  }
  writeln();
}

// Returns a Dense array of ones
proc ones(Dom: domain(2), type eltType=real) {
  var A: [Dom] eltType = 1 : eltType;
  return A;
}

{
  // Case 1: Product of a Dense matrix (3x4) and a Sparse matrix (3x3) should
  // throw an error. [eltType = real]

  const BparentDom = {0..2,0..2};
  var D34 = {0..2,0..3};

  var BDom: sparse subdomain(BparentDom) dmapped CS(compressRows=true,sortedIndices=true);
  var B: [BDom] real;
  BDom += [(0,0),(0,1),(1,1),(2,2)];

  B(0,0) = 0.0;
  B(0,1) = 5.0;
  B(1,1) = -9.0;
  B(2,2) = 5.0;

  var C34 = ones(D34);

  //Real
  writeln("B =");
  writeArr(B,{0..#3,0..#3});

  writeln("C34 =");
  writeArr(C34,{0..#3,0..#4});

  // Halt
  writeln("// Product of a Dense matrix (3x4) and a Sparse matrix (3x3), should throw an error. [eltType = real]");
  writeln("C34B = dot(C34,B) =");
  var C34B = dot(C34,B);
}
