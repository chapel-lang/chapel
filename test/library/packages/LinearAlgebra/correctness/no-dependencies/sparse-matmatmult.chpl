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

// Usage similar to primers/sparse.chpl, but using the CSR domain.
// Combination of 0-based, 1-based, and other-based parent domains.
{
    const AparentDom = {0..4,1..2};
    var ADom: sparse subdomain(AparentDom) dmapped CS(); // CSR domain
    var A: [ADom] real;
    ADom += [(1,2),(0,1)];
    A(0,1) = 0.1;
    A(1,2) = 1.2;
    writeln("A =");
    writeArr(A,AparentDom);

    const BparentDom = {1..2,3..5};
    var BDom: sparse subdomain(BparentDom) dmapped CS(); // CSR domain
    var B: [BDom] real;
    BDom += [(1,3),(1,5)];
    B(1,3) = 1;
    B(1,5) = 1;
    writeln("B =");
    writeArr(B,BparentDom);

    var C = A.dot(B);
    writeln("C = A.dot(B) =");
    writeArr(C, {0..4,3..5});
}

// Usage similar to primers/LinearAlgebralib.chpl
// Testing 1-based parent domains
{
    const AparentDom = {1..3,1..3};
    var ADom = CSRDomain(AparentDom);
    ADom+= (1,2);
    ADom+= [(3,1),(1,3)];
    var A = CSRMatrix(ADom);
    var B = CSRMatrix(ADom);
    A(1,3) = 20;
    A(3,1) = 50;
    B(1,3) = 1;
    B(3,1) = 2;

    writeln("A =");
    writeArr(A,AparentDom);
    writeln("B =");
    writeArr(B,AparentDom);

    var C = A.dot(B);
    writeln("C = A.dot(B) =");
    writeArr(C, AparentDom);
}

// Usage similar to that in correctness-Sparse-import.chpl
{
    var A = CSRMatrix({0..2, 0..3});
    var B = CSRMatrix({0..3, 0..4});

    writeln("// Empty matrices should have an empty result when multiplied");
    writeln("A.dot(B) = ", A.dot(B));

    var xDom: sparse subdomain({0..2, 0..2}) dmapped CS(sortedIndices=true);
    xDom += [(0,0),(0,1)];

    var X = eye(xDom);
    writeln("X = ");
    writeArr(X,{0..2, 0..2});
}

