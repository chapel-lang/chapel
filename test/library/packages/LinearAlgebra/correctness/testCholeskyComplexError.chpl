use LinearAlgebra;

var dom = {1..3,1..3};
var arr :[dom] complex;
for(j,k) in dom do
    arr[j,k] = j+5i;
var L = cholesky(arr);
writeln(L);
