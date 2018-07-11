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
proc g(x: 2*Wrapper(GenericClass(?t))) {
  writeln(x, " ", t:string);
}



var a = new Wrapper(1);
var b = new Wrapper(2);
var c = new Wrapper(new GenericClass(int, 3));
var d = new Wrapper(new GenericClass(int, 4));

f( (a,b) );
g( (c,d) );
