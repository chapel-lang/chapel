use LayoutCS;

config const N = 10;
const ParentDom = {7..#2*N align 1, 17..#3*N align 5};

enum layoutTypes {coo, csr, csc};
config param layoutType = layoutTypes.coo;

var csrDom: sparse subdomain(ParentDom) dmapped CS(compressRows=true);
var cscDom: sparse subdomain(ParentDom) dmapped CS(compressRows=false);
var cooDom: sparse subdomain(ParentDom);

var SparseDom = if layoutType == layoutTypes.csr then 
                  csrDom
               else if layoutType == layoutTypes.csc then
                  cscDom
               else
                  cooDom;
var SparseMat: [SparseDom] int;

writeln(ParentDom.alignedLow);

const rowToAdd = ParentDom.dim(1).alignedLow + ParentDom.stride[1]*2;
const colToAdd = ParentDom.dim(2).alignedLow + ParentDom.stride[2]*3;
var row = [i in ParentDom.dim(2)] (rowToAdd, i);
var col = [i in ParentDom.dim(1)] (i, colToAdd);

//add one full row and column
SparseDom += row;
SparseDom += col;

SparseMat = 5;

for i in ParentDom.dim(1) {
  for j in ParentDom.dim(2) {
    write(SparseMat[i, j], " ");
  }
  writeln();
}

writeln("size:\t\t", SparseDom.size);
writeln("low:\t\t",SparseDom.low);
writeln("high:\t\t",SparseDom.high);
writeln("stride:\t\t",SparseDom.stride);
writeln("alignment:\t",SparseDom.alignment);

// first and last indices depend on the layout type
if layoutType == layoutTypes.csc {
  const expectedFirst = (rowToAdd, ParentDom.dim(2).low);
  const expectedLast = (rowToAdd, ParentDom.dim(2).high);
  writeln("first:\t\t", SparseDom.first == expectedFirst);
  writeln("last:\t\t", SparseDom.last == expectedLast);
}
else {
  const expectedFirst = (ParentDom.dim(1).low, colToAdd);
  const expectedLast = (ParentDom.dim(1).high, colToAdd);
  writeln("first:\t\t", SparseDom.first == expectedFirst);
  writeln("last:\t\t", SparseDom.last == expectedLast);
}

writeln("alignedLow:\t",SparseDom.alignedLow);
writeln("alignedHigh:\t",SparseDom.alignedHigh);