// not sure we want users to be using the exposed CSR representation
{
    var Adata = [10,20,30,40,50];
    var Aindices = [0,2,0,1,2];
    var Aindptr = [0,2,3,5];

    var A = CSRMatrix((3,3), Adata, Aindices, Aindptr);
    writeln("A =");
    writeArr(A,{0..#3,0..#3});

    var Bdata = [100,200,300,400,500];
    var Bindices = [1,2,0,1,2];
    var Bindptr = [0,3,3,5];

    var B = CSRMatrix((3,3), Bdata, Bindices, Bindptr);
    writeln("B =");
    writeArr(B,{0..#3,0..#3});

    var C = A.dot(B);
    writeln("C = A.dot(B) =");
    writeArr(C, {0..#3,0..#3});
}

{
  // Case 1: Product of a Dense matrix (3x3) and a Sparse matrix (3x3). [eltType = real]
  // Case 2: Product of a Sparse matrix (3x3) and a Dense matrix (3x3). [eltType = real]
  // Case 3: Product of a Sparse matrix (3x3) and a Dense matrix (3x4). [eltType = real]
  // Case 4: Product of a Dense matrix (4x3) and a Sparse matrix (3x3). [eltType = real]
  // Case 5: Product of a Dense matrix (3x3) and a Sparse matrix (3x3). [eltType = complex]
  // Case 6: Product of a Sparse matrix (3x3) and a Dense matrix (3x3). [eltType = complex]
  // Case 7: Product of a Sparse matrix (3x3) and a Dense matrix (3x4). [eltType = complex]
  // Case 8: Product of a Dense matrix (4x3) and a Sparse matrix (3x3). [eltType = complex]

  const BparentDom = {0..2,0..2};
  var D = {0..2,0..2};
  var D34 = {0..2,0..3};
  var D43 = {0..3,0..2};

  var BDom: sparse subdomain(BparentDom) dmapped CS(compressRows=true,sortedIndices=true);
  var B: [BDom] real;
  var B_complex: [BDom] complex;
  BDom += [(0,0),(0,1),(1,1),(2,2)];

  B(0,0) = 0.0;
  B(0,1) = 5.0;
  B(1,1) = -9.0;
  B(2,2) = 5.0;

  B_complex(0,0) = 0.0 + 0.0i;
  B_complex(0,1) = 5.0 + 0.0i;
  B_complex(1,1) = -9.0 + 0.0i;
  B_complex(2,2) = 5.0 + 0.0i;

  var C = ones(D);
  var C34 = ones(D34);
  var C43 = ones(D43);

  var C_complex = ones(D, eltType=complex);
  var C34_complex = ones(D34, eltType=complex);
  var C43_complex = ones(D43, eltType=complex);


  //Real
  writeln("B =");
  writeArr(B,{0..#3,0..#3});

  writeln("C =");
  writeArr(C,{0..#3,0..#3});

  writeln("C34 =");
  writeArr(C34,{0..#3,0..#4});

  writeln("C43 =");
  writeArr(C43,{0..#4,0..#3});

  var BC = dot(B,C);
  var CB = dot(C,B);
  var BC34 = dot(B,C34);
  var C43B = dot(C43,B);

  writeln("// Product of a Sparse matrix (3x3) and a Dense matrix (3x3) [eltType = real]");
  writeln("BC = dot(B,C) =");
  writeArr(BC,{0..#3,0..#3});

  writeln("// Product of a Dense matrix (3x3) and a Sparse matrix (3x3) [eltType = real]");
  writeln("CB = dot(C,B) =");
  writeArr(CB,{0..#3,0..#3});

  writeln("// Product of a Sparse matrix (3x3) and a Dense matrix (3x4) [eltType = real]");
  writeln("BC34 = dot(B,C34) =");
  writeArr(BC34,{0..#3,0..#4});

  writeln("// Product of a Dense matrix (4x3) and a Sparse matrix (3x3) [eltType = real]");
  writeln("C43B = dot(C43,B) =");
  writeArr(C43B,{0..#4,0..#3});


  //Complex
  writeln("B_complex =");
  writeArr(B_complex,{0..#3,0..#3});

  writeln("C_complex =");
  writeArr(C_complex,{0..#3,0..#3});

  writeln("C34_complex =");
  writeArr(C34_complex,{0..#3,0..#4});

  writeln("C43_complex =");
  writeArr(C43_complex,{0..#4,0..#3});

  var BC_complex = dot(B_complex,C_complex);
  var CB_complex = dot(C_complex,B_complex);
  var BC34_complex = dot(B_complex,C34_complex);
  var C43B_complex = dot(C43_complex,B_complex);

  writeln("// Product of a Sparse matrix (3x3) and a Dense matrix (3x3) [eltType = complex]");
  writeln("BC_complex = dot(B_complex,C_complex) =");
  writeArr(BC_complex,{0..#3,0..#3});

  writeln("// Product of a Dense matrix (3x3) and a Sparse matrix (3x3) [eltType = complex]");
  writeln("CB_complex = dot(C_complex,B_complex) =");
  writeArr(CB_complex,{0..#3,0..#3});

  writeln("// Product of a Sparse matrix (3x3) and a Dense matrix (3x4) [eltType = complex]");
  writeln("BC34_complex = dot(B_complex,C34_complex) =");
  writeArr(BC34_complex,{0..#3,0..#4});

  writeln("// Product of a Dense matrix (4x3) and a Sparse matrix (3x3) [eltType = complex]");
  writeln("C43B_complex = dot(C43_complex,B_complex) =");
  writeArr(C43B_complex,{0..#4,0..#3});
}
