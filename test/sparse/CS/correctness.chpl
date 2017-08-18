use LayoutCS;
use Random;

/*
  Tests used for development of LayoutCS

  Matrix originated from Colorado State's useful example:

    http://www.cs.colostate.edu/~mcrob/toolbox/c++/sparseMatrix/sparse_matrix_compression.html
 */

proc main() {
  var D = {1..6, 1..6};

  const csrDmap = new dmap(new CS(compressRows=true)),
        cscDmap = new dmap(new CS(compressRows=false)),
        csDmap  = new dmap(new CS());

  var csrDom: sparse subdomain(D) dmapped csrDmap,
      cscDom: sparse subdomain(D) dmapped cscDmap;

  var indices = [(1,1), (1,2), (1,3), (1,4),
                 (2,2), (2,3),
                 (3,3), (3,4), (3,5), (3,6),
                 (4,4), (4,5),
                 (5,6),
                 (6,6)];

  writeln('DSI');
  writeln('===');

  // dsiFirst && dsiLast, for tricky cases
  {

    var csrD: sparse subdomain(D) dmapped csrDmap,
        cscD: sparse subdomain(D) dmapped cscDmap;

    // Empty domain
    assert(csrD.first == (0,0));
    assert(cscD.first == (0,0));
    assert(csrD.last == (0,0));
    assert(cscD.last == (0,0));

    csrD += [(1,2), (2,1), (4,5), (5,4)];
    cscD += [(1,2), (2,1), (4,5), (5,4)];

    // Differentiate first & last for CSC & CSR
    assert(csrD.first == (1,2));
    assert(cscD.first == (2,1));
    assert(csrD.last == (5,4));
    assert(cscD.last == (4,5));

    csrD -= [(1,2), (2,1), (4,5)];
    cscD -= [(1,2), (2,1), (4,5)];

    // 1-element domain
    assert(csrD.first == (5,4));
    assert(cscD.first == (5,4));
    assert(csrD.last == (5,4));
    assert(cscD.last == (5,4));
  }

  // dsiEqualsDmap && default value
  assert(csDmap == csrDmap);

  // dsiBulkAdd with domain
  csrDom += D;
  cscDom += D;

  assert(csrDom.size == D.size);
  assert(cscDom.size == D.size);

  // dsiRemove with domain
  csrDom -= D;
  cscDom -= D;

  assert(csrDom.size == 0);
  assert(cscDom.size == 0);

  // dsiBulkAdd when nnz = 0
  csrDom += indices[..5];
  cscDom += indices[..5];


  // dsiRemove
  cscDom -= indices[5];
  csrDom -= indices[5];

  assert(csrDom.size == 4);
  assert(cscDom.size == 4);

  // dsiRemove, "promoted"
  csrDom -= indices[1..4];
  cscDom -= indices[1..4];

  assert(csrDom.size == 0);
  assert(cscDom.size == 0);

  // dsiAdd
  csrDom += indices[5];
  cscDom += indices[5];

  // dsiAdd duplicates
  csrDom += indices[4];
  cscDom += indices[4];

  // Shuffle indices to test dsiBulkAdd with unsorted data
  shuffle(indices);

  // dsiBulkAdd when nnz > 0, with duplicates
  csrDom += indices;
  cscDom += indices;

  // proc ==()
  assert(csrDom == cscDom);

  // dsiClone
  var copyDom = csrDom;
  assert(copyDom == csrDom);

  var csrCopy: [csrDom] int,
      cscCopy: [cscDom] int;

  const csrOnes: [csrDom] int = 1,
        cscOnes: [cscDom] int = 1;

  // standalone these()
  forall r in csrDom {
    csrCopy[r] = 1;
  }
  assert(csrOnes.equals(csrCopy));
  csrCopy = 0;

  forall c in cscDom {
    cscCopy[c] = 1;
  }
  assert(cscOnes.equals(cscCopy));
  cscCopy = 0;

  // leader/follower these()
  for (r,c) in zip(csrDom, cscDom) {
    csrCopy[r] = 1;
    cscCopy[c] = 1;
  }
  assert(csrOnes.equals(csrCopy));
  assert(cscOnes.equals(cscCopy));
  csrCopy = 0;
  cscCopy = 0;

  for (r,c) in zip(csrDom, cscDom) {
    csrCopy[r] = 1;
    cscCopy[c] = 1;
  }
  assert(csrOnes.equals(csrCopy));
  assert(cscOnes.equals(cscCopy));

  // parallel leader/follower these()
  forall (r1, r2) in zip(csrDom, csrDom) {
    assert(r1 == r2);
  }

  forall (c1, c2) in zip(cscDom, cscDom) {
    assert(c1 == c2);
  }

  writeln('csrDom:');
  writeDSI(csrDom);
  writeln('cscDom:');
  writeDSI(cscDom);

  // dsiAssignDomain
  var csrDom2 = csrDom;
  var cscDom2 = cscDom;
  assert(csrDom2 == csrDom);
  assert(cscDom2 == cscDom);

  var csrArr: [csrDom] int,
      cscArr: [cscDom] int;

  // dimIter (pretty trivial tests)
  // csrDom[5, ..]
  for i in csrDom.dimIter(2, 5) {
    assert(i == 6);
  }
  // csrDom[.., 1]
  for i in cscDom.dimIter(1, 1) {
    assert(i == 1);
  }

  // Parallel .these
  forall (i,j) in csrArr.domain {
    csrArr[i,j] = 10*i + j;
  }

  forall (i,j) in cscArr.domain {
    cscArr[i,j] = 10*i + j;
  }

  writeln('csrArr non-zeroes:');
  writeln(csrArr);

  writeln('csrArr:');
  writeDense(csrArr);

  writeln('cscArr non-zeroes:');
  writeln(cscArr);

  writeln('cscArr:');
  writeDense(cscArr);


  writeln('Internals');
  writeln('=========');
  writeln('CSR');
  writeln('---');
  writeInternals(csrArr);
  writeln();
  writeln('CSC');
  writeln('---');
  writeInternals(cscArr);
}

proc writeDSI(D) {
  writeln("size:\t\t", D.size);
  writeln("numIndices:\t",D.numIndices);
  writeln("low:\t\t",D.low);
  writeln("high:\t\t",D.high);
  writeln("stride:\t\t",D.stride);
  writeln("alignment:\t",D.alignment);
  writeln("first:\t\t",D.first);
  writeln("last:\t\t",D.last);
  writeln("alignedLow:\t",D.alignedLow);
  writeln("alignedHigh:\t",D.alignedHigh);
  writeln(D);
}

proc writeDense(A: [?D]) {
  for i in D.dim(1) {
    for j in D.dim(2) {
      writef('%2n ', A[i, j]);
    }
    writeln();
  }
}

proc writeInternals(A) {
  const row = A.domain._value.compressRows;
  if row then writeln('Row Start Index:');
  else writeln('Column Start Index:');

  var dimension = if row then 1 else 2;

  for i in A.domain.dim(dimension) {
    write(A.domain._value.startIdx(i), ' ');
  }
  writeln();

  if row then writeln('Column Index:');
  else writeln('Row Index:');

  for i in 1..A.domain._value.nnz {
    write(A.domain._value.idx(i), ' ');
  }
  writeln();
}
