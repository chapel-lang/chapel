use Types;

class C { var x: int; }
record ContainingNonNilableOwned {
  var z: owned C;
}
if isDefaultInitializable(ContainingNonNilableOwned) != false {
  compilerError("error matching ContainingNonNilableOwned");
}

proc testArrays() {
  var A3:[1..1] ContainingNonNilableOwned = [new ContainingNonNilableOwned(new C(1))];
  if isDefaultInitializable(A3.type) != false {
    compilerError("error matching array of ContainingNonNilableOwned");
  }
}
testArrays();
