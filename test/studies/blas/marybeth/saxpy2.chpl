def saxpy(alpha, x:[D] float, y:[D] float) {
  for i in D do  
    y(i) +=  alpha*x(i);
}

var n = 10;
var D: domain(1) = [1..n];
var x: [D] float = 10.0;
var y: [D] float = 1..n;
var alpha = 2.0;

saxpy(alpha,x,y);

writeln("y = ",y);
