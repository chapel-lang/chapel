use LayoutCS;

// TODO: offset & stride
// const ParentDom = {7..#2*N align 1, 17..#3*N align 5};
const globalDom = {1..10, 1..10};  // a default-distributed domain

proc main() {

  sparseTests(true);
  sparseTests(false);

}

/* Some tests of sparse arrays */
proc sparseTests(param row) {
  write('Testing CS');
  if row then writeln('R'); else writeln('C');

  var D: sparse subdomain(globalDom) dmapped CS(row=row);
  var A: [D] real;
  writeln(A);

  var r = [i in globalDom.dim(2)] (globalDom.dim(1).alignedLow +
      globalDom.stride[1]*2, i);
  var c = [i in globalDom.dim(1)] (i, globalDom.dim(2).alignedLow +
      globalDom.stride[2]*3);
  //add one full row and column
  D += r;
  D += c;

  A = 5;

  for i in globalDom.dim(1) {
    for j in globalDom.dim(2) {
      write(A[i, j], " ");
    }
    writeln();
  }

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

}
