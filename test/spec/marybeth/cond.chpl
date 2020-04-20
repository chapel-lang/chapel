const D = {1..5};

var x, y: [D] real;
var alpha = 2.0;

[i in D] y(i) = 3.0*i;
scale(x, y, alpha);
writeln(x);

proc scale(x, y, alpha: real) {
  if (x.size != y.size) then
    writeln("Error:  Inputs are not the same length.");
  else {
    if (alpha == 0.0) {
      x = 0.0;
    } else if (alpha == 1.0) {
      x = y;
    } else {
      x = alpha*y;
    }
  }
}
