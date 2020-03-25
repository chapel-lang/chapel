class GenericClass {
  type t;
  var v: int;
}

record Wrapper {
  var f;
}

proc g(x: Wrapper(borrowed GenericClass)) {
  writeln(x, " : ", x.type:string);
}

var c = new Wrapper(new unmanaged GenericClass(int, 3));

g(c); // error - type mismatch
