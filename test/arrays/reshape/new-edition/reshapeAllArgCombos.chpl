config param copy = false;

var x = [1, 2, 3, 4];

ref a = reshape(x, 1..2, 1..2);
a[1, 1] = 5;
writeln(x);

ref b = reshape(x, 1..2, 1..2, copy=false);
b[1, 2] = 6;
writeln(x);

ref c = reshape(x, 1..2, 1..2, checkDims=true);
b[2, 1] = 7;
writeln(x);

ref d = reshape(x, 1..2, 1..2, checkDims=true, copy=false);
b[2, 2] = 8;
writeln(x);

ref e = reshape(x, 1..2, 1..2, copy=false, checkDims=true);
e[1, 1] = 9;
writeln(x);
