pragma "use default init"
class GenericClass {
  type t;
  var v: int;
}

pragma "use default init"
record Wrapper {
  var f;
}

proc f(x: 2*Wrapper) {
  writeln(x);
}

var a = new Wrapper(1);
var b = new Wrapper(new GenericClass(int, 3));

f( (a,b) );
