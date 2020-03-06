use BlockDist;
use LayoutCS;

config const N = 10;
const ParentDom = {7..#2*N align 1, 17..#3*N align 5}
  dmapped Block({7..#2*N, 17..#3*N});

var SparseDom: sparse subdomain (ParentDom);
var SparseMat: [SparseDom] int;

writeln(ParentDom.alignedLow);

var row = [i in ParentDom.dim(2)] (ParentDom.dim(1).alignedLow + 
    ParentDom.stride[1]*2, i);
var col = [i in ParentDom.dim(1)] (i, ParentDom.dim(2).alignedLow + 
    ParentDom.stride[2]*3);
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
writeln("first:\t\t",SparseDom.first);
writeln("last:\t\t",SparseDom.last);
writeln("alignedLow:\t",SparseDom.alignedLow);
writeln("alignedHigh:\t",SparseDom.alignedHigh);
