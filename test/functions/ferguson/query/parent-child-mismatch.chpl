class ParentGenericClass {
  type t;
}

class ChildGenericClass : ParentGenericClass {
  var v: t;
}

record Wrapper {
  var f;
}

proc g(x: Wrapper(unmanaged ParentGenericClass)) {
  writeln(x, " : ", x.type:string);
}

var c = new Wrapper(new unmanaged ChildGenericClass(int, 3));

g(c); // error - type mismatch
