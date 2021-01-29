proc foo() {
  return ([1,2,3], [1.2,3.4,5.6]);
}

var x = foo();
type t = x.type;
var A: [1..2] t;
writeln(A);
