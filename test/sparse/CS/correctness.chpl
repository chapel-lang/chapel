use LayoutCS;

// TODO: Cleanup
/*
  Testing various array properties with test matrix inspired by
  Colorado State's useful example matrix:

    http://www.cs.colostate.edu/~mcrob/toolbox/c++/sparseMatrix/sparse_matrix_compression.html
 */

proc main() {
  var D = {1..6, 1..6};

  var csrDom: sparse subdomain(D) dmapped CS(row=true),
      cscDom: sparse subdomain(D) dmapped CS(row=false);

  var indices = [(1,1), (1,2), (1,3), (1,4),
                 (2,2), (2,3),
                 (3,3), (3,4), (3,5), (3,6),
                 (4,4), (4,5),
                 (5,6),
                 (6,6)];

  writeln('DSI');
  writeln('===');

  // bulkAdd when nnz = 0
  csrDom += indices[..4];
  cscDom += indices[..4];

  // dsiAdd, with duplicates
  csrDom += indices[4];
  csrDom += indices[5];
  cscDom += indices[4];
  cscDom += indices[5];

  // bulkAdd when nnz > 0, with duplicates
  csrDom += indices;
  cscDom += indices;

  // TODO: Remove
  // TODO: .these (parallel)
  // TODO: IndsIterSafeForRemoving() {

  writeln('csrDom:');
  writeDSI(csrDom);
  writeln('cscDom:');
  writeDSI(cscDom);


  var csrArr: [csrDom] real,
      cscArr: [cscDom] real;

  for (i,j) in csrArr.domain {
    csrArr[i,j] = 10*i + j;
  }

  for (i,j) in cscArr.domain {
    cscArr[i,j] = 10*i + j;
  }

  writeln('csrArr non-zeroes:');
  writeln(csrArr);

  writeln('csrArr:');
  for i in D.dim(1) {
    for j in D.dim(2) {
      write(cscArr[i, j], " ");
    }
    writeln();
  }

  writeln('cscArr non-zeroes:');
  writeln(cscArr);

  writeln('cscArr:');
  for i in D.dim(1) {
    for j in D.dim(2) {
      write(cscArr[i, j], " ");
    }
    writeln();
  }


  writeln('Internals');
  writeln('=========');
  writeln();
  writeln('CSC');
  writeln('---');
  writeln('Column Start Index:');
  for i in cscArr.domain.dim(2) {
    // 1 2 4 7 10 12
    write(cscArr.domain._value.startIdx(i), ' ');
  }

  writeln();
  writeln('Row Index:');
  for i in 1..cscArr.domain._value.nnz {
    // 1 2 3 4 2 3 3 4 5 6 4 5 6 6
    write(cscArr.domain._value.idx(i), ' ');
  }
  writeln();

  writeln('CSR');
  writeln('---');
  writeln('Row Start Index:');
  for i in csrArr.domain.dim(1) {
    // 1 5 7 11 13 14
    write(csrArr.domain._value.startIdx(i), ' ');
  }
  writeln();
  writeln('Column Index:');
  for i in 1..csrArr.domain._value.nnz {
    // 1 2 3 4 2 3 3 4 5 6 4 5 6 6
    write(csrArr.domain._value.idx(i), ' ');
  }

  writeln();

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

