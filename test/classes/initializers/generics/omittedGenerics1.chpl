// Covers some cases not handled by
// test/classes/initializers/base-class-generic-args.chpl, namely params that
// only supplied either a type or an initialization expression, but not both.
class SoManyParams {
  param a: int;
  param b = -6;
  var otherField = 10;

  proc init(otherFieldVal) {
    otherField = otherFieldVal;
    // the above is used to make the initializer actually do something.  The
    // other two fields should be given their appropriate default value.
  }
}

proc main() {
  var c = new borrowed SoManyParams(7);
  writeln(c.type:string);
  writeln(c);
}
