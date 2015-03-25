// callConstructorThruTypeVar.chpl
//
// Demonstrate calling a constructor through a type variable and argument.
//

record R {
  var _i : int;
  proc R(i : int) { _i = i; }
}

type T = R;
  
var v = new T(3);

writeln(v);

proc foo(type U) {
  var w = new U(7);
  writeln(w);
}

foo(R);


