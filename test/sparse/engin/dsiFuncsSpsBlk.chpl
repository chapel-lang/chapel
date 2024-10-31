use BlockDist;
use CompressedSparseLayout;

config const N = 10;
const ParentDom = {7..#2*N align 1, 17..#3*N align 5}
  dmapped new blockDist({7..#2*N, 17..#3*N});

var SparseDom: sparse subdomain (ParentDom);
var SparseMat: [SparseDom] int;

writeln(ParentDom.low);

var row = [i in ParentDom.dim(1)] (ParentDom.dim(0).low + 
    ParentDom.stride[0]*2, i);
var col = [i in ParentDom.dim(0)] (i, ParentDom.dim(1).low + 
    ParentDom.stride[1]*3);
//add one full row and column
SparseDom += row;
SparseDom += col;

SparseMat = 5;

for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i, j], " ");
  }
  writeln();
}

writeln("size:\t\t", SparseDom.size);
writeln("lowBound:\t",SparseDom.lowBound);
writeln("highBound:\t",SparseDom.highBound);
writeln("stride:\t\t",SparseDom.stride);
writeln("alignment:\t",SparseDom.alignment);
writeln("first:\t\t",SparseDom.first);
writeln("last:\t\t",SparseDom.last);
writeln("low:\t",SparseDom.low);
writeln("high:\t",SparseDom.high);
