// callConstructorThruTypeFn.chpl
//
// Demonstrate calling a constructor using a type function as the base.
//

record R {
  var _i : int;
  proc init(i : int) { _i = i; }
}

record S {
  var _i : int;
  var _r : real;
  proc init(i : int) { _i = i; _r = 3.1416; }
}

proc chooseARecordType(param derived = false) type {
  if (derived) then return S;
  else return R;
}
  
var v = new chooseARecordType(true)(3);

writeln(v);

 
