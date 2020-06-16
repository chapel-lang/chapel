use Types;

class C { var x: int; }
record ContainingNonNilableOwned {
  var z: owned C;
}
if isDefaultInitializable(ContainingNonNilableOwned) != false {
  compilerError("error matching ContainingNonNilableOwned");
}

record ContainingNonNilableOwnedAndInit {
  var z: owned C;
  proc init() {
    this.z = new owned C(1);
  }
}
if isDefaultInitializable(ContainingNonNilableOwnedAndInit) != true {
  compilerError("error matching ContainingNonNilableOwnedAndInit");
}

record GenericContainingNonNilableOwnedAndInit {
  type t;
  var z: t;
  proc init(type t) {
    this.t = t;
    this.z = new owned C(1);
  }
}
if isDefaultInitializable(GenericContainingNonNilableOwnedAndInit(owned C)) != true {
  compilerError("error matching GenericContainingNonNilableOwnedAndInit(owned C)");
}

record ContainingNonNilableWithFieldDefault {
  var c: unmanaged object = new	unmanaged object();
}
if isDefaultInitializable(ContainingNonNilableWithFieldDefault) != true {
  compilerError("error matching ContainingNonNilableWithFieldDefault");
}

record ContainingRecordWithInit {
  var z: ContainingNonNilableOwnedAndInit;
}
if isDefaultInitializable(ContainingRecordWithInit) != true {
  compilerError("error matching ContainingRecordWithInit");
}

record ContainingContainingNonNilableOwned {
  var z: ContainingNonNilableOwned;
}
if isDefaultInitializable(ContainingContainingNonNilableOwned) != false {
  compilerError("error matching ContainingContainingNonNilableOwned");
}

record ContainingDefaultContainingNonNilableOwned {
  var z: ContainingNonNilableOwned = new ContainingNonNilableOwned(new C(1));
}
if isDefaultInitializable(ContainingDefaultContainingNonNilableOwned) != true {
  compilerError("error matching ContainingDefaultContainingNonNilableOwned");
}

record NoZeroArgInit {
  var x: int;
  proc init(arg: int) {
    this.x = arg;
  }
}
if isDefaultInitializable(NoZeroArgInit) != false {
  compilerError("error matching NoZeroArgInit");
}


if isDefaultInitializable((owned C, owned C)) != false {
  compilerError("error matching tuple of owned");
}

if isDefaultInitializable((ContainingNonNilableOwned, ContainingNonNilableOwned)) != false {
  compilerError("error matching tuple of ContainingNonNilableOwned");
}

if isDefaultInitializable((ContainingNonNilableWithFieldDefault, ContainingNonNilableOwnedAndInit)) != true {
  compilerError("error matching tuple of ContainingNonNilableWithFieldDefault, ContainingNonNilableOwnedAndInit");
}

proc testArrays() {
  var A0:[1..1] owned C? = [new owned C?(1)];
  if isDefaultInitializable(A0.type) != true {
    compilerError("error matching array of owned C?");
  }

  var A1:[1..1] owned C = [new owned C(1)];
  if isDefaultInitializable(A1.type) != false {
    compilerError("error matching array of owned C");
  }

  var A2:[1..1] ContainingNonNilableOwnedAndInit = [new ContainingNonNilableOwnedAndInit()];
  if isDefaultInitializable(A2.type) != true {
    compilerError("error matching array of ContainingNonNilableOwnedAndInit");
  }
}
testArrays();

proc test11() {
  var d = {1..10};
  assert(isDefaultInitializable(d.type));
  assert(isDefaultInitializable(d));
  var dd: d.type;
}
test11();

proc test12() {
  assert(isDefaultInitializable(defaultDist.type));
  assert(isDefaultInitializable(defaultDist));
  var dd: defaultDist.type;
}
test11();
