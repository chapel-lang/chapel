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

var obj = new owned OtherGenericClass(int, 3);
var c = new Wrapper(obj.borrow());

g(c); // error - type mismatch
