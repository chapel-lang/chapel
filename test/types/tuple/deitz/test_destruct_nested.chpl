var t : (integer, (integer, integer));
t(1) = 1;
t(2)(1) = 2;
t(2)(2) = 3;
writeln(t);

var i, j, k;

(i, (j, k)) = t;

writeln(i);
writeln(j);
writeln(k);
