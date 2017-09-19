use LayoutCS;

config const N = 10;
const ParentDom = {0..#N, 0..#N};
var SparseDom: sparse subdomain(ParentDom);

const denseRow = [i in ParentDom.dim(2)] (3,i);
SparseDom += denseRow;

writeln(SparseDom.member(3, 5));
writeln(SparseDom.member(-1,-1)); //recently resolved bug - Engin
