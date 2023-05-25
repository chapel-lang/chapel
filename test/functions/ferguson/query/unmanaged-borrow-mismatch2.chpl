class GenericClass {
  type t;
  var v: int;
}

record Wrapper {
  var f;
}

proc g(x: Wrapper(unmanaged GenericClass)) {
  writeln(x, " : ", x.type:string);
}

var c = new Wrapper((new owned GenericClass(int, 3)).borrow());

g(c); // error - type mismatch
