proc saxpy(alpha, x:[] real, y:[] real) {
    y +=  alpha*x;
}

var n = 10;
var D: domain(1) = {1..n};
var x: [D] real = 10.0;
var y: [D] real = 1..n;
var alpha = 2.0;

saxpy(alpha,x,y);

writeln("y = ",y);
