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
proc g(x: 2*Wrapper(unmanaged GenericClass(?t))) {
  writeln(x, " ", t:string);
}



var a = new Wrapper(1);
var b = new Wrapper(2);
var c = new Wrapper(new unmanaged GenericClass(int, 3));
var d = new Wrapper(new unmanaged GenericClass(int, 4));

f( (a,b) );
g( (c,d) );

delete c.f;
delete d.f;
