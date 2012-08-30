param n = 10;

var D: domain(1) = {1..n};

var x : [D] real;
var y : [D] real;
var alpha = 2.0;

for i in D do {
    y(i) = i;
    x(i) = 10.0;
}

for i in D do 
    y(i) = y(i) + alpha*x(i);

writeln(y);
