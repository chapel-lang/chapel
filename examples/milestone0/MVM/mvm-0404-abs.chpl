-- this is the code from the powerpoint slides of
-- January, 2004 with the following changes:
-- a) swapped m and n
-- b) renamed reduce to sum
-- c) changed whitespace

var m = 100;
var n = 200;

var Mat     = (1..m, 1..n);
var VectIn  = Mat.project(2);
var VectOut = Mat.project(1);

var A = [i,j:Mat] (if i=j then 1.0 else 0.0);
var y = [i:Col] float(i);
var x : [VectOut] float;

A = [i,j:Mat] (if i=j then 1 else 0);
y = [i:Col] i;

x = sum(dim=2) [Mat] A*y(*,..);
