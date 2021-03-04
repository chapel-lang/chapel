// Deprecated in Chapel 1.24, to be removed in Chapel 1.25

import LinearAlgebra;
use LinearAlgebra.Sparse;

var data = [10,20,30,40,50];
var indices = [0,2,0,0,2];
var indptr = [0,2,3,5];

var A = CSRMatrix((3,3), data, indices, indptr);
