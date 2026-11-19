/*
  Formals used to initialize fields should have an ``in`` intent.
*/

// This initializer uses a default intent, which violates the rule.
record badRecord {
  var x: int;
  proc init(newX: int) {
    x = newX;
  }
}

// This initializer uses an ``in`` intent, which is correct.
record goodRecord {
  var x: int;
  proc init(in newX: int) {
    x = newX;
  }
}
