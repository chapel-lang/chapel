use LinearAlgebra;
use LinearAlgebra.Sparse;
use BlockDist;
use Lists;

config const n = 16;
config const stride = 7;

// TODO: As of now distributed sparse is not supported, though it is
//       likely to run fine with the current implementation of transpose.
//       The `isSparseArr()` in the `where` clause prevents `transpose`
//       being called on a distributed sparse domain/array.

// Domains

var parentDom2D = {1..n, 1..n};
// var distParentDom2D = {1..n, 1..n} dmapped Block({1..n, 1..n});

proc getSparse(d, stride) {
  var sdom: sparse subdomain(d);
  var indices: [1..0] (if d.rank==1 then d.idxType else d.rank*d.idxType);
  var count = 0;
  for ind in d {
    if (count % stride == 0) then indices.push_back(ind);
    count += 1;
  }
  sdom += indices;
  return sdom;
}

proc printDomain(d) {
  var indexList: list(if d.rank==1 then d.idxType else d.rank*d.idxType);
  for i in d do
    indexList.append(i);
  indexList.sort();
  writeln(indexList);
}


var sparseDom2D = getSparse(parentDom2D, stride);
// var distSparseDom2D = getSparse(distParentDom2D, stride);

printDomain(sparseDom2D);
printDomain(transpose(sparseDom2D));

// printDomain(distSparseDom2D);
// printDomain(transpose(distSparseDom2D));



// Arrays
var sparseArr2D = getSparseArr(sparseDom2D);
// var distSparseArr2D = getSparseArr(distSparseDom2D);

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

    if (j == D.shape(2)) then writeln();
  }
}

print_sparsearr(sparseArr2D);
writeln();
print_sparsearr(transpose(sparseArr2D));

// print_sparsearr(distSparseArr2D);
// print_sparsearr(transpose(distSparseArr2D));
