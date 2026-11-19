/*
  Formals used to initialize fields should have an ``in`` intent.
*/

// This initializer uses a default intent, which violates the rule.
record badRecord {
  var x: int;
  proc init(x: int) {
    this.x = x;
  }
}

// This initializer uses an ``in`` intent, which is correct.
record goodRecord {
  var x: int;
  proc init(in x: int) {
    this.x = x;
  }
}
