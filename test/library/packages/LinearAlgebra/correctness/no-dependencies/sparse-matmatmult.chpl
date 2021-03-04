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
