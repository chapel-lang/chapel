// Covers some cases not handled by
// test/classes/initializers/base-class-generic-args.chpl, namely params that
// did not specify anything about themselves, as we expect omitted
// initialization of that field to error.
class Doomed {
  param a;
  var otherField = 10;

  proc init(otherFieldVal) {
    otherField = otherFieldVal;
    // the above is used to make the initializer actually do something.  It
    // should fail to resolve, though, because we have no idea what to do with
    // the param field.
  }
}

proc main() {
  var c = new unmanaged Doomed(7);
  writeln(c);
  delete c;
}
