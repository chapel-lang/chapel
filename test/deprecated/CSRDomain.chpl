// Deprecated in Chapel 1.24, to be removed in Chapel 1.25

import LinearAlgebra;
use LinearAlgebra.Sparse;

var indices = [0,2,0,0,2];
var indptr = [0,2,3,5];

var D = CSRDomain((3,3), indices, indptr);
