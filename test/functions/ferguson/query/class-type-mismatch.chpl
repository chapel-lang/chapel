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

proc g(x: Wrapper(GenericClass(?))) {
  writeln(x, " : ", x.type:string);
}

var c = new Wrapper(new borrowed OtherGenericClass(int, 3));

g(c); // error - type mismatch
