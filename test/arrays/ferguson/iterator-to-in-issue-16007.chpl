proc foo(in a : [] int) {
  writeln(a);
}

var a : [1..10] int = 1;
var b : [1..10] int = 2;
foo(a+b);
