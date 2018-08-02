class GenericClass {
  type t;
  var v: int;
}

record Wrapper {
  var f;
}

proc f(x: 2*Wrapper) {
  writeln(x);
}

var a = new Wrapper(1);
var b = new Wrapper(new owned GenericClass(int, 3));

f( (a,b) );
