use BlockDist;

class C {
  var X: [1..3] real;




}

const D = {1..3} dmapped Block({1..3});
var A: [D] real;

var myC = new owned C(A);
writeln("myC is: ", myC);
