class GenericClass {
  type t;
  var v: int;
}

record Wrapper {
  var f;
}

proc g(x: Wrapper(owned GenericClass)) {
  writeln(x, " : ", x.type:string);
}

var c = new Wrapper(new shared GenericClass(int, 3));

g(c); // error - type mismatch
