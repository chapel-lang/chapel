use LinearAlgebra;
use LinearAlgebra.Sparse;
use BlockDist;
use LayoutCS;
use List;

config const n = 20;

proc getSparse(d, stride) {
  var sdom: sparse subdomain(d);
  var indices: list(2*d.idxType);
  for (i,j) in d by stride {
    indices.append((i,j));
  }
  sdom += indices.toArray();
  return sdom;
}

proc getSparseArr(d) {
  var arr: [d] real;
  var v: real = 1.0;
  for ind in d {
    arr(ind) = v;
    v+=1;
  }
  return arr;
}

proc print_sparsearr(A: [?D] ?eltType) {
  for (i,j) in D.parentDom {
    if D.contains(i,j) then
      write(A(i,j), " "); 
    else
      write(". ");

    if (j == D.shape(1)) then writeln();
  }
}

var parentDom2D = {1..n, 1..n};
var Adom = getSparse(parentDom2D, 2);
var Bdom = getSparse(parentDom2D, 3);
var A = getSparseArr(Adom);
var B = getSparseArr(Bdom);

print_sparsearr(A);
writeln();
print_sparsearr(B);
writeln();
print_sparsearr(A.plus(B));
writeln();
print_sparsearr(A.minus(B));
writeln();
print_sparsearr(A.times(B));
writeln();
print_sparsearr(A.elementDiv(B));
writeln();

