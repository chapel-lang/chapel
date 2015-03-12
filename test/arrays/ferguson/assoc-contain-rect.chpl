record MyArray {
  var D: domain(1);
  var A: [D] int;
}

var aD: domain(int);
var aA: [aD] MyArray;

for i in 1..100 {
  writeln(i);
  aD += i;
}

