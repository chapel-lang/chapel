-- this is a slightly updated, concrete version based on my
-- latest understandings of the language

var m : integer = 100; 
var n : integer = 200;

var Mat     : domain(2) distribute(block2) = (1..m, 1..n);
var VectIn  : domain(1) align(*, ..) to Mat = Mat.project(2);
var VectOut : domain(1) align(.., *) to Mat = Mat.project(1);

-- The above align syntax may be incorrect.  Possibly:
--      ... : domain(1) align(*, Mat.project(2)) = Mat.project(2);
--      ... : domain(1) align(Mat.project(1), *) = Mat.project(1);

var A : [Mat] float;
var v : [VectIn] float;
var s : [VectOut] float;

A = [i,j:Mat] (if i=j then 1 else 0);
v = [i:Col] i;

s = sum(dim=2) [Mat] A*v(*,..);
