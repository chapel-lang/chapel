config var n = 10;

var X : [1..n] domain(int);

for i in 1..n do X[i] = {i};

var A = + reduce [i in 1..n] X[i];

writeln(+ reduce A);

var A2 = + reduce for i in 1..n do X[i];

writeln(+ reduce A2);
