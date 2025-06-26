//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use LinearAlgebra;

/* START_EXAMPLE */
// example1.chpl
var A = Matrix([0.0, 1.0, 1.0],
               [1.0, 0.0, 1.0],
               [1.0, 1.0, 0.0]);
var I = eye(3,3);
var B = A + I;
/* STOP_EXAMPLE */
