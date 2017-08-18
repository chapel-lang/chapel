use LayoutCS;

const ParentDom = {1..10, 1..10};

config type sparseLayoutType = DefaultDist;
const layout = new sparseLayoutType;
var SparseDom: sparse subdomain(ParentDom) dmapped new dmap(layout);

SparseDom += [(3,8), (5,7)];
/*SparseDom += [3,5];*/

var SparseArr: [SparseDom] int;

writeln(SparseArr);

SparseArr.IRV=5;
writeln(SparseArr);

SparseDom += [(1,5),(9,9), (4,4)];
writeln(SparseArr);
