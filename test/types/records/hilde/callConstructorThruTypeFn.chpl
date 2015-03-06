// callConstructorThruTypeFn.chpl
//
// Demonstrate calling a constructor using a type function as the base.
//

record R {
  var _i : int;
  proc R(i : int) { _i = i; }
}

record S : R {
  var _r : real;
  proc S(i : int) { _i = i; _r = 3.1416; }
}

proc select(param derived = false) type {
  if (derived) then return S;
  else return R;
}
  
var v = new select(true)(3);

writeln(v);

 
