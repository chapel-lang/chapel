-- this is the code from the powerpoint slides of
-- January, 2004 with the following changes:
-- a) swapped m and n
-- b) renamed reduce to sum
-- c) changed whitespace

var m : integer = 100; 
var n : integer = 200;

var Grid : array (1..4, 1..4) of locale
         = reshape(Locales(1..16), (1..4, 1..4));

var Mat : domain(2) distribute(block2) to (Grid)
        = (1..m, 1..n);
var Col : domain(1) align(*, Mat.2) = Mat.2;
var Row : domain(1) align(Mat.1, *) = Mat.1;

var A : array Mat of float;
var y : array Col of float;
var x : array Row of float;

A = [i,j:Mat] (if i=j then 1 else 0);
y = [i:Col] i;

x = sum(dim=1) [Mat] A*y(*,..);
