class GenericClass {
  type t;
  var v: int;
}

record Wrapper {
  var f;
}

proc g(x: Wrapper(unmanaged GenericClass(?))) {
  writeln(x, " : ", x.type:string);
}
var obj = new owned GenericClass(int, 3);
var c = new Wrapper(obj.borrow());

g(c); // error - type mismatch
