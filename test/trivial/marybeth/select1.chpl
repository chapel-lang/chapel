const vectorDomain = {1..5};

var x, y: [vectorDomain] real;
var alpha = 2.0;

for i in vectorDomain {
  y(i) = 3.0*i;
}

scale(x, y, alpha);

writeln(x);

proc scale(x, y, alpha: real) {
  if (x.size != y.size) {
    writeln("Error:  Input vectors are not the same length.");
    return;
  }
  select alpha {
    when 0.0 do x = 0.0;
    when 1.0 do x = y;
    otherwise x = alpha*y;
  }
}
