var t : (int, (int, int));
t(0) = 1;
t(1)(0) = 2;
t(1)(1) = 3;
writeln(t);

var i : int, j : int, k : int;

(i, (j, k)) = t;

writeln(i);
writeln(j);
writeln(k);
