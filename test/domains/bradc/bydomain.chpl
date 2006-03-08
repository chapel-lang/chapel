config var n = 8;

var D: domain(1) = [1..n];
var DOdd: domain(1) = D by 2;

var A: [D] integer;

for i in DOdd {
  A(i) = i;
  A(i+1) = -i;
}

writeln("A is: ", A);
