-- this code is based on the mvm consensus code
-- from January, 2004's powerpoint slides but
-- makes changes that Brad prefers:
-- a) uses a more explicit grid declaration
-- b) makes Col and Row 2D subdomains of Mat
-- c) renames x and y to s and v
-- d) uses the shorthand array declaration syntax
-- e) uses floating point literals
-- f) doesn't need to spread v since its 2D
-- g) outputs the answer

var m : integer = 100; 
var n : integer = 200;

var Grid : [(i,j):1..4, 1..4] locale
         = Locale((i-1)*4 + j);

var Mat : domain(2) distribute(block2) to (Grid)
        = (1..m, 1..n);
var Col : domain(Mat) = (*, Mat.2);
var Row : domain(Mat) = (Mat.1, *);

var A : [Mat] float;
var v : [Col] float;
var s : [Row] float;

A = [i,j:Mat] (i=j);
v = [*,i:Col] i;

s = sum(dim=1) [Mat] A*v;

write(v);
