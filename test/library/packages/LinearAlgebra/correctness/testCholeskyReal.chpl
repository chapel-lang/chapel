use LinearAlgebra;

var dom = {1..3,1..3};
var arr :[dom] real;
for(i,j) in dom do
    arr[i,j] = i+j;
var L = cholesky(arr);
writeln(L);
