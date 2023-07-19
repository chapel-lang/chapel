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

var aObj = new owned GenericClass(int, 3);
var bObj = new owned GenericClass(real, 4);
var cObj = new owned OtherGenericClass(int, 3);
var dObj = new owned OtherGenericClass(real, 4);

var a = new Wrapper(aObj.borrow());
var b = new Wrapper(bObj.borrow());
var c = new Wrapper(cObj.borrow());
var d = new Wrapper(dObj.borrow());

// these should work
f(a);
f(b);
g(a);
g(b);

f(c); // error -- mismatch
