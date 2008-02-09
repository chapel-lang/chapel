-- this is my favorite version based on my current
-- understanding of the language -- it's a bit more
-- abstract, but not completely.

var m = 100; 
var n = 200;

var Mat     : domain(2) distribute(block(2)) = (1..m, 1..n);
var VectIn  : domain(Mat) = (*, Mat.project(2));
var VectOut : domain(Mat) = (Mat.project(1), *);

var A = [i,j:Mat] (if i=j then 1.0 else 0.0);
var v = [i:Col] float(i);
var s : [VectOut] float;




s = sum(dim=2) [Mat] A*v;
