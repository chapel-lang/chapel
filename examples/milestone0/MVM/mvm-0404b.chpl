-- this is my favorite concrete version based on my current
-- understanding of the language.


var m : integer = 100; 
var n : integer = 200;

var Mat     : domain(2) distribute(block(2)) = (1..m, 1..n);
var VectIn  : domain(Mat) = (*, Mat.project(2));
var VectOut : domain(Mat) = (Mat.project(1), *);

var A : [Mat] float;
var v : [VectIn] float;
var s : [VectOut] float;

A = [i,j:Mat] (if i=j then 1 else 0);
v = [i:Col] i;

s = sum(dim=2) [Mat] A*v;
