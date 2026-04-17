config param copy = false;

var x = [1, 2, 3, 4];

ref a = reshape(x, 1..2, 1..2);
a[1, 1] = 5;
writeln(x);

ref b = reshape(x, 1..2, 1..2, copy=false);
b[1, 2] = 6;
writeln(x);

ref c = reshape(x, 1..2, 1..2, checkDims=true);
c[2, 1] = 7;
writeln(x);

ref d = reshape(x, 1..2, 1..2, checkDims=true, copy=false);
d[2, 2] = 8;
writeln(x);

ref e = reshape(x, 1..2, 1..2, copy=false, checkDims=true);
e[1, 1] = 9;
writeln(x);

ref f = reshape(x, 1..2, 1..2, true);
f[1, 2] = 10;
writeln(x);

ref g = reshape(x, 1..2, 1..2, true, false);
g[2, 1] = 11;
writeln(x);

ref h = reshape(x, 1..2, 1..2, copy=false, true);
h[2, 2] = 12;
writeln(x);

ref i = reshape(x, 1..2, 1..2, true, copy=false);
i[1, 1] = 13;
writeln(x);

ref j = reshape(x, 1..2, 1..2, copy=true);
j[1, 2] = 6;
writeln(x);

ref k = reshape(x, 1..2, 1..2, checkDims=true, copy=true);
k[2, 2] = 8;
writeln(x);

ref l = reshape(x, 1..2, 1..2, copy=true, checkDims=true);
l[1, 1] = 9;
writeln(x);

ref m = reshape(x, 1..2, 1..2, true, true);
m[2, 1] = 11;
writeln(x);

ref n = reshape(x, 1..2, 1..2, copy=true, true);
n[2, 2] = 12;
writeln(x);

ref o = reshape(x, 1..2, 1..2, true, copy=true);
o[1, 1] = 13;
writeln(x);

