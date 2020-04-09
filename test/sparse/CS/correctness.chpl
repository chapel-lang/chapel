use LayoutCS;
use Random;

/*
  Tests used for development of LayoutCS

  Matrix originated from Colorado State's useful example:

    http://www.cs.colostate.edu/~mcrob/toolbox/c++/sparseMatrix/sparse_matrix_compression.html
 */

proc main() {
  var D = {1..6, 1..6};

  const csrsDmap = new dmap(new CS(compressRows=true,sortedIndices=true)),
        cscsDmap = new dmap(new CS(compressRows=false,sortedIndices=true)),
        csruDmap = new dmap(new CS(compressRows=true,sortedIndices=false)),
        cscuDmap = new dmap(new CS(compressRows=false,sortedIndices=false)),
        csDmap  = new dmap(new CS());

  var csrsDom: sparse subdomain(D) dmapped csrsDmap,
      cscsDom: sparse subdomain(D) dmapped cscsDmap,
      csruDom: sparse subdomain(D) dmapped csruDmap,
      cscuDom: sparse subdomain(D) dmapped cscuDmap;

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

    var csrsD: sparse subdomain(D) dmapped csrsDmap,
        cscsD: sparse subdomain(D) dmapped cscsDmap,
        csruD: sparse subdomain(D) dmapped csruDmap,
        cscuD: sparse subdomain(D) dmapped cscuDmap;

    // Empty domain
    assert(csrsD.first == (0,0));
    assert(cscsD.first == (0,0));
    assert(csrsD.last == (0,0));
    assert(cscsD.last == (0,0));
    assert(csruD.first == (0,0));
    assert(cscuD.first == (0,0));
    assert(csruD.last == (0,0));
    assert(cscuD.last == (0,0));

    var insert_indices = [(1,2), (2,1), (4,5), (5,4)];
    shuffle( insert_indices, 123456789 );
    csrsD += insert_indices;
    cscsD += insert_indices;
    csruD += insert_indices;
    cscuD += insert_indices;

    // Differentiate first & last for CSC & CSR
    assert(csrsD.first == (1,2));
    assert(cscsD.first == (2,1));
    assert(csrsD.last == (5,4));
    assert(cscsD.last == (4,5));
    assert(csruD.first == (1,2));
    assert(cscuD.first == (2,1));
    assert(csruD.last == (5,4));
    assert(cscuD.last == (4,5));

    var remove_indices = [(1,2), (2,1), (4,5)];
    shuffle( remove_indices, 987654321 );
    csrsD -= remove_indices;
    cscsD -= remove_indices;
    csruD -= remove_indices;
    cscuD -= remove_indices;

    // 1-element domain
    assert(csrsD.first == (5,4));
    assert(cscsD.first == (5,4));
    assert(csrsD.last == (5,4));
    assert(cscsD.last == (5,4));
    assert(csruD.first == (5,4));
    assert(cscuD.first == (5,4));
    assert(csruD.last == (5,4));
    assert(cscuD.last == (5,4));
  }

  // dsiEqualsDmap && default value
  if LayoutCSDefaultToSorted {
    assert(csDmap == csrsDmap);
  } else {
    assert(csDmap == csruDmap);
  }

  // dsiBulkAdd with domain
  csrsDom += D;
  cscsDom += D;
  csruDom += D;
  cscuDom += D;

  assert(csrsDom.size == D.size);
  assert(cscsDom.size == D.size);
  assert(csruDom.size == D.size);
  assert(cscuDom.size == D.size);

  // dsiRemove with domain
  csrsDom -= D;
  cscsDom -= D;
  csruDom -= D;
  cscuDom -= D;

  assert(csrsDom.size == 0);
  assert(cscsDom.size == 0);
  assert(csruDom.size == 0);
  assert(cscuDom.size == 0);

  // dsiBulkAdd when nnz = 0
  csrsDom += indices[..4];
  cscsDom += indices[..4];
  csruDom += indices[..4];
  cscuDom += indices[..4];


  // dsiRemove
  cscsDom -= indices[4];
  csrsDom -= indices[4];
  cscuDom -= indices[4];
  csruDom -= indices[4];

  assert(csrsDom.size == 4);
  assert(cscsDom.size == 4);
  assert(csruDom.size == 4);
  assert(cscuDom.size == 4);

  // dsiRemove, "promoted"
  csrsDom -= indices[0..3];
  cscsDom -= indices[0..3];
  csruDom -= indices[0..3];
  cscuDom -= indices[0..3];

  assert(csrsDom.size == 0);
  assert(cscsDom.size == 0);
  assert(csruDom.size == 0);
  assert(cscuDom.size == 0);

  // dsiAdd
  csrsDom += indices[4];
  cscsDom += indices[4];
  csruDom += indices[4];
  cscuDom += indices[4];

  // dsiAdd duplicates
  csrsDom += indices[3];
  cscsDom += indices[3];
  csruDom += indices[3];
  cscuDom += indices[3];

  // Shuffle indices to test dsiBulkAdd with unsorted data
  shuffle(indices, 231564879);

  // dsiBulkAdd when nnz > 0, with duplicates
  csrsDom += indices;
  cscsDom += indices;
  csruDom += indices;
  cscuDom += indices;

  // proc ==()
  assert(csrsDom == cscsDom);
  assert(csruDom == cscuDom);
  assert(csrsDom == csruDom);
  assert(cscsDom == cscuDom);
  assert(csrsDom == cscuDom);
  assert(cscsDom == csruDom);

  // dsiClone
  var copyDom = csrsDom;
  assert(copyDom == csrsDom);

  var csrsCopy: [csrsDom] int,
      cscsCopy: [cscsDom] int,
      csruCopy: [csruDom] int,
      cscuCopy: [cscuDom] int;

  const csrsOnes: [csrsDom] int = 1,
        cscsOnes: [cscsDom] int = 1,
        csruOnes: [csruDom] int = 1,
        cscuOnes: [cscuDom] int = 1;

  // standalone these()
  forall r in csrsDom {
    csrsCopy[r] = 1;
  }
  assert(csrsOnes.equals(csrsCopy));
  csrsCopy = 0;

  forall c in cscsDom {
    cscsCopy[c] = 1;
  }
  assert(cscsOnes.equals(cscsCopy));
  cscsCopy = 0;

  forall r in csruDom {
    csruCopy[r] = 1;
  }
  assert(csruOnes.equals(csruCopy));
  csruCopy = 0;

  forall c in cscuDom {
    cscuCopy[c] = 1;
  }
  assert(cscuOnes.equals(cscuCopy));
  cscuCopy = 0;

  // leader/follower these()
  for (r,c, _, _) in zip(csrsDom, cscsDom, csruDom, cscuDom) {
    csrsCopy[r] = 1;
    cscsCopy[c] = 1;
    csruCopy[r] = 1;
    cscuCopy[c] = 1;
  }
  assert(csrsOnes.equals(csrsCopy));
  assert(cscsOnes.equals(cscsCopy));
  assert(csruOnes.equals(csruCopy));
  assert(cscuOnes.equals(cscuCopy));
  csrsCopy = 0;
  cscsCopy = 0;
  csruCopy = 0;
  cscuCopy = 0;

  for (r,c, _, _) in zip(csrsDom, cscsDom, csruDom, cscuDom) {
    csrsCopy[r] = 1;
    cscsCopy[c] = 1;
    csruCopy[r] = 1;
    cscuCopy[c] = 1;
  }
  assert(csrsOnes.equals(csrsCopy));
  assert(cscsOnes.equals(cscsCopy));
  assert(csruOnes.equals(csruCopy));
  assert(cscuOnes.equals(cscuCopy));

  // parallel leader/follower these()
  forall (r1, r2) in zip(csrsDom, csrsDom) {
    assert(r1 == r2);
  }

  forall (c1, c2) in zip(cscsDom, cscsDom) {
    assert(c1 == c2);
  }

  forall (r1, r2) in zip(csruDom, csruDom) {
    assert(r1 == r2);
  }

  forall (c1, c2) in zip(cscuDom, cscuDom) {
    assert(c1 == c2);
  }

  writeln('csrsDom:');
  writeDSI(csrsDom);
  writeln('cscsDom:');
  writeDSI(cscsDom);
  writeln('csruDom:');
  writeDSI(csruDom);
  writeln('cscuDom:');
  writeDSI(cscuDom);

  // dsiAssignDomain
  var csrsDom2 = csrsDom;
  var cscsDom2 = cscsDom;
  var csruDom2 = csruDom;
  var cscuDom2 = cscuDom;
  assert(csrsDom2 == csrsDom);
  assert(cscsDom2 == cscsDom);
  assert(csruDom2 == csruDom);
  assert(cscuDom2 == cscuDom);

  var csrsArr: [csrsDom] int,
      cscsArr: [cscsDom] int,
      csruArr: [csruDom] int,
      cscuArr: [cscuDom] int;

  // dimIter (pretty trivial tests)
  // csrsDom[5, ..]
  for i in csrsDom.dimIter(1, 5) {
    assert(i == 6);
  }
  // csrsDom[.., 1]
  for i in cscsDom.dimIter(0, 1) {
    assert(i == 1);
  }
  // csruDom[5, ..]
  for i in csruDom.dimIter(1, 5) {
    assert(i == 6);
  }
  // csruDom[.., 1]
  for i in cscuDom.dimIter(0, 1) {
    assert(i == 1);
  }

  // Parallel .these
  forall (i,j) in csrsArr.domain {
    csrsArr[i,j] = 10*i + j;
  }

  forall (i,j) in cscsArr.domain {
    cscsArr[i,j] = 10*i + j;
  }

  forall (i,j) in csruArr.domain {
    csruArr[i,j] = 10*i + j;
  }

  forall (i,j) in cscuArr.domain {
    cscuArr[i,j] = 10*i + j;
  }

  writeln('csrsArr non-zeroes:');
  writeln(csrsArr);

  writeln('csrsArr:');
  writeDense(csrsArr);

  writeln('cscsArr non-zeroes:');
  writeln(cscsArr);

  writeln('cscsArr:');
  writeDense(cscsArr);

  writeln('csruArr non-zeroes:');
  writeln(csruArr);

  writeln('csruArr:');
  writeDense(csruArr);

  writeln('cscuArr non-zeroes:');
  writeln(cscuArr);

  writeln('cscuArr:');
  writeDense(cscuArr);


  writeln('Internals');
  writeln('=========');
  writeln('CSR-sorted');
  writeln('---');
  writeInternals(csrsArr);
  writeln();
  writeln('CSC-sorted');
  writeln('---');
  writeInternals(cscsArr);
  writeln();
  writeln('CSR-unsorted');
  writeln('---');
  writeInternals(csruArr);
  writeln();
  writeln('CSC-unsorted');
  writeln('---');
  writeInternals(cscuArr);
}

proc writeDSI(D) {
  writeln("size:\t\t", D.size);
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
  for i in D.dim(0) {
    for j in D.dim(1) {
      writef('%2n ', A[i, j]);
    }
    writeln();
  }
}

proc writeInternals(A) {
  const row = A.domain._value.compressRows;
  if row then writeln('Row Start Index:');
  else writeln('Column Start Index:');

  var dimension = if row then 0 else 1;

  for i in A.domain.dim(dimension) {
    write(A.domain._value.startIdx(i), ' ');
  }
  writeln();

  if row then writeln('Column Index:');
  else writeln('Row Index:');

  for i in 1..A.domain._value.getNNZ() {
    write(A.domain._value.idx(i), ' ');
  }
  writeln();
}
