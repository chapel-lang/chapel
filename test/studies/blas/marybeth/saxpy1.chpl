def saxpy(alpha, x:[] float, y:[] float) {
    y +=  alpha*x;
}

var n = 10;
var D: domain(1) = [1..n];
var x: [D] float = 10.0;
var y: [D] float = 1..n;
var alpha = 2.0;

saxpy(alpha,x,y);

writeln("y = ",y);
