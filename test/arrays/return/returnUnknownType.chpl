proc anyType(type t): [1..5] {
  var x: [1..5] t;
  return x;
}

var x = anyType(int);
var y = anyType(real);
writeln(x);
writeln(y);
