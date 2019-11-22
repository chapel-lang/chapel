use LayoutCS;

const ParentDom = {1..10, 1..10};

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

const indices = [(3,8), (5,7), (1,5), (9,9), (4,4)];
SparseDom += indices[..1];

var SparseArr: [SparseDom] int;

for i in indices do write(SparseArr[i], " ");
writeln();

SparseArr.IRV=5;
for i in indices do write(SparseArr[i], " ");
writeln();

SparseDom += indices[3..];
for i in indices do write(SparseArr[i], " ");
writeln();
