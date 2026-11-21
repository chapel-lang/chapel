/*
  Formals used to initialize fields should have an ``in`` intent.

  This is a correctness concern, not a style preference.  A formal with the
  default intent only borrows the actual argument (the default intent for
  records is ``const ref``), so an initializer that uses it to initialize a
  field relies on whatever the default intent happens to permit.  For a type
  that cannot be copied, such as ``owned``, that does not work at all.  The
  ``in`` intent makes the initializer take a copy (or ownership) of the
  argument, which is what storing it into a field means.
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
