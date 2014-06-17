var t : (int, (int, int));
t(1) = 1;
t(2)(1) = 2;
t(2)(2) = 3;
writeln(t);

var i : int, j : int, k : int;

(i, (j, k)) = t;

writeln(i);
writeln(j);
writeln(k);
