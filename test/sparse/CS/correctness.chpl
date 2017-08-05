use LayoutCS;

/*
  Tests used for development of LayoutCS

  Matrix originated from Colorado State's useful example:

    http://www.cs.colostate.edu/~mcrob/toolbox/c++/sparseMatrix/sparse_matrix_compression.html
 */

proc main() {
  var D = {1..6, 1..6};

  var csrDom: sparse subdomain(D) dmapped CS(row=true),
      cscDom: sparse subdomain(D) dmapped CS(row=false);

  const indices = [(1,1), (1,2), (1,3), (1,4),
                   (2,2), (2,3),
                   (3,3), (3,4), (3,5), (3,6),
                   (4,4), (4,5),
                   (5,6),
                   (6,6)];

  writeln('DSI');
  writeln('===');

  // TODO: dsiBulkAdd overloads / unsorted data

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

  // dsiBulkAdd when nnz > 0, with duplicates
  csrDom += indices;
  cscDom += indices;

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
  const row = A.domain._value.row;
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
