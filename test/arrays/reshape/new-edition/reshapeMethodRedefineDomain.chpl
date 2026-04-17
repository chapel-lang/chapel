var x = [1, 2, 3, 4];

var D = {1..2, 1..2};
ref z = reshape(x, D);

writeln(z);
z[1, 1] = 5;

writeln(x);

D = {1..100, 1..100};
writeln(z);
writeln(x);

