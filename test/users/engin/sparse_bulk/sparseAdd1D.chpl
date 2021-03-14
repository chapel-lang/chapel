config const N=100;

const ParentDom = {0..#N};
var SparseDom: sparse subdomain(ParentDom);

//domain for multiples of 5
var FSparseDom: sparse subdomain(ParentDom);
for i in ParentDom by 5 {
  FSparseDom += i;
}

//domain for multiples of 7
var SSparseDom: sparse subdomain(ParentDom);
for i in ParentDom by 7 {
  SSparseDom += i;
}

SparseDom += FSparseDom;
SparseDom += SSparseDom;

for i in SparseDom do writeln(i);
