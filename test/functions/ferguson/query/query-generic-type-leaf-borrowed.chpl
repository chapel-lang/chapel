class GenericClass {
  type t;
  var v: int;
}

class OtherGenericClass {
  type t;
  var v: int;
}

record Wrapper {
  var f;
}

proc f(x: Wrapper(borrowed GenericClass)) {
  writeln(x);
}
proc g(x: Wrapper(borrowed GenericClass(?))) {
  writeln(x);
}


var a = new Wrapper((new owned GenericClass(int, 3)).borrow());
var b = new Wrapper((new owned GenericClass(real, 4)).borrow());
var c = new Wrapper((new owned OtherGenericClass(int, 3)).borrow());
var d = new Wrapper((new owned OtherGenericClass(real, 4)).borrow());

// these should work
f(a);
f(b);
g(a);
g(b);

f(c); // error -- mismatch
