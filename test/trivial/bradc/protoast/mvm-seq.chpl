-- this is the code from the powerpoint slides of
-- January, 2004 with the following changes:
-- a) swapped m and n
-- b) renamed reduce to sum
-- c) changed whitespace

var m : integer = 100; 
var n : integer = 200;

var Mat : domain(2) = (1..m, 1..n);
var Col : domain(2) = (1 /* * */, 1..n);  -- used to be flood
var Row : domain(2) = (1..m, 1 /* * */);  -- used to be flood

var A : [Mat] float;
var y : [Col] float;
var x : [Row] float;

A = [i,j in Mat]  (i-1)*n + j; -- (if i=j then 1 else 0);
y = [i in Col] i;

-- x = sum(dim=2) [Mat] A*y;
