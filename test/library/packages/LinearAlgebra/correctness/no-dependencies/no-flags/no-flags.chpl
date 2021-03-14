/* Confirm that compiling a linear algebra program without any BLAS calls or
   without any BLAS headers available will still work */
use LinearAlgebra;

var A = Matrix(4,4);
var B = Matrix(4,4);

A = 1;
B = 1;


var C = A + B;
